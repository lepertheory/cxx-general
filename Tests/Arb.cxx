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
  MOD,
  ADD,
  SUB,
  CGT,
  CGE,
  CLT,
  CLE,
  CEQ,
  CNE
};

bool testOp (Arb const& l, Operation const op, Arb const& r);

// This is where it all happens.
int main () {
  
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
    testOp(test1, MOD, test2);
    testOp(test1, ADD, test2);
    testOp(test1, SUB, test2);
    testOp(test1, CGT, test2);
    testOp(test1, CGE, test2);
    testOp(test1, CLT, test2);
    testOp(test1, CLE, test2);
    testOp(test1, CEQ, test2);
    testOp(test1, CNE, test2);
    
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
    
    cout << "pow:  " << test1.pow(test2) << endl;
    cout << "root: " << test1.root(test2) << endl;
    
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
  
}

bool testOp (Arb const& l, Operation const op, Arb const& r) {
  
  bool retval = false;
  string output;
  
  try {
    output = l.toString();
    switch (op) {
      case MUL: output += "  * "; break;
      case DIV: output += "  / "; break;
      case MOD: output += "  % "; break;
      case ADD: output += "  + "; break;
      case SUB: output += "  - "; break;
      case CGT: output += " >  "; break;
      case CGE: output += " >= "; break;
      case CLT: output += " <  "; break;
      case CLE: output += " <= "; break;
      case CEQ: output += " == "; break;
      case CNE: output += " != "; break;
    }
    output += r.toString() + " = ";
    switch (op) {
      case MUL: output += (l  * r).toString(); break;
      case DIV: output += (l  / r).toString(); break;
      case MOD: output += (l  % r).toString(); break;
      case ADD: output += (l  + r).toString(); break;
      case SUB: output += (l  - r).toString(); break;
      case CGT: output += ((l >  r) ? "true" : "false"); break;
      case CGE: output += ((l >= r) ? "true" : "false"); break;
      case CLT: output += ((l <  r) ? "true" : "false"); break;
      case CLE: output += ((l <= r) ? "true" : "false"); break;
      case CEQ: output += ((l == r) ? "true" : "false"); break;
      case CNE: output += ((l != r) ? "true" : "false"); break;
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
