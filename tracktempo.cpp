
#include "tracktempo.hpp"

#include <vector>
using namespace std;

class ptracktempo {
	public:

	ptracktempo(double seconds_per_tick) {
	}

	void addTempoMark(unsigned int tick, double seconds_per_tick) {
	}

	void delTempoMark(unsigned int tick) {
	}

	double getTickTime(unsigned int tick) {
		return 0.0;
	}

	private:

	vector<int> x;
};

tracktempo::tracktempo(double seconds_per_tick)
	{ p = new ptracktempo(seconds_per_tick); }
tracktempo::tracktempo(const tracktempo & rhs)
	{ p = new ptracktempo(*((ptracktempo *)rhs.p)); }
tracktempo::~tracktempo() { delete ((ptracktempo *)p); }
tracktempo & tracktempo::operator=(const tracktempo & rhs) {
	if (this == &rhs) return *this;
	delete ((ptracktempo *)p);
	p = new ptracktempo(*((ptracktempo *)rhs.p));
	return *this;
}
void tracktempo::addTempoMark(unsigned int tick, double seconds_per_tick)
	{ return ((ptracktempo *)p)->addTempoMark(tick, seconds_per_tick); }
void tracktempo::delTempoMark(unsigned int tick)
	{ return ((ptracktempo *)p)->delTempoMark(tick); }
double tracktempo::getTickTime(unsigned int tick) const
	{ return ((ptracktempo *)p)->getTickTime(tick); }
