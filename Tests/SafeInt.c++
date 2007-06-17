/*****************************************************************************
 * SafeInt.c++
 *****************************************************************************
 * Unit tests the SafeInt class.
 *****************************************************************************/

// Standard includes.
#include <string>
#include <iostream>
#include <vector>
#include <limits>

// Internal includes.
#include "demangle.h++"

// Testing include.
#include "SafeInt.h++"

// Bring in namespaces.
using namespace std;
using namespace DAC;

/*
 * Test two types.
 */
template <class T, class U> int test2 () {
  
  {
    string temp;
    cout << "  With type " << demangle<U>(temp) << "..." << endl;
  }
  
  
  
  return 0;
  
}

/*
 * Build edge cases.
 */
template <class T> void buildEdges (vector<T>& edges) {
  
  
  
}

/*
 * Test a given type.
 */
template <class T> int test () {
  
  {
    string temp;
    cout << "Testing type " << demangle<T>(temp) << "..." << endl;
  }
  
  // Build edge cases.
  vector<T> edges;
  buildEdges(edges);
  
  // Test type combinations.
  if (test2<T, bool          >()) { return 1; }
  if (test2<T, char          >()) { return 1; }
  if (test2<T, signed char   >()) { return 1; }
  if (test2<T, unsigned char >()) { return 1; }
  if (test2<T, wchar_t       >()) { return 1; }
  if (test2<T, short         >()) { return 1; }
  if (test2<T, unsigned short>()) { return 1; }
  if (test2<T, int           >()) { return 1; }
  if (test2<T, unsigned int  >()) { return 1; }
  if (test2<T, long          >()) { return 1; }
  if (test2<T, unsigned long >()) { return 1; }
  
  // Test SafeIntUtils first.
  //cout << "  Testing SafeIntUtil::SafeCast()..." << endl;
  
  // All tests passed.
  return 0;
  
}

/*
 * Here we go.
 */
int main (int argc, char** argv) {
  
  // Ignore argc and argv.
  if (argc && argv) {}
  
  // Test every possible type.
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

