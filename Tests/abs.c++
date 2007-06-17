/*****************************************************************************
 * abs.c++
 *****************************************************************************
 * Tests the abs.h++ header.
 *****************************************************************************/

// Standard includes.
#include <string>
#include <iostream>

// Internal includes.
#include "demangle.h++"
#include "NumInfo.h++"

// Testing includes.
#include "abs.h++"

// Bring in namespaces.
using namespace std;
using namespace DAC;

// Test.
template <class T, NumTypes::Type> class Test;
template <class T> class Test<T, NumTypes::SINT> { public: static int op (); };
template <class T> class Test<T, NumTypes::UINT> { public: static int op (); };
template <class T> class Test<T, NumTypes::FLPT> { public: static int op (); };

/*
 * Test a signed integer.
 */
template <class T> int Test<T, NumTypes::SINT>::op () {
  
  // All tests passed.
  return 0;
  
}

/*
 * Test an unsigned integer.
 */
template <class T> int Test<T, NumTypes::UINT>::op () {
  
  // All tests passed.
  return 0;
  
}

/*
 * Test a floating-point.
 */
template <class T> int Test<T, NumTypes::FLPT>::op () {
  
  // All tests passed.
  return 0;
  
}

/*
 * Run a test.
 */
template <class T> int test () {
  
  // Tell what we are testing.
  {
    string buffer;
    cout << "Testing " << demangle<T>(buffer) << "..." << endl;
  }
  
  return Test<T, NumType<T>::type>::op();
  
}

/*
 * This is it.
 */
int main () {
  
  // Run tests on all types we can think of.
  if (test<bool          >()) { return 1; }
  if (test<char          >()) { return 1; }
  if (test<signed char   >()) { return 1; }
  if (test<unsigned char >()) { return 1; }
  if (test<short         >()) { return 1; }
  if (test<unsigned short>()) { return 1; }
  if (test<int           >()) { return 1; }
  if (test<unsigned int  >()) { return 1; }
  if (test<long          >()) { return 1; }
  if (test<unsigned long >()) { return 1; }
  if (test<float         >()) { return 1; }
  if (test<double        >()) { return 1; }
  if (test<long double   >()) { return 1; }
  
  // All tests successful.
  return 0;
  
}

