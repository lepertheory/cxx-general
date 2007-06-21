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
#include <algorithm>

// Internal includes.
#include "demangle.h++"
#include "NumInfo.h++"
#include "to_string.h++"

// Testing include.
#include "SafeInt.h++"

// Bring in namespaces.
using namespace std;
using namespace DAC;

/*****************************************************************************/
// Types.

/*
 * Test a single type.
 */
template <class T, bool is_signed> class Test1;
template <class T> class Test1<T, false> { public: static int op (); };
template <class T> class Test1<T, true > { public: static int op (); };

/*
 * Test a combination of two types.
 */
template <class T, class U, RelTypes::Type> class Test2;
template <class T, class U> class Test2<T, U, RelTypes::UE_UE> { public: static int op (vector<T> const& edges1, vector<U> const& edges2); };
template <class T, class U> class Test2<T, U, RelTypes::UL_US> { public: static int op (vector<T> const& edges1, vector<U> const& edges2); };
template <class T, class U> class Test2<T, U, RelTypes::US_UL> { public: static int op (vector<T> const& edges1, vector<U> const& edges2); };
template <class T, class U> class Test2<T, U, RelTypes::UE_SE> { public: static int op (vector<T> const& edges1, vector<U> const& edges2); };
template <class T, class U> class Test2<T, U, RelTypes::UL_SS> { public: static int op (vector<T> const& edges1, vector<U> const& edges2); };
template <class T, class U> class Test2<T, U, RelTypes::US_SL> { public: static int op (vector<T> const& edges1, vector<U> const& edges2); };
template <class T, class U> class Test2<T, U, RelTypes::SE_UE> { public: static int op (vector<T> const& edges1, vector<U> const& edges2); };
template <class T, class U> class Test2<T, U, RelTypes::SL_US> { public: static int op (vector<T> const& edges1, vector<U> const& edges2); };
template <class T, class U> class Test2<T, U, RelTypes::SS_UL> { public: static int op (vector<T> const& edges1, vector<U> const& edges2); };
template <class T, class U> class Test2<T, U, RelTypes::SE_SE> { public: static int op (vector<T> const& edges1, vector<U> const& edges2); };
template <class T, class U> class Test2<T, U, RelTypes::SL_SS> { public: static int op (vector<T> const& edges1, vector<U> const& edges2); };
template <class T, class U> class Test2<T, U, RelTypes::SS_SL> { public: static int op (vector<T> const& edges1, vector<U> const& edges2); };

/*
 * Create edge cases, single type.
 */
template <class T, bool is_signed> class Build_Edges1;
template <class T> class Build_Edges1<T, false> { public: static void op (vector<T>& edges); };
template <class T> class Build_Edges1<T, true > { public: static void op (vector<T>& edges); };

/*
 * Create edge cases, two types.
 */
template <class T, class U, RelTypes::Type> class Build_Edges2;
template <class T, class U> class Build_Edges2<T, U, RelTypes::UE_UE> { public: static void op (vector<T>& edges1, vector<U>& edges2); };
template <class T, class U> class Build_Edges2<T, U, RelTypes::UL_US> { public: static void op (vector<T>& edges1, vector<U>& edges2); };
template <class T, class U> class Build_Edges2<T, U, RelTypes::US_UL> { public: static void op (vector<T>& edges1, vector<U>& edges2); };
template <class T, class U> class Build_Edges2<T, U, RelTypes::UE_SE> { public: static void op (vector<T>& edges1, vector<U>& edges2); };
template <class T, class U> class Build_Edges2<T, U, RelTypes::UL_SS> { public: static void op (vector<T>& edges1, vector<U>& edges2); };
template <class T, class U> class Build_Edges2<T, U, RelTypes::US_SL> { public: static void op (vector<T>& edges1, vector<U>& edges2); };
template <class T, class U> class Build_Edges2<T, U, RelTypes::SE_UE> { public: static void op (vector<T>& edges1, vector<U>& edges2); };
template <class T, class U> class Build_Edges2<T, U, RelTypes::SL_US> { public: static void op (vector<T>& edges1, vector<U>& edges2); };
template <class T, class U> class Build_Edges2<T, U, RelTypes::SS_UL> { public: static void op (vector<T>& edges1, vector<U>& edges2); };
template <class T, class U> class Build_Edges2<T, U, RelTypes::SE_SE> { public: static void op (vector<T>& edges1, vector<U>& edges2); };
template <class T, class U> class Build_Edges2<T, U, RelTypes::SL_SS> { public: static void op (vector<T>& edges1, vector<U>& edges2); };
template <class T, class U> class Build_Edges2<T, U, RelTypes::SS_SL> { public: static void op (vector<T>& edges1, vector<U>& edges2); };

/*
 * Test SafeIntUtil::SafeCast().
 */
