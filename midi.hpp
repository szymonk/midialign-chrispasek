#ifndef MIDI_H
#define MIDI_H

#include "tracktempo.hpp"

class event {
	public:
		virtual int getStartTicks() = 0;
		virtual int getDurationTicks() = 0;
		virtual double getStart() = 0;
		virtual double getDuration() = 0;
		virtual bool isNote() = 0;
};

class track {
	public:
		virtual event & events(unsigned int) = 0;
		virtual const event & events(unsigned int) const = 0;
		virtual const tracktempo & getTrackTempo() = 0;
		virtual void setTrackTempo(const tracktempo &) = 0;
		virtual unsigned int eventCount() = 0;
};

class midi {
	public:
		midi(const char *);
		bool save(const char *);
		track & tracks(unsigned int);
		const track & tracks(unsigned int) const;
		unsigned int trackCount();
	private:
		midi() {};
		void operator=(const midi &) {};
		void * p;
};

#endif
