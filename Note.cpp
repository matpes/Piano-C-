#include "Note.h"
#include "Errorsi.h"


void Note::write(ostream & it)
{
	string s;
	s = hight;
	if (sharp) {
		s += '#';
	}
	s += '0' + octave;
	if (getDuration() == Duration(1, 8)) {
		s[0] += 'a' - 'A';
		it << setw(3) << left << s;
	}
	else {
		it << setw(6) << left << s;
	}
	
}

void Note::writeFormatt(ofstream & it)
{
	it << "<pitch>\n";
	it << "<step>" << hight << "</step>\n";
	it << "<octave>" << octave << "</octave>\n";
	if (sharp) { it << "<alter>" << 1 << "</alter>\n"; }
	it << "</pitch>\n";
	it << "<duration>" << 8 / (du.getDenominator()) << "</duration>\n";
}

Note::Note(Duration dur, int octave, int numSym, char height, bool sharp) :MusicSymbol(dur)
{
	try {
		if (octave < 2 || octave >6)throw OctaveOutOfRange();
	}
	catch (OctaveOutOfRange e) {
		octave = 6;
	}
	if (height<'A' || height >'G') throw HeightOutOfRange();
	this->octave = octave;
	this->numSym = numSym;
	this->hight = height;
	this->sharp = sharp;

}

void Note::det_color(Note * n, uint8_t & R, uint8_t & G, uint8_t & B)
{
	static map<string, pair<int, pair<int, int>>> mapa;
	mapa["C"] = pair<int, pair<int, int>>(255, pair < int, int>(0, 0));
	mapa["C#"] = pair<int, pair<int, int>>(255, pair < int, int>(127, 0));
	mapa["D"] = pair<int, pair<int, int>>(255, pair < int, int>(255, 0));
	mapa["D#"] = pair<int, pair<int, int>>(127, pair < int, int>(255, 0));
	mapa["E"] = pair<int, pair<int, int>>(0, pair < int, int>(255, 0));
	mapa["E#"] = pair<int, pair<int, int>>(0, pair < int, int>(255, 0));
	mapa["F"] = pair<int, pair<int, int>>(0, pair < int, int>(255, 127));
	mapa["F#"] = pair<int, pair<int, int>>(0, pair < int, int>(255, 255));
	mapa["G"] = pair<int, pair<int, int>>(0, pair < int, int>(127, 255));
	mapa["G#"] = pair<int, pair<int, int>>(0, pair < int, int>(0, 255));
	mapa["A"] = pair<int, pair<int, int>>(127, pair < int, int>(0, 255));
	mapa["A#"] = pair<int, pair<int, int>>(255, pair < int, int>(0, 255));
	mapa["B"] = pair<int, pair<int, int>>(255, pair < int, int>(0, 127));

	string x = "";
	x += n->hight;
	if (n->sharp) { x += "#"; }
	uint8_t x4R, x4G, x4B;
	x4R = mapa[x].first;
	x4G = mapa[x].second.first;
	x4B = mapa[x].second.second;

	switch (n->octave) {
	case 2:
		x4R -= (x4R * 6) / 8;
		x4G -= (x4G * 6) / 8;
		x4B -= (x4B * 6) / 8;
		break;
	case 3:
		x4R -= (x4R * 3) / 8;
		x4G -= (x4G * 3) / 8;
		x4B -= (x4B * 3) / 8;
		break;
	case 4:
		break;
	case 5:
		x4R += ((255 - x4R) * 3) / 8;
		x4G += ((255 - x4G) * 3) / 8;
		x4B += ((255 - x4B) * 3) / 8;
		break;
	case 6:
		x4R += ((255 - x4R) * 6) / 8;
		x4G += ((255 - x4G) * 6) / 8;
		x4B += ((255 - x4B) * 6) / 8;
		break;
	}
	R = x4R;
	G = x4G;
	B = x4B;
	return;
}


Note::~Note()
{
}
