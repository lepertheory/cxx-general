/*****************************************************************************
 * test.cxx
 *****************************************************************************
 * Tests the class ArbInt.
 *****************************************************************************/

// STL includes.
#include <iostream>

// Internal includes.
#include "src/ArbInt.hxx"
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
  LNT,
  BCM
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
        testOp(test1, LNT, test2);
        testOp(test1, BCM, test2);
        /** /
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
      case LNT: output += "   !"; break;
      case BCM: output += "   ~"; break;
    }
    output += r.toString() + " = ";
    switch (op) {
      /**/
      case MUL: output += (l  * r).toString(); break;
      case DIV: output += (l  / r).toString(); break;
      case MOD: output += (l  % r).toString(); break;
      case ADD: output += (l  + r).toString(); break;
      case SUB: output += (l  - r).toString(); break;
      case SHL: output += ((r > ArbInt<T>(10000)) ? "Skipped, huge." : (l << r).toString()); break;
      case SHR: output += (l >> r).toString(); break;
      case CGT: output += ((l >  r) ? "true" : "false"); break;
      case CGE: output += ((l >= r) ? "true" : "false"); break;
      case CLT: output += ((l <  r) ? "true" : "false"); break;
      case CLE: output += ((l <= r) ? "true" : "false"); break;
      case CEQ: output += ((l == r) ? "true" : "false"); break;
      case CNE: output += ((l != r) ? "true" : "false"); break;
      case BND: output += (l  & r).toString(); break;
      case BIO: output += (l  | r).toString(); break;
      case BXO: output += (l  ^ r).toString(); break;
      case LND: output += ((l && r) ? "true" : "false"); break;
      case LIO: output += ((l || r) ? "true" : "false"); break;
      case PIN: output += "++" + l.toString() + ": " + (++ArbInt<T>(l)).toString() + "  ++" + r.toString() + ": " + (++ArbInt<T>(r)).toString(); break;
      case OIN: output += l.toString() + "++: " + (ArbInt<T>(l)++).toString() + "  " + r.toString() + "++: " + (ArbInt<T>(r)++).toString();  break;
      case PDE: output += "--" + l.toString() + ": " + (--ArbInt<T>(l)).toString() + "  --" + r.toString() + ": " + (--ArbInt<T>(r)).toString(); break;
      case ODE: output += l.toString() + "--: " + (ArbInt<T>(l)--).toString() + "  " + r.toString() + "--: " + (ArbInt<T>(r)--).toString();  break;
      case LNT: output += "!" + l.toString() + ": " + (!l ? "true" : "false" ) + "  !" + r.toString() + ": " + (!r ? "true" : "false"); break;
      case BCM: output += "~" + l.toString() + ": " + (~l).toString() + "  ~" + r.toString() + ": " + (~r).toString(); break;
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
