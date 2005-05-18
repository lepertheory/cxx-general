/*****************************************************************************
 * SafeInt-test.cxx
 *****************************************************************************
 * Tests the class SafeInt.
 *****************************************************************************/

// STL includes.
#include <iostream>
#include <limits>

// Internal includes.
#include "SafeInt.hxx"
#include "Exception.hxx"
#include "toString.hxx"

// Namespace declarations.
using namespace std;
using namespace DAC;

// Operations.
enum Op {
  OP_MUL
};

// This is where it all happens.
int main (int argc, char** argv, char** envp);

// Run the gamut on a pair of types.
template <class T1, class T2> void gamut (char const* const type1, char const* const type2);

// Test casting a number from one type to another.
template <class ArgT, class FromT, class ToT> void testVal (ArgT const value);

// Run all tests on two numbers.
template <class T> void test (T const& l, T const& r);

// Cast.
template <class T, class U> void castl (T const& l, U const& r);
template <class T, class U> void castr (T const& l, U const& r);

// Cast and catch any errors.
template <class T, class U> SafeInt<U> catchCast (SafeInt<T> const& value);

// Test all know operations.
template <class T, class U> void testOp (T const& l, Op const op, U const& r);

int main (int argc, char** argv, char** envp) {
  
  try {
    
    SafeInt<signed long long int> test1;
    SafeInt<signed long long int> test2;
    
    cout << "test1: ";
    cin  >> test1;
    cout << "test1: " << test1 << endl;
    cout << "test2: ";
    cin  >> test2;
    cout << "test2: " << test2 << endl;
    
    test(test1, test2);
    
    /*
    gamut<bool,                   bool                  >("bool",                   "bool"                  );
    gamut<bool,                   unsigned char         >("bool",                   "unsigned char"         );
    gamut<bool,                   signed   char         >("bool",                   "signed char"           );
    gamut<bool,                   unsigned short int    >("bool",                   "unsigned short int"    );
    gamut<bool,                   signed   short int    >("bool",                   "signed short int"      );
    gamut<bool,                   unsigned int          >("bool",                   "unsigned int"          );
    gamut<bool,                   signed   int          >("bool",                   "signed int"            );
    gamut<bool,                   unsigned long int     >("bool",                   "unsigned long int"     );
    gamut<bool,                   signed   long int     >("bool",                   "signed long int"       );
    gamut<bool,                   unsigned long long int>("bool",                   "unsigned long long int");
    gamut<bool,                   signed   long long int>("bool",                   "signed long long int"  );
    gamut<unsigned char,          bool                  >("unsigned char",          "bool"                  );
    gamut<unsigned char,          unsigned char         >("unsigned char",          "unsigned char"         );
    gamut<unsigned char,          signed   char         >("unsigned char",          "signed char"           );
    gamut<unsigned char,          unsigned short int    >("unsigned char",          "unsigned short int"    );
    gamut<unsigned char,          signed   short int    >("unsigned char",          "signed short int"      );
    gamut<unsigned char,          unsigned int          >("unsigned char",          "unsigned int"          );
    gamut<unsigned char,          signed   int          >("unsigned char",          "signed int"            );
    gamut<unsigned char,          unsigned long int     >("unsigned char",          "unsigned long int"     );
    gamut<unsigned char,          signed   long int     >("unsigned char",          "signed long int"       );
    gamut<unsigned char,          unsigned long long int>("unsigned char",          "unsigned long long int");
    gamut<unsigned char,          signed   long long int>("unsigned char",          "signed long long int"  );
    gamut<signed   char,          bool                  >("signed char",            "bool"                  );
    gamut<signed   char,          unsigned char         >("signed char",            "unsigned char"         );
    gamut<signed   char,          signed   char         >("signed char",            "signed char"           );
    gamut<signed   char,          unsigned short int    >("signed char",            "unsigned short int"    );
    gamut<signed   char,          signed   short int    >("signed char",            "signed short int"      );
    gamut<signed   char,          unsigned int          >("signed char",            "unsigned int"          );
    gamut<signed   char,          signed   int          >("signed char",            "signed int"            );
    gamut<signed   char,          unsigned long int     >("signed char",            "unsigned long int"     );
    gamut<signed   char,          signed   long int     >("signed char",            "signed long int"       );
    gamut<signed   char,          unsigned long long int>("signed char",            "unsigned long long int");
    gamut<signed   char,          signed   long long int>("signed char",            "signed long long int"  );
    gamut<unsigned short int,     bool                  >("unsigned short int",     "bool"                  );
    gamut<unsigned short int,     unsigned char         >("unsigned short int",     "unsigned char"         );
    gamut<unsigned short int,     signed   char         >("unsigned short int",     "signed char"           );
    gamut<unsigned short int,     unsigned short int    >("unsigned short int",     "unsigned short int"    );
    gamut<unsigned short int,     signed   short int    >("unsigned short int",     "signed short int"      );
    gamut<unsigned short int,     unsigned int          >("unsigned short int",     "unsigned int"          );
    gamut<unsigned short int,     signed   int          >("unsigned short int",     "signed int"            );
    gamut<unsigned short int,     unsigned long int     >("unsigned short int",     "unsigned long int"     );
    gamut<unsigned short int,     signed   long int     >("unsigned short int",     "signed long int"       );
    gamut<unsigned short int,     unsigned long long int>("unsigned short int",     "unsigned long long int");
    gamut<unsigned short int,     signed   long long int>("unsigned short int",     "signed long long int"  );
    gamut<signed   short int,     bool                  >("signed short int",       "bool"                  );
    gamut<signed   short int,     unsigned char         >("signed short int",       "unsigned char"         );
    gamut<signed   short int,     signed   char         >("signed short int",       "signed char"           );
    gamut<signed   short int,     unsigned short int    >("signed short int",       "unsigned short int"    );
    gamut<signed   short int,     signed   short int    >("signed short int",       "signed short int"      );
    gamut<signed   short int,     unsigned int          >("signed short int",       "unsigned int"          );
    gamut<signed   short int,     signed   int          >("signed short int",       "signed int"            );
    gamut<signed   short int,     unsigned long int     >("signed short int",       "unsigned long int"     );
    gamut<signed   short int,     signed   long int     >("signed short int",       "signed long int"       );
    gamut<signed   short int,     unsigned long long int>("signed short int",       "unsigned long long int");
    gamut<signed   short int,     signed   long long int>("signed short int",       "signed long long int"  );
    gamut<unsigned int,           bool                  >("unsigned int",           "bool"                  );
    gamut<unsigned int,           unsigned char         >("unsigned int",           "unsigned char"         );
    gamut<unsigned int,           signed   char         >("unsigned int",           "signed char"           );
    gamut<unsigned int,           unsigned short int    >("unsigned int",           "unsigned short int"    );
    gamut<unsigned int,           signed   short int    >("unsigned int",           "signed short int"      );
    gamut<unsigned int,           unsigned int          >("unsigned int",           "unsigned int"          );
    gamut<unsigned int,           signed   int          >("unsigned int",           "signed int"            );
    gamut<unsigned int,           unsigned long int     >("unsigned int",           "unsigned long int"     );
    gamut<unsigned int,           signed   long int     >("unsigned int",           "signed long int"       );
    gamut<unsigned int,           unsigned long long int>("unsigned int",           "unsigned long long int");
    gamut<unsigned int,           signed   long long int>("unsigned int",           "signed long long int"  );
    gamut<signed   int,           bool                  >("signed int",             "bool"                  );
    gamut<signed   int,           unsigned char         >("signed int",             "unsigned char"         );
    gamut<signed   int,           signed   char         >("signed int",             "signed char"           );
    gamut<signed   int,           unsigned short int    >("signed int",             "unsigned short int"    );
    gamut<signed   int,           signed   short int    >("signed int",             "signed short int"      );
    gamut<signed   int,           unsigned int          >("signed int",             "unsigned int"          );
    gamut<signed   int,           signed   int          >("signed int",             "signed int"            );
    gamut<signed   int,           unsigned long int     >("signed int",             "unsigned long int"     );
    gamut<signed   int,           signed   long int     >("signed int",             "signed long int"       );
    gamut<signed   int,           unsigned long long int>("signed int",             "unsigned long long int");
    gamut<signed   int,           signed   long long int>("signed int",             "signed long long int"  );
    gamut<unsigned long int,      bool                  >("unsigned long int",      "bool"                  );
    gamut<unsigned long int,      unsigned char         >("unsigned long int",      "unsigned char"         );
    gamut<unsigned long int,      signed   char         >("unsigned long int",      "signed char"           );
    gamut<unsigned long int,      unsigned short int    >("unsigned long int",      "unsigned short int"    );
    gamut<unsigned long int,      signed   short int    >("unsigned long int",      "signed short int"      );
    gamut<unsigned long int,      unsigned int          >("unsigned long int",      "unsigned int"          );
    gamut<unsigned long int,      signed   int          >("unsigned long int",      "signed int"            );
    gamut<unsigned long int,      unsigned long int     >("unsigned long int",      "unsigned long int"     );
    gamut<unsigned long int,      signed   long int     >("unsigned long int",      "signed long int"       );
    gamut<unsigned long int,      unsigned long long int>("unsigned long int",      "unsigned long long int");
    gamut<unsigned long int,      signed   long long int>("unsigned long int",      "signed long long int"  );
    gamut<signed   long int,      bool                  >("signed long int",        "bool"                  );
    gamut<signed   long int,      unsigned char         >("signed long int",        "unsigned char"         );
    gamut<signed   long int,      signed   char         >("signed long int",        "signed char"           );
    gamut<signed   long int,      unsigned short int    >("signed long int",        "unsigned short int"    );
    gamut<signed   long int,      signed   short int    >("signed long int",        "signed short int"      );
    gamut<signed   long int,      unsigned int          >("signed long int",        "unsigned int"          );
    gamut<signed   long int,      signed   int          >("signed long int",        "signed int"            );
    gamut<signed   long int,      unsigned long int     >("signed long int",        "unsigned long int"     );
    gamut<signed   long int,      signed   long int     >("signed long int",        "signed long int"       );
    gamut<signed   long int,      unsigned long long int>("signed long int",        "unsigned long long int");
    gamut<signed   long int,      signed   long long int>("signed long int",        "signed long long int"  );
    gamut<unsigned long long int, bool                  >("unsigned long long int", "bool"                  );
    gamut<unsigned long long int, unsigned char         >("unsigned long long int", "unsigned char"         );
    gamut<unsigned long long int, signed   char         >("unsigned long long int", "signed char"           );
    gamut<unsigned long long int, unsigned short int    >("unsigned long long int", "unsigned short int"    );
    gamut<unsigned long long int, signed   short int    >("unsigned long long int", "signed short int"      );
    gamut<unsigned long long int, unsigned int          >("unsigned long long int", "unsigned int"          );
    gamut<unsigned long long int, signed   int          >("unsigned long long int", "signed int"            );
    gamut<unsigned long long int, unsigned long int     >("unsigned long long int", "unsigned long int"     );
    gamut<unsigned long long int, signed   long int     >("unsigned long long int", "signed long int"       );
    gamut<unsigned long long int, unsigned long long int>("unsigned long long int", "unsigned long long int");
    gamut<unsigned long long int, signed   long long int>("unsigned long long int", "signed long long int"  );
    gamut<signed   long long int, bool                  >("signed long long int",   "bool"                  );
    gamut<signed   long long int, unsigned char         >("signed long long int",   "unsigned char"         );
    gamut<signed   long long int, signed   char         >("signed long long int",   "signed char"           );
    gamut<signed   long long int, unsigned short int    >("signed long long int",   "unsigned short int"    );
    gamut<signed   long long int, signed   short int    >("signed long long int",   "signed short int"      );
    gamut<signed   long long int, unsigned int          >("signed long long int",   "unsigned int"          );
    gamut<signed   long long int, signed   int          >("signed long long int",   "signed int"            );
    gamut<signed   long long int, unsigned long int     >("signed long long int",   "unsigned long int"     );
    gamut<signed   long long int, signed   long int     >("signed long long int",   "signed long int"       );
    gamut<signed   long long int, unsigned long long int>("signed long long int",   "unsigned long long int");
    gamut<signed   long long int, signed   long long int>("signed long long int",   "signed long long int"  );
    */
    
  } catch (Exception& e) {
    
    cout << "Exception: [" << e.type() << "]: " << e << endl;
    exit(1);
    
  } catch (exception& e) {
    
    cout << "Unexpected exception: [" << demangle(e) << "]: " << e.what() << endl;
    exit(1);
    
  } catch (...) {
    
    cout << "Unexpected exception." << endl;
    exit(1);
    
  }
  
}

