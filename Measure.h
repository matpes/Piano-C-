#pragma once
#include <list>
#include <iterator>
#include <algorithm>
#include "MusicSymbol.h"
#include "Note.h"
#include "Pause.h"
#include "Chord.h"
using namespace std;
//TAKT
class Measure
{
protected:
	
	list<MusicSymbol*> *Hand;
	Duration totalMeasureDuration;
	Duration currentMeasureDuration;

public:
	Measure() {};
	Measure(const Measure& m);
	Measure(Duration totalMeasure);

	auto getList()const { return Hand; }

	auto begin() {
		return Hand->begin();
	}

	auto back() {
		return --end();
	}

	list<MusicSymbol*>::iterator end() {
		return Hand->end();
	}
	Duration getTotalMeasure() const {
		return totalMeasureDuration;
	}

	Duration getcurrentMeasure() const {
		return currentMeasureDuration;
	}

	bool isEnoughSpace(Duration d) {
		return (totalMeasureDuration - currentMeasureDuration) >= d;
	}

	void addMusicSymbol(MusicSymbol* ms);

	friend ostream& operator <<(ostream & it, const Measure & me) {
		for (MusicSymbol *mus : *me.Hand) {
			it << *mus;
		}
		return it;
	}
	~Measure();
};