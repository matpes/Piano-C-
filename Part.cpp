#include "Part.h"


Duration Part::zero = Duration(0, 8);
Duration Part::eight = Duration(1, 8);
Duration Part::four = Duration(1, 4);



Part::Part(Duration dur)
{
	duration = dur;
	leftHand = new list<Measure>();
	rightHand = new list<Measure>();
	right = new Measure(dur);
	left = new Measure(dur);
}

void Part::addMusicSymbol(MusicSymbol * ms, int d)
{
	if (right->isEnoughSpace(ms->getDuration()) && left->isEnoughSpace(ms->getDuration())) {
		addSafeMusicSymbol(ms, d);
	}
	else {
		ms->setDuration(eight);
		ms->setDivided(true);
		addSafeMusicSymbol(ms, d);
		addSafeMusicSymbol(ms, d);
	}
}

void Part::addSafeMusicSymbol(MusicSymbol * ms, int da) //METODA U KOJU UBACUJEM AKO SIGURNO IMA MESTA I KOJA MI NAPRAVI NOVI TAKT KADA SE POPUNI
{
	if (typeid(*ms) == typeid(Note)) {
		Note *note = dynamic_cast<Note*>(ms);
		if (note->isRight(da)) {
			fillIfRightMissing();
			right->addMusicSymbol(note);
		}
		else {
			fillIfLeftMissing();
			left->addMusicSymbol(note);
		}
	}
	else {
		fillIfMissing();
		if (typeid(*ms) == typeid(Pause)) {
			left->addMusicSymbol(ms);
			right->addMusicSymbol(ms);
		}
		else {
			Chord *c = dynamic_cast<Chord*>(ms);
			Chord *lc = new Chord(c);
			Chord *rc = new Chord(c);
			for_each(c->begin(), c->end(), [&lc, &rc, da](Note *n) {
				if (n->isRight(da)) {
					rc->addNote(n);
				}
				else {
					lc->addNote(n);
				}
			});
			if (lc->isNotEmpty())left->addMusicSymbol(lc);
			if (rc->isNotEmpty())right->addMusicSymbol(rc);
		}
	}
	if (right->getcurrentMeasure() == duration || left->getcurrentMeasure() == duration) {
		fillIfMissing();
		rightHand->push_back(*right);
		leftHand->push_back(*left);
		right = new Measure(duration);
		left = new Measure(duration);
	}
}

void Part::finish() {
	Duration durr;
	while (right->getcurrentMeasure() < duration) {
		durr = duration - right->getcurrentMeasure();
		if (durr.getDenominator() == 8) {
			right->addMusicSymbol(new Pause(eight));
			durr -= eight;
		}
		while (durr > zero) {
			right->addMusicSymbol(new Pause(four));
			durr -= four;
		}
	}
	while(left->getcurrentMeasure() < duration) {
		 durr = duration - left->getcurrentMeasure();
		if (durr.getDenominator() == 8) {
			left->addMusicSymbol(new Pause(eight));
			durr -= eight;
		}
		while (durr > zero) {
			left->addMusicSymbol(new Pause(four));
			durr -= four;
		}
	}
	rightHand->push_back(*right);
	leftHand->push_back(*left);
	right = nullptr;
	left = nullptr;
}

void Part::fillIfMissing()
{
	fillIfLeftMissing();
	fillIfRightMissing();
}

void Part::fillIfRightMissing()
{
	if (right->getcurrentMeasure() < left->getcurrentMeasure()) {
		Duration durr = left->getcurrentMeasure() - right->getcurrentMeasure();
		if (durr.getDenominator() == 8) {
			right->addMusicSymbol(new Pause(eight));
			durr -= eight;
		}
		while (durr > zero) {
			right->addMusicSymbol(new Pause(four));
			durr -= four;
		}
	}
}

void Part::fillIfLeftMissing()
{
	if (left->getcurrentMeasure() < right->getcurrentMeasure()) {
		Duration durr = right->getcurrentMeasure() - left->getcurrentMeasure();
		if (durr.getDenominator() == 8) {
			left->addMusicSymbol(new Pause(eight));
			durr -= eight;
		}
		while (durr > zero) {
			left->addMusicSymbol(new Pause(four));
			durr -= four;
		}
	}
}

void Part::kopiraj(const Part & p)
{
	for (Measure m : *p.leftHand) {
		Measure me = Measure(m);
		leftHand->push_back(me);
	}
	for (Measure m : *p.rightHand) {
		Measure me = Measure(m);
		rightHand->push_back(me);
	}
	right = new Measure(*p.right);
	left = new Measure(*p.left);
}

Part::~Part()
{
	delete right;
	delete left;
	delete rightHand;
	delete leftHand;

}
