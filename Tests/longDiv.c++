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
#include <build_edges.h++>

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

// Output an error message.
template <class T> void errMsg (string const& message, T const base, T const divisor, size_t const digits, vector<T> const& dividend, T const remainder, vector<T> const& quotient);

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
  
  build_edges(edges);
  
  {
    
    cout << "  Testing longDiv()..." << endl;
    
    for (typename vector<T>::const_iterator base = edges.begin(); base != edges.end(); ++base) {
      cout << "    Base: " << to_string(*base) << "..." << endl;
      
      for (typename vector<T>::const_iterator divisor = edges.begin(); divisor != edges.end(); ++divisor) {
        cout << "      Divisor: " << to_string(*divisor) << "..." << endl;
        
        vector<T>                                  longedges;
        vector<typename vector<T>::const_iterator> digitpos ;
        for (size_t digits = 1; digits <= 5; ++digits) {
          cout << "        Digits: " << digits << "..." << endl;
          
          // Add a new digit. Last digit position being set to end of edges is
          // used as an indicator to move to the next number of digits, so to
          // make it valid we need to set it back to the beginning.
          if (!digitpos.empty()) {
            digitpos.back() = edges.begin();
          }
          longedges.push_back(0            );
          digitpos .push_back(edges.begin());
          
          while (digitpos.back() != edges.end()) {
            
            // Set the number.
            for (size_t digit = 0; digit != digits; ++digit) {
              longedges[digit] = *digitpos[digit];
            }
            
            cout << "          Number: ";
            for (typename vector<T>::const_iterator i = longedges.begin(); i != longedges.end(); ++i) {
              cout << to_string(*i) << " ";
            }
            cout.flush();
            
            vector<T> quotient            ;
            vector<T> dividend (longedges);
            T         remainder           ;
            bool      caughtok  = false   ;
            
            // This is the test.
            try {
              remainder = longDiv(quotient, dividend, *divisor, *base);
              
            } catch (LongDiv::Errors::DigitOverflow& e) {
              if (dividend[e.Digit()] < *base && !is_negative(dividend[e.Digit()])) {
                cout << "Unexpected digit overflow, digit " << to_string(e.Digit()) << ", FAILED!" << endl;
                errMsg("Unexpected digit overflow, digit " + to_string(e.Digit()) + ".", *base, *divisor, digits, longedges, remainder, quotient);
                return 1;
              }
              cout << "Caught digit overflow on digit " << to_string(e.Digit()) << ", OK!" << endl;
              caughtok = true;
              
            } catch (LongDiv::Errors::DivideByZero&) {
              if (*divisor != 0) {
                cout << "Unexpected divide by zero error, FAILED!" << endl;
                errMsg("Unexpected divide by zero error.", *base, *divisor, digits, longedges, remainder, quotient);
                return 1;
              }
              cout << " Caught divide by zero, OK!" << endl;
              caughtok = true;
              
            } catch (LongDiv::Errors::MinBase&) {
              if (*base >= 2) {
                cout << "Unexpected minimum base error, FAILED!" << endl;
                errMsg("Unexpected minimum base error.", *base, *divisor, digits, longedges, remainder, quotient);
                return 1;
              }
              cout << "Caught minimum base error, OK!" << endl;
              caughtok = true;
              
            } catch (LongDiv::Errors::MaxBase&) {
              if (*base <= 1 << (numeric_limits<T>::digits >> 1)) {
                cout << "Unexpected maximum base error, FAILED!" << endl;
                errMsg("Unexpected maximum base error.", *base, *divisor, digits, longedges, remainder, quotient);
                return 1;
              }
              cout << "Caught maximum base error, OK!" << endl;
              caughtok = true;
              
            } catch (LongDiv::Errors::DivisorOverflow&) {
              if (*divisor < *base && !is_negative(*divisor)) {
                cout << "Unexpected divisor overflow error, FAILED!" << endl;
                errMsg("Unexpected divisor overflow error.", *base, *divisor, digits, longedges, remainder, quotient);
                return 1;
              }
              cout << "Caught divisor overflow error, OK!" << endl;
              caughtok = true;
              
            } catch (Exception& e) {
              cout << "Unexpected exception of type " << e.type() << ": " << e.what() << " FAILED!" << endl;
              errMsg("Unexpected exception of type " + string(e.type()) + ": " + e.what(), *base, *divisor, digits, longedges, remainder, quotient);
              return 1;
              
            } catch (...) {
              cout << "Unexpected exception of unknown type caught. FAILED!" << endl;
              errMsg("Unexpected exception of unknown type caught.", *base, *divisor, digits, longedges, remainder, quotient);
              throw;
            }
            
            if (!caughtok) {
              
              for (typename vector<T>::const_iterator testdigit = dividend.begin(); testdigit != dividend.end(); ++testdigit) {
                if (*testdigit >= *base || is_negative(*testdigit)) {
                  cout << "Missing digit overflow error, digit " << to_string(testdigit - longedges.begin()) << ". FAILED!" << endl;
                  errMsg("Missing digit overflow error, digit " + to_string(testdigit - longedges.begin()) + ".", *base, *divisor, digits, longedges, remainder, quotient);
                  return 1;
                }
              }
              
              if (*divisor == 0) {
                cout << "Missing divide by zero error. FAILED!" << endl;
                errMsg("Missing divide by zero error.", *base, *divisor, digits, longedges, remainder, quotient);
                return 1;
              }
              
              if (*base > 1 << (numeric_limits<T>::digits >> 1)) {
                cout << "Missing maximum base error. FAILED!" << endl;
                errMsg("Missing maximum base error.", *base, *divisor, digits, longedges, remainder, quotient);
                return 1;
              }
              
              if (*base < 2) {
                cout << "Missing minimum base error. FAILED!" << endl;
                errMsg("Missing minimum base error.", *base, *divisor, digits, longedges, remainder, quotient);
                return 1;
              }
              
              if (*divisor >= *base || is_negative(*divisor)) {
                cout << "Missing divisor overflow error. FAILED!" << endl;
                errMsg("Missing divisor overflow error.", *base, *divisor, digits, longedges, remainder, quotient);
                return 1;
              }
              
              // Re-multiply.
              {
                T carry = 0;
                for (typename vector<T>::iterator muldigit = quotient.begin(); muldigit != quotient.end(); ++muldigit) {
                  *muldigit = *muldigit * *divisor + carry;
                  carry = 0;
                  if (*muldigit >= *base) {
                    carry     = *muldigit / *base;
                    *muldigit = *muldigit - *base * carry;
                  }
                }
                if (carry != 0) {
                  quotient.push_back(carry);
                } 
              }
              
              // Re-add the remainder.
              {
                T carry = remainder;
                for (typename vector<T>::iterator adddigit = quotient.begin(); adddigit != quotient.end(); ++adddigit) {
                  *adddigit = *adddigit + carry;
                  carry = 0;
                  if (*adddigit >= *base) {
                    carry     = *adddigit / *base;
                    *adddigit = *adddigit - *base * carry;
                  } else {
                    break;
                  }
                }
                if (carry != 0) {
                  quotient.push_back(carry);
                }
              }
              
              // Trim zeros.
              {
                typename vector<T>::size_type              noinsignificantzeros = dividend.size  ();
                typename vector<T>::const_reverse_iterator i                    = dividend.rbegin();
                while (i != dividend.rend() && *i == 0) {
                  --noinsignificantzeros;
									++i;
                }
                dividend.resize(noinsignificantzeros);
              }
              
              if (quotient == dividend) {
                cout << "== ";
                for (typename vector<T>::const_iterator i = quotient.begin(); i != quotient.end(); ++i) {
                  cout << to_string(*i) << " ";
                }
                cout << "OK!" << endl;
              } else {
                cout << "!= ";
                for (typename vector<T>::const_iterator i = quotient.begin(); i != quotient.end(); ++i) {
                  cout << to_string(*i) << " ";
                }
                cout << "FAILED!" << endl;
                return 1;
              }
              
            }
            
            // Increment the number and perform carry. Leave the highest-order
            // digit at end to indicate that we have tested all combinations.
            ++digitpos[0];
            for (size_t digit = 0; digit != digits; ++digit) {
              if (digitpos[digit] == edges.end()) {
                if (digit + 1 != digits) {
                  digitpos[digit] = edges.begin();
                  ++digitpos[digit + 1];
                }
              } else {
                break;
              }
            }
            
          }
          
        }
          
      }
      
    }
    
  }
  
  return 0;
  
}

/*
 * Output an error message.
 */
template <class T> void errMsg (string const& message, T const base, T const divisor, size_t const digits, vector<T> const& dividend, T const remainder, vector<T> const& quotient) {
  cerr << message << endl;
  cerr << "Base:      " << to_string(base     ) << "\n"
       << "Divisor:   " << to_string(divisor  ) << "\n"
       << "Digits:    " << to_string(digits   ) << "\n"
       << "Remainder: " << to_string(remainder) << "\n";
  cerr << "Dividend: ";
  for (typename vector<T>::const_iterator i = dividend.begin(); i != dividend.end(); ++i) {
    cerr << " " << to_string(*i);
  }
  cerr << "\n";
  cerr << "Quotient: ";
  for (typename vector<T>::const_iterator i = quotient.begin(); i != quotient.end(); ++i) {
    cerr << " " << to_string(*i);
  }
  cerr << endl;
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

