/*****************************************************************************
 * Arbint-test.c++
 *****************************************************************************
 * Tests the class ArbInt.
 *****************************************************************************/

// STL includes.
#include <iostream>

// Internal includes.
#include "ArbInt.h++"
#include "demangle.h++"
#include "Exception.h++"

// Namespace declarations.
using namespace std;
using namespace DAC;

enum Operation {
  MUL,
  DIV,
  MOD,
  ADD,
  SUB,
  SHL,
  SHR,
  CGT,
  CGE,
  CLT,
  CLE,
  CEQ,
  CNE,
  BND,
  BIO,
  BXO,
  LND,
  LIO,
  PIN,
  OIN,
  PDE,
  ODE,
  BCM
};

bool testOp (ArbInt const& l, Operation const op, ArbInt const& r);

// This is where it all happens.
int main () {
  
  try {
    
    ArbInt test1;
    ArbInt test2;
    
    /**/
    cout << "test1: ";
    cin  >> test1;
    cout << "test1: " << test1 << endl;
    cout << "test2: ";
    cin  >> test2;
    cout << "test2: " << test2 << endl;
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
        //*/
        testOp(test1, MUL, test2);
        testOp(test1, DIV, test2);
        testOp(test1, MOD, test2);
        testOp(test1, ADD, test2);
        testOp(test1, SUB, test2);
        testOp(test1, SHL, test2);
        testOp(test1, SHR, test2);
        testOp(test1, CGT, test2);
        testOp(test1, CGE, test2);
        testOp(test1, CLT, test2);
        testOp(test1, CLE, test2);
        testOp(test1, CEQ, test2);
        testOp(test1, CNE, test2);
        testOp(test1, BND, test2);
        testOp(test1, BIO, test2);
        testOp(test1, BXO, test2);
        testOp(test1, LND, test2);
        testOp(test1, LIO, test2);
        testOp(test1, PIN, test2);
        testOp(test1, OIN, test2);
        testOp(test1, PDE, test2);
        testOp(test1, ODE, test2);
        testOp(test1, BCM, test2);
        /** /
      }
    }
    //*/
    
    test1 = 5;
    cout << "testing scalar types: " << endl;
    cout << "uint: " << test1 << " * " << static_cast<unsigned>(5) << " = " << (test1 * static_cast<unsigned>(5)) << endl;
    cout << "sint: " << test1 << " * " << 3                        << " = " << (test1 * 3)                        << endl;
    cout << "flpt: " << test1 << " * " << -0.6                     << " = " << (test1 * -0.6)                     << endl;
    
    return 0;
    
  } catch (Exception& e) {
    
    string errtype;
    cout << "Exception (" << e.type(errtype) << "): " << e << endl;
    exit(1);
    
  } catch (exception& e) {
    
    string errtype;
    cout << "Exception (" << demangle(errtype, e) << "): " << e.what() << endl;
    exit(1);
    
  } catch (...) {
    
    cout << "Unexpected exception caught." << endl;
    exit(1);
    
  }
  
}

