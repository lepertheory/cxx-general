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

enum Operation { ADD, SUB, MUL, DIV };

// Test operation.
template <class lT, class rT> void testOp (SafeInteger<lT> const& left, Operation const op, SafeInteger<rT> const& right) {
  try {
    switch (op) {
      case ADD:
        cout << left << " + " << right << " = ";
        cout << (left + right);
      break;
      case SUB:
        cout << left << " - " << right << " = ";
        cout << (left - right);
      break;
      case MUL:
        cout << left << " * " << right << " = ";
        cout << (left * right);
      break;
      case DIV:
        cout << left << " / " << right << " = ";
        cout << (left / right);
      break;
    }
  } catch (SafeIntegerErrors::Base& e) {
    cout << e;
  } catch (Exception& e) {
    cout << "Unexpected error: " << e;
  } catch (...) {
    cout << "Unexpected unknown error.";
  }
  cout << endl;
};
template <class lT, class rT> void testOp (SafeInteger<lT> const& left, Operation const op, rT const right) {
  try {
    switch (op) {
      case ADD:
        cout << left << " + " << toString(right) << " = ";
        cout << (left + right);
      break;
      case SUB:
        cout << left << " - " << toString(right) << " = ";
        cout << (left - right);
      break;
      case MUL:
        cout << left << " * " << toString(right) << " = ";
        cout << (left * right);
      break;
      case DIV:
        cout << left << " / " << toString(right) << " = ";
        cout << (left / right);
      break;
    }
  } catch (SafeIntegerErrors::Base& e) {
    cout << e;
  } catch (Exception& e) {
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
          cout << "      Over maximum: ";
          testOp(test1, ADD, test2);
        }
        {
          cout << "    Subtraction:" << endl;
          test1 = numeric_limits<unsigned char>::min();
          test2 = 1;
          cout << "      Under minimum: ";
          testOp(test1, SUB, test2);
        }
        {
          cout << "    Multiplication:" << endl;
          test1 = numeric_limits<unsigned char>::max() / 2 + 1;
          test2 = 2;
          cout << "      Over maximum: ";
          testOp(test1, MUL, test2);
        }
        {
          cout << "    Division:" << endl;
          test1 = 1;
          test2 = 0;
          cout << "      Divide by zero: ";
          testOp(test1, DIV, test2);
        }
      }
      {
        cout << "  With signed char:" << endl;
        SafeInteger<signed char> test2;
        {
          cout << "    Addition:" << endl;
          test1 = 0;
          test2 = -1;
          cout << "      Under minimum: ";
          testOp(test1, ADD, test2.Value());
          test1 = numeric_limits<unsigned char>::max();
          test2 = 1;
          cout << "      Over maximum:  ";
          testOp(test1, ADD, test2.Value());
        }
        {
          cout << "    Subtraction:" << endl;
          test1 = 0;
          test2 = 1;
          cout << "      Under minimum: ";
          testOp(test1, SUB, test2.Value());
          test1 = numeric_limits<unsigned char>::max();
          test2 = -1;
          cout << "      Over maximum:  ";
          testOp(test1, SUB, test2.Value());
        }
        {
          cout << "    Multiplication:" << endl;
          test1 = numeric_limits<unsigned char>::max() / 2 + 1;
          test2 = 2;
          cout << "      Over maximum:  ";
          testOp(test1, MUL, test2.Value());
          test1 = 1;
          test2 = -1;
          cout << "      Under minimum: ";
          testOp(test1, MUL, test2.Value());
        }
        {
          cout << "    Division:" << endl;
          test1 = 1;
          test2 = 0;
          cout << "      Divide by zero: ";
          testOp(test1, DIV, test2.Value());
        }
      }
      {
        cout << "  With unsigned short int:" << endl;
        SafeInteger<unsigned short int> test2;
        {
          cout << "    Addition:" << endl;
          test1 = 0;
          test2 = numeric_limits<unsigned char>::max() + 1;
          cout << "      Over maximum: ";
          testOp(test1, ADD, test2.Value());
        }
        {
          cout << "    Subtraction:" << endl;
          test1 = numeric_limits<unsigned char>::max();
          test2 = numeric_limits<unsigned char>::max() + 1;
          cout << "      Under minimum: ";
          testOp(test1, SUB, test2.Value());
        }
        {
          cout << "    Multiplication:" << endl;
          test1 = numeric_limits<unsigned char>::max();
          test2 = numeric_limits<unsigned short int>::max();
          cout << "      Over maximum: ";
          testOp(test1, MUL, test2.Value());
        }
        {
          cout << "    Division:" << endl;
          test1 = 1;
          test2 = 0;
          cout << "      Divide by zero: ";
          testOp(test1, DIV, test2.Value());
        }
      }
    }
    {
      cout << "Testing signed char:" << endl;
      SafeInteger<signed char> test1;
      {
        cout << "  With unsigned char:" << endl;
        SafeInteger<unsigned char> test2;
        {
          cout << "    Addition:" << endl;
          test1 = 0;
          test2 = numeric_limits<signed char>::max() + 1;
          cout << "      Over maximum: ";
          testOp(test1, ADD, test2.Value());
          test1 = -1;
          test2 = numeric_limits<signed char>::max() + 2;
          cout << "      Over maximum: ";
          testOp(test1, ADD, test2.Value());
        }
        {
          cout << "    Subtraction:" << endl;
          test1 = 0;
          test2 = -numeric_limits<signed char>::min() + 1;
          cout << "      Under minimum: ";
          testOp(test1, SUB, test2.Value());
          test1 = 1;
          test2 = -numeric_limits<signed char>::min() + 2;
          cout << "      Under minimum: ";
          testOp(test1, SUB, test2.Value());
        }
        {
          cout << "    Multiplication:" << endl;
          test1 = numeric_limits<signed char>::max() / 2 + 1;
          test2 = 2;
          cout << "      Over maximum:  ";
          testOp(test1, MUL, test2.Value());
          test1 = -1;
          test2 = -numeric_limits<signed char>::min() + 1;
          cout << "      Under minimum: ";
          testOp(test1, MUL, test2.Value());
        }
        {
          cout << "    Division:" << endl;
          test1 = 1;
          test2 = 0;
          cout << "      Divide by zero: ";
          testOp(test1, DIV, test2.Value());
        }
      }
      {
        cout << "  With signed char:" << endl;
        SafeInteger<signed char> test2;
        {
          cout << "    Addition:" << endl;
          test1 = numeric_limits<signed char>::max();
          test2 = 1;
          cout << "      Over maximum:  ";
          testOp(test1, ADD, test2);
          test1 = numeric_limits<signed char>::min();
          test2 = -1;
          cout << "      Under minimum: ";
          testOp(test1, ADD, test2);
        }
        {
          cout << "    Subtraction:" << endl;
          test1 = numeric_limits<signed char>::max();
          test2 = -1;
          cout << "      Over maximum:  ";
          testOp(test1, SUB, test2);
          test1 = numeric_limits<signed char>::min();
          test2 = 1;
          cout << "      Under minimum: ";
          testOp(test1, SUB, test2);
        }
        {
          cout << "    Multiplication:" << endl;
          test1 = numeric_limits<signed char>::max() / 2 + 1;
          test2 = 2;
          cout << "      Over maximum:  ";
          testOp(test1, MUL, test2);
          test1 = numeric_limits<signed char>::min() / 2 - 1;
          test2 = 2;
          cout << "      Under minimum: ";
          testOp(test1, MUL, test2);
          test1 = numeric_limits<signed char>::max() / 2 + 2;
          test2 = -2;
          cout << "      Under minimum: ";
          testOp(test1, MUL, test2);
          test1 = numeric_limits<signed char>::min();
          test2 = -1;
          cout << "      Over maximum:  ";
          testOp(test1, MUL, test2);
        }
        {
          cout << "    Division:" << endl;
          test1 = numeric_limits<signed char>::min();
          test2 = -1;
          cout << "      Over maximum:   ";
          testOp(test1, DIV, test2);
          test1 = 1;
          test2 = 0;
          cout << "      Divide by zero: ";
          testOp(test1, DIV, test2);
        }
      }
      {
        cout << "  With unsigned short int:" << endl;
        SafeInteger<unsigned short int> test2;
        {
          cout << "    Addition:" << endl;
          test1 = numeric_limits<signed char>::min();
          test2 = 255;
          cout << "      Over maximum: ";
          testOp(test1, ADD, test2.Value());
        }
      }
    }
    
  } catch (Exception& e) {
    
    cout << "Exception: " << e << endl;
    exit(1);
    
  } catch (...) {
    
    cout << "Unexpected exception." << endl;
    exit(1);
    
  }
  
};
