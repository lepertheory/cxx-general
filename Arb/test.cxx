/*****************************************************************************
 * test.cxx
 *****************************************************************************
 * Tests the class Arb.
 *****************************************************************************/

// STL includes.
#include <iostream>

// Internal includes.
#include "ArbInt.hxx"
#include "Arb.hxx"
#include "demangle.hxx"
#include "Exception.hxx"

// Namespace declarations.
using namespace std;
using namespace DAC;

enum Operation {
  MUL,
  DIV,
  MOD,
  ADD,
  SUB
};

template <class T> bool testOp (ArbInt<T> const& l, Operation const op, ArbInt<T> const& r);

// This is where it all happens.
int main (int argc, char** argv, char** envp) {
  
  try {
    
    ArbInt<unsigned int> test1;
    ArbInt<unsigned int> test2;
    
    /**/
    cout << "test1: ";
    cin  >> test1;
    cout << "test1: " << test1 << endl;
    cout << "test2: ";
    cin  >> test2;
    cout << "test2: " << test2 << endl;
    
    testOp(test1, ADD, test2);
    testOp(test1, SUB, test2);
    testOp(test1, MUL, test2);
    testOp(test1, DIV, test2);
    testOp(test1, MOD, test2);
    //*/
    
    /** /
    Arb test;
    
    cout << "test: ";
    cin  >> test;
    cout << "test: " << test << endl;
    //*/
    
    /** /
    unsigned long long int testdigs[] = {
               0ULL,          1ULL,     2ULL,   254ULL,   255ULL,   256ULL, 16382ULL, 16383ULL,      16384ULL,      16385ULL,
           32766ULL,      32767ULL, 32768ULL, 32769ULL, 65534ULL, 65535ULL, 65536ULL, 65537ULL, 4294967294ULL, 4294967295ULL,
      4294967296ULL, 4294967297ULL
    };
    
    for (int i = 0; i != 22; ++i) {
      for (int j = 0; j != 22; ++j) {
        test1 = testdigs[i];
        test2 = testdigs[j];
        testOp(test1, ADD, test2);
        testOp(test1, SUB, test2);
        testOp(test1, MUL, test2);
        testOp(test1, DIV, test2);
        testOp(test1, MOD, test2);
      }
    }
    //*/
    
    return 0;
    
  } catch (Exception& e) {
    
    cout << "Exception (" << e.type() << "): " << e << endl;
    exit(1);
    
  } catch (exception& e) {
    
    cout << "Exception (" << demangle(e) << "): " << e.what() << endl;
    exit(1);
    
  } catch (...) {
    
    cout << "Unexpected exception caught." << endl;
    exit(1);
    
  }
  
}

template <class T> bool testOp (ArbInt<T> const& l, Operation const op, ArbInt<T> const& r) {
  
  bool retval = false;
  string output;
  
  try {
    ArbInt<T> test;
    output = l.toString();
    switch (op) {
      case ADD: output += "  + "; break;
      case SUB: output += "  - "; break;
      case MUL: output += "  * "; break;
      case DIV: output += "  / "; break;
      case MOD: output += "  % "; break;
    }
    output += r.toString() + " = ";
    switch (op) {
      /**/
      case ADD: output += (l + r).toString(); break;
      case SUB: output += (l - r).toString(); break;
      case MUL: output += (l * r).toString(); break;
      case DIV: output += (l / r).toString(); break;
      case MOD: output += (l % r).toString(); break;
      //*/
      /** /
      case ADD:
        test = l + r;
        retval = (l == (test - r));
        output += test.toString() + " (" + l.toString() + " == " + "(" + test.toString() + " - " + r.toString() + "))";
      break;
      case SUB:
        test = l - r;
        retval = (l == (test + r));
        output += test.toString() + " (" + l.toString() + " == " + "(" + test.toString() + " + " + r.toString() + "))";
      break;
      case MUL:
        test = l * r;
        retval = (((r == ArbInt<T>(0)) && (test == ArbInt<T>(0))) || (l == (test / r)));
        output += test.toString() + " (" + ((r == ArbInt<T>(0)) ? "0 == 0" : l.toString() + " == " + "(" + test.toString() + " / " + r.toString()) + "))";
      break;
      case DIV:
        test = l / r;
        retval = (l == (test * r + (l % r)));
        output += test.toString() + " (" + l.toString() + " == (" + test.toString() + " * " + r.toString() + "(" + (l % r).toString() + ")))";
      break;
      case MOD:
        test = l % r;
        retval = (test == l - ((l / r) * r));
        output += test.toString() + " (" + test.toString() + " == " + l.toString() + " - ((" + l.toString() + " / " + r.toString() + ") * " + r.toString() + "))";
      break;
      //*/
    }
  } catch (Exception& e) {
    output += "Exception (" + e.type() + "): " + string(e.what());
    retval = false;
  } catch (exception& e) {
    output += "Exception (" + demangle(e) + "): " + string(e.what());
    retval = false;
  } catch (...) {
    output += "Unexpected exception caught.";
    retval = false;
  }
  
  if (!retval) {
    cout << output << endl;
  }
  
  return retval;
  
}