template <class T, class U, RelTypes::Type> class Test_SafeCast;
template <class T, class U> class Test_SafeCast<T, U, RelTypes::UE_UE> { public: static int op (vector<T> const& edges); };
template <class T, class U> class Test_SafeCast<T, U, RelTypes::UL_US> { public: static int op (vector<T> const& edges); };
template <class T, class U> class Test_SafeCast<T, U, RelTypes::US_UL> { public: static int op (vector<T> const& edges); };
template <class T, class U> class Test_SafeCast<T, U, RelTypes::UE_SE> { public: static int op (vector<T> const& edges); };
template <class T, class U> class Test_SafeCast<T, U, RelTypes::UL_SS> { public: static int op (vector<T> const& edges); };
template <class T, class U> class Test_SafeCast<T, U, RelTypes::US_SL> { public: static int op (vector<T> const& edges); };
template <class T, class U> class Test_SafeCast<T, U, RelTypes::SE_UE> { public: static int op (vector<T> const& edges); };
template <class T, class U> class Test_SafeCast<T, U, RelTypes::SL_US> { public: static int op (vector<T> const& edges); };
template <class T, class U> class Test_SafeCast<T, U, RelTypes::SS_UL> { public: static int op (vector<T> const& edges); };
template <class T, class U> class Test_SafeCast<T, U, RelTypes::SE_SE> { public: static int op (vector<T> const& edges); };
template <class T, class U> class Test_SafeCast<T, U, RelTypes::SL_SS> { public: static int op (vector<T> const& edges); };
template <class T, class U> class Test_SafeCast<T, U, RelTypes::SS_SL> { public: static int op (vector<T> const& edges); };

/*
 * Test SafeIntUtil::RawCast().
 */
template <class T, class U, RelTypes::Type> class Test_RawCast;
template <class T, class U> class Test_RawCast<T, U, RelTypes::UE_UE> { public: static int op (vector<T> const& edges); };
template <class T, class U> class Test_RawCast<T, U, RelTypes::UL_US> { public: static int op (vector<T> const& edges); };
template <class T, class U> class Test_RawCast<T, U, RelTypes::US_UL> { public: static int op (vector<T> const& edges); };
template <class T, class U> class Test_RawCast<T, U, RelTypes::UE_SE> { public: static int op (vector<T> const& edges); };
template <class T, class U> class Test_RawCast<T, U, RelTypes::UL_SS> { public: static int op (vector<T> const& edges); };
template <class T, class U> class Test_RawCast<T, U, RelTypes::US_SL> { public: static int op (vector<T> const& edges); };
template <class T, class U> class Test_RawCast<T, U, RelTypes::SE_UE> { public: static int op (vector<T> const& edges); };
template <class T, class U> class Test_RawCast<T, U, RelTypes::SL_US> { public: static int op (vector<T> const& edges); };
template <class T, class U> class Test_RawCast<T, U, RelTypes::SS_UL> { public: static int op (vector<T> const& edges); };
template <class T, class U> class Test_RawCast<T, U, RelTypes::SE_SE> { public: static int op (vector<T> const& edges); };
template <class T, class U> class Test_RawCast<T, U, RelTypes::SL_SS> { public: static int op (vector<T> const& edges); };
template <class T, class U> class Test_RawCast<T, U, RelTypes::SS_SL> { public: static int op (vector<T> const& edges); };

/*****************************************************************************/
// Functions.

// Test a single type.
template <class T> int test1 ();

// Test a combination of types.
template <class T, class U> int test2 ();

/*****************************************************************************
 * Definitions.
 *****************************************************************************/

/*
 * Create edge cases, single type, unsigned.
 */
template <class T> void Build_Edges1<T, false>::op(vector<T>& edges) {
  edges.push_back(0);
  edges.push_back(1);
  if (numeric_limits<T>::max() > 1) {
    if (numeric_limits<T>::max() > 2) {
      edges.push_back(numeric_limits<T>::max() - 1);
    }
    edges.push_back(numeric_limits<T>::max());
  }
}

/*
 * Create edge cases, single type, signed.
 */
template <class T> void Build_Edges1<T, true>::op(vector<T>& edges) {
  if (numeric_limits<T>::min() < -1) {
    edges.push_back(numeric_limits<T>::min());
    if (numeric_limits<T>::min() < -2) {
      edges.push_back(numeric_limits<T>::min() + 1);
    }
  }
  edges.push_back(-1);
  edges.push_back( 0);
  edges.push_back( 1);
  if (numeric_limits<T>::max() > 1) {
    if (numeric_limits<T>::max() > 2) {
      edges.push_back(numeric_limits<T>::max() - 1);
    }
    edges.push_back(numeric_limits<T>::max());
  }
}

/*
 * Create edge cases, two types, same length both unsigned.
 */
