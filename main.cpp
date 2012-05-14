
#include <iostream>
using namespace std;

#include "midi.hpp"

void run(char ** params) {
	midi m("plik");
	cout << "Hello!" << endl;
	m.save("abc");
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
