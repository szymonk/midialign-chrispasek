
#include "midi.hpp"

#include <vector>
#include <fstream>
#include <iostream>
#include <stdint.h>
#include <cstring>
#include <algorithm>
#include <cassert>

#define EVENT_COMMAND_MASK         0xF0
#define EVENT_CHANNEL_MASK         0xF

#define FORMAT_SINGLE_TRACK        0
#define FORMAT_MULT_TRACKS_SYNC    1
#define FORMAT_MULT_TRACKS_ASYNC   2

#define CMD_NOTE_OFF               0x8
#define CMD_NOTE_ON                0x9
#define CMD_KEY_AFTER_TOUCH        0xA
#define CMD_CONTROL_CHANGE         0xB
#define CMD_PROGRAM_CHANGE         0xC
#define CMD_CHANNEL_AFTER_TOUCH    0xD
#define CMD_PITCH_WHEEL_CHANGE     0xE
#define CMD_META_EVENT             0xF

#define META_TEMPO_CHANGE          0x51

const char * FILE_FORMAT[] = {
	"single-track",
	"multiple tracks, synchronous",
	"multiple tracks, asynchronous"
};

const char * cmd2str[] = {
	"0", "1", "2", "3", "4", "5", "6", "7",
	"CMD_NOTE_OFF",
	"CMD_NOTE_ON",
	"CMD_KEY_AFTER_TOUCH",
	"CMD_CONTROL_CHANGE",
	"CMD_PROGRAM_CHANGE",
	"CMD_CHANNEL_AFTER_TOUCH",
	"CMD_PITCH_WHEEL_CHANGE",
	"CMD_META_EVENT",
};

using namespace std;

typedef unsigned long long tick_t;

inline uint16_t getUint16_t(uint8_t * ptr, int offset) {
	ptr += offset;
	uint16_t v = *((uint16_t *) ptr);
	return ((v << 8) | (v >> 8));
}

uint32_t getUint32_t(ifstream &in) {
	uint32_t result = 0;
	uint8_t tmp;
	for (int i = 0; i < 4; ++i) {
		in.read((char *) &tmp, 1);
		result <<= 8;
		result |= tmp;
	}
	return result;
}

uint32_t getUint24_t(ifstream &in) {
	uint32_t result = 0;
	uint8_t tmp;
	for (int i = 0; i < 3; ++i) {
		in.read((char *) &tmp, 1);
		result <<= 8;
		result |= tmp;
	}
	return result;
}

class pevent : public event {
	public:

	pevent() : len(0), pairVelocity(0), raw(NULL), rawlen(0) {}
	pevent(tick_t start, track * home)
		: start(start), len(0), pairVelocity(0), raw(NULL), rawlen(0), home(home) {}
	pevent(const pevent& o) {
		//cerr << "BBB" << endl;
		start = o.start;
		len = o.len;
		rawlen = o.rawlen;
		pairVelocity = o.pairVelocity;
		home = o.home;
		if (o.rawlen > 0) {
			//cerr << "CCC " << (int) o.raw[0] << endl;
			raw = new uint8_t[rawlen];
			memcpy(raw, o.raw, rawlen);
			for (int i = 0; i < rawlen; ++i)
				raw[i] = o.raw[i];
			//copy(o.raw, o.raw + rawlen, raw);
		}
		else
			raw = NULL;
	}

	~pevent() {
		if (raw != NULL) delete [] raw;
	}

	virtual int getStartTicks() { return start; }

	virtual int getDurationTicks() {
		if (! isNote())
			throw "Unable to get duration ticks for a non-note event.";
		else
			return len;
	}

	virtual double getStart() {
		return home->getTrackTempo().getTickTime(start);
	}

	virtual double getDuration() {
		return home->getTrackTempo().getTickTime(start + len) -
		       home->getTrackTempo().getTickTime(start);
	}

	virtual bool isNote() {
		if (rawlen == 0) return false;
		return (getCommand() == CMD_NOTE_ON);
	}

	uint8_t getCommand() {
		return (raw[0] >> 4);
	}

	uint8_t getMetaCommand() {
		if (getCommand() != CMD_META_EVENT)
			throw "getMetaCommand: Not a meta command!";
		return (raw[1] == META_TEMPO_CHANGE);
	}

	/**
	 * Test if events are the same wrt. channel and first byte after
	 * a command byte.
	 * Particulary, if this and o are EVENT_NOTEs, tells if
	 * they are the same pitch and on the same channel.
	 **/
	bool equalChannelAndPitch(pevent &o) {
		return (
			((raw[0] & EVENT_CHANNEL_MASK) == (o.raw[0] & EVENT_CHANNEL_MASK)) &&
			(raw[1] == o.raw[1])
			);
		/*if (rawlen != o.rawlen)
			return false;
		if (rawlen > 0) {
			if ((raw[0] & EVENT_CHANNEL_MASK) != (o.raw[0] & EVENT_CHANNEL_MASK))
				return false;
			if (strncmp((char *) raw + 1, (char *) o.raw + 1, rawlen - 1) != 0)
				return false;
		}
		return true;*/
	}