template <class T, class U> void Build_Edges2<T, U, RelTypes::UE_UE>::op(vector<T>& edges1, vector<U>& edges2) {
  size_t oldend = edges1.end() - edges1.begin();
  for (typename vector<U>::const_iterator i = edges2.begin(); i != edges2.end(); ++i) {
    if (find(edges1.begin(), edges1.end(), static_cast<T>(*i)) == edges1.end()) {
      edges1.push_back(*i);
    }
  }
  for (typename vector<T>::const_iterator i = edges1.begin(); i != edges1.begin() + oldend; ++i) {
    if (find(edges2.begin(), edges2.end(), static_cast<U>(*i)) == edges2.end()) {
      edges2.push_back(*i);
    }
  }
}

/*
 * Create edge cases, two types, first longer, both unsigned.
 */
template <class T, class U> void Build_Edges2<T, U, RelTypes::UL_US>::op(vector<T>& edges1, vector<U>& edges2) {
  size_t oldend = edges1.end() - edges1.begin();
  for (typename vector<U>::const_iterator i = edges2.begin(); i != edges2.end(); ++i) {
    if (find(edges1.begin(), edges1.end(), static_cast<T>(*i)) == edges1.end()) {
      edges1.push_back(*i);
    }
  }
  for (typename vector<T>::const_iterator i = edges1.begin(); i != edges1.begin() + oldend; ++i) {
    if (*i <= numeric_limits<U>::max()) {
      if (find(edges2.begin(), edges2.end(), static_cast<U>(*i)) == edges2.end()) {
        edges2.push_back(*i);
      }
    }
  }
}

/*
 * Create edge cases, two types, first shorter, both unsigned.
 */
template <class T, class U> void Build_Edges2<T, U, RelTypes::US_UL>::op(vector<T>& edges1, vector<U>& edges2) {
  size_t oldend = edges2.end() - edges2.begin();
  for (typename vector<T>::const_iterator i = edges1.begin(); i != edges1.end(); ++i) {
    if (find(edges2.begin(), edges2.end(), static_cast<U>(*i)) == edges2.end()) {
      edges2.push_back(*i);
    }
  }
  for (typename vector<U>::const_iterator i = edges2.begin(); i != edges2.begin() + oldend; ++i) {
    if (*i <= numeric_limits<T>::max()) {
      if (find(edges1.begin(), edges1.end(), static_cast<T>(*i)) == edges1.end()) {
        edges1.push_back(*i);
      }
    }
  }
}

/*
 * Create edge cases, two types, same length, second signed.
 */
template <class T, class U> void Build_Edges2<T, U, RelTypes::UE_SE>::op(vector<T>& edges1, vector<U>& edges2) {
  size_t oldend = edges2.end() - edges2.begin();
  for (typename vector<T>::const_iterator i = edges1.begin(); i != edges1.end(); ++i) {
    if (find(edges2.begin(), edges2.end(), static_cast<U>(*i)) == edges2.end()) {
      edges2.push_back(*i);
    }
  }
  for (typename vector<U>::const_iterator i = edges2.begin(); i != edges2.begin() + oldend; ++i) {
    if (*i >= 0) {
      if (find(edges1.begin(), edges1.end(), static_cast<T>(*i)) == edges1.end()) {
        edges1.push_back(*i);
      }
    }
  }
}

/*
 * Create edge cases, two types, first longer and unsigned.
 */
template <class T, class U> void Build_Edges2<T, U, RelTypes::UL_SS>::op(vector<T>& edges1, vector<U>& edges2) {
  size_t oldend = edges1.end() - edges1.begin();
  for (typename vector<U>::const_iterator i = edges2.begin(); i != edges2.end(); ++i) {
    if (*i >= 0) {
      if (find(edges1.begin(), edges1.end(), static_cast<T>(*i)) == edges1.end()) {
        edges1.push_back(*i);
      }
    }
  }
  for (typename vector<T>::const_iterator i = edges1.begin(); i != edges1.begin() + oldend; ++i) {
    if (*i < static_cast<T>(numeric_limits<U>::max())) {
      if (find(edges2.begin(), edges2.end(), static_cast<U>(*i)) == edges2.end()) {
        edges2.push_back(*i);
      }
    }
  }
}

/*
 * Create edge cases, two types, first shorter and unsigned.
 */
template <class T, class U> void Build_Edges2<T, U, RelTypes::US_SL>::op(vector<T>& edges1, vector<U>& edges2) {
  size_t oldend = edges2.end() - edges2.begin();
  for (typename vector<T>::const_iterator i = edges1.begin(); i != edges1.end(); ++i) {
    if (find(edges2.begin(), edges2.end(), static_cast<U>(*i)) == edges2.end()) {
      edges2.push_back(*i);
    }
  }
  for (typename vector<U>::const_iterator i = edges2.begin(); i != edges2.begin() + oldend; ++i) {
    if (*i >= 0 && *i <= numeric_limits<T>::max()) {
      if (find(edges1.begin(), edges1.end(), static_cast<T>(*i)) == edges1.end()) {
        edges1.push_back(*i);
      }
    }
  }
}

