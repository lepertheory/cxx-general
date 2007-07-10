/*****************************************************************************
 * longDiv.c++
 *****************************************************************************
 * Unit tests for longDiv().
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
#include "longDiv.h++"

// Bring in namespaces.
using namespace std;
using namespace DAC;

/*****************************************************************************/
// Declarations.

// Main.
int main ();

// Run the test suite on a given type.
template <class T> int test ();

// Create edge cases.
template <class T, bool is_signed> class Build_Edges;
template <class T> class Build_Edges<T, false> { public: static void op (T& edges); };
template <class T> class Build_Edges<T, true > { public: static void op (T& edges); };

/*****************************************************************************/
// Definitions.

/*
 * Run the test suite on a given type.
 */
template <class T> int test () {
  
  {
    string tmpstr;
    cout << "Testing type " << demangle<T>(tmpstr) << "..." << endl;
  }
  
  vector<T> edges;
  
  Build_Edges<vector<T>, numeric_limits<T>::is_signed>::op(edges);
  
  {
    
    cout << "  Testing longDiv()..." << endl;
    
    vector<T>                                  longedges;
    vector<typename vector<T>::const_iterator> digitpos ;
    
    for (size_t digits = 1; digits <= 5; ++digits) {
      cout << "    Digits: " << digits << "..." << endl;
      
      longedges.push_back(0            );
      digitpos .push_back(edges.begin());
      
      while (digitpos[digits - 1] != edges.end()) {
        for (size_t digit = 0; digit != digits; ++digit) {
          
          if (digitpos[digit] == edges.end()) {
            if (digit + 1 == digits) {
              break;
            }
            ++digitpos[digit + 1];
            digitpos[digit] == edges.begin();
          }
          
          longedges[digit] = *digitpos[digit];
          
          cout << "      Number:";
          for (typename vector<T>::const_iterator i = longedges.begin(); i != longedges.end(); ++i) {
            cout << " " << to_string(*i);
          }
          cout << "..." << endl;
          
          for (typename vector<T>::const_iterator divisor = edges.begin(); divisor != edges.end(); ++divisor) {
            cout << "        Divisor: " << to_string(*divisor) << "..." << endl;
            for (typename vector<T>::const_iterator base = edges.begin(); divisor != edges.end(); ++divisor) {
              cout << "          Base: " << to_string(*base);
              cout.flush();
              
              vector<T> quotient ;
              T         remainder;
              
              try {
                remainder = longDiv(quotient, longedges, *divisor, *base);
              } catch (LongDiv::Errors::DigitOverflow<T>& e) {
              } catch (LongDiv::Errors::DivideByZero& e) {
              } catch (Exception& e) {
              } catch (...) {
              }
              
            }
          }
          
        }
        ++digitpos[0];
      }
      
    }
    
  }
  
  return 0;
  
}

/*
 * Create edge cases, unsigned type.
 */
template <class T> void Build_Edges<T, false>::op (T& edges) {
  edges.push_back(0);
  edges.push_back(1);
  if (numeric_limits<typename T::value_type>::max() > 1) {
    if (numeric_limits<typename T::value_type>::max() > 2) {
      if (numeric_limits<typename T::value_type>::max() > 3) {
        if (numeric_limits<typename T::value_type>::max() > 5) {
          edges.push_back((numeric_limits<typename T::value_type>::max() >> 1) - 1);
        }
        edges.push_back(numeric_limits<typename T::value_type>::max() >> 1);
      }
      edges.push_back(numeric_limits<typename T::value_type>::max() - 1);
    }
    edges.push_back(numeric_limits<typename T::value_type>::max());
  }
}

/*
 * Create edge cases, signed type.
 */
template <class T> void Build_Edges<T, true>::op (T& edges) {
  if (numeric_limits<typename T::value_type>::min() < -1) {
    edges.push_back(numeric_limits<typename T::value_type>::min());
    if (numeric_limits<typename T::value_type>::min() < -2) {
      edges.push_back(numeric_limits<typename T::value_type>::min() + 1);
      if (numeric_limits<typename T::value_type>::min() < -3) {
        edges.push_back(numeric_limits<typename T::value_type>::min() >> 1);
        if (numeric_limits<typename T::value_type>::min() < -5) {
          edges.push_back((numeric_limits<typename T::value_type>::min() >> 1) + 1);
        }
      }
    }
  }
  Build_Edges<T, false>::op(edges);
}

/*
 * Main.
 */
int main () {
  
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
  
  return 0;
  
}