	template<class InputIterator>
	void setRaw(InputIterator begin, InputIterator end) {
		if (raw != NULL) delete [] raw;
		raw = new uint8_t[end - begin];
		rawlen = end - begin;
		copy(begin, end, raw);
	}

	pevent& operator=(const pevent& rhs) {
		//cerr << "AAA" << endl;
		start = rhs.start;
		len = rhs.len;
		rawlen = rhs.rawlen;
		pairVelocity = rhs.pairVelocity;
		home = rhs.home;
		if (raw != NULL)
			delete [] raw;
		if (rhs.raw != NULL) {
			raw = new uint8_t[rawlen];
			memcpy(raw, rhs.raw, rawlen);
		}
		return *this;
	}

	bool operator==(const pevent& o) const {
		if (start != o.start) return false;
		if (len != o.len) return false;
		if (pairVelocity != o.pairVelocity) return false;
		if (rawlen != o.rawlen) return false;
		if (home != o.home) return false;
		if (strncmp((char *) raw, (char *) o.raw, rawlen) != 0) return false;
		return true;
	}


	tick_t start;
	tick_t len;
	uint8_t pairVelocity;

#ifdef DEBUG
	uint8_t * raw;
	int rawlen;
#endif

	private:

#ifndef DEBUG
	uint8_t * raw;
	int rawlen;
#endif

	track * home;
};

class ptrack : public track {
	public :

	ptrack() : thisTracktempo(tracktempo(0.0)) {}

	void load(ifstream &in, uint16_t tpq) {
		this->tpq = tpq;
		// assume default 120 bpm => 2 quarter notes per second
		thisTracktempo = tracktempo(1000000000.0/(2.0 * ((double) tpq)));
		char sig[4];
		in.read(sig, 4);
		if (strncmp(sig, "MTrk", 4) != 0)
			throw "Not a valid MIDI track: \"MTrk\" signature not found.";
		eventsCol.clear();
		uint32_t remaining = getUint32_t(in);
		tick_t globalTicks = 0;
		while (remaining > 0) {
			uint8_t x;

			/* Read delta ticks. */
			vector<uint8_t> deltav;
			do
			{
				in >> x;
				deltav.push_back(x);
				--remaining;
			} while ((deltav.back() & (1u << 7)) != 0);
			tick_t deltaTicks = 0;
			for (unsigned i = 0; i < deltav.size() - 1; ++i) {
				deltaTicks <<= 7;
				deltaTicks |= (deltav[i] & 0x7F);
			}
			deltaTicks <<= 8;
			deltaTicks |= deltav.back();
			globalTicks += deltaTicks;

			/* Read event description: command and data. */
			uint8_t rawbuf[260];
			in.read((char *) rawbuf, 1);
			--remaining;

			pevent ev(globalTicks, this);
			bool skip = false;
			ev.start = globalTicks;
			switch (rawbuf[0] >> 4) {
				case CMD_NOTE_OFF:
					in.read((char *) rawbuf + 1, 2);
					ev.setRaw(rawbuf, rawbuf + 3);
					remaining -= 2;
					
					{
						__typeof__(eventsCol.rbegin()) noteOn = eventsCol.rbegin();
						for (; noteOn != eventsCol.rend(); noteOn++)
							if (noteOn->isNote() && noteOn->equalChannelAndPitch(ev)) break;
						if (noteOn == eventsCol.rend())
							cerr << "Warning: NOTE_OFF event without corresponding NOTE_ON." << endl;
						else {
							noteOn->pairVelocity = rawbuf[2];
							noteOn->len = ev.start - noteOn->start;
						}
					}
					break;
				case CMD_NOTE_ON:
					in.read((char *) rawbuf + 1, 2);
					ev.setRaw(rawbuf, rawbuf + 3);
					remaining -= 2;
					break;
				case CMD_KEY_AFTER_TOUCH:
				case CMD_CONTROL_CHANGE:
				case CMD_PITCH_WHEEL_CHANGE:
					in.read((char *) rawbuf + 1, 2);
					ev.setRaw(rawbuf, rawbuf + 3);
					remaining -= 2;
					break;
				case CMD_PROGRAM_CHANGE:
				case CMD_CHANNEL_AFTER_TOUCH:
					in.read((char *) rawbuf + 1, 1);
					ev.setRaw(rawbuf, rawbuf + 2);
					remaining -= 1;
					break;
				case CMD_META_EVENT:
					in.read((char *) rawbuf + 1, 2);
					remaining -= 2;
					if (rawbuf[1] == META_TEMPO_CHANGE) {
						// Handle tempo change.
						uint32_t mspq = getUint24_t(in); // microsec per quarter note
						//in.read((char *) rawbuf + 3, rawbuf[2]);
						thisTracktempo.addTempoMark(ev.start, ((double) mspq) / ((double) tpq));
					}
					else {
						in.read((char *) rawbuf + 3, rawbuf[2]);
					}
					remaining -= rawbuf[2];
					ev.setRaw(rawbuf, rawbuf + 3 + rawbuf[2]);
					break;
				default:
					cerr << "Warning: skipping unknown MIDI command type: " << (rawbuf[0] >> 4) << endl;
					in.read((char *) rawbuf + 1, 1);
					remaining -= 1;
					skip = true;
					break;
			}
#ifdef DEBUG
			if (! skip) {
				pevent z;
				z = ev;
				//assert (z.rawlen > 0);
				assert (z.rawlen == ev.rawlen);
				assert (strncmp((char *) z.raw, (char *) ev.raw, ev.rawlen) == 0);
				cerr << "Event type: " << cmd2str[ev.getCommand()];
				cerr << ", start: " << ev.start;
				if (ev.getCommand() == CMD_META_EVENT) {
					cerr << ", meta command: " << (int) ev.raw[1];
					if (ev.getMetaCommand() == META_TEMPO_CHANGE) {
						cerr << ", META_TEMPO_CHANGE";
					}
				}
				cerr << endl;
				cerr << "remaining bytes: " << remaining << endl;
			}
#endif
			if (! skip) {
				if (ev.getCommand() != CMD_NOTE_OFF)
				{
					if (! (ev.getCommand() == CMD_META_EVENT &&
							ev.getMetaCommand() == META_TEMPO_CHANGE)) {
						eventsCol.push_back(ev);
						assert (eventsCol.back() == ev);
					}
				}
			}
		}
#ifdef DEBUG
		cerr << "# Debug: Finished reading a track. Here are the events:" << endl;
		for (__typeof__(eventsCol.begin()) it = eventsCol.begin(); it != eventsCol.end(); it++) {
			assert (it->getCommand() != CMD_NOTE_OFF);
			cerr << "# Event type: " << cmd2str[(int) it->getCommand()] << "(" << (int) it->getCommand() << ")";
			cerr << ", start: " << it->start;
			if (it->getCommand() == CMD_NOTE_ON)
					cerr << ", duration: " << it->len
					     << ", velocity of a paired NOTE_OFF: " << (int) it->pairVelocity;
			cerr << endl;
		}
#endif
	}

