#include <iostream>
#include <string>
#include <fstream>
#include <regex>
#include <map>
#include "Composition.h"
#include "Errorsi.h"
#include "MXMLFormatter.h"
#include "MIDIFormatter.h"
#include "BMPFormatter.h"
using namespace std;

void shiftNotes(int shift, Composition & comp) {
	Part * p = comp.getPart();
	auto RL = p->getRList();
	auto LL = p->getLList();
	for (Measure m : *RL) {
		for (MusicSymbol * ms : *m.getList()) {
			ms->setOctave(shift);
		}
	}
	
	for (Measure m : *LL) {
		for (MusicSymbol * ms : *m.getList()) {
			ms->setOctave(shift);
		}
	}
}

void createComposition(map<char, pair<string, int>>& map, ifstream & dat, Composition & comp, int shift ) {
	string line = "";
	smatch res;
	regex rx("(.)(#?)(.)");
	char c;
	Chord *chord = nullptr;
	while (getline(dat, line)) {
		int i = 0;
		while (i < (signed)line.size()) {
			c = line[i];
			if (c == '[') {
				regex rx1("[^\\[]*\\[([^\\]]*)\\](.*)");
				smatch res1;
				regex_match(line, res1, rx1);
				string s = res1.str(1);
				line = res1.str(2);
				i = 0;
				if (s.length() == 1 || s[1] == ' ') {//TO SU OSMINE
					
						regex rx2("([^ ]+) ?");
						sregex_iterator begin(s.begin(), s.end(), rx2);
						sregex_iterator end;

						while (begin != end) {
							smatch sm = *begin;
							char t = sm.str(1)[0];
							regex_match(map[t].first, res, rx);
							comp.addMusicSymbol(new Note(Duration(1, 8), stoi(res.str(3)) + shift, map[t].second, res.str(1)[0], res.str(2) == "#"));
							begin++;
						}
				}
				else {
					chord = new Chord(Duration(1, 4));
					for (char c : s) {
						if (c != ' ') {
							regex_match(map[c].first, res, rx);
							chord->addNote(new Note(Duration(1, 4), stoi(res.str(3)) + shift, map[c].second, res.str(1)[0], res.str(2) == "#"));
						}																					//VLOODON OVDE IMA =="#"
					}
					comp.addMusicSymbol(chord);
				}
			}
			else {
				i++;
				if (c == ' ') {
					comp.addMusicSymbol(new Pause(Duration(1, 8)));
				}
				else {
					if (c == '|') {
						comp.addMusicSymbol(new Pause(Duration(1, 4)));
					}
					else {
						regex_match(map[c].first, res, rx);
						comp.addMusicSymbol(new Note(Duration(1, 4), stoi(res.str(3)) + shift, map[c].second, res.str(1)[0], res.str(2) == "#"));
					}
				}
			}
		}
	}
}

Composition * loadFromFile(string X, Duration d, int shift, map<char, pair<string, int>>& m) {
	Composition *comp = new Composition(d);
	ifstream dat;
	string line = "";
	dat.open(X);
	while (!dat.is_open()) {
		cout << "That file does not exist. Please insert new name!\n";
		cin >> X;
		X += ".txt";
		dat.open(X);
		//throw FileNotOpened();
	}
	createComposition(m, dat, *comp, shift);
	dat.close();
	comp->finish();
	return comp;
}

