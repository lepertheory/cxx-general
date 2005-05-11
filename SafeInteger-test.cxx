/*****************************************************************************
 * text.cxx
 *****************************************************************************
 * Tests the class SafeInteger.
 *****************************************************************************/

// STL includes.
#include <iostream>
#include <vector>

// Internal includes.
#include "SafeInteger.hxx"
#include "toString.hxx"
#include "Exception.hxx"
#include "demangle.hxx"
#include "Arb.hxx"

// Namespace declarations.
using namespace std;
using namespace DAC;

enum Operation { MUL, DIV, MOD, ADD, SUB };

// Run all tests on two given types.
template <class T1, class T2> void gamut (char const* const type1, char const* const type2);

// Test operation.
template <class LT, class RT> void testOp (LT const l, Operation const op, RT const& r);

// This is where it all happens.
int main (int argc, char** argv, char** envp) {
  
  try {
    
    gamut<bool,               bool              >("bool              ", "bool              ");
    gamut<bool,               unsigned char     >("bool              ", "unsigned char     ");
    gamut<bool,               signed   char     >("bool              ", "signed   char     ");
    gamut<bool,               unsigned short int>("bool              ", "unsigned short int");
    gamut<bool,               signed   short int>("bool              ", "signed   short int");
    gamut<bool,               unsigned int      >("bool              ", "unsigned int      ");
    gamut<bool,               signed   int      >("bool              ", "signed   int      ");
    gamut<bool,               unsigned long int >("bool              ", "unsigned long int ");
    gamut<bool,               signed   long int >("bool              ", "signed   long int ");
    gamut<unsigned char,      bool              >("unsigned char     ", "bool              ");
    gamut<unsigned char,      unsigned char     >("unsigned char     ", "unsigned char     ");
    gamut<unsigned char,      signed   char     >("unsigned char     ", "signed   char     ");
    gamut<unsigned char,      unsigned short int>("unsigned char     ", "unsigned short int");
    gamut<unsigned char,      signed   short int>("unsigned char     ", "signed   short int");
    gamut<unsigned char,      unsigned int      >("unsigned char     ", "unsigned int      ");
    gamut<unsigned char,      signed   int      >("unsigned char     ", "signed   int      ");
    gamut<unsigned char,      unsigned long int >("unsigned char     ", "unsigned long int ");
    gamut<unsigned char,      signed   long int >("unsigned char     ", "signed   long int ");
    gamut<signed   char,      bool              >("signed   char     ", "bool              ");
    gamut<signed   char,      unsigned char     >("signed   char     ", "unsigned char     ");
    gamut<signed   char,      signed   char     >("signed   char     ", "signed   char     ");
    gamut<signed   char,      unsigned short int>("signed   char     ", "unsigned short int");
    gamut<signed   char,      signed   short int>("signed   char     ", "signed   short int");
    gamut<signed   char,      unsigned int      >("signed   char     ", "unsigned int      ");
    gamut<signed   char,      signed   int      >("signed   char     ", "signed   int      ");
    gamut<signed   char,      unsigned long int >("signed   char     ", "unsigned long int ");
    gamut<signed   char,      signed   long int >("signed   char     ", "signed   long int ");
    gamut<unsigned short int, bool              >("unsigned short int", "bool              ");
    gamut<unsigned short int, unsigned char     >("unsigned short int", "unsigned char     ");
    gamut<unsigned short int, signed   char     >("unsigned short int", "signed   char     ");
    gamut<unsigned short int, unsigned short int>("unsigned short int", "unsigned short int");
    gamut<unsigned short int, signed   short int>("unsigned short int", "signed   short int");
    gamut<unsigned short int, unsigned int      >("unsigned short int", "unsigned int      ");
    gamut<unsigned short int, signed   int      >("unsigned short int", "signed   int      ");
    gamut<unsigned short int, unsigned long int >("unsigned short int", "unsigned long int ");
    gamut<unsigned short int, signed   long int >("unsigned short int", "signed   long int ");
    gamut<signed   short int, bool              >("signed   short int", "bool              ");
    gamut<signed   short int, unsigned char     >("signed   short int", "unsigned char     ");
    gamut<signed   short int, signed   char     >("signed   short int", "signed   char     ");
    gamut<signed   short int, unsigned short int>("signed   short int", "unsigned short int");
    gamut<signed   short int, signed   short int>("signed   short int", "signed   short int");
    gamut<signed   short int, unsigned int      >("signed   short int", "unsigned int      ");
    gamut<signed   short int, signed   int      >("signed   short int", "signed   int      ");
    gamut<signed   short int, unsigned long int >("signed   short int", "unsigned long int ");
    gamut<signed   short int, signed   long int >("signed   short int", "signed   long int ");
    gamut<unsigned int,       bool              >("unsigned int      ", "bool              ");
    gamut<unsigned int,       unsigned char     >("unsigned int      ", "unsigned char     ");
    gamut<unsigned int,       signed   char     >("unsigned int      ", "signed   char     ");
    gamut<unsigned int,       unsigned short int>("unsigned int      ", "unsigned short int");
    gamut<unsigned int,       signed   short int>("unsigned int      ", "signed   short int");
    gamut<unsigned int,       unsigned int      >("unsigned int      ", "unsigned int      ");
    gamut<unsigned int,       signed   int      >("unsigned int      ", "signed   int      ");
    gamut<unsigned int,       unsigned long int >("unsigned int      ", "unsigned long int ");
    gamut<unsigned int,       signed   long int >("unsigned int      ", "signed   long int ");
    gamut<signed   int,       bool              >("signed   int      ", "bool              ");
    gamut<signed   int,       unsigned char     >("signed   int      ", "unsigned char     ");
    gamut<signed   int,       signed   char     >("signed   int      ", "signed   char     ");
    gamut<signed   int,       unsigned short int>("signed   int      ", "unsigned short int");
    gamut<signed   int,       signed   short int>("signed   int      ", "signed   short int");
    gamut<signed   int,       unsigned int      >("signed   int      ", "unsigned int      ");
    gamut<signed   int,       signed   int      >("signed   int      ", "signed   int      ");
    gamut<signed   int,       unsigned long int >("signed   int      ", "unsigned long int ");
    gamut<signed   int,       signed   long int >("signed   int      ", "signed   long int ");
    gamut<unsigned long int,  bool              >("unsigned long int ", "bool              ");
    gamut<unsigned long int,  unsigned char     >("unsigned long int ", "unsigned char     ");
    gamut<unsigned long int,  signed   char     >("unsigned long int ", "signed   char     ");
    gamut<unsigned long int,  unsigned short int>("unsigned long int ", "unsigned short int");
    gamut<unsigned long int,  signed   short int>("unsigned long int ", "signed   short int");
    gamut<unsigned long int,  unsigned int      >("unsigned long int ", "unsigned int      ");
    gamut<unsigned long int,  signed   int      >("unsigned long int ", "signed   int      ");
    gamut<unsigned long int,  unsigned long int >("unsigned long int ", "unsigned long int ");
    gamut<unsigned long int,  signed   long int >("unsigned long int ", "signed   long int ");
    gamut<signed   long int,  bool              >("signed   long int ", "bool              ");
    gamut<signed   long int,  unsigned char     >("signed   long int ", "unsigned char     ");
    gamut<signed   long int,  signed   char     >("signed   long int ", "signed   char     ");
    gamut<signed   long int,  unsigned short int>("signed   long int ", "unsigned short int");
    gamut<signed   long int,  signed   short int>("signed   long int ", "signed   short int");
    gamut<signed   long int,  unsigned int      >("signed   long int ", "unsigned int      ");
    gamut<signed   long int,  signed   int      >("signed   long int ", "signed   int      ");
    gamut<signed   long int,  unsigned long int >("signed   long int ", "unsigned long int ");
    gamut<signed   long int,  signed   long int >("signed   long int ", "signed   long int ");
  
  } catch (Exception& e) {
    
    cout << "Exception: " << e << endl;
    exit(1);
    
  } catch (...) {
    
    cout << "Unexpected exception." << endl;
    exit(1);
    
  }
  
};