/*
 * Create edge cases, two types, same length, first signed.
 */
template <class T, class U> void Build_Edges2<T, U, RelTypes::SE_UE>::op(vector<T>& edges1, vector<U>& edges2) {
  Build_Edges2<U, T, IntRel<U, T>::type>::op(edges2, edges1);
}

/*
 * Create edge cases, two types, first longer and signed.
 */
template <class T, class U> void Build_Edges2<T, U, RelTypes::SL_US>::op(vector<T>& edges1, vector<U>& edges2) {
  Build_Edges2<U, T, IntRel<U, T>::type>::op(edges2, edges1);
}

/*
 * Create edge cases, two types, first shorter and signed.
 */
template <class T, class U> void Build_Edges2<T, U, RelTypes::SS_UL>::op(vector<T>& edges1, vector<U>& edges2) {
  Build_Edges2<U, T, IntRel<U, T>::type>::op(edges2, edges1);
}

/*
 * Create edge cases, two types, equal length, both signed.
 */
template <class T, class U> void Build_Edges2<T, U, RelTypes::SE_SE>::op(vector<T>& edges1, vector<U>& edges2) {
  size_t oldend = edges1.end() - edges1.begin();
  for (typename vector<U>::const_iterator i = edges2.begin(); i != edges2.end(); ++i) {
    if (find(edges1.begin(), edges1.end(), static_cast<T>(*i)) == edges1.end()) {
      edges1.push_back(*i);
    }
  }
  for (typename vector<T>::const_iterator i = edges1.begin(); i != edges1.begin() + oldend; ++i) {
    if (find(edges2.begin(), edges2.end(), static_cast<U>(*i)) == edges2.end()) {
      edges2.push_back(*i);
    }
  }
}

/*
 * Create edge cases, two types, first longer, both signed.
 */
template <class T, class U> void Build_Edges2<T, U, RelTypes::SL_SS>::op(vector<T>& edges1, vector<U>& edges2) {
  size_t oldend = edges1.end() - edges1.begin();
  for (typename vector<U>::const_iterator i = edges2.begin(); i != edges2.end(); ++i) {
    if (find(edges1.begin(), edges1.end(), static_cast<T>(*i)) == edges1.end()) {
      edges1.push_back(*i);
    }
  }
  for (typename vector<T>::const_iterator i = edges1.begin(); i != edges1.begin() + oldend; ++i) {
    if (*i >= numeric_limits<U>::min() && *i <= numeric_limits<U>::max()) {
      if (find (edges2.begin(), edges2.end(), static_cast<U>(*i)) == edges2.end()) {
        edges2.push_back(*i);
      }
    }
  }
}

/*
 * Create edge cases, two types, first shorter, both signed.
 */
template <class T, class U> void Build_Edges2<T, U, RelTypes::SS_SL>::op(vector<T>& edges1, vector<U>& edges2) {
  Build_Edges2<U, T, IntRel<U, T>::type>::op(edges2, edges1);
}

/*
 * Test a single type.
 */
template <class T> int test1 () {
  
  // Work area.
  vector<T> edges;
  
  {
    string temp;
    cout << "Testing type " << demangle<T>(temp) << "..." << endl;
  }
  
  Build_Edges1<T, numeric_limits<T>::is_signed>::op(edges);
  
  // Call sign-specific tests.
  if (Test1<T, numeric_limits<T>::is_signed>::op()) {
    // Failure.
    return 1;
  }
  
  // Test with other types.
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
  
  // All tests passed.
  return 0;
  
}

/*
 * Test a single type, unsigned.
 */
template <class T> int Test1<T, false>::op () {
  
  // Test SafeIntUtils first.
  //cout << "  Testing SafeIntUtil::SafeCast()..." << endl;
  
  // All tests passed.
  return 0;
  
}

/*
 * Test a single type, signed.
 */
template <class T> int Test1<T, true>::op () {
  
  // All tests passed.
  return 0;
  
}

/*
 * Test two types.
 */
