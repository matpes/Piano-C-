#pragma once
#include "MusicSymbol.h"
#include "Note.h"
#include <iterator>
#include <list>
class Chord :
	public MusicSymbol
{
	list<Note*> *chord;
protected:
	void write(ostream& it)override;
	void writeFormatt(ofstream & it)override;
public:
	bool first = true;
	Chord(Duration dur);
	Chord(Chord *c);
	Chord(const Chord& c);
	virtual void setOctave(int o);
	virtual void setSign(int c);
	Chord& operator = (const Chord&c);
	Chord& operator = (Chord&&c);
	Chord(Chord&& c):MusicSymbol(c.du) {
		chord = c.chord;
		c.chord = nullptr;
	}
	void setDuration(Duration d) override;
	void setDivided(bool s) override;
	void setSharp() override;
	bool isNotEmpty()const {
		return chord->size();
	}
	auto getChord()const {
		return chord;
	}
	auto begin() {
		return chord->begin();
	}
	auto back() {
		return --end();
	}

	list<Note *>::iterator end() {
		return chord->end();
	}

	void addNote(Note *n) {
		chord->push_back(n);
	}
	virtual ~Chord();
};

