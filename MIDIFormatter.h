#ifndef MIDIForrmmater_H_
#define MIDIForrmater_H_


#include "Formatter.h"
#include "Composition.h"
#include "MidiFile.h"
#include <iostream>
#include <algorithm>
using namespace std;
using namespace smf;
class MIDIFormatter: public Formatter
{
	
public:
	MIDIFormatter();
	void createFile(string x)override;
	virtual ~MIDIFormatter();
};

#endif // !MIDIForrmmater_H_