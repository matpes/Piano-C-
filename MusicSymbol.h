#pragma once
#include "Duration.h"
#include <fstream>
#include <iomanip>
class MusicSymbol {
protected:
	bool Divided = false;
	Duration du;
	virtual void write(ostream& it)=0;
	virtual void writeFormatt(ofstream& it) = 0;
public:
	MusicSymbol(Duration dur):du(dur){ }
	virtual void setOctave(int o) = 0;
	virtual void setSign (int c) = 0;
	virtual void setSharp() { }
	MusicSymbol(const MusicSymbol& m) {
		Divided = m.Divided;
		du = m.du;
	}
	MusicSymbol& operator =(const MusicSymbol& m) {
		if (this != &m) {
			Divided = m.Divided;
			du = m.du;
		}
		return *this;
	}

	Duration getDuration()const{
		return du;
	}

	virtual void setDuration(Duration d) {
		this->du = d;
	}

	bool isDivided()const {
		return Divided;
	}

	virtual void setDivided(bool s) {
		Divided = s;
	}

	friend ostream& operator <<(ostream & it, MusicSymbol & ms) {
		ms.write(it);
		return it;
	}

	friend ofstream& operator << (ofstream & it, MusicSymbol & ms) {
		ms.writeFormatt(it);
		return it;
	}

	virtual ~MusicSymbol();
};