bool testOp (ArbInt const& l, Operation const op, ArbInt const& r) {
  
  bool retval = false;
  string output;
  
  try {
    ArbInt test;
    output = l.to_string();
    switch (op) {
      case MUL: output += "  * "; break;
      case DIV: output += "  / "; break;
      case MOD: output += "  % "; break;
      case ADD: output += "  + "; break;
      case SUB: output += "  - "; break;
      case SHL: output += " << "; break;
      case SHR: output += " >> "; break;
      case CGT: output += " >  "; break;
      case CGE: output += " >= "; break;
      case CLT: output += " <  "; break;
      case CLE: output += " <= "; break;
      case CEQ: output += " == "; break;
      case CNE: output += " != "; break;
      case BND: output += "  & "; break;
      case BIO: output += "  | "; break;
      case BXO: output += "  ^ "; break;
      case LND: output += " && "; break;
      case LIO: output += " || "; break;
      case PIN: output += "  ++"; break;
      case OIN: output += "++  "; break;
      case PDE: output += "  --"; break;
      case ODE: output += "--  "; break;
      case BCM: output += "   ~"; break;
    }
    output += r.to_string() + " = ";
    switch (op) {
      /**/
      case MUL: output += (l  * r).to_string(); break;
      case DIV: output += (l  / r).to_string(); break;
      case MOD: output += (l  % r).to_string(); break;
      case ADD: output += (l  + r).to_string(); break;
      case SUB: output += (l  - r).to_string(); break;
      case SHL: output += ((r > ArbInt(10000)) ? "Skipped, huge." : (l << r).to_string()); break;
      case SHR: output += (l >> r).to_string(); break;
      case CGT: output += ((l >  r) ? "true" : "false"); break;
      case CGE: output += ((l >= r) ? "true" : "false"); break;
      case CLT: output += ((l <  r) ? "true" : "false"); break;
      case CLE: output += ((l <= r) ? "true" : "false"); break;
      case CEQ: output += ((l == r) ? "true" : "false"); break;
      case CNE: output += ((l != r) ? "true" : "false"); break;
      case BND: output += (l  & r).to_string(); break;
      case BIO: output += (l  | r).to_string(); break;
      case BXO: output += (l  ^ r).to_string(); break;
      case LND: output += ((l && r) ? "true" : "false"); break;
      case LIO: output += ((l || r) ? "true" : "false"); break;
      case PIN: output += "++" + l.to_string() + ": " + (++ArbInt(l)).to_string() + "  ++" + r.to_string() + ": " + (++ArbInt(r)).to_string(); break;
      case OIN: output += l.to_string() + "++: " + (ArbInt(l)++).to_string() + "  " + r.to_string() + "++: " + (ArbInt(r)++).to_string();  break;
      case PDE: output += "--" + l.to_string() + ": " + (--ArbInt(l)).to_string() + "  --" + r.to_string() + ": " + (--ArbInt(r)).to_string(); break;
      case ODE: output += l.to_string() + "--: " + (ArbInt(l)--).to_string() + "  " + r.to_string() + "--: " + (ArbInt(r)--).to_string();  break;
      case BCM: output += "~" + l.to_string() + ": " + (~l).to_string() + "  ~" + r.to_string() + ": " + (~r).to_string(); break;
      //*/
      /** /
      case ADD:
        test = l + r;
        retval = (l == (test - r));
        output += test.to_string() + " (" + l.to_string() + " == " + "(" + test.to_string() + " - " + r.to_string() + "))";
      break;
      case SUB:
        test = l - r;
        retval = (l == (test + r));
        output += test.to_string() + " (" + l.to_string() + " == " + "(" + test.to_string() + " + " + r.to_string() + "))";
      break;
      case MUL:
        test = l * r;
        retval = (((r == ArbInt<T>(0)) && (test == ArbInt<T>(0))) || (l == (test / r)));
        output += test.to_string() + " (" + ((r == ArbInt<T>(0)) ? "0 == 0" : l.to_string() + " == " + "(" + test.to_string() + " / " + r.to_string()) + "))";
      break;
      case DIV:
        test = l / r;
        retval = (l == (test * r + (l % r)));
        output += test.to_string() + " (" + l.to_string() + " == (" + test.to_string() + " * " + r.to_string() + "(" + (l % r).to_string() + ")))";
      break;
      case MOD:
        test = l % r;
        retval = (test == l - ((l / r) * r));
        output += test.to_string() + " (" + test.to_string() + " == " + l.to_string() + " - ((" + l.to_string() + " / " + r.to_string() + ") * " + r.to_string() + "))";
      break;
      //*/
    }
  } catch (Exception& e) {
    string errtype;
    output += "Exception (" + e.type(errtype) + "): " + string(e.what());
    retval = false;
  } catch (exception& e) {
    string errtype;
    output += "Exception (" + demangle(errtype, e) + "): " + string(e.what());
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