template <class T, class U> int test2 () {
  
  // Work area.
  vector<T> edges1;
  vector<U> edges2;
  
  {
    string temp;
    cout << "  With type " << demangle<U>(temp) << "..." << endl;
  }
  
  Build_Edges1<T, numeric_limits<T>::is_signed>::op(edges1);
  Build_Edges1<U, numeric_limits<U>::is_signed>::op(edges2);
  Build_Edges2<T, U, IntRel<T, U>::type>::op(edges1, edges2);
  
  if (Test_SafeCast<T, U, IntRel<T, U>::type>::op(edges1)) { return 1; }
  if (Test_RawCast <T, U, IntRel<T, U>::type>::op(edges1)) { return 1; }
  
  // Call type-specific tests.
  if (Test2<T, U, IntRel<T, U>::type>::op(edges1, edges2)) {
    // Failure.
    return 1;
  }
  
  // All tests successful.
  return 0;
  
}

/*
 * Test two types, equal length, both unsigned.
 */
template <class T, class U> int Test2<T, U, RelTypes::UE_UE>::op (vector<T> const& edges1, vector<U> const& edges2) {
  
  // Shush compiler warnings.
  if (&edges1 && &edges2) {}
  
  // All tests passed.
  return 0;
  
}

/*
 * Test two types, first longer, both unsigned.
 */
template <class T, class U> int Test2<T, U, RelTypes::UL_US>::op (vector<T> const& edges1, vector<U> const& edges2) {
  
  // Shush compiler warnings.
  if (&edges1 && &edges2) {}
  
  // All tests passed.
  return 0;
  
}

/*
 * Test two types, first shorter, both unsigned.
 */
template <class T, class U> int Test2<T, U, RelTypes::US_UL>::op (vector<T> const& edges1, vector<U> const& edges2) {
  
  // Shush compiler warnings.
  if (&edges1 && &edges2) {}
  
  // All tests passed.
  return 0;
  
}

/*
 * Test two types, equal length, first unsigned.
 */
template <class T, class U> int Test2<T, U, RelTypes::UE_SE>::op (vector<T> const& edges1, vector<U> const& edges2) {
  
  // Shush compiler warnings.
  if (&edges1 && &edges2) {}
  
  // All tests passed.
  return 0;
  
}

/*
 * Test two types, first longer, first unsigned.
 */
template <class T, class U> int Test2<T, U, RelTypes::UL_SS>::op (vector<T> const& edges1, vector<U> const& edges2) {
  
  // Shush compiler warnings.
  if (&edges1 && &edges2) {}
  
  // All tests passed.
  return 0;
  
}

/*
 * Test two types, first shorter, first unsigned.
 */
template <class T, class U> int Test2<T, U, RelTypes::US_SL>::op (vector<T> const& edges1, vector<U> const& edges2) {
  
  // Shush compiler warnings.
  if (&edges1 && &edges2) {}
  
  // All tests passed.
  return 0;
  
}

/*
 * Test two types, equal length, first signed.
 */
template <class T, class U> int Test2<T, U, RelTypes::SE_UE>::op (vector<T> const& edges1, vector<U> const& edges2) {
  
  // Shush compiler warnings.
  if (&edges1 && &edges2) {}
  
  // All tests passed.
  return 0;
  
}

/*
 * Test two types, first longer, first signed.
 */
template <class T, class U> int Test2<T, U, RelTypes::SL_US>::op (vector<T> const& edges1, vector<U> const& edges2) {
  
  // Shush compiler warnings.
  if (&edges1 && &edges2) {}
  
  // All tests passed.
  return 0;
  
}

/*
 * Test two types, first shorter, first signed.
 */
template <class T, class U> int Test2<T, U, RelTypes::SS_UL>::op (vector<T> const& edges1, vector<U> const& edges2) {
  
  // Shush compiler warnings.
  if (&edges1 && &edges2) {}
  
  // All tests passed.
  return 0;
  
}

/*
 * Test two types, equal length, both signed.
 */
template <class T, class U> int Test2<T, U, RelTypes::SE_SE>::op (vector<T> const& edges1, vector<U> const& edges2) {
  
  // Shush compiler warnings.
  if (&edges1 && &edges2) {}
  
  // All tests passed.
  return 0;
  
}

/*
 * Test two types, first longer, both signed.
 */
template <class T, class U> int Test2<T, U, RelTypes::SL_SS>::op (vector<T> const& edges1, vector<U> const& edges2) {
  
  // Shush compiler warnings.
  if (&edges1 && &edges2) {}
  
  // All tests passed.
  return 0;
  
}

/*
 * Test two types, first shorter, both signed.
 */
template <class T, class U> int Test2<T, U, RelTypes::SS_SL>::op (vector<T> const& edges1, vector<U> const& edges2) {
  
  // Shush compiler warnings.
  if (&edges1 && &edges2) {}
  
  // All tests passed.
  return 0;
  
}

/*
 * Test SafeIntUtil::SafeCast, equal lengths, both unsigned.
 */
