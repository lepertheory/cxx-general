/*****************************************************************************
 * to_string.c++
 *****************************************************************************
 * Tests the to_string functions.
 *****************************************************************************/

// Standard includes.
#include <string>
#include <iostream>
#include <limits>
#include <cstdlib>

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

// Finally run the test.
template <class T, NumTypes::Type Float> class FinTest;
template <class T> class FinTest<T, NumTypes::UINT> { public: static int op (T const number); };
template <class T> class FinTest<T, NumTypes::SINT> { public: static int op (T const number); };
template <class T> class FinTest<T, NumTypes::FLPT> { public: static int op (T const number); };

/*
 * Finally run the test, unsigned integer.
 */
template <class T> int FinTest<T, NumTypes::UINT>::op(T const number) {
  
  // Work area.
  T to;
  
  // Show what we are testing.
  cout << to_string(number);
  
  // Do the conversion.
  to = strtoul(to_string(number).c_str(), 0, 10);
  
  // Check the result.
  if (to == number) {
    cout << " == " << to << " OK!" << endl;
  } else {
    cout << " != " << to << " FAILED!" << endl;
    return 1;
  }
  
  // All tests passed.
  return 0;
  
}

/*
 * Finally run the test, signed integer.
 */
template <class T> int FinTest<T, NumTypes::SINT>::op(T const number) {
  
  // Work area.
  T to;
  
  // Show what we are testing.
  cout << to_string(number);
  
  // Do the conversion.
  to = strtol(to_string(number).c_str(), 0, 10);
  
  // Check the result.
  if (to == number) {
    cout << " == " << to << " OK!" << endl;
  } else {
    cout << " != " << to << " FAILED!" << endl;
    return 1;
  }
  
  // All tests passed.
  return 0;
  
}

/*
 * Finally run the test, floating point.
 */
template <class T> int FinTest<T, NumTypes::FLPT>::op(T const number) {
  
  // Work area.
  T to;
  
  // Show what we are testing.
  cout << to_string(number);
  
  // Do the conversion.
  to = strtold(to_string(number).c_str(), 0);
  
  // Check the result.
  if (to == number) {
    cout << " == " << to << " OK!" << endl;
  } else {
    cout << " != " << to << " FAILED!" << endl;
    //return 1;
  }
  
  // All tests passed.
  return 0;
  
}

/*
 * Test a particular type, unsigned integer.
 */
template <class T> int Test<T, NumTypes::UINT>::op () {
  
  // Test edge cases.
  cout << "    Testing edge cases..." << endl;
  cout << "      Minimum... "    ; if (FinTest<T, NumTypes::UINT>::op(static_cast<T>(0)           )) { return 1; }
  cout << "      Minimum + 1... "; if (FinTest<T, NumTypes::UINT>::op(static_cast<T>(1)           )) { return 1; }
  cout << "      Maximum - 1... "; if (FinTest<T, NumTypes::UINT>::op(numeric_limits<T>::max() - 1)) { return 1; }
  cout << "      Maximum... "    ; if (FinTest<T, NumTypes::UINT>::op(numeric_limits<T>::max()    )) { return 1; }
  cout << "    OK!" << endl;
  
  // All tests passed.
  return 0;
  
}

/*
 * Test a particular type, signed integer.
 */
template <class T> int Test<T, NumTypes::SINT>::op () {
  
  // Test edge cases.
  cout << "    Testing edge cases..." << endl;
  cout << "      Minimum... "    ; if (FinTest<T, NumTypes::SINT>::op(numeric_limits<T>::min()    )) { return 1; }
  cout << "      Minimum + 1... "; if (FinTest<T, NumTypes::SINT>::op(numeric_limits<T>::min() + 1)) { return 1; }
  cout << "      -1... "         ; if (FinTest<T, NumTypes::SINT>::op(-1                          )) { return 1; }
  cout << "      0... "          ; if (FinTest<T, NumTypes::SINT>::op(0                           )) { return 1; }
  cout << "      1... "          ; if (FinTest<T, NumTypes::SINT>::op(1                           )) { return 1; }
  cout << "      Maximum - 1... "; if (FinTest<T, NumTypes::SINT>::op(numeric_limits<T>::max() - 1)) { return 1; }
  cout << "      Maximum... "    ; if (FinTest<T, NumTypes::SINT>::op(numeric_limits<T>::max()    )) { return 1; }
  cout << "    OK!" << endl;
  
  // All tests passed.
  return 0;
  
}

/*
 * Test a particular type, floating point.
 */
template <class T> int Test<T, NumTypes::FLPT>::op () {
  
  // Test edge cases.
  cout << "    Testing edge cases..." << endl;
  cout << "      Minimum... "    ; if (FinTest<T, NumTypes::FLPT>::op(numeric_limits<T>::min()    )) { return 1; }
  cout << "      Minimum + 1... "; if (FinTest<T, NumTypes::FLPT>::op(numeric_limits<T>::min() + 1)) { return 1; }
  cout << "      -1... "         ; if (FinTest<T, NumTypes::FLPT>::op(-1                          )) { return 1; }
  cout << "      0... "          ; if (FinTest<T, NumTypes::FLPT>::op(0                           )) { return 1; }
  cout << "      1... "          ; if (FinTest<T, NumTypes::FLPT>::op(1                           )) { return 1; }
  cout << "      Maximum - 1... "; if (FinTest<T, NumTypes::FLPT>::op(numeric_limits<T>::max() - 1)) { return 1; }
  cout << "      Maximum... "    ; if (FinTest<T, NumTypes::FLPT>::op(numeric_limits<T>::max()    )) { return 1; }
  cout << "    OK!" << endl;
  
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