template <class T1, class T2> void gamut (char const* const type1, char const* const type2) {
  
  cout << "Running gamut on " << type1 << " : " << type2 << endl;
  
  cout << "  Type 1 range: " << toString(numeric_limits<T1>::min()) << " to " << toString(numeric_limits<T1>::max()) << endl;
  cout << "  Type 2 range: " << toString(numeric_limits<T2>::min()) << " to " << toString(numeric_limits<T2>::max()) << endl;
  
  if (numeric_limits<T1>::digits >= numeric_limits<T2>::digits) {
    
    if (numeric_limits<T1>::is_signed) {
      if (numeric_limits<T2>::is_signed) {
        testVal<T2,  T1, T2>(numeric_limits<T2>::min());
        testVal<int, T1, T2>(-1);
        testVal<int, T1, T2>(0);
        testVal<int, T1, T2>(1);
        testVal<T2,  T1, T2>(numeric_limits<T2>::max());
      } else {
        testVal<int, T1, T2>(0);
        testVal<int, T1, T2>(1);
        testVal<T2,  T1, T2>(numeric_limits<T2>::max());
      }
    } else {
      if (numeric_limits<T2>::is_signed) {
        testVal<int, T1, T2>(0);
        testVal<int, T1, T2>(1);
        testVal<T2,  T1, T2>(numeric_limits<T2>::max());
      } else {
        testVal<int, T1, T2>(0);
        testVal<int, T1, T2>(1);
        testVal<T2,  T1, T2>(numeric_limits<T2>::max());
      }
    }
    
  } else {
    
    if (numeric_limits<T1>::is_signed) {
      if (numeric_limits<T2>::is_signed) {
        testVal<T1,  T1, T2>(numeric_limits<T1>::min());
        testVal<int, T1, T2>(-1);
        testVal<int, T1, T2>(0);
        testVal<int, T1, T2>(1);
        testVal<T1,  T1, T2>(numeric_limits<T1>::max());
      } else {
        testVal<int, T1, T2>(0);
        testVal<int, T1, T2>(1);
        testVal<T1,  T1, T2>(numeric_limits<T1>::max());
      }
    } else {
      if (numeric_limits<T2>::is_signed) {
        testVal<int, T1, T2>(0);
        testVal<int, T1, T2>(1);
        testVal<T1,  T1, T2>(numeric_limits<T1>::max());
      } else {
        testVal<int, T1, T2>(0);
        testVal<int, T1, T2>(1);
        testVal<T1,  T1, T2>(numeric_limits<T1>::max());
      }
    }
    
  }
  
  cout << endl;
  
}

