
#include "dump_midi.hpp"

#include <iostream>
using namespace std;

void dump_midi_stderr(const midi & m) {
	cerr << "MIDI File (" << m.trackCount() << " tracks)" << endl;
}
