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

template <class T> void testOp (ArbInt<T> const& l, Operation const op, ArbInt<T> const& r);

// This is where it all happens.
int main (int argc, char** argv, char** envp) {
  
  try {
    
    ArbInt<unsigned int> test1;
    ArbInt<unsigned int> test2;
    
    cout << "test1: ";
    cin  >> test1;
    cout << "test1: " << test1 << endl;
    cout << "test2; ";
    cin  >> test2;
    cout << "test2: " << test2 << endl;
    
    //testOp(test1, ADD, test2);
    //testOp(test1, SUB, test2);
    //testOp(test1, MUL, test2);
    testOp(test1, DIV, test2);
    testOp(test1, MOD, test2);
    
    /*
    Arb test;
    
    cout << "test: ";
    cin  >> test;
    cout << "test: " << test << endl;
    */
    
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

template <class T> void testOp (ArbInt<T> const& l, Operation const op, ArbInt<T> const& r) {
  
  try {
    cout << l;
    switch (op) {
      case ADD: cout << "  + "; break;
      case SUB: cout << "  - "; break;
      case MUL: cout << "  * "; break;
      case DIV: cout << "  / "; break;
      case MOD: cout << "  % "; break;
    }
    cout << r << " = ";
    switch (op) {
      case ADD: cout << (l  + r); break;
      case SUB: cout << (l  - r); break;
      case MUL: cout << (l  * r); break;
      case DIV: cout << (l  / r); break;
      case MOD: cout << (l  % r); break;
    }
    cout << endl;
  } catch (Exception& e) {
    cout << "Exception (" << e.type() << "): " << e << endl;
  } catch (exception& e) {
    cout << "Exception (" << demangle(e) << "): " << e.what() << endl;
  } catch (...) {
    cout << "Unexpected exception caught." << endl;
  }
  
}
