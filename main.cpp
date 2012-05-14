
#include <iostream>
using namespace std;

#include "midi.hpp"
#include "align_midi_events.hpp"
#include "find_nice_tempo.hpp"

void run(char ** params) {
	if ((!params[0]) || (!params[1])) {
		cerr << "\nUsage:\n" <<
			"\tmidialign <input.midi> <output.midi>\n\n";
		return;
	}
	#ifdef DEBUG
		cerr << "Reading file '" << params[0] << "'..." << endl;
	#endif
	midi m(params[0]);
	#ifdef DEBUG
		cerr << "Finding a nice tempo..." << endl;
	#endif
	tracktempo t = find_nice_tempo(0, 0);
	#ifdef DEBUG
		cerr << "Aligning MIDI events..." << endl;
	#endif
	align_midi_events(m, t);
	#ifdef DEBUG
		cerr << "Saving output file '" << params[1] << "'..." << endl;
	#endif
	m.save(params[1]);
}

int main(int argc, char ** argv) {
	#ifdef DEBUG
		cerr << "Warning! This is a debug build." << endl;
	#endif
	try {
		run(argv+1);
		return 0;
	} catch(const char * e) {
		cerr << "Error! " << e << endl;
	} catch(...) {
		cerr << "Error! (unknown reason)" << endl;
	}
	return 1;
}
