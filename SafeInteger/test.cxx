/*****************************************************************************
 * text.cxx
 *****************************************************************************
 * Tests the class SafeInteger.
 *****************************************************************************/

// STL includes.
#include <iostream>

// Internal includes.
#include "SafeInteger.hxx"
#include "toString.hxx"

// Namespace declarations.
using namespace std;
using namespace DAC;

// Test addition.
template <class lT, class rT> void testAdd (SafeInteger<lT> const& left, SafeInteger<rT> const& right) {
  cout << left << " + " << right << " = ";
  try {
    cout << (left + right);
  } catch (SafeIntegerErrors::Overflow e) {
    cout << e;
  } catch (Exception e) {
    cout << "Unexpected error: " << e;
  } catch (...) {
    cout << "Unexpected unknown error.";
  }
  cout << endl;
};
template <class lT, class rT> void testAdd (SafeInteger<lT> const& left, rT const right) {
  cout << left << " + " << toString(right) << " = ";
  try {
    cout << (left + right);
  } catch (SafeIntegerErrors::Overflow e) {
    cout << e;
  } catch (Exception e) {
    cout << "Unexpected error: " << e;
  } catch (...) {
    cout << "Unexpected unknown error.";
  }
  cout << endl;
};

// Test subtraction.
template <class lT, class rT> void testSub (SafeInteger<lT> const& left, SafeInteger<rT> const& right) {
  cout << left << " - " << right << " = ";
  try {
    cout << (left - right);
  } catch (SafeIntegerErrors::Overflow e) {
    cout << e;
  } catch (Exception e) {
    cout << "Unexpected error: " << e;
  } catch (...) {
    cout << "Unexpected unknown error.";
  }
  cout << endl;
};
template <class lT, class rT> void testSub (SafeInteger<lT> const& left, rT const right) {
  cout << left << " - " << toString(right) << " = ";
  try {
    cout << (left - right);
  } catch (SafeIntegerErrors::Overflow e) {
    cout << e;
  } catch (Exception e) {
    cout << "Unexpected error: " << e;
  } catch (...) {
    cout << "Unexpected unknown error.";
  }
  cout << endl;
};

// This is where it all happens.
int main (int argc, char** argv, char** envp) {
  
  try {
    
    {
      cout << "Testing unsigned char:" << endl;
      SafeInteger<unsigned char> test1;
      {
        cout << "  With unsigned char:" << endl;
        SafeInteger<unsigned char> test2;
        {
          cout << "    Addition:" << endl;
          test1 = numeric_limits<unsigned char>::max();
          test2 = 1;
          cout << "      Max + 1: ";
          testAdd(test1, test2);
        }
        {
          cout << "    Subtraction:" << endl;
          test1 = numeric_limits<unsigned char>::min();
          test2 = 1;
          cout << "      Min - 1: ";
          testSub(test1, test2);
        }
      }
      {
        cout << "  With signed char:" << endl;
        SafeInteger<signed char> test2;
        {
          cout << "    Addition:" << endl;
          test1 = 0;
          test2 = -1;
          cout << "      0 + -1:  ";
          testAdd(test1, test2.Value());
          test1 = numeric_limits<unsigned char>::max();
          test2 = 1;
          cout << "      Max + 1: ";
          testAdd(test1, test2.Value());
        }
        {
          cout << "    Subtraction:" << endl;
          test1 = 0;
          test2 = 1;
          cout << "      0 - 1:    ";
          testSub(test1, test2.Value());
          test1 = numeric_limits<unsigned char>::max();
          test2 = -1;
          cout << "      Max - -1: ";
          testSub(test1, test2.Value());
        }
      }
    }
    
  } catch (Exception e) {
    
    cout << "Exception: " << e << endl;
    exit(1);
    
  }
  
};