map<char, pair<string, int>> loadHashMap(string x) {
	ifstream dat;
	string line = "";
	dat.open(x);
	if (!dat.is_open()) {
		cout << "Excel nije uspesno otvooren!\n";
		throw FileNotOpened();
	}
	map<char, pair<string, int>> mapa;
	regex rx("(.),(.*),([0-9]*).*");
	smatch res;
	while (getline(dat, line)) {
		regex_match(line, res, rx);
		char c = res.str(1)[0];
		string s = res.str(2);
		int midi = stoi(res.str(3));
		mapa[c]=pair<string, int>(s, midi);
	}
	dat.close();
	return mapa;
}
int main(int argc, char* argv[]) {

	map<char, pair<string, int>> map;
	const string Meni = "\n1. Ucitavanje podataka\n2. Osnovna manipulacija nad kompozicijom\n3. Eksportovaje kompozicije\n0. Kraj rada\n\n";
	const string Unestie = "Molimo Vas unesite zeljenu opciju.\n";
	const string MeniUcitavanje = "\t1. Ucitavanje podataka o kompoziciji\n\t2. Ucitavanje podataka o notama\n";
	const string MeniManipulacija = "\t1. Prikaz podataka o kompoziciji\n\t2. Iteriranje kroz kompoziciju uz mogucnost izmene muzickih simbola\n\t3. Promena takta kompozicije i pomeranje kompozicije kroz oktave\n";
	const string MeniEksportovanje = "\tTip fromata?\n\t\t1. MIDI fomrat\n\t\t2. MUSICXML fromat\n\t\t3. BMP fromat\n";
	const string promenaTaktaIPomeranjeKomp = "\t\t1. Promena takta kompozicije\n\t\t2. Pomeranje kompozicije\n";
	const string IteriranjeKrozKompoziciju = "\t\t1. Prelazak na sledeci takt\n\t\t2. Povratak na prethodni takt\n\t\t3. Zapocinjanje iteracije kroz note takta\n";
	const string IteriranjeKrozTakt = "\t\t\t11/12. Prelazak na sledecu notu\n\t\t\t21/22. Povratak na prethodnu notu\n\t\t\t3. Menjanje oktave ili visine note \n\t\t\t4. Dodavanje ili uklanjanje povisilice \n";
	const string MenjOktOrHeighNote = "\t\t\t\t1. Menjanje oktave note\n\t\t\t\t2. Menjanje visine note\n";
	string x; //Naziv datoteke iz koje se ucitava
	string compos;
	//string n1, n2;
	regex *rx = nullptr;
	smatch *result = new smatch();
	//smatch res;
	int option;
	int shift = 0;
	int i;
	Composition *composition = nullptr;
	Duration duration = Duration(0,8);
	bool compositionLoaded = false;
	bool measureLoaded = false;
	bool somethingChanged = true;
	int rbtd, rbtl, rbnd, rbnl;
	rbtd = rbtl = rbnd = rbnl = 1;
	list<Measure>::iterator iter1;
	list<Measure>::iterator iter2;
	list<MusicSymbol*>::iterator noteIter1;
	list<MusicSymbol*>::iterator noteIter2;
	MXMLFormatter MXMLformater;
	MIDIFormatter MIDIformater;
	BMPFormatter BMPformater;
	//BMPFormatter BMPformater;
	bool swich = true;
	char yes;
	cout << Meni;
	try {
		do {
			cout << Unestie;
			cin >> option;

			switch (option) {
			case 0: //KRAJ RADA
				if (somethingChanged) {
					cout << "Do you want to save changed changed composition? [y/n] ";
					cin >> yes;
					if (yes == 'y') {
						cout << "Which format do you want? ";
						cin >> yes;
						if (yes == '1') {
							MIDIformater.addComp(composition);
							MIDIformater.createFile(x);
							cout << "File was created. You can find it in the source directory.\n";
						}
						else {
							if (yes == '2') {
								MXMLformater.addComp(composition);
								MXMLformater.createFile(x);
								cout << "File was created. You can find it in the source directory.\n";
							}
							else {
								BMPformater.addComp(composition);
								BMPformater.createFile(x);
								cout << "File was created. You can find it in the source directory.\n";
							}
						}
						
					}
				}
				cout << "Prijatan ostatak dana!!!!!!!!!!!!!!!!! OK\n";
				delete composition;
				delete rx;
				delete result;
				break;
			case 1: //UCITAVANJE KOMPOZICIJE +
				cout << MeniUcitavanje;
				cin >> option;
				switch (option) {
				case 1: default:
					if (!measureLoaded) {
						cout << "Please load note details!\n";
						break;
					}
					if (compositionLoaded) {
						cout << "Are you sure you want to reload the compostion? [y/n] ";
						char c;
						cin >> c;
						if (c == 'n')break;
						delete composition;
						//NASTAVI DOLE, DA UCITA KOMPOZICIJU
					}
					cout << "Insert measure length. ";
					//if(duration) delete duration;
					cin >> duration;
					
					compositionLoaded = true;
					cout << "State the name of composition's file.\n";
					cin >> compos;
					//compos = "fur_elise.txt";
					shift = 0;
					composition = loadFromFile(compos + ".txt", duration, shift, map);
					break;
				case 2: //UCITAVANJE PODATAKA O NOTAMA +
					measureLoaded = true;
					map = loadHashMap("map.csv");
					
					break;
				}
				break;
			case 2: //MANIPULACIJA KOMPOZICIJE
				if (!compositionLoaded) {
					cout << "Please load composition first!\n";
					break;
				}
				cout << MeniManipulacija;
				cin >> option;
				switch (option) {
				case 1:
					cout << *composition;
					break;
				case 2:
					somethingChanged = true;
					cout << IteriranjeKrozKompoziciju;
					iter1 = composition->beginR();
					iter2 = composition->beginL();
					cout << rbtd << " " << *iter1 << endl;
					cout << rbtl << " " << *iter2 << endl;
					while (option) {
						cin >> option;
						switch (option) {
						case 1:
							if (++iter1 != composition->endR()) {
								++iter2;
								++rbtd;
								++rbtl;
								cout << rbtd << " " << *iter1 << endl;
								cout << rbtl << " " << *iter2 << endl;
							}
							else {
								--iter1;
								cout << "You have reached the end of the composition!\n";
							}
							break;

						case 2:
							if (iter1 != composition->beginR()) {
								--iter1;
								--iter2;
								--rbtl;
								--rbtd;
								cout << rbtd << " " << *iter1 << endl;
								cout << rbtl << " " << *iter2 << endl;
							}
							else {
								cout << "You are at the start of the composition!\n";
							}
							break;
						case 3:
							cout << IteriranjeKrozTakt;
							rbnd = rbnl = 1;
							noteIter1 = iter1->begin();
							noteIter2 = iter2->begin();
							cout << rbnd << " " << **noteIter1 << endl;
							cout << rbnl << " " << **noteIter2 << endl;
							while (option) {
								cin >> option;
								switch (option) {
								case 11:
									//PRELAZAK NA SLEDECU NOTU
									swich = true;
									if (++noteIter1 != iter1->end()) {
										++rbnd;
										cout << rbnd << " " << **noteIter1 << endl;
									}
									else {
										--noteIter1;
										cout << "You have reached the end of the measure!\n";
									}
									break;
								case 21:
									//POVRATAK NA PRETHODNU NOTU
									swich = true;
									if (noteIter1 != iter1->begin()) {
									--noteIter1;
									--rbnd;
									cout << rbnd << " " << **noteIter1 << endl;
									}
									else {
										cout << "You are at the start of the measure!\n";
									}
									break;
								case 12:
									//PRELAZAK NA SLEDECU NOTU
									swich = false;
									if (++noteIter2 != iter2->end()) {
										++rbnl;
										cout << rbnl << " " << **noteIter2 << endl;
									}
									else {
										--noteIter2;
										cout << "You have reached the end of the measure!\n";
									}
									break;
								case 22:
									//POVRATAK NA PRETHODNU NOTU
									swich = false;
									if (noteIter2 != iter2->begin()) {
									--noteIter2;
									--rbnl;
									cout << rbnl << " " << **noteIter2 << endl;
									}
									else {
										cout << "You are at the start of the measure!\n";
									}
									break;
								case 3:
									cout << MenjOktOrHeighNote;
									cin >> option;
									switch (option) {
									case 1:
										cin >> i;
										if (swich) {
											(*noteIter1)->setOctave(i);
										}
										else {
											(*noteIter2)->setOctave(i);
										}
										break;
									case 2:
										cin >> i;
										if (swich) {
											(*noteIter1)->setSign(i);
										}
										else {
											(*noteIter2)->setSign(i);
										}
										break;

									}
									break;
								case 4:
									if (swich) {
										(*noteIter1)->setSharp();
									}
									else {
										(*noteIter2)->setSharp();
									}
									break;
								}
							}
							break;
						}
					}
					option = 5;
					break;
				case 3:
					somethingChanged = true;
					// PROMENA TAKTA ILI PROMENA OKTAVE SVAKE NOTE
					cout << promenaTaktaIPomeranjeKomp;
					cin >> option;
					switch (option) {
					case 1:
						cout << "Unesite duzinu novog takta. ";
						cin >> duration;
						break;
					case 2:
						cout << "Za koliko zelite da pomerite oktavu svake note? ";
						cin >> shift;
						/*
						shiftNotes (shift);
						
						
						*/ 
						
						break;
					}
					delete composition;
					composition = loadFromFile(compos + ".txt", duration, shift, map);
					break;
				}

				break;
			case 3: //EKSPORTOVANJE
				if (!compositionLoaded) {
					cout << "Please load composition first!\n";
					break;
				}
				cout << MeniEksportovanje;
				cin >> option;
				x = compos;
				x += '0' + duration.getNumerator();
				x += '_';
				x += '0' + duration.getDenominator();
				somethingChanged = false;
				switch (option) {
				case 2:
					MXMLformater.addComp(composition);
					MXMLformater.createFile(x);
					cout << "File was created. You can find it in the source directory.\n";
					break;
				case 1:
					MIDIformater.addComp(composition);
					MIDIformater.createFile(x);
					cout << "File was created. You can find it in the source directory.\n";

					break;
				case 3:
					BMPformater.addComp(composition);
					BMPformater.createFile(x);
					cout << "File was created. You can find it in the source directory.\n";
					break;
				}
				break;
			}


		} while (option);
	}
	catch (exception e) {
		cout << e.what();
	}
	//delete composition;
	system("PAUSE");
}