template <class T, class U> int Test_SafeCast<T, U, RelTypes::UE_UE>::op (vector<T> const& edges) {
  
  cout << "    Testing SafeIntUtil::SafeCast..." << endl;
  
  for (typename vector<T>::const_iterator i = edges.begin(); i != edges.end(); ++i) {
    U val;
    cout << "      Testing " << to_string(*i) << " = ";
    try {
      val = SafeIntUtil::SafeCast<T, U, IntRel<T, U>::type>::op(*i);
    } catch (exception const& e) {
      string temp;
      cout << "Unexpected error of type " << demangle(temp, e) << " thrown, FAILURE!" << endl;
      return 1;
    } catch (...) {
      cout << "Unexpected error of unknown type thrown, FAILURE!" << endl;
      return 1;
    }
    cout << to_string(val);
    if (val == static_cast<U>(*i)) {
      cout << " SUCCESS!" << endl;
    } else {
      cout << " FAILURE!" << endl;
      return 1;
    }
  }
  
  // All tests passed.
  return 0;
  
}

/*
 * Test SafeIntUtil::SafeCast, first longer, both unsigned.
 */
template <class T, class U> int Test_SafeCast<T, U, RelTypes::UL_US>::op (vector<T> const& edges) {
  
  cout << "    Testing SafeIntUtil::SafeCast..." << endl;
  
  for (typename vector<T>::const_iterator i = edges.begin(); i != edges.end(); ++i) {
    U val;
    cout << "      Testing " << to_string(*i) << " = ";
    try {
      val = SafeIntUtil::SafeCast<T, U, IntRel<T, U>::type>::op(*i);
    } catch (typename SafeInt<T>::Errors::Overflow) {
      if (*i > static_cast<T>(numeric_limits<U>::max())) {
        cout << "Overflow SUCCESS!" << endl;
        continue;
      } else {
        cout << "Unexpected overflow error thrown, FAILURE!" << endl;
        return 1;
      }
    } catch (exception const& e) {
      string temp;
      cout << "Unexpected error of type " << demangle(temp, e) << " thrown, FAILURE!" << endl;
      return 1;
    } catch (...) {
      cout << "Unexpected error of unknown type thrown, FAILURE!" << endl;
      return 1;
    }
    cout << to_string(val);
    if (*i > static_cast<T>(numeric_limits<U>::max())) {
      cout << " Missing Overflow exception, FAILURE!" << endl;
      return 1;
    }
    if (val == static_cast<U>(*i)) {
      cout << " SUCCESS!" << endl;
    } else {
      cout << " FAILURE!" << endl;
      return 1;
    }
  }
  
  // All tests passed.
  return 0;
  
}

/*
 * Test SafeIntUtil::SafeCast(), first shorter, both unsigned.
 */
template <class T, class U> int Test_SafeCast<T, U, RelTypes::US_UL>::op (vector<T> const& edges) {
  return Test_SafeCast<T, U, RelTypes::UE_UE>::op(edges);
}

/*
 * Test SafeIntUtil::SafeCast(), equal lengths, first unsigned.
 */
template <class T, class U> int Test_SafeCast<T, U, RelTypes::UE_SE>::op (vector<T> const& edges) {
  return Test_SafeCast<T, U, RelTypes::UE_UE>::op(edges);
}

/*
 * Test SafeIntUtil::SafeCast(), first longer, first unsigned.
 */
template <class T, class U> int Test_SafeCast<T, U, RelTypes::UL_SS>::op (vector<T> const& edges) {
  return Test_SafeCast<T, U, RelTypes::UL_US>::op(edges);
}

/*
 * Test SafeIntUtil::SafeCast(), first shorter, first unsigned.
 */
template <class T, class U> int Test_SafeCast<T, U, RelTypes::US_SL>::op (vector<T> const& edges) {
  return Test_SafeCast<T, U, RelTypes::UE_UE>::op(edges);
}

/*
 * Test SafeIntUtil::SafeCast(), equal lengths, first signed.
 */
template <class T, class U> int Test_SafeCast<T, U, RelTypes::SE_UE>::op (vector<T> const& edges) {
  
  cout << "    Testing SafeIntUtil::SafeCast..." << endl;
  
  for (typename vector<T>::const_iterator i = edges.begin(); i != edges.end(); ++i) {
    U val;
    cout << "      Testing " << to_string(*i) << " = ";
    try {
      val = SafeIntUtil::SafeCast<T, U, IntRel<T, U>::type>::op(*i);
    } catch (typename SafeInt<T>::Errors::Overflow) {
      if (*i < 0) {
        cout << "Overflow SUCCESS!" << endl;
        continue;
      } else {
        cout << "Unexpected overflow error thrown, FAILURE!" << endl;
        return 1;
      }
    } catch (exception const& e) {
      string temp;
      cout << "Unexpected error of type " << demangle(temp, e) << " thrown, FAILURE!" << endl;
      return 1;
    } catch (...) {
      cout << "Unexpected error of unknown type thrown, FAILURE!" << endl;
      return 1;
    }
    cout << to_string(val);
    if (*i < 0) {
      cout << " Missing Overflow exception, FAILURE!" << endl;
      return 1;
    }
    if (static_cast<T>(val) == *i) {
      cout << " SUCCESS!" << endl;
    } else {
      cout << " FAILURE!" << endl;
      return 1;
    }
  }
  
  // All tests passed.
  return 0;
  
}

