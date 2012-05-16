#include <cstdio>
#include "../tracktempo.hpp"

int main(void) {

	//~ printf("initializing...\n");
	tracktempo T(0.0);
	T.addTempoMark(4, 1.0);
	T.addTempoMark(2, 1.0);
	T.addTempoMark(0, 2.0);
	T.addTempoMark(3, 3.0);
	T.delTempoMark(2);
	
	printf("0.0 == %lf\n", T.getTickTime(0));
	printf("2.0 == %lf\n", T.getTickTime(1));
	printf("4.0 == %lf\n", T.getTickTime(2));
	printf("6.0 == %lf\n", T.getTickTime(3));
	printf("9.0 == %lf\n", T.getTickTime(4));
	printf("10.0 == %lf\n", T.getTickTime(5));
	printf("11.0 == %lf\n", T.getTickTime(6));

	printf("3 == %llu\n", T.nextTempoMarkAfter(0));
	printf("3 == %llu\n", T.nextTempoMarkAfter(1));
	printf("3 == %llu\n", T.nextTempoMarkAfter(2));
	printf("4 == %llu\n", T.nextTempoMarkAfter(3));
	printf("0 == %llu\n", T.nextTempoMarkAfter(4));
	printf("0 == %llu\n", T.nextTempoMarkAfter(5));
	printf("0 == %llu\n", T.nextTempoMarkAfter(6));
	
	printf("2.0 == %lf\n", T.readTempoMark(0));
	printf("2.0 == %lf\n", T.readTempoMark(1));
	printf("2.0 == %lf\n", T.readTempoMark(2));
	printf("3.0 == %lf\n", T.readTempoMark(3));
	printf("1.0 == %lf\n", T.readTempoMark(4));
	printf("1.0 == %lf\n", T.readTempoMark(5));
	printf("1.0 == %lf\n", T.readTempoMark(6));

	return 0;
}