template <class ArgT, class FromT, class ToT> void testVal (ArgT const value) {
  
  cout << "  " << toString(value) << ": " << toString(static_cast<ToT>(static_cast<FromT>(value))) << endl;
  
}

template <class T> void test (T const& l, T const& r) {
  
  try {
    castl(SafeInt<bool                  >(l), r);
  } catch (Exception& e) {
    cout << "Exception: [" << e.type() << "]: " << e << endl;
  } catch (exception& e) {
    cout << "Unexpected exception: [" << demangle(e) << "]: " << e.what() << endl;
  } catch (...) {
    cout << "Unexpected exception." << endl;
  }
  try {
    castl(SafeInt<signed   char         >(l), r);
  } catch (Exception& e) {
    cout << "Exception: [" << e.type() << "]: " << e << endl;
  } catch (exception& e) {
    cout << "Unexpected exception: [" << demangle(e) << "]: " << e.what() << endl;
  } catch (...) {
    cout << "Unexpected exception." << endl;
  }
  try {
    castl(SafeInt<unsigned char         >(l), r);
  } catch (Exception& e) {
    cout << "Exception: [" << e.type() << "]: " << e << endl;
  } catch (exception& e) {
    cout << "Unexpected exception: [" << demangle(e) << "]: " << e.what() << endl;
  } catch (...) {
    cout << "Unexpected exception." << endl;
  }
  try {
    castl(SafeInt<signed   short int    >(l), r);
  } catch (Exception& e) {
    cout << "Exception: [" << e.type() << "]: " << e << endl;
  } catch (exception& e) {
    cout << "Unexpected exception: [" << demangle(e) << "]: " << e.what() << endl;
  } catch (...) {
    cout << "Unexpected exception." << endl;
  }
  try {
    castl(SafeInt<unsigned short int    >(l), r);
  } catch (Exception& e) {
    cout << "Exception: [" << e.type() << "]: " << e << endl;
  } catch (exception& e) {
    cout << "Unexpected exception: [" << demangle(e) << "]: " << e.what() << endl;
  } catch (...) {
    cout << "Unexpected exception." << endl;
  }
  try {
    castl(SafeInt<signed   int          >(l), r);
  } catch (Exception& e) {
    cout << "Exception: [" << e.type() << "]: " << e << endl;
  } catch (exception& e) {
    cout << "Unexpected exception: [" << demangle(e) << "]: " << e.what() << endl;
  } catch (...) {
    cout << "Unexpected exception." << endl;
  }
  try {
    castl(SafeInt<unsigned int          >(l), r);
  } catch (Exception& e) {
    cout << "Exception: [" << e.type() << "]: " << e << endl;
  } catch (exception& e) {
    cout << "Unexpected exception: [" << demangle(e) << "]: " << e.what() << endl;
  } catch (...) {
    cout << "Unexpected exception." << endl;
  }
  try {
    castl(SafeInt<signed   long int     >(l), r);
  } catch (Exception& e) {
    cout << "Exception: [" << e.type() << "]: " << e << endl;
  } catch (exception& e) {
    cout << "Unexpected exception: [" << demangle(e) << "]: " << e.what() << endl;
  } catch (...) {
    cout << "Unexpected exception." << endl;
  }
  try {
    castl(SafeInt<unsigned long int     >(l), r);
  } catch (Exception& e) {
    cout << "Exception: [" << e.type() << "]: " << e << endl;
  } catch (exception& e) {
    cout << "Unexpected exception: [" << demangle(e) << "]: " << e.what() << endl;
  } catch (...) {
    cout << "Unexpected exception." << endl;
  }
  try {
    castl(SafeInt<signed   long long int>(l), r);
  } catch (Exception& e) {
    cout << "Exception: [" << e.type() << "]: " << e << endl;
  } catch (exception& e) {
    cout << "Unexpected exception: [" << demangle(e) << "]: " << e.what() << endl;
  } catch (...) {
    cout << "Unexpected exception." << endl;
  }
  try {
    castl(SafeInt<unsigned long long int>(l), r);
  } catch (Exception& e) {
    cout << "Exception: [" << e.type() << "]: " << e << endl;
  } catch (exception& e) {
    cout << "Unexpected exception: [" << demangle(e) << "]: " << e.what() << endl;
  } catch (...) {
    cout << "Unexpected exception." << endl;
  }
  
}

