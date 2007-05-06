/*****************************************************************************
 * demangle.c++
 *****************************************************************************
 * Unit tests for the demangle header.
 *****************************************************************************/

// Standard includes.
#include <string>
#include <iostream>

// Testing include.
#include <demangle.h++>

// Bring in namespaces.
using namespace std;
using namespace DAC;

/*
 * Run a test.
 */
template <typename T> int test (string const& type) {
  
  // Work area.
  string buffer;
  
  // Output the type being tested.
  cout << "  Testing type " << type << "... ";
  if (demangle<T>(buffer) == type) {
    cout << buffer << " OK!" << endl;
  } else {
    cout << buffer << " **********WARNING**********" << endl;
  }
  
  // All tests passed.
  return 0;
  
}

/*
 * This is it.
 */
int main () {
  
  // Impossible to test all possible types, and it doesn't matter, just make
  // sure it mostly works or at least fails gracefully.
  cout << "Testing built-in types..." << endl;
  if (test<bool          >("bool"          )) { return 1; }
  if (test<char          >("char"          )) { return 1; }
  if (test<signed char   >("signed char"   )) { return 1; }
  if (test<unsigned char >("unsigned char" )) { return 1; }
  if (test<wchar_t       >("wchar_t"       )) { return 1; }
  if (test<short         >("short"         )) { return 1; }
  if (test<unsigned short>("unsigned short")) { return 1; }
  if (test<int           >("int"           )) { return 1; }
  if (test<unsigned int  >("unsigned int"  )) { return 1; }
  if (test<long          >("long"          )) { return 1; }
  if (test<unsigned long >("unsigned long" )) { return 1; }
  if (test<float         >("float"         )) { return 1; }
  if (test<double        >("double"        )) { return 1; }
  if (test<long double   >("long double"   )) { return 1; }
  if (test<void*         >("void*"         )) { return 1; }
  if (test<void const*   >("void const*"   )) { return 1; }
  if (test<std::string   >("std::string"   )) { return 1; }
  
  // All tests passed.
  cout << "OK!" << endl;
  return 0;
  
}

