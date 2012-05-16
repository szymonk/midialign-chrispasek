
#include "dump_midi.hpp"

#include <iostream>
using namespace std;

void dump_track_stderr(const track & t, int tnum) {
	cerr << "Track " << tnum << endl;
}

void dump_midi_stderr(const midi & m) {
	cerr << "MIDI File (" << m.trackCount() << " tracks)" << endl;
	for (unsigned int i = 0; i < m.trackCount(); i++)
		dump_track_stderr(m.tracks(i), i+1);
}
