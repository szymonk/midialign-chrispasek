
#include "dump_midi.hpp"

#include <iostream>
using namespace std;

void dump_tracktempo_stderr(const tracktempo & tt) {
	cerr << "\ttempo:" << endl;
	bool first = true;
	for (tick_t i = 0;; i = tt.nextTempoMarkAfter(i)) {
		if ((!i) && (!first)) break;
		first = false;
		double v = tt.readTempoMark(i);
		cerr << "\t\ttick " << i << ": " << v << "s/tick ("
			<< (1/v) << " ticks/s)" << endl;
	}
}

void dump_track_stderr(const track & t, int tnum) {
	cerr << "Track " << tnum << endl;
	dump_tracktempo_stderr(t.getTrackTempo());
	cerr << "\tevents:" << endl;
	char buf[256];
	for (unsigned int i = 0; i < t.eventCount(); i++) {
		event const  & e = t.events(i);
		e.getDescription(buf, 256);
		if (e.isNote()) {
			cerr << "\t\tnote:  ticks [" << e.getStartTicks() << "-" <<
				(e.getStartTicks()+e.getDurationTicks()) <<
				"], time [" << e.getStart() << "s-" <<
				(e.getStart()+e.getDuration()) <<
				"s]\n\t\t       " << buf << endl;
		} else {
			cerr << "\t\tevent: tick [" << e.getStartTicks() <<
				"], time [" << e.getStart() <<
				"s]\n\t\t       " << buf << endl;
		}
	}
}

void dump_midi_stderr(const midi & m) {
	cerr << "MIDI File (" << m.trackCount() << " tracks)" << endl;
	for (unsigned int i = 0; i < m.trackCount(); i++)
		dump_track_stderr(m.tracks(i), i+1);
}
