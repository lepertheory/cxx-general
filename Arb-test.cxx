/*****************************************************************************
 * Arb-test.cxx
 *****************************************************************************
 * Tests the class Arb.
 *****************************************************************************/

// STL includes.
#include <iostream>
#include <string>

// Internal includes.
#include "demangle.hxx"
#include "Exception.hxx"

// Testing include.
#include "Arb.hxx"

// Namespace declarations.
using namespace std;
using namespace DAC;

enum Operation {
  MUL,
  DIV,
  ADD,
  SUB
};

bool testOp (Arb const& l, Operation const op, Arb const& r);

// This is where it all happens.
int main (int argc, char** argv, char** envp) {
  
  try {
    
    Arb test1;
    Arb test2;
    
    cout << "test1: ";
    cin  >> test1;
    cout << "test1: " << test1 << endl;
    cout << "test2: ";
    cin  >> test2;
    cout << "test2: " << test2 << endl;
    
    testOp(test1, MUL, test2);
    testOp(test1, DIV, test2);
    testOp(test1, ADD, test2);
    testOp(test1, SUB, test2);
    
    cout << "original: " << test1;
    cout << "  ++" << ++test1;
    cout << "  final: " << test1 << endl;
    cout << "original: " << test1;
    cout << "  " << test1++ << "++";
    cout << "  final: " << test1 << endl;
    cout << "original: " << test1;
    cout << "  --" << --test1;
    cout << "  final: " << test1<< endl;
    cout << "original: " << test1;
    cout << "  " << test1-- << "--";
    cout << "  final: " << test1<< endl;
    
    // All tests successful.
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
  
};

bool testOp (Arb const& l, Operation const op, Arb const& r) {
  
  bool retval = false;
  string output;
  
  try {
    output = l.toString();
    switch (op) {
      case MUL: output += "  * "; break;
      case DIV: output += "  / "; break;
      case ADD: output += "  + "; break;
      case SUB: output += "  - "; break;
    }
    output += r.toString() + " = ";
    switch (op) {
      case MUL: output += (l  * r).toString(); break;
      case DIV: output += (l  / r).toString(); break;
      case ADD: output += (l  + r).toString(); break;
      case SUB: output += (l  - r).toString(); break;
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
