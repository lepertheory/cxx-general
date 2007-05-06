/*****************************************************************************
 * to_string.c++
 *****************************************************************************
 * Tests the to_string functions.
 *****************************************************************************/

// Standard includes.
#include <string>
#include <iostream>

// Internal includes.
#include "demangle.h++"

// Testing include.
#include "to_string.h++"
#include "NumInfo.h++"

// Bring in namespaces.
using namespace std;
using namespace DAC;

// Test a particular type, different tests for float vs int.
template <class T, NumTypes::Type Float> class Test;
template <class T> class Test<T, NumTypes::UINT> { public: static int op (); };
template <class T> class Test<T, NumTypes::SINT> { public: static int op (); };
template <class T> class Test<T, NumTypes::FLPT> { public: static int op (); };

/*
 * Test a particular type, unsigned integer.
 */
template <class T> int Test<T, NumTypes::UINT>::op () {
  
  // Test edge cases.
  cout << "    Testing edge cases..." << endl;
  if (
  
  // All tests passed.
  return 0;
  
}

/*
 * Test a particular type, signed integer.
 */
template <class T> int Test<T, NumTypes::SINT>::op () {
  
  // All tests passed.
  return 0;
  
}

/*
 * Test a particular type, floating point.
 */
template <class T> int Test<T, NumTypes::FLPT>::op () {
  
  // All tests passed.
  return 0;
  
}

/*
 * Test a particular type.
 */
template <class T> int test () {
  
  // Show the type we are testing.
  {
    string buffer;
    cout << "  Testing type " << demangle<T>(buffer) << "..." << endl;
  }
  
  // Break out into specific tests.
  if (Test<T, NumType<T>::type>::op()) {
    cout << "FAILED!" << endl;
    return 1;
  }
  
  // All tests passed.
  cout << "  OK!" << endl;
  return 0;
  
}

/*
 * Main.
 */
int main () {
  
  // Test to_string().
  cout << "Testing to_string()..." << endl;
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
  if (test<float         >()) { return 1; }
  if (test<double        >()) { return 1; }
  if (test<long double   >()) { return 1; }
  
  // All tests passed.
  cout << "OK!" << endl;
  return 0;
  
}

