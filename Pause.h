#pragma once
#include "MusicSymbol.h"
class Pause :
	public MusicSymbol
{
protected:
	void write(ostream&it)override;
	void writeFormatt(ofstream & it)override;
public:
	Pause(Duration dur);
	Pause (Pause *p):Pause(*p){}
	virtual void setOctave(int o) { }
	virtual void setSign(int c) { }
	virtual ~Pause();
};