/*
 * Test SafeIntUtil::SafeCast(), first longer, first signed.
 */
template <class T, class U> int Test_SafeCast<T, U, RelTypes::SL_US>::op (vector<T> const& edges) {
  
  cout << "    Testing SafeIntUtil::SafeCast..." << endl;
  
  for (typename vector<T>::const_iterator i = edges.begin(); i != edges.end(); ++i) {
    U val;
    cout << "      Testing " << to_string(*i) << " = ";
    try {
      val = SafeIntUtil::SafeCast<T, U, IntRel<T, U>::type>::op(*i);
    } catch (typename SafeInt<T>::Errors::Overflow) {
      if (*i < 0 || *i > static_cast<T>(numeric_limits<U>::max())) {
        cout << "Overflow SUCCESS!" << endl;
        continue;
      } else {
        cout << "Unexpected overflow error thrown, FAILURE!" << endl;
        return 1;
      }
    } catch (exception const& e) {
      string temp;
      cout << "Unexpected error of type " << demangle(temp, e) << " thrown, FAILURE!" << endl;
      return 1;
    } catch (...) {
      cout << "Unexpected error of unknown type thrown, FAILURE!" << endl;
      return 1;
    }
    cout << to_string(val);
    if (*i < 0 || *i > static_cast<T>(numeric_limits<U>::max())) {
      cout << " Missing Overflow exception, FAILURE!" << endl;
      return 1;
    }
    if (static_cast<T>(val) == *i) {
      cout << " SUCCESS!" << endl;
    } else {
      cout << " FAILURE!" << endl;
      return 1;
    }
  }
  
  // All tests passed.
  return 0;
  
}

/*
 * Test SafeIntUtil::SafeCast(), first shorter, first signed.
 */
template <class T, class U> int Test_SafeCast<T, U, RelTypes::SS_UL>::op (vector<T> const& edges) {
  return Test_SafeCast<T, U, RelTypes::SE_UE>::op(edges);
}

/*
 * Test SafeIntUtil::SafeCast(), equal lengths, both signed.
 */
template <class T, class U> int Test_SafeCast<T, U, RelTypes::SE_SE>::op (vector<T> const& edges) {
  return Test_SafeCast<T, U, RelTypes::UE_UE>::op(edges);
}

/*
 * Test SafeIntUtil::SafeCast(), first longer, both signed.
 */
template <class T, class U> int Test_SafeCast<T, U, RelTypes::SL_SS>::op (vector<T> const& edges) {
  
  cout << "    Testing SafeIntUtil::SafeCast..." << endl;
  
  for (typename vector<T>::const_iterator i = edges.begin(); i != edges.end(); ++i) {
    U val;
    cout << "      Testing " << to_string(*i) << " = ";
    try {
      val = SafeIntUtil::SafeCast<T, U, IntRel<T, U>::type>::op(*i);
    } catch (typename SafeInt<T>::Errors::Overflow) {
      if (*i < static_cast<T>(numeric_limits<U>::min()) || *i > static_cast<T>(numeric_limits<U>::max())) {
        cout << "Overflow SUCCESS!" << endl;
        continue;
      } else {
        cout << "Unexpected overflow error thrown, FAILURE!" << endl;
        return 1;
      }
    } catch (exception const& e) {
      string temp;
      cout << "Unexpected error of type " << demangle(temp, e) << " thrown, FAILURE!" << endl;
      return 1;
    } catch (...) {
      cout << "Unexpected error of unknown type thrown, FAILURE!" << endl;
      return 1;
    }
    cout << to_string(val);
    if (*i < static_cast<T>(numeric_limits<U>::min()) || *i > static_cast<T>(numeric_limits<U>::max())) {
      cout << " Missing Overflow exception, FAILURE!" << endl;
      return 1;
    }
    if (static_cast<T>(val) == *i) {
      cout << " SUCCESS!" << endl;
    } else {
      cout << " FAILURE!" << endl;
      return 1;
    }
  }
  
  // All tests passed.
  return 0;
  
}

/*
 * Test SafeIntUtil::SafeCast(), first shorter, both signed.
 */
template <class T, class U> int Test_SafeCast<T, U, RelTypes::SS_SL>::op (vector<T> const& edges) {
  return Test_SafeCast<T, U, RelTypes::SE_SE>::op(edges);
}

/*
 * Test SafeIntUtil::RawCast(), equal length, both unsigned.
 */
