/*****************************************************************************
 * longDiv.c++
 *****************************************************************************
 * Small utility that gives access to the long division function.
 *****************************************************************************/

// Standard includes.
#include <string>
#include <iostream>
#include <vector>

// Internal includes.
#include "longDiv.h++"
#include "to_string.h++"
#include "SafeInt.h++"

// Bring in namespaces.
using namespace std;
using namespace DAC;

/*****************************************************************************/
// Types.

typedef char DivType;

/*****************************************************************************/
// Functions.

// Program entry.
int main ();

/*****************************************************************************
 * Definitions.
 *****************************************************************************/

/*
 * Program entry.
 */
int main () {
	
	DivType         base     ;
	vector<DivType> dividend ;
	DivType         digit    ;
	DivType         divisor  ;
	vector<DivType> quotient ;
	DivType         remainder;
	SafeInt<int>    tmpin    ;
	
	cout << "Base:      "; cin >> tmpin; base = tmpin;
	cout << "Dividend:  ";
	while (cin >> tmpin && tmpin) {
		digit = tmpin;
		dividend.push_back(digit);
	}
	cout << "Divisor:   "; cin >> tmpin; divisor = tmpin;
	
	remainder = longDiv(quotient, dividend, divisor, base);
	cout << "Result:   ";
	for (vector<DivType>::const_iterator i = quotient.begin(); i != quotient.end(); ++i) {
		cout << " " << to_string(*i);
	}
	cout << "\n";
	cout << "Remainder: " << to_string(remainder) << "\n";
	
	return 0;
	
}

