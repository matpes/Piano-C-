#pragma once
#include "MusicSymbol.h"
#include <string>
#include <map>
class Note : public MusicSymbol
{
private:
	int octave;
	int numSym;
	char hight;
	bool sharp;
protected:
	void write(ostream & it)override;
	void writeFormatt(ofstream & it)override;
public:
	Note(Duration dur, int octave, int numSym, char height, bool sharp=false);
	Note(Note *n):MusicSymbol(n->du) {
		octave = n->octave;
		numSym = n->numSym;
		hight = n->hight;
		sharp = n->sharp;
	}
	Note(const Note & n) : MusicSymbol(n.du){
		octave = n.octave;
		numSym = n.numSym;
		hight = n.hight;
		sharp = n.sharp;
	}
	Note& operator = (const Note & n){
		if (this != &n) {
			du = n.du;
			octave = n.octave;
			numSym = n.numSym;
			hight = n.hight;
			sharp = n.sharp;
		}
		return *this;
	}
	static void det_color(Note *n, uint8_t &R, uint8_t &G, uint8_t &B);

	bool isRight(int d = 0) {
		return octave > 3 + d;
	}
	bool isLeft(int d = 0) {
		return octave < 4 + d;
	}
	virtual void setOctave(int o) {
		if (octave + o >= 6) {
			octave = 6;
		}
		else {
			if (octave + o <= 2) {
				octave = 2;
			}
			else {
				octave = o;
			}
		}
	}
	virtual void setSign(int c) {

		if (c == 0)return;
		if (c > 0) {
			if (hight == 'A' || hight == 'B') { hight = 'B'; return; }
			if (hight + c <= 'G') { hight += c; return; }
			else {
				if (hight + c == 'H') { hight = 'A'; return; }
				else { hight = 'B'; return; }
			}
		}
		else {
			while (c < 0) {
				if (hight != 'C') {
					if (hight == 'A') {
						hight = 'G';
					}
					else {
						hight -= 1;
					}
				}
				c++;
			}
		}
	}
	int getNumSym()const { return numSym; }
	int Oct() const { return octave; }
	char Hight() const { return hight; }
	bool isSharp() const { return sharp; }
	void setSharp()override {
		sharp = !sharp;
	}
	virtual ~Note();
};

