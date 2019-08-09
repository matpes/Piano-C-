#pragma once
#include <exception>
using namespace std;
class OctaveOutOfRange : public exception {
	const char * what()const override
	{
		return "You have entered wrong value for octave. Octave must be between 2 and 6\n";
	}
};

class HeightOutOfRange : public exception {
	const char * what() const override {
		return "You have entered wrong value for height. Height must be between A and G\n";
	}
};

class FileNotOpened : public exception {
	const char * what() const override {
		return "File wasn't opened successfully!\n";
	}
};

class WrongDuration : public exception {
	const char * what() const override {
		return "Denominator in duration must be 4 or 8\n";
	}
};

/*
class  : public exception {
	const char * what() const override {
		return "";
	}
};
*/