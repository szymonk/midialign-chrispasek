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
	
	if (T.readTempoMark(1) != 2.0) { printf("Test failed\n"); return 1; }

	if ( T.getTickTime(0) != 0.0) { printf("0.0 == %lf Test failed\n",T.getTickTime(0)); return 1; }
	if ( T.getTickTime(1) != 2.0) { printf("2.0 == %lf Test failed\n",T.getTickTime(1)); return 1; }
	if ( T.getTickTime(2) != 4.0) { printf("4.0 == %lf Test failed\n",T.getTickTime(2)); return 1; } 
	if ( T.getTickTime(3) != 6.0) { printf("6.0 == %lf Test failed\n",T.getTickTime(3)); return 1; } 
	if ( T.getTickTime(4) != 9.0) { printf("9.0 == %lf Test failed\n",T.getTickTime(4)); return 1; } 
	if ( T.getTickTime(5) != 10.0) { printf("10.0 == %lf Test failed\n",T.getTickTime(5)); return 1; }
	if ( T.getTickTime(6) != 11.0) { printf("11.0 == %lf Test failed\n",T.getTickTime(6)); return 1; } 

	if ( T.nextTempoMarkAfter(0) != 3) { printf("3 == %llu Test failed\n",T.nextTempoMarkAfter(0)); return 1; } 
	if ( T.nextTempoMarkAfter(1) != 3) { printf("3 == %llu Test failed\n",T.nextTempoMarkAfter(1)); return 1; } 
	if ( T.nextTempoMarkAfter(2) != 3) { printf("3 == %llu Test failed\n",T.nextTempoMarkAfter(2)); return 1; } 
	if ( T.nextTempoMarkAfter(3) != 4) { printf("4 == %llu Test failed\n",T.nextTempoMarkAfter(3)); return 1; } 
	if ( T.nextTempoMarkAfter(4) != 0) { printf("0 == %llu Test failed\n",T.nextTempoMarkAfter(4)); return 1; } 
	if ( T.nextTempoMarkAfter(5) != 0) { printf("0 == %llu Test failed\n",T.nextTempoMarkAfter(5)); return 1; } 
	if ( T.nextTempoMarkAfter(6) != 0) { printf("0 == %llu Test failed\n",T.nextTempoMarkAfter(6)); return 1; } 
	
	if ( T.readTempoMark(0) != 2.0) { printf("2.0 == %lf Test failed\n",T.readTempoMark(0)); return 1; } 
	if ( T.readTempoMark(1) != 2.0) { printf("2.0 == %lf Test failed\n",T.readTempoMark(1)); return 1; } 
	if ( T.readTempoMark(2) != 2.0) { printf("2.0 == %lf Test failed\n",T.readTempoMark(2)); return 1; } 
	if ( T.readTempoMark(3) != 3.0) { printf("3.0 == %lf Test failed\n",T.readTempoMark(3)); return 1; } 
	if ( T.readTempoMark(4) != 1.0) { printf("1.0 == %lf Test failed\n",T.readTempoMark(4)); return 1; } 
	if ( T.readTempoMark(5) != 1.0) { printf("1.0 == %lf Test failed\n",T.readTempoMark(5)); return 1; } 
	if ( T.readTempoMark(6) != 1.0) { printf("1.0 == %lf Test failed\n",T.readTempoMark(6)); return 1; } 

	tracktempo S(1.0);
	//~ if ( T!=S ) { printf("T==S\n"); return 1; }

	return 0;
}
