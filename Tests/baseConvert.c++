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
#include <demangle.h++>
#include <to_string.h++>

// Testing include.
#include "baseConvert.h++"

// Bring in namespaces.
using namespace std;
using namespace DAC;

/*****************************************************************************/
// Declarations.

// Run test suite on a given type.
template <class T> int test ();

// Create edge cases.
template <class T, bool issigned> class Build_Edges;
template <class T> class Build_Edges<T, true > { public: static void op (vector<T>& edges); };
template <class T> class Build_Edges<T, false> { public: static void op (vector<T>& edges); };

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
  
  Build_Edges<T, numeric_limits<T>::is_signed>::op(edges);
  
  // All tests passed.
  return 0;
  
}

/*
 * Create edge cases, signed type.
 */
template <class T> void Build_Edges<T, true>::op (vector<T>& edges) {
  if (numeric_limits<T>::min() < -1) {
    edges.push_back(numeric_limits<T>::min());
    if (numeric_limits<T>::min() < -2) {
      edges.push_back(numeric_limits<T>::min() + 1);
      if (numeric_limits<T>::min() < -3) {
        edges.push_back(numeric_limits<T>::min() >> 1);
        if (numeric_limits<T>::min() >> 1 < -5) {
          edges.push_back((numeric_limits<T>::min() >> 1) + 1);
        }
      }
    }
  }
  edges.push_back(-1);
  edges.push_back( 0);
  edges.push_back( 1);
  if (numeric_limits<T>::max() > 1) {
    if (numeric_limits<T>::max() > 2) {
      if (numeric_limits<T>::max() > 3) {
        if (numeric_limits<T>::max() > 5) {
          edges.push_back((numeric_limits<T>::max() >> 1) - 1);
        }
        edges.push_back(numeric_limits<T>::max() >> 1);
      }
      edges.push_back(numeric_limits<T>::max() - 1);
    }
    edges.push_back(numeric_limits<T>::max());
  }
}

/*
 * Create edge cases, unsigned type.
 */
template <class T> void Build_Edges<T, false>::op (vector<T>& edges) {
  edges.push_back(0);
  edges.push_back(1);
  if (numeric_limits<T>::max() > 1) {
    if (numeric_limits<T>::max() > 2) {
      if (numeric_limits<T>::max() > 3) {
        if (numeric_limits<T>::max() > 5) {
          edges.push_back((numeric_limits<T>::max() >> 1) - 1);
        }
        edges.push_back(numeric_limits<T>::max() >> 1);
      }
      edges.push_back(numeric_limits<T>::max() - 1);
    }
    edges.push_back(numeric_limits<T>::max());
  }
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

