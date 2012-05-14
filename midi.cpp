
#include "midi.hpp"

#include <vector>
using namespace std;

class pmidi {
	public:

	pmidi(const char * fn) {
	}

	~pmidi() {
	}

	bool save(const char * fn) {
		return false;
	}

	track & tracks(unsigned int) {
		throw "x";
	}

	const track & tracks(unsigned int) const {
		throw "x";
	}

	unsigned int trackCount() {
		return t.size();
	}

	private:

	vector<char> t;
};

midi::midi(const char * fn) { p = new pmidi(fn); }
midi::~midi() { delete ((pmidi *)p); }
bool midi::save(const char * fn) { return ((pmidi *)p)->save(fn); }
track & midi::tracks(unsigned int t) { return ((pmidi *)p)->tracks(t); }
const track & midi::tracks(unsigned int t) const { return ((pmidi *)p)->tracks(t); }
unsigned int midi::trackCount() { return ((pmidi *)p)->trackCount(); }
