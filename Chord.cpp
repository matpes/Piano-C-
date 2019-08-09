#include "Chord.h"


void Chord::write(ostream & it)
{
	Note * n = chord->front();
	string s;
	s = n->Hight();
	if (n->isSharp()) {
		s += '#';
	}
	s += 'C';
	if (getDuration() == Duration(1, 8)) {
		s[0] += 'a' - 'A';
		it << setw(3) << left << s;
	}
	else {
		it << setw(6) << left << s;
	}
	
}

void Chord::writeFormatt(ofstream & it)
{
	for (Note* n : *chord) {
		it << "<note>\n";
		if (n != chord->front()) {
			it << "<chord/>\n";
		}
		if (n->isDivided()) {
			
			if (first) {
				it << "<tie type=\"start\"/>\n";
			}
			else {
				it << "<tie type=\"end\"/>\n";
			}
		}
		it << *n;
		it << "</note>\n";
	}
}

Chord::Chord(Duration dur):MusicSymbol(dur)
{
	chord = new list<Note*>();


}

Chord::Chord(Chord * c) :MusicSymbol(c->du)
{
	chord = new list<Note*>();
	Divided = c->isDivided();
	first = c->first;
}

Chord::Chord(const Chord & c) :MusicSymbol(c.du) {
	chord = new list<Note*>();
	for (Note* n : *c.chord) {
		Note *newNote = new Note(*n);
		newNote->setDivided(n->isDivided());
		chord->push_back(newNote);
	}
	Divided = c.Divided;
	first = c.first;
}

void Chord::setOctave(int o)
{
	for_each(chord->begin(), chord->end(), [o](Note *n) {n->setOctave(o); });

}

void Chord::setSign(int c)
{
	for (Note* n : *chord) {
		n->setSign(c);
	}
}

Chord & Chord::operator=(const Chord & c)
{
	if (this != &c) {
		chord = new list<Note*>();
		for (Note* n : *c.chord) {
			Note *newNote = new Note(*n);
			chord->push_back(newNote);
		}
	}
	return *this;
}

Chord & Chord::operator=(Chord && c)
{
	if (this != &c) {
		chord = c.chord;
		c.chord = nullptr;
	}
	return *this;
}

void Chord::setDuration(Duration d)
{
	du = d;
	for_each(chord->begin(), chord->end(), [d](Note *n) {n->setDuration(d); });
}

void Chord::setDivided(bool s)
{
	Divided = true;
	for_each(chord->begin(), chord->end(), [s](Note *n) {n->setDivided(s); });
}

void Chord::setSharp()
{
	for_each(chord->begin(), chord->end(), [](Note *n) {n->setSharp(); });
}



Chord::~Chord()
{
	for (Note * n : *chord) {
		delete n;
	}
	delete chord;
}
