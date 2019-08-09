#include "Composition.h"




void Composition::premesti(Composition & c)
{
	part = c.part;
	dur = c.dur;
	c.part = nullptr;
	c.dur = nullptr;
}

void Composition::kopiraj(const Composition & c)
{
	dur = new Duration(c.dur);
	part = c.part;
}

Composition::Composition(Duration d)
{

	dur = new Duration(d);
	part = new Part(dur);
}

void Composition::addMusicSymbol(MusicSymbol * ms)
{
	part->addMusicSymbol(ms);
}

void Composition::finish()
{
	part->finish();
}

Composition::~Composition()
{
	delete part;
}
