#include "MIDIFormatter.h"



MIDIFormatter::MIDIFormatter():Formatter()
{
}

void MIDIFormatter::createFile(string x)
{
	MidiFile outputFile;
	outputFile.absoluteTicks();
	vector<uchar> midiEvent;
	midiEvent.resize(3);
	int tpq = 48;
	outputFile.setTicksPerQuarterNote(tpq);
	outputFile.addTrack(1);
	int actionTime = 0;
	midiEvent[2] = 64;
	bool div = true;
	bool divv = true;
	for_each(compozicija->beginR(), compozicija->endR(), [&outputFile, &midiEvent, &actionTime, &tpq, &div, &divv](Measure &m) {
		
		for_each(m.begin(), m.end(), [&m, &outputFile, &midiEvent, &actionTime, &tpq, &div, &divv](MusicSymbol* ms) {
			midiEvent[0] = 0x90;
			if (typeid(*ms) == typeid(Pause)) {
				actionTime += tpq / 2 * (8 / ms->getDuration().getDenominator());
			}
			if (typeid(*ms) == typeid(Note))
			{
				Note *n = static_cast<Note*>(ms);
				if (n->isDivided()) {
					if (div) {
						midiEvent[1] = n->getNumSym();
						outputFile.addEvent(0, actionTime, midiEvent);
						div = false;
					}
					else {
						actionTime += tpq;
						midiEvent[0] = 0x80;
						outputFile.addEvent(0, actionTime, midiEvent);
						div = true;
					}
				}
				else {
					midiEvent[1] = n->getNumSym();
					outputFile.addEvent(0, actionTime, midiEvent);
					actionTime += tpq / 2 * (8 / n->getDuration().getDenominator());
					midiEvent[0] = 0x80;
					outputFile.addEvent(0, actionTime, midiEvent);
				}
			}
			if (typeid(*ms) == typeid(Chord)) {
				Chord * c = static_cast<Chord*>(ms);

				if (c->isDivided()) {
					if (m.getTotalMeasure().getDenominator() == 4) {
						if (*m.begin() == ms) {
							c->first = false;
						}
					}
					else {
						if (divv) {
							divv = false;
						}
						else {
							divv = true;
							c->first = false;
						}
					}
					if (c->first) {
						for (Note * n : *c) {
							midiEvent[1] = n->getNumSym();
							outputFile.addEvent(0, actionTime, midiEvent);
						}
						div = false;//?
					}
					else {
						actionTime += tpq;
						midiEvent[0] = 0x80;
						for (Note * n : *c) {
							outputFile.addEvent(0, actionTime, midiEvent);
						}
						div = true;//?
					}
				}
				else {
					for (Note * n : *c) {
						midiEvent[1] = n->getNumSym();
						outputFile.addEvent(0, actionTime, midiEvent);
					}

					actionTime += tpq / 2 * (8 / c->getDuration().getDenominator());
					midiEvent[0] = 0x80;
					for (Note * n : *c) {
						outputFile.addEvent(0, actionTime, midiEvent);
					}
				}
			}
		});
	});
	div = divv = true;
	actionTime = 0;
	for_each(compozicija->beginL(), compozicija->endL(), [&divv, &outputFile, &midiEvent, &actionTime, &tpq, &div](Measure &m) {
		
		for_each(m.begin(), m.end(), [&divv, &m, &outputFile, &midiEvent, &actionTime, &tpq, &div](MusicSymbol* ms) {
			midiEvent[0] = 0x90;
			if (typeid(*ms) == typeid(Pause)) {
				actionTime += tpq / 2 * (8 / ms->getDuration().getDenominator());
			}
			if (typeid(*ms) == typeid(Note))
			{
				Note *n = static_cast<Note*>(ms);
				if (n->isDivided()) {
					if (div) {
						midiEvent[1] = n->getNumSym();
						outputFile.addEvent(1, actionTime, midiEvent);
						div = false;
					}
					else {
						actionTime += tpq;
						midiEvent[0] = 0x80;
						outputFile.addEvent(1, actionTime, midiEvent);
						div = true;
					}
				}
				else {
					midiEvent[1] = n->getNumSym();
					outputFile.addEvent(0, actionTime, midiEvent);
					actionTime += tpq / 2 * (8 / n->getDuration().getDenominator());
					midiEvent[0] = 0x80;
					outputFile.addEvent(0, actionTime, midiEvent);
				}
			}
			if (typeid(*ms) == typeid(Chord)) {
				Chord * c = static_cast<Chord*>(ms);
				
				if (c->isDivided()) {
					if (m.getTotalMeasure().getDenominator() == 4) {
						if (*m.begin() == ms) {
							c->first = false;
						}
					}
					else {
						if (divv) {
							divv = false;
						}
						else {
							divv = true;
							c->first = false;
						}
					}
					if (c->first) {
						for (Note * n : *c) {
							midiEvent[1] = n->getNumSym();
							outputFile.addEvent(1, actionTime, midiEvent);
						}
						div = false;//?
					}
					else {
						actionTime += tpq;
						midiEvent[0] = 0x80;
						for (Note * n : *c) {
							outputFile.addEvent(1, actionTime, midiEvent);
						}
						div = true;//?
					}
				}
				else {
					for (Note * n : *c) {
						midiEvent[1] = n->getNumSym();
						outputFile.addEvent(1, actionTime, midiEvent);
					}

					actionTime += tpq / 2 * (8 / c->getDuration().getDenominator());
					midiEvent[0] = 0x80;
					for (Note * n : *c) {
						outputFile.addEvent(1, actionTime, midiEvent);
					}
				}
			}
		});
	});
	outputFile.sortTracks();
	outputFile.write(x + ".mid");

}


MIDIFormatter::~MIDIFormatter()
{
}