template <class T, class U> int Test_RawCast<T, U, RelTypes::UE_UE>::op (vector<T> const& edges) {
  
  // Hush compiler warnings.
  if (&edges) {}
  
  /*
  cout << "    Testing SafeIntUtil::RawCast..." << endl;
  
  for (typename vector<T>::const_iterator i = edges.begin(); i != edges.end(); ++i) {
    U val;
    cout << "      Testing " << *i << " 
  }
  */
  
  // All tests passed.
  return 0;
  
}

/*
 * Test SafeIntUtil::RawCast(), first longer, both unsigned.
 */
template <class T, class U> int Test_RawCast<T, U, RelTypes::UL_US>::op (vector<T> const& edges) {
  
  // Hush compiler warnings.
  if (&edges) {}
  
  // All tests passed.
  return 0;
  
}

/*
 * Test SafeIntUtil::RawCast(), first shorter, both unsigned.
 */
template <class T, class U> int Test_RawCast<T, U, RelTypes::US_UL>::op (vector<T> const& edges) {
  
  // Hush compiler warnings.
  if (&edges) {}
  
  // All tests passed.
  return 0;
  
}

/*
 * Test SafeIntUtil::RawCast(), equal length, first unsigned.
 */
template <class T, class U> int Test_RawCast<T, U, RelTypes::UE_SE>::op (vector<T> const& edges) {
  
  // Hush compiler warnings.
  if (&edges) {}
  
  // All tests passed.
  return 0;
  
}

/*
 * Test SafeIntUtil::RawCast(), first longer, first unsigned.
 */
template <class T, class U> int Test_RawCast<T, U, RelTypes::UL_SS>::op (vector<T> const& edges) {
  
  // Hush compiler warnings.
  if (&edges) {}
  
  // All tests passed.
  return 0;
  
}

/*
 * Test SafeIntUtil::RawCast(), first shorter, first unsigned.
 */
template <class T, class U> int Test_RawCast<T, U, RelTypes::US_SL>::op (vector<T> const& edges) {
  
  // Hush compiler warnings.
  if (&edges) {}
  
  // All tests passed.
  return 0;
  
}

/*
 * Test SafeIntUtil::RawCast(), equal length, first signed.
 */
template <class T, class U> int Test_RawCast<T, U, RelTypes::SE_UE>::op (vector<T> const& edges) {
  
  // Hush compiler warnings.
  if (&edges) {}
  
  // All tests passed.
  return 0;
  
}

/*
 * Test SafeIntUtil::RawCast(), first longer, first signed.
 */
template <class T, class U> int Test_RawCast<T, U, RelTypes::SL_US>::op (vector<T> const& edges) {
  
  // Hush compiler warnings.
  if (&edges) {}
  
  // All tests passed.
  return 0;
  
}

/*
 * Test SafeIntUtil::RawCast(), first shorter, first signed.
 */
template <class T, class U> int Test_RawCast<T, U, RelTypes::SS_UL>::op (vector<T> const& edges) {
  
  // Hush compiler warnings.
  if (&edges) {}
  
  // All tests passed.
  return 0;
  
}

/*
 * Test SafeIntUtil::RawCast(), equal length, both signed.
 */
template <class T, class U> int Test_RawCast<T, U, RelTypes::SE_SE>::op (vector<T> const& edges) {
  
  // Hush compiler warnings.
  if (&edges) {}
  
  // All tests passed.
  return 0;
  
}

/*
 * Test SafeIntUtil::RawCast(), first longer, both signed.
 */
template <class T, class U> int Test_RawCast<T, U, RelTypes::SL_SS>::op (vector<T> const& edges) {
  
  // Hush compiler warnings.
  if (&edges) {}
  
  // All tests passed.
  return 0;
  
}

/*
 * Test SafeIntUtil::RawCast(), first shorter, both signed.
 */
template <class T, class U> int Test_RawCast<T, U, RelTypes::SS_SL>::op (vector<T> const& edges) {
  
  // Hush compiler warnings.
  if (&edges) {}
  
  // All tests passed.
  return 0;
  
}

/*
 * Here we go.
 */
int main () {
  
  // Test every possible type.
  if (test1<bool          >()) { return 1; }
  if (test1<char          >()) { return 1; }
  if (test1<signed char   >()) { return 1; }
  if (test1<unsigned char >()) { return 1; }
  if (test1<wchar_t       >()) { return 1; }
  if (test1<short         >()) { return 1; }
  if (test1<unsigned short>()) { return 1; }
  if (test1<int           >()) { return 1; }
  if (test1<unsigned int  >()) { return 1; }
  if (test1<long          >()) { return 1; }
  if (test1<unsigned long >()) { return 1; }
  
  // All tests passed.
  return 0;
  
}

