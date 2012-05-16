#ifndef TRACKTEMPO_H
#define TRACKTEMPO_H

class tracktempo {
	public:
		tracktempo(double seconds_per_tick);
		tracktempo(const tracktempo &);
		~tracktempo();
		tracktempo & operator=(const tracktempo &);
		void addTempoMark(unsigned int tick, double seconds_per_tick);
		void delTempoMark(unsigned int tick);
		double getTickTime(unsigned int tick) const;
	private:
		void * p;
};

#endif
