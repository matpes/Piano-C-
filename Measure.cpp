#include "Measure.h"
Measure::Measure(const Measure & m)
{
	Hand = new list<MusicSymbol *>();
	for (MusicSymbol*m : *m.Hand) {
		Hand->push_back(m);
	}



}
Measure::Measure(Duration totalMeasure):totalMeasureDuration(totalMeasure), currentMeasureDuration(Duration(0, 8))
{
	Hand = new list<MusicSymbol*>();
}

void Measure::addMusicSymbol(MusicSymbol * ms)
{
	Hand->push_back(ms);
	currentMeasureDuration += ms->getDuration();
}

Measure::~Measure()
{
	Hand->clear();
}