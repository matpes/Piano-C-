#ifndef Composition_H_
#define Composition_H_

#include <list>
#include <iterator>
#include <iostream>
#include <string>
#include "Part.h"
using namespace std;
class Composition
{
private:
	Part *part;
	Duration *dur; //Trajanje takta
	void premesti(Composition & c);
	void kopiraj(const Composition & c);

public:
	Duration * getDuration()const {
		return dur;
	}
	Composition() = default;
	Composition(Duration d);
	Composition(const Composition& c) {
		kopiraj(c);
	}
	Composition(Composition &&c) {
		premesti(c);
	}
	Composition& operator = (const Composition& c) {
		if (this != &c) {
			kopiraj(c);
		}
		return *this;
	}
	Composition& operator = (Composition &&c) {
		if (this != &c) {
			premesti(c);
		}
		return *this;
	}
	void addMusicSymbol(MusicSymbol* ms);	
	void finish();

	Part * getPart()const { return part; }

	friend ostream& operator <<(ostream & it, const Composition& c) {
		it << *c.part << endl;
		return it;
	}

	auto beginR() {
		return part->beginR();
	}
	
	auto beginL() {
		return part->beginL();
	}

	auto endL() {
		return part->endL();
	}

	auto endR() {
		return part->endR();
	}
	
	~Composition();
};


#endif // !Composition_H_