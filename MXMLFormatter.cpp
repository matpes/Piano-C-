#include "MXMLFormatter.h"



MXMLFormatter::MXMLFormatter():Formatter()
{
}


void MXMLFormatter::createFile(string x)
{
	x.append(".musicxml");
	string always = "<?xml version=\"1.0\" encoding=\"UTF-8\"\n standalone=\"no\"?>\n<!DOCTYPE score-partwise PUBLIC\n\t\t\"-//Recordare//DTD MusicXML 3.1 Partwise//EN\"\n		\"http://www.musicxml.org/dtds/partwise.dtd\">\n			<score-partwise version=\"3.1\">\n			<part-list>\n			<score-part id = \"Right\"></score-part>\n			<score-part id = \"Left\"></score-part>\n			</part-list>\n";
	string finish = "</score-partwise>";
	int g, d;
	g = compozicija->getDuration()->getNumerator();
	d = compozicija->getDuration()->getDenominator();
	string attributesR = "<attributes>\n			<divisions>2</divisions>\n			<time>\n			<beats>";
	attributesR += ('0' + g);
	attributesR += "</beats>\n			<beat-type>";
	attributesR += ('0' + d);
	attributesR += "</beat-type>\n			</time>\n			<clef>\n			<sign>G</sign>\n			<line>2</line>\n			</clef>\n			</attributes>\n";
	string attributesL = "<attributes>\n			<divisions>2</divisions>\n			<time>\n			<beats>";
	attributesL += ('0' + g);
	attributesL += "</beats>\n			<beat-type>";
	attributesL += ('0' + d);
	attributesL += "</beat-type>\n			</time>\n			<clef>\n			<sign>F</sign>\n			<line>4</line>\n			</clef>\n			</attributes>\n";
	file.open(x);
	bool ok = true;
	file << always;
	Part * p = compozicija->getPart();
	auto RL = p->getRList();
	auto LL = p->getLList();
	file << "<part id=\"Right\">\n";
	bool first = true;
	bool ffirst = true;
	for_each(compozicija->beginR(), compozicija->endR(), [this, &ok, &attributesR, &first, &ffirst](Measure &m) {
		file << "<measure>\n";
		if (ok) {
			file << attributesR;
			ok = false;
		}
		
		for_each(m.begin(), m.end(), [this, &first, &m, &ffirst](MusicSymbol* ms) {
			if (typeid(*ms) == typeid(Chord)) {
				Chord * ccc = dynamic_cast<Chord *>(ms);
				if (ccc->isDivided()) {
					if (!ffirst) {
						ccc->first = false;
						ffirst = true;
					}
					else {
						ffirst = false;
					}

				}
			}
			if (typeid(*ms) == typeid(Note)) {
				file << "<note>\n";
				file << *ms;
				if (ms->isDivided()) {
					if (first) {
						file << "<tie type=\"start\"/>";
						first = false;
					}
					else {
						file << "<tie type=\"end\"/>";
						first = true;
					}
				}
				/*if (ms->isDivided()) {
					if (first) {
						file << "<tie type=\"start\"/>";
						first = false;
					}
					else {
						file << "<tie type=\"end\"/>";
						first = true;
					}
				}*/
				file << "</note>\n";
			}
			else {
				file << *ms;
			}
		
		
		
		});
		file << "</measure>\n";


	});
	
	ok = true;
	file << "	</part>\n";
	file << "<part id=\"Left\">";
	first = ffirst = true;
	for_each(compozicija->beginL(), compozicija->endL(), [this, &ok, &attributesL, &first, &ffirst](Measure &m) {
		file << "<measure>\n";
		if (ok) {
			file << attributesL;
			ok = false;
		}
		
		for_each(m.begin(), m.end(), [this, &first, &ffirst, &m](MusicSymbol* ms) {
			if (typeid(*ms) == typeid(Chord)) {
				Chord * ccc = dynamic_cast<Chord *>(ms);
				if (ccc->isDivided()) {
					if (!ffirst) {
						ccc->first = false;
						ffirst = true;
					}
					else {
						ffirst = false;
					}
				}
			}

			if (typeid(*ms) == typeid(Note)) {
				file << "<note>\n";
				file << *ms;
				if (ms->isDivided()) {
					if (first) {
						file << "<tie type=\"start\"/>";
						first = false;
					}
					else {
						file << "<tie type=\"end\"/>";
						first = true;
					}
				}
				file << "</note>\n";
			}
			else {
				file << *ms;
			}



		});
		file << "</measure>\n";

	});
	file << "	</part>\n";
	file << finish;
	file.close();
}

MXMLFormatter::~MXMLFormatter()
{
}
