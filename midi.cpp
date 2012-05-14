
#include "midi.hpp"

midi::midi(const char *) {
	throw 1;
}

bool midi::save(const char *) {
	return false;
}

track & midi::tracks(unsigned int) {
	throw 1;
}

const track & midi::tracks(unsigned int) const {
	throw 1;
}

unsigned int midi::trackCount() {
	return 0;
}
