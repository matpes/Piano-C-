#include "Pause.h"



void Pause::write(ostream & it)
{
	char c = 'p';
	if (getDuration() == Duration(1, 4)) {
		c = c - 'a' + 'A';
		it << setw(6) << left << c;
	}
	else {
		it << setw(3) << left << c;
	}
}

void Pause::writeFormatt(ofstream & it)
{
	it << "<note>\n";
	it << "<rest/>\n";
	it << "<duration>"<< 8/(du.getDenominator()) <<"</duration>\n";
	it << "</note>\n";

}

Pause::Pause(Duration dur):MusicSymbol(dur)
{
}


Pause::~Pause()
{
}
