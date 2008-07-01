/*****************************************************************************
 * abs.c++
 *****************************************************************************
 * Tests the abs.h++ header.
 *****************************************************************************/

// Standard includes.
#include <string>
#include <iostream>
#include <vector>
#include <limits>
#include <cstdlib>

// Internal includes.
#include "demangle.h++"
#include "NumInfo.h++"
#include "testcommon.h++"
#include "Exception.h++"
#include "to_string.h++"

// Testing includes.
#include "abs.h++"

// Bring in namespaces.
using namespace std;
using namespace DAC;

// Test.
template <class T, NumTypes::Type> class Test;
template <class T> class Test<T, NumTypes::SINT> { public: static int op (); };
template <class T> class Test<T, NumTypes::UINT> { public: static int op (); };

/*
 * Test a signed integer.
 */
template <class T> int Test<T, NumTypes::SINT>::op () {
  
	// Types.
	typedef vector<T> Edges;
	
	// Build edge cases.
	Edges edges;
	build_edges(edges);
	
	// Run tests.
	for (typename Edges::iterator edge = edges.begin(); edge != edges.end(); ++edge) {
		cout << "  " << to_string(*edge) << "... ";
		
		// Overflow is undefined.
		if (*edge == numeric_limits<T>::min()) {
			cout << "Undefined." << endl;
			
		// Should equal the built-in function.
		} else {
			if (*edge != numeric_limits<T>::min()) {
				T testval    = DAC::abs(*edge);
				T controlval = abs(*edge);
				cout << to_string(testval);
				if (testval != controlval) {
					cout << " != " << to_string(controlval) << " Failed!" << endl;
					return 1;
				}
				cout << " == " << to_string(controlval) << " OK!" <<  endl; 
			}
		}
		
	}
	
  // All tests passed.
  return 0;
  
}

/*
 * Test an unsigned integer.
 */
template <class T> int Test<T, NumTypes::UINT>::op () {
  
	// Types.
	typedef vector<T> Edges;
	
	// Build edge cases.
	Edges edges;
	build_edges(edges);
	
	// Run tests.
	for (typename Edges::iterator edge = edges.begin(); edge != edges.end(); ++edge) {
		cout << "  " << to_string(*edge) << "... ";
		
		// This should simply be a no-op.
		T testval = DAC::abs(*edge);
		if (testval != *edge) {
			cout << to_string(*edge) << " != " << to_string(testval) << "... Failed!" << endl;
			return 1;
		}
		cout << to_string(*edge) << " == " << to_string(testval) << "... OK!" << endl;
		
	}
	
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
  
  // All tests successful.
  return 0;
  
}