// Run all tests on two given types.
template <class T1, class T2> void gamut (char const* const type1, char const* const type2) {
  
  vector<Arb> edge1;
  vector<Arb> edge2;
  
  try {
    
    if (numeric_limits<T1>::is_signed) {
      edge1.push_back(Arb(numeric_limits<T1>::min()));
    }
    edge1.push_back(Arb(0));
    
    /*
    if (numeric_limits<T1>::is_signed) {
      edge1.push_back(Arb(numeric_limits<T1>::min()));
      edge1.push_back(Arb(numeric_limits<T1>::min() + 1));
      edge1.push_back(Arb(-1));
    }
    edge1.push_back(Arb(0));
    edge1.push_back(Arb(1));
    edge1.push_back(Arb(numeric_limits<T1>::max() - 1));
    edge1.push_back(Arb(numeric_limits<T1>::max()));
    
    if (numeric_limits<T2>::is_signed) {
      edge2.push_back(Arb(numeric_limits<T2>::min()));
      edge2.push_back(Arb(numeric_limits<T2>::min() + 1));
      edge2.push_back(Arb(-1));
    }
    edge2.push_back(Arb(0));
    edge2.push_back(Arb(1));
    edge2.push_back(Arb(numeric_limits<T2>::max() - 1));
    edge2.push_back(Arb(numeric_limits<T2>::max()));
    */
    
    
  } catch (SafeIntegerErrors::Base& e) {
    cout << e << endl;
  } catch (Exception& e) {
    cout << "Unexpected error: " << e << endl;
  } catch (exception& e) {
    cout << "Unexpected error: Type: " << demangle(e) << "  Reason: " << e.what() << endl;
  } catch (...) {
    cout << "Unexpected unknown error." << endl;
  }
  
}

// Test operation.
template <class LT, class RT> void testOp (LT const l, Operation const op, RT const r) {
  
  try {
    
    cout << l;
    switch (op) {
      case MUL: cout << " * "; break;
      case DIV: cout << " / "; break;
      case MOD: cout << " % "; break;
      case SUB: cout << " - "; break;
      case ADD: cout << " + "; break;
    }
    cout << r << " = ";
    switch (op) {
      case MUL: cout << (l * r); break;
      case DIV: cout << (l / r); break;
      case MOD: cout << (l % r); break;
      case SUB: cout << (l - r); break;
      case ADD: cout << (l + r); break;
    }
    
  } catch (SafeIntegerErrors::Base& e) {
    cout << e;
  } catch (Exception& e) {
    cout << "Unexpected error: " << e;
  } catch (exception& e) {
    cout << "Unexpected error: Type: " << demangle(e) << "  Reason: " << e.what();
  } catch (...) {
    cout << "Unexpected unknown error.";
  }
  
  cout << endl;
  
}