template <class T, class U> void castl (T const& l, U const& r) {
  
  try {
    castr(l, SafeInt<bool                  >(r));
  } catch (Exception& e) {
    cout << "Exception: [" << e.type() << "]: " << e << endl;
  } catch (exception& e) {
    cout << "Unexpected exception: [" << demangle(e) << "]: " << e.what() << endl;
  } catch (...) {
    cout << "Unexpected exception." << endl;
  }
  try {
    castr(l, SafeInt<signed   char         >(r));
  } catch (Exception& e) {
    cout << "Exception: [" << e.type() << "]: " << e << endl;
  } catch (exception& e) {
    cout << "Unexpected exception: [" << demangle(e) << "]: " << e.what() << endl;
  } catch (...) {
    cout << "Unexpected exception." << endl;
  }
  try {
    castr(l, SafeInt<unsigned char         >(r));
  } catch (Exception& e) {
    cout << "Exception: [" << e.type() << "]: " << e << endl;
  } catch (exception& e) {
    cout << "Unexpected exception: [" << demangle(e) << "]: " << e.what() << endl;
  } catch (...) {
    cout << "Unexpected exception." << endl;
  }
  try {
    castr(l, SafeInt<signed   short int    >(r));
  } catch (Exception& e) {
    cout << "Exception: [" << e.type() << "]: " << e << endl;
  } catch (exception& e) {
    cout << "Unexpected exception: [" << demangle(e) << "]: " << e.what() << endl;
  } catch (...) {
    cout << "Unexpected exception." << endl;
  }
  try {
    castr(l, SafeInt<unsigned short int    >(r));
  } catch (Exception& e) {
    cout << "Exception: [" << e.type() << "]: " << e << endl;
  } catch (exception& e) {
    cout << "Unexpected exception: [" << demangle(e) << "]: " << e.what() << endl;
  } catch (...) {
    cout << "Unexpected exception." << endl;
  }
  try {
    castr(l, SafeInt<signed   int          >(r));
  } catch (Exception& e) {
    cout << "Exception: [" << e.type() << "]: " << e << endl;
  } catch (exception& e) {
    cout << "Unexpected exception: [" << demangle(e) << "]: " << e.what() << endl;
  } catch (...) {
    cout << "Unexpected exception." << endl;
  }
  try {
    castr(l, SafeInt<unsigned int          >(r));
  } catch (Exception& e) {
    cout << "Exception: [" << e.type() << "]: " << e << endl;
  } catch (exception& e) {
    cout << "Unexpected exception: [" << demangle(e) << "]: " << e.what() << endl;
  } catch (...) {
    cout << "Unexpected exception." << endl;
  }
  try {
    castr(l, SafeInt<signed   long int     >(r));
  } catch (Exception& e) {
    cout << "Exception: [" << e.type() << "]: " << e << endl;
  } catch (exception& e) {
    cout << "Unexpected exception: [" << demangle(e) << "]: " << e.what() << endl;
  } catch (...) {
    cout << "Unexpected exception." << endl;
  }
  try {
    castr(l, SafeInt<unsigned long int     >(r));
  } catch (Exception& e) {
    cout << "Exception: [" << e.type() << "]: " << e << endl;
  } catch (exception& e) {
    cout << "Unexpected exception: [" << demangle(e) << "]: " << e.what() << endl;
  } catch (...) {
    cout << "Unexpected exception." << endl;
  }
  try {
    castr(l, SafeInt<signed   long long int>(r));
  } catch (Exception& e) {
    cout << "Exception: [" << e.type() << "]: " << e << endl;
  } catch (exception& e) {
    cout << "Unexpected exception: [" << demangle(e) << "]: " << e.what() << endl;
  } catch (...) {
    cout << "Unexpected exception." << endl;
  }
  try {
    castr(l, SafeInt<unsigned long long int>(r));
  } catch (Exception& e) {
    cout << "Exception: [" << e.type() << "]: " << e << endl;
  } catch (exception& e) {
    cout << "Unexpected exception: [" << demangle(e) << "]: " << e.what() << endl;
  } catch (...) {
    cout << "Unexpected exception." << endl;
  }
  
}

