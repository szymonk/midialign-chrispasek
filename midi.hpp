#ifndef MIDI_H
#define MIDI_H

#include "tracktempo.hpp"

class event {
	public:
		virtual int getStartTicks() const = 0;
		virtual int getDurationTicks() const = 0;
		virtual double getStart() const = 0;
		virtual double getDuration() const = 0;
		virtual bool isNote() const = 0;
		virtual void getDescription(char * /*buffer*/, unsigned int /*length*/) const = 0;
};

class track {
	public:
		virtual event & events(unsigned int) = 0;
		virtual const event & events(unsigned int) const = 0;
		virtual const tracktempo & getTrackTempo() const = 0;
		virtual void setTrackTempo(const tracktempo &) = 0;
		virtual unsigned int eventCount() const = 0;
};

class midi {
	public:
		midi(const char *);
		~midi();
		bool save(const char *);
		track & tracks(unsigned int);
		const track & tracks(unsigned int) const;
		unsigned int trackCount() const;
	private:
		midi() {};
		midi(const midi &) {};
		void operator=(const midi &) {};
		void * p;
};

#endif
