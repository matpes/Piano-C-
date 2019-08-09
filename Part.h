#pragma once
#include "Measure.h"

using namespace std;
class Part
{private:
	static Duration zero;
	static Duration eight;
	static Duration four;
	Duration duration;
	//RED - lista taktova
	list<Measure> *leftHand;
	list<Measure> *rightHand;
	Measure *right, *left;
	void fillIfMissing();
	void fillIfRightMissing();
	void fillIfLeftMissing();
	void kopiraj(const Part&p);
public:
	Part(Duration dur);
	Part(const Part& p) {
		kopiraj(p);
	}
	Part(Part && p) {
		leftHand = p.leftHand;
		rightHand = p.rightHand;
		right = p.right;
		left = p.left;
		p.left = p.right = nullptr;
		p.leftHand = p.rightHand = nullptr;
	}
	Part& operator = (const Part & p) {
		if (this != &p) {
			kopiraj(p);
		}
		return *this;
	}
	Part& operator = (Part && p) {
		if (this != &p) {
			leftHand = p.leftHand;
			rightHand = p.rightHand;
			right = p.right;
			left = p.left;
			p.left = p.right = nullptr;
			p.leftHand = p.rightHand = nullptr;
		}
		return *this;
	}
	auto getLList()const { return leftHand; }
	auto getRList()const { return rightHand; }
	auto beginL() {
		return leftHand->begin();
	}

	auto beginR() {
		return rightHand->begin();
	}

	auto endL() {
		return leftHand->end();
	}

	auto endR() {
		return rightHand->end();
	}
	void addSafeMusicSymbol(MusicSymbol * ms, int d = 0);
	void finish();
	void addMusicSymbol(MusicSymbol * ms, int d = 0);
	friend ostream & operator << (ostream & it, Part & p) {
		list<Measure>::iterator ml = p.beginL();
		list<Measure>::iterator mr = p.beginR();
		int i, j;
		j = 10;
		while (ml != p.endL()) {
			i = j;
			while (i > 0 && mr != p.endR()) {
				it << *mr << "|";
				++mr;
				--i;
			}
			it << endl;
			i = j;
			while (i > 0 && ml != p.endL()) {
				it << *ml << "|";
				++ml;
				--i;
			}
			it << endl << endl;
		}
		
		it << endl;
		/*for (Measure m : *p.leftHand) {
			it << m << "|";
		}*/
		return it;
	}
	~Part();
};

