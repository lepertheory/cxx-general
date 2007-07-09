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
  
  {
    cout << "  Testing longdiv()... " << endl;
    vector<T>                         longedges;
    vector<typename vector<T>::const_iterator> digitpos ;
    for (unsigned int digits = 1; digits <= 5; ++digits) {
      cout << "    Digits: " << digits << "..." << endl;
      longedges.push_back(0            );
      digitpos .push_back(edges.begin());
      while (digitpos[digits - 1] != edges.end()) {
        for (unsigned int digit = 0; digit != digits; ++digit) {
          if (digitpos[digit] == edges.end()) {
            if (digit + 1 == digits) {
              break;
            }
            ++digitpos[digit + 1];
            digitpos[digit] = edges.begin();
          }
          longedges[digit] = *digitpos[digit];
          cout << "      Number:";
          for (typename vector<T>::const_iterator i = longedges.begin(); i != longedges.end(); ++i) {
            cout << " " << to_string(*i);
          }
          cout << "..." << endl;
          for (typename vector<T>::const_iterator divisor = edges.begin(); divisor != edges.end(); ++divisor) {
            cout << "        Divisor: " << to_string(*divisor) << "..." << endl;
            for (typename vector<T>::const_iterator base = edges.begin(); base != edges.end(); ++base) {
              cout << "          Base: " << to_string(*base);
              cout.flush();
              vector<T> dividend(longedges);
              T         remainder          ;
              try {
                remainder = BaseConvertUtil::longDiv(dividend, *divisor, *base);
              } catch (BaseConvert::Errors::DigitOverflow& e) {
                if (dividend[e.Digit() - 1] < *base) {
                  cout << " Unexpected digit overflow. digit: " << digit << "  divisor: " << *divisor << "  base: " << *base << "  number: " << dividend[e.Digit() - 1] << endl;
                  return 1;
                }
                cout << " Caught digit overflow on digit " << e.Digit() << ", OK!" << endl;
                continue;
              } catch (BaseConvert::Errors::DivideByZero& e) {
                if (*divisor != 0) {
                  cout << " Unexpected divide by zero. digit: " << digit << "  divisor: " << *divisor << "  base: " << *base << endl;
                  return 1;
                }
                cout << " Caught divide by zero, OK!" << endl;
                continue;
              } catch (Exception& e) {
                cout << " Unexpected exception of type " << e.type() << " thrown: " << e.what() << endl;
                return 1;
              } catch (...) {
                cout << " Unexpected exception of unknown type caught." << endl;
                return 1;
              }
              for (typename vector<T>::const_iterator testdigit = dividend.begin(); testdigit != dividend.end(); ++testdigit) {
                if (*testdigit >= *base) {
                  cout << " Missing exception. digit: " << digit << "  divisor: " << *divisor << "  base: " << *base << "  number: " << *testdigit << endl;
                  return 1;
                }
              }
              cout << " OK!" << endl;
            }
          }
        }
        ++digitpos[0];
      }
    }
  }
  
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