template <class T, class U> void castr (T const& l, U const& r) {
  
  testOp(l, OP_MUL, r);
  
}

template <class T, class U> void testOp (T const& l, Op const op, U const& r) {
  
  try {
    
    cout << l;
    switch (op) {
      case OP_MUL: cout << " * "; break;
    }
    cout << r << " = ";
    switch (op) {
      case OP_MUL: cout << (l * r); break;
    }
    cout << endl;
    
  } catch (Exception& e) {
    
    cout << "Exception: [" << e.type() << "]: " << e << endl;
    
  } catch (exception& e) {
    
    cout << "Unexpected exception: [" << demangle(e) << "]: " << e.what() << endl;
    
  } catch (...) {
    
    cout << "Unexpected exception." << endl;
    
  }
  
}

template <class T, class U> SafeInt<U> catchCast (SafeInt<T> const& value) {
  
  try {
    
    return SafeInt<U>(value);
    
  } catch (Exception& e) {
    
    cout << "Exception: [" << e.type() << "]: " << e << endl;
    
  } catch (exception& e) {
    
    cout << "Unexpected exception: [" << demangle(e) << "]: " << e.what() << endl;
    
  } catch (...) {
    
    cout << "Unexpected exception." << endl;
    
  }
  
  return SafeInt<U>(0);
  
}
