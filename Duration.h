#pragma once
#include <iostream>
#include <regex>
#include "Errorsi.h"
using namespace std;
class Duration
{
private:
	int denominator, numerator; // numerator:denuminator
	
public:
	Duration() = default;
	Duration(const Duration & d) {
		numerator = d.numerator;
		denominator = d.denominator;
	}
	Duration(Duration *d) {
		numerator = d->numerator;
		denominator = d->denominator;
	}

	Duration(int numeratorr, int denominatorr ) : denominator(denominatorr), numerator(numeratorr) {
		if (denominatorr != 4 && denominator != 8) throw WrongDuration();
	}
	
	Duration(double d);

	int getDenominator()const {
		return denominator;
	}

	int getNumerator()const {
		return numerator;
	}

	bool operator < (const Duration & d) const {
		return ((numerator +0.0)/ denominator) < ((d.numerator+0.0) / d.denominator);
	}
	

	bool operator > (const Duration & d) const {
		return ((numerator + 0.0) / denominator) > ((d.numerator + 0.0) / d.denominator);
	}

	bool operator == (const Duration & d) const {
		return ((numerator == d.numerator && denominator == d.denominator)|| (numerator == 2*d.numerator && denominator == 2*d.denominator));
		
	}

	bool operator!= (const Duration & d) const {
		return ((*this) < d || (*this) > d);
	}

	bool operator >= (const Duration & d) const {
		return !((*this) < d);
	}

	bool operator <= (const Duration & d) const {
		return !((*this) > d);
	}
	operator double() const {
		return (numerator + 0.0) / denominator;
	}
	Duration operator - (const Duration & d)const {
		return Duration(abs(((numerator + 0.0) / denominator) - ((d.numerator + 0.0) / d.denominator)));
	}

	Duration operator + (const Duration & d)const {
		return Duration(((numerator + 0.0) / denominator) + ((d.numerator + 0.0) / d.denominator));
	}

	Duration& operator +=(const Duration& d) {
		*this = *this + d;
		return *this;
	}

	Duration& operator -=(Duration d) {
		*this = *this - d;
		return *this;
	}

	friend ostream& operator << (ostream& it, const Duration & d) {
		return it << d.numerator << "/" << d.denominator;
	}

	friend istream& operator >> (istream& ut, Duration& d) {
		string x;
		ut >> x;
		regex rx ("([0-9]+)([^0-9]+)([0-9]+)");
		smatch result;
		regex_match(x, result, rx);
		d = Duration(stoi(result.str(1)), stoi(result.str(3)));
		return ut;
	}

	~Duration();
};