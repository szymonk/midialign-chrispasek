#ifndef TRACKTEMPO_H
#define TRACKTEMPO_H

typedef unsigned long long tick_t;

class tracktempo {
	public:
		tracktempo(double seconds_per_tick);
		tracktempo(const tracktempo &);
		~tracktempo();
		tracktempo & operator=(const tracktempo &);
		void addTempoMark(tick_t tick, double seconds_per_tick);
		void delTempoMark(tick_t tick);
		double getTickTime(tick_t tick) const;
	private:
		void * p;
};

#endif
