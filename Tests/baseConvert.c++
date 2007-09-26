/*****************************************************************************
 * baseConvert.c++
 *****************************************************************************
 * Unit tests for the baseConvert() functions.
 *****************************************************************************/

// Standard includes.
#include <string>
#include <iostream>
#include <limits>
#include <vector>

// Internal includes.
#include "demangle.h++"
#include "to_string.h++"
#include "build_edges.h++"

// Testing include.
#include "baseConvert.h++"

// Bring in namespaces.
using namespace std;
using namespace DAC;

/*****************************************************************************/
// Declarations.

// Run test suite on a given type.
template <class T> int test ();

/*****************************************************************************/
// Definitions.

/*
 * Run test suite on a given type.
 */
template <class T> int test () {
  
  {
    string tmpstr;
    cout << "Testing type " << demangle<T>(tmpstr) << "..." << endl;
  }
  
  vector<T> edges;
  
  build_edges(edges);
	cout << "  edges:\n";
	for (typename vector<T>::const_iterator i = edges.begin(); i != edges.end(); ++i) {
		cout << "    " << to_string(*i) << "\n";
	}
  
  // All tests passed.
  return 0;
  
}

/*
 * This is it.
 */
int main () {
  
  // Test types.
  if (test<bool          >()) { return 1; }
  if (test<char          >()) { return 1; }
  if (test<signed char   >()) { return 1; }
  if (test<unsigned char >()) { return 1; }
  if (test<wchar_t       >()) { return 1; }
  if (test<short         >()) { return 1; }
  if (test<unsigned short>()) { return 1; }
  if (test<int           >()) { return 1; }
  if (test<unsigned int  >()) { return 1; }
  if (test<long          >()) { return 1; }
  if (test<unsigned long >()) { return 1; }
  
  // All tests passed.
  return 0;
  
}