	virtual event & events(unsigned int i) {
		return eventsCol[i];
	}

	virtual const event & events(unsigned int i) const {
		return eventsCol[i];
	}

	virtual const tracktempo & getTrackTempo() {
		return thisTracktempo;
	}

	virtual void setTrackTempo(const tracktempo &t) {
		thisTracktempo = t;
	}

	virtual unsigned int eventCount() {
		return eventsCol.size();
	}

	private:

	vector<pevent> eventsCol;
	tracktempo thisTracktempo;
	uint16_t tpq;
};

class pmidi {
	public:

	pmidi(const char * fn) {
		ifstream in(fn, ifstream::in);

		in.read((char *) header, 14);
		if (strncmp((char *) header, "MThd", 4) != 0)
			throw "Not a MIDI file!";
		if (strncmp((char *) header + 4, "\0\0\0\6", 4) != 0)
			throw "Incorrect header length.";

#ifdef DEBUG
		uint16_t ff = getUint16_t(header, 8);
		cerr << "File format: " << FILE_FORMAT[ff] << endl;
#endif

		uint16_t tracksCount = getUint16_t(header, 10);
		t.resize(tracksCount);

		tpq = getUint16_t(header, 12);

#ifdef DEBUG
		cerr << "Tracks count: " << tracksCount << endl;
		cerr << "Ticks per quarter note: " << tpq << endl;
#endif

		for (unsigned i = 0; i < tracksCount; ++i)
			t[i].load(in, tpq);


		in.close();
	}

	~pmidi() {
	}

	bool save(const char * fn) {
		return false;
	}

	track & tracks(unsigned int i) {
		return t[i];
	}

	const track & tracks(unsigned int i) const {
		return t[i];
	}

	unsigned int trackCount() {
		return t.size();
	}

	private:

	vector<ptrack> t;
	uint8_t header[14];

	/* Ticks per quarter note. */
	uint16_t tpq;
};

midi::midi(const char * fn) { p = new pmidi(fn); }
midi::~midi() { delete ((pmidi *)p); }
bool midi::save(const char * fn) { return ((pmidi *)p)->save(fn); }
track & midi::tracks(unsigned int t)
	{ return ((pmidi *)p)->tracks(t); }
const track & midi::tracks(unsigned int t) const
	{ return ((pmidi *)p)->tracks(t); }
unsigned int midi::trackCount() { return ((pmidi *)p)->trackCount(); }
