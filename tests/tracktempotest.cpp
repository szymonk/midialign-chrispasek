#include <cstdio>
#include "../tracktempo.hpp"

#define FAIL printf("Test failed (%s, line %d)\n", __FILE__, __LINE__)

int main(void) {
	tracktempo T(0.0);
	T.addTempoMark(4, 1.0);
	T.addTempoMark(2, 1.0);
	T.addTempoMark(0, 2.0);
	T.addTempoMark(3, 3.0);
	T.delTempoMark(2);

	if ( T.getTickTime(0) != 0.0) { FAIL; return 1; }
	if ( T.getTickTime(1) != 2.0) { FAIL; return 1; }
	if ( T.getTickTime(2) != 4.0) { FAIL; return 1; }
	if ( T.getTickTime(3) != 6.0) { FAIL; return 1; }
	if ( T.getTickTime(4) != 9.0) { FAIL; return 1; }
	if ( T.getTickTime(5) != 10.0) { FAIL; return 1; }
	if ( T.getTickTime(6) != 11.0) { FAIL; return 1; }

	if ( T.nextTempoMarkAfter(0) != 3) { FAIL; return 1; }
	if ( T.nextTempoMarkAfter(1) != 3) { FAIL; return 1; }
	if ( T.nextTempoMarkAfter(2) != 3) { FAIL; return 1; }
	if ( T.nextTempoMarkAfter(3) != 4) { FAIL; return 1; }
	if ( T.nextTempoMarkAfter(4) != 0) { FAIL; return 1; }
	if ( T.nextTempoMarkAfter(5) != 0) { FAIL; return 1; }
	if ( T.nextTempoMarkAfter(6) != 0) { FAIL; return 1; }
	
	if ( T.readTempoMark(0) != 2.0) { FAIL; return 1; }
	if ( T.readTempoMark(1) != 2.0) { FAIL; return 1; }
	if ( T.readTempoMark(2) != 2.0) { FAIL; return 1; }
	if ( T.readTempoMark(3) != 3.0) { FAIL; return 1; }
	if ( T.readTempoMark(4) != 1.0) { FAIL; return 1; }
	if ( T.readTempoMark(5) != 1.0) { FAIL; return 1; }
	if ( T.readTempoMark(6) != 1.0) { FAIL; return 1; }

	tracktempo S(1.0);
	//~ if ( T!=S ) { printf("T==S\n"); return 1; }

	return 0;
}
