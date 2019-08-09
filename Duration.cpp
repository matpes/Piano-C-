#include "Duration.h"

Duration::Duration(double d)
{
	int dur = (int) (d * 8);
	if (dur & 1) {
		//Duration(dur , 8);
		numerator = dur;
		denominator = 8;
	}
	else {
		//Duration(dur / 2, 4);
		numerator = dur / 2;
		denominator = 4;
	}
}

Duration::~Duration()
{
}
