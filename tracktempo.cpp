
#include "tracktempo.hpp"

#include <vector>
#include <list>

#ifdef DEBUG
	#include <cstdio>
#endif

using namespace std;

typedef pair<unsigned int, double> tTempoMark;

class ptracktempo {
	public:

	ptracktempo(double seconds_per_tick) {
		tempoList.push_back(tTempoMark(0,seconds_per_tick));		// tick 0
	}

	void addTempoMark(unsigned int tick, double seconds_per_tick) {
		list<tTempoMark>::iterator it = tempoList.end();
		it--;
		while(it->first > tick) {
			it--;
		}
		if(it->first != tick) {
			it++;
			tempoList.insert(it,tTempoMark(tick,seconds_per_tick));	//~ add element
		}
		else {
			it->second = seconds_per_tick;
		}
	#ifdef DEBUG
		//~ for(it = tempoList.begin(); it!=tempoList.end(); it++){
			//~ printf("%d %lf\n", it->first, it->second);
		//~ } printf("\n");
	#endif	
	}

	void delTempoMark(unsigned int tick) {
		if (tick == 0) {
			return;
		}
		list<tTempoMark>::iterator it = tempoList.end();
		it--;
		while(it->first > tick) {
			it--;
		}
		if(it->first == tick) {
			tempoList.erase(it);	//~ remove element
		}
	#ifdef DEBUG
		//~ for(it = tempoList.begin(); it!=tempoList.end(); it++){
			//~ printf("%d %lf\n", it->first, it->second);
		//~ } printf("\n");
	#endif
	}

	double getTickTime(unsigned int tick) {
		double total = 0.0;
		list<tTempoMark>::iterator it = tempoList.begin();
		unsigned int prev_tick = 0;
		double prev_tempo = it->second;
		it++;
		while (it != tempoList.end() && it->first < tick) {
			total += (it->first - prev_tick)*(prev_tempo);
			prev_tick = it->first;
			prev_tempo = it->second;
			it++;
		}
		total += (tick - prev_tick)*(prev_tempo);
	#ifdef DEBUG
		//~ printf("%3d %10lf\n", tick, total);
	#endif
		return total;
	}

	//~ TODO destructor if needed
	//~ ~ptracktempo(){};

	private:

	//~ TODO replace list with vector, for linear insertions, but getTickTime() in O(lg)
	list<tTempoMark> tempoList;
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
