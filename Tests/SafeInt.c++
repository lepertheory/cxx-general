/*****************************************************************************
 * SafeInt.c++
 *****************************************************************************
 * Tests the class SafeInt.
 *****************************************************************************/

// Config.
#define RANDOM_TESTS 1000

// STL includes.
#include <iostream>
#include <limits>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>

// Internal includes.
#include "SafeInt.h++"
#include "Exception.h++"
#include "to_string.h++"
#include "demangle.h++"

// Namespace declarations.
using namespace std;
using namespace DAC;

// Operations.
enum Op {
  OP_MUL,
  OP_DIV,
  OP_MOD,
  OP_ADD,
  OP_SUB,
  OP_SHL,
  OP_SHR,
  OP_CGT,
  OP_CGE,
  OP_CLT,
  OP_CLE,
  OP_CEQ,
  OP_CNE,
  OP_BAN,
  OP_BIO,
  OP_BXO,
  OP_LAN,
  OP_LIO
};

// Relationship types.
enum RelType {
  SE_SE, SE_UE, SS_SL, SS_UL, SL_SS, SL_US,
  UE_UE, UE_SE, US_SL, US_UL, UL_SS, UL_US, INT_ONLY
};

// Determine the relationship between two types.
template <class T, class U> class Relationship {
  public:
    static RelType const value =
      (numeric_limits<T>::is_integer && numeric_limits<U>::is_integer) ? (
        (numeric_limits<T>::digits > numeric_limits<U>::digits) ? (
          numeric_limits<T>::is_signed ? (
            numeric_limits<U>::is_signed ? (
              SL_SS
            ) : (
              SL_US
            )
          ) : (
            numeric_limits<U>::is_signed ? (
              UL_SS
            ) : (
              UL_US
            )
          )
        ) : (
          (numeric_limits<T>::digits < numeric_limits<U>::digits) ? (
            numeric_limits<T>::is_signed ? (
              numeric_limits<U>::is_signed ? (
                SS_SL
              ) : (
                SS_UL
              )
            ) : ( 
              std::numeric_limits<U>::is_signed ? (
                US_SL
              ) : (
                US_UL
              )
            )
          ) : (
            numeric_limits<T>::is_signed ? (
              numeric_limits<U>::is_signed ? (
                SE_SE
              ) : (
                SE_UE
              )
            ) : (
              numeric_limits<U>::is_signed ? (
                UE_SE
              ) : (
                UE_UE
              )
            )
          )
        )
      ) : (
        INT_ONLY
      )
    ;
};

// Edge case builder.
template <class T, RelType> class EdgeBuilder;
template <class T> class EdgeBuilder<T, SE_SE> {
  public:
    static void op (vector<T>& edges) {
      edges.clear();
      edges.push_back(numeric_limits<T>::min()    );
      edges.push_back(numeric_limits<T>::min() + 1);
      edges.push_back(                          -1);
      edges.push_back(                           0);
      edges.push_back(                           1);
      edges.push_back(numeric_limits<T>::max() - 1);
      edges.push_back(numeric_limits<T>::max()    );
    };
};
template <class T> class EdgeBuilder<T, UE_UE> {
  public:
    static void op (vector<T>& edges) {
      edges.clear();
      edges.push_back(0                           );
      edges.push_back(1                           );
      edges.push_back(numeric_limits<T>::max() - 1);
      edges.push_back(numeric_limits<T>::max()    );
    };
};

/*
 * Generate a random number within the bounds of a type.
 */
template <class T> T randrange () {
  
  return static_cast<T>((static_cast<long double>(numeric_limits<T>::max()) - numeric_limits<T>::min()) * (rand() / (static_cast<long double>(RAND_MAX) + 1.0)) + numeric_limits<T>::min());
  
}

/*
 * Test conversion constructor with native type.
 */
template <class T> bool test_conversionConstructorNative (T const value) {
  SafeInt<T> test(value);
  if (test != value) {
    cout << "Conversion constructor of native type does not set " << to_string(value) << " properly, set as " << to_string(test) << ".\n";
    return false;
  }
  return true;
}

/*
 * Test copy constructor.
 */
template <class T> bool test_copyConstructor (T const value) {
  SafeInt<T> test1(value);
  SafeInt<T> test2(test1);
  if (test1 != test2) {
    cout << "Copy constructor does not copy " << to_string(test1) << " properly, copied as " << to_string(test2) << ".\n";
    return false;
  }
  return true;
}

/*
 * Test pre-increment operator.
 */
template <class T> bool test_preIncrementOperator (T const value) {
  SafeInt<T> test  (value);
  SafeInt<T> before(test );
  bool threw(false);
  try {
    if (++test == before) {
      cout << "Pre-increment operator does not increment before returning value.\n";
      return false;
    }
  } catch (typename SafeInt<T>::Errors::Overflow) {
    threw = true;
  }
  if (value == numeric_limits<T>::max()) {
    if (!threw) {
      cout << "Pre-increment operator did not throw an overflow error on " << to_string(before) << ".\n";
      return false;
    }
  } else {
    if (threw) {
      cout << "Pre-increment operator threw an unexpected overflow error on " << to_string(before) << ".\n";
      return false;
    }
    if (test != before + 1) {
      cout << "Pre-increment operator does not increment. " << to_string(before) << " -> " << to_string(test) << "\n";
      return false;
    }
  }
  return true;
}

/*
 * Test post-increment operator.
 */
template <class T> bool test_postIncrementOperator (T const value) {
  SafeInt<T> test  (value);
  SafeInt<T> before(test );
  bool threw(false);
  try {
    if (test++ != before) {
      cout << "Post-increment operator changes before returning value.\n";
      return false;
    }
  } catch (typename SafeInt<T>::Errors::Overflow) {
    threw = true;
  }
  if (value == numeric_limits<T>::max()) {
    if (!threw) {
      cout << "Post-increment operator did not throw an overflow error on " << to_string(before) << ".\n";
      return false;
    }
  } else {
    if (threw) {
      cout << "Post-increment operator threw an unexpected overflow error on " << to_string(before) << ".\n";
      return false;
    }
    if (test != before + 1) {
      cout << "Post-increment operator does not increment. " << to_string(before) << " -> " << to_string(test) << "\n";
      return false;
    }
  }
  return true;
}

/*
 * Test pre-decrement operator.
 */
template <class T> bool test_preDecrementOperator (T const value) {
  SafeInt<T> test  (value);
  SafeInt<T> before(test );
  bool threw(false);
  try {
    if (--test == before) {
      cout << "Pre-decrement operator does not decrement before returning value.\n";
      return false;
    }
  } catch (typename SafeInt<T>::Errors::Overflow) {
    threw = true;
  }
  if (value == numeric_limits<T>::min()) {
    if (!threw) {
      cout << "Pre-decrement operator did not throw an overflow error on " << to_string(before) << ".\n";
      return false;
    }
  } else {
    if (threw) {
      cout << "Pre-decrement operator threw an unexpected overflow error on " << to_string(before) << ".\n";
      return false;
    }
    if (test != before - 1) {
      cout << "Pre-decrement operator does not decrement. " << to_string(before) << " -> " << to_string(test) << "\n";
      return false;
    }
  }
  return true;
}

/*
 * Test post-decrement operator.
 */
template <class T> bool test_postDecrementOperator (T const value) {
  SafeInt<T> test  (value);
  SafeInt<T> before(test );
  bool threw(false);
  try {
    if (test-- != before) {
      cout << "Post-decrement operator changes before returning value.\n";
      return false;
    }
  } catch (typename SafeInt<T>::Errors::Overflow) {
    threw = true;
  }
  if (value == numeric_limits<T>::min()) {
    if (!threw) {
      cout << "Post-decrement operator did not throw an overflow error on " << to_string(before) << ".\n";
      return false;
    }
  } else {
    if (threw) {
      cout << "Post-decrement operator threw an unexpected overflow error on " << to_string(before) << ".\n";
      return false;
    }
    if (test != before - 1) {
      cout << "Post-decrement operator does not decrement." << to_string(before) << " -> " << to_string(test) << "\n";
      return false;
    }
  }
  return true;
}

/*
 * Test unary plus operator.
 */
template <class T> bool test_unaryPlusOperator (T const value) {
  SafeInt<T> test(value);
  if (+test != value) {
    cout << "Unary plus operator returns different value.\n";
    return false;
  }
  return true;
}

template <class T, RelType> class UnaryMinusOperatorTester;
template <class T> class UnaryMinusOperatorTester<T, SE_SE> {
  public:
    static bool op (T const value) {
      SafeInt<T> test  (value);
      SafeInt<T> before(test );
      bool threw(false);
      try {
        test = -test;
      } catch (typename SafeInt<T>::Errors::Overflow) {
        threw = true;
      }
      if (value == numeric_limits<T>::min()) {
        if (!threw) {
          cout << "Unary minus did not throw an overflow error on " << to_string(value) << ".\n";
          return false;
        }
      } else {
        if (threw) {
          cout << "Unary minus threw an unexpected overflow error on " << to_string(value) << ".\n";
          return false;
        }
        if (test != before * -1) {
          cout << "Unary minus does not negate properly.\n";
          return false;
        }
        if (test.abs() != before.abs()) {
          cout << "Unary minus does not preserve absolute value.\n";
          return false;
        }
      }
      return true;
    };
};
template <class T> class UnaryMinusOperatorTester<T, UE_UE> {
  public:
    static bool op (T const value) {
      SafeInt<T> test  (value);
      SafeInt<T> before(test );
      bool threw(false);
      try {
        test = -test;
      } catch (typename SafeInt<T>::Errors::Overflow) {
        threw = true;
      }
      if (value) {
        if (!threw) {
          cout << "Unary minus did not throw an overflow error on " << to_string(value) << ".\n";
          return false;
        }
      } else {
        if (threw) {
          cout << "Unary minus threw an unexpected overflow error on " << to_string(value) << ".\n";
          return false;
        }
        if (test != value) {
          cout << "Unary minus changed value of 0.\n";
          return false;
        }
      }
      return true;
    };
};
  
/*
 * Test unary minus operator.
 */
template <class T> bool test_unaryMinusOperator (T const value) {
  return UnaryMinusOperatorTester<T, Relationship<T, T>::value>::op(value);
}

/*
 * Test the not operator.
 */
template <class T> bool test_notOperator (T const value) {
  SafeInt<T> test(value);
  if (static_cast<bool>(value) == !test) {
    cout << "Not operator fails to negate for " << to_string(value) << ".\n";
    return false;
  }
  return true;
}

/*
 * Test bitwise compliment. SafeInt<bool> does not guarantee that it will
 * equal a given bool when the operator is applied. The reason for this is
 * that SafeInt<bool> is considered to have only one bit.
 */
template <class T> bool test_bitwiseCompliment (T const value);
template <> bool test_bitwiseCompliment (bool const value) {
  SafeInt<bool> test  (value);
  SafeInt<bool> before(test );
  test = ~test;
  if (test == before) {
    cout << "Bitwise compliment did not invert " << to_string(value) << ".\n";
    return false;
  }
  if (~test != before) {
    cout << "Bitwise compliment of " << to_string(before) << " to " << to_string(value) << " is not reversible.\n";
    return false;
  }
  return true;
}
template <class T> bool test_bitwiseCompliment (T const value) {
  SafeInt<T> test  (value);
  SafeInt<T> before(test );
  test = ~test;
  if (test == before) {
    cout << "Bitwise compliment did not invert " << to_string(value) << ".\n";
    return false;
  }
  if (test != static_cast<T>(~value)) {
    cout << "Bitwise compliment returns " << to_string(test) << " for " << to_string(value) << ".\n";
    return false;
  }
  if (~test != before) {
    cout << "Bitwise compliment of " << to_string(before) << " to " << to_string(value) << " is not reversible.\n";
    return false;
  }
  return true;
}

/*
 * Test bool cast.
 */
/*
template <class T> bool test_boolCast (T const value) {
  SafeInt<T> test(value);
  if (!(static_cast<bool>(test) && value) && !(!static_cast<bool>(test) && !value)) {
    cout << "Truthiness does not agree.\n";
    return false;
  }
  return true;
}
*/

/*
 * Test char cast.
 */
/*
template <class T> bool test_charCast (T const value) {
  SafeInt<T> test(value);
  bool threw(false);
  try {
    if (static_cast<char>(test) != static_cast<char>(value)) {
      cout << "Values do not agree for " << to_string(value) << ".\n";
      return false;
    }
  } catch (typename SafeInt<T>::Errors::Overflow) {
    threw = true;
  }
  */
  /*
  if (SafeInt<size_t>(test.bitsInNumber()) > numeric_limits<char>::digits + (numeric_limits<char>::is_signed ? 1 : 0) || (test < 0 && !numeric_limits<char>::is_signed)) {
    if (!threw) {
      cout << "Did not throw overflow for value " << to_string(value) << ".\n";
      return false;
    }
  } else {
    if (threw) {
      cout << "Threw overflow for value " << to_string(value) << ".\n";
      return false;
    }
  }
  */
  /*
  // FIXME: Cast operators should get edge cases for both types.
  if (test.bitsInNumber() > numeric_limits<char>::digits 
  return true;
}
*/

/*
 * Test type operator.
 */
template <class T, class U> bool test_typeOperator (T const value1, U const value2) {
  if (value2 == value2) {}
  SafeInt<T> test(value1);
  bool threw(false);
  try {
    if (static_cast<U>(test) != SafeInt<T>(value1)) {
      cout << "Values do not agree for " << to_string(value1) << ".\n";
      return false;
    }
  } catch (typename SafeInt<T>::Errors::Overflow) {
    threw = true;
  }
  if (test > numeric_limits<U>::max() || test < numeric_limits<U>::min()) {
    if (!threw) {
      cout << "Did not throw overflow for " << to_string(value1) << ".\n";
      return false;
    }
  } else {
    if (threw) {
      cout << "Threw overflow for " << to_string(value1) << ".\n";
      return false;
    }
  }
  return true;
}

/*
 * Run a unary op test.
 */
template <class T> bool do_unaryTest (vector<T> const& values, bool (*test)(T const)) {
  
  // First test edge cases.
  for (typename vector<T>::const_iterator i = values.begin(); i != values.end(); ++i) {
    if (!test(*i)) {
      return false;
    }
  }
  
  // Now do random testing.
  for (size_t i = 0; i != RANDOM_TESTS; ++i) {
    T randnum(randrange<T>());
    if (!test(randnum)) {
      return false;
    }
  }
  
  // Success.
  return true;
  
}

/*
 * Run a binary test.
 */
template <class T, class U> bool do_binaryTest (vector<T> const& values1, vector<U> const& values2, bool (*test)(T const, U const)) {
  
  // First test edge cases.
  for (typename vector<T>::const_iterator i = values1.begin(); i != values1.end(); ++i) {
    for (typename vector<U>::const_iterator j = values2.begin(); j != values2.end(); ++j) {
      if (!test(*i, *j)) {
        return false;
      }
    }
  }
  
  // Now do random testing.
  for (size_t i = 0; i != RANDOM_TESTS; ++i) {
    T randnum1(randrange<T>());
    for (size_t j = 0; j != RANDOM_TESTS; ++j) {
      U randnum2(randrange<U>());
      if (!test(randnum1, randnum2)) {
        return false;
      }
    }
  }
  
  // Success.
  return true;
  
}

/*
 * Combine edge cases.
 */
template <class T, class U, class V> void combine_edges (vector<T>& combined, vector<U> const& values1, vector<V> const& values2) {
  
  // Clear combined vector.
  combined.clear();
  
  // Insert all values1 that we can.
  for (typename vector<U>::const_iterator i = values1.begin(); i != values1.end(); ++i) {
    if (SafeInt<U>(*i) >= numeric_limits<T>::min() && SafeInt<U>(*i) <= numeric_limits<T>::max()) {
      combined.push_back(*i);
    }
  }
  
  // Now do values2. Don't insert duplicates.
  for (typename vector<V>::const_iterator i = values2.begin(); i != values2.end(); ++i) {
    if (SafeInt<V>(*i) >= numeric_limits<T>::min() && SafeInt<V>(*i) <= numeric_limits<T>::max()) {
      bool insertit(true);
      for (typename vector<T>::const_iterator j = combined.begin(); j != combined.end(); ++j) {
        if (SafeInt<V>(*i) == SafeInt<T>(*j)) {
          insertit = false;
          break;
        }
      }
      if (insertit) {
        combined.push_back(*i);
      }
    }
  }
  
}

/*
 * Test type 2.
 */
template <class T, class U> bool test_type2 (string const& name1, vector<T> const& tests1, string const& name2) {
 
  if (&name1 == &name1) {}
  
  // Edge cases.
  vector<U> tests2;
  vector<T> comb_tests1;
  vector<U> comb_tests2;
  vector<U> dummy;
  EdgeBuilder<U, Relationship<U, U>::value>::op(tests2);
  combine_edges(comb_tests1, tests1, tests2);
  combine_edges(comb_tests2, tests1, tests2);
  dummy.push_back(0);
  
  cout << "    ";
  cout.width(38);
  cout << left << (name2 + " operator: "); if (!do_binaryTest(comb_tests1, dummy, test_typeOperator)) { return false; } cout << "OK\n";
  
  return true;
  
}

/*
 * Test type 1.
 */
template <class T> bool test_type1 (string const& name) {
  
  cout << "Testing " << name << ":\n";
  
  cout << "  Unary operations:\n";
  vector<T> tests;
  EdgeBuilder<T, Relationship<T, T>::value>::op(tests);
  
  cout << "    Default constructor:                  ";
  {
    SafeInt<T> test;
    if (test) {
      cout << "Default constructor creates value other than 0.\n";
      return false;
    }
  }
  cout << "OK\n";
  cout << "    Conversion constructor, native type:  "; if (!do_unaryTest(tests, test_conversionConstructorNative)) { return false; } cout << "OK\n";
  cout << "    Copy constructor:                     "; if (!do_unaryTest(tests, test_copyConstructor            )) { return false; } cout << "OK\n";
  cout << "    Pre-increment operator:               "; if (!do_unaryTest(tests, test_preIncrementOperator       )) { return false; } cout << "OK\n";
  cout << "    Post-increment operator:              "; if (!do_unaryTest(tests, test_postIncrementOperator      )) { return false; } cout << "OK\n";
  cout << "    Pre-decrement operator:               "; if (!do_unaryTest(tests, test_preDecrementOperator       )) { return false; } cout << "OK\n";
  cout << "    Post-decrement operator:              "; if (!do_unaryTest(tests, test_postDecrementOperator      )) { return false; } cout << "OK\n";
  cout << "    Unary plus operator:                  "; if (!do_unaryTest(tests, test_unaryPlusOperator          )) { return false; } cout << "OK\n";
  cout << "    Unary minus operator:                 "; if (!do_unaryTest(tests, test_unaryMinusOperator         )) { return false; } cout << "OK\n";
  cout << "    Not operator:                         "; if (!do_unaryTest(tests, test_notOperator                )) { return false; } cout << "OK\n";
  cout << "    Bitwise compliment:                   "; if (!do_unaryTest(tests, test_bitwiseCompliment          )) { return false; } cout << "OK\n";
  
  return test_type2<T, char          >(name, tests, "char"          ) &&
         test_type2<T, signed char   >(name, tests, "signed char"   ) &&
         test_type2<T, unsigned char >(name, tests, "unsigned char" ) &&
         test_type2<T, wchar_t       >(name, tests, "wchar_t"       ) &&
         test_type2<T, short         >(name, tests, "short"         ) &&
         test_type2<T, unsigned short>(name, tests, "unsigned short") &&
         test_type2<T, int           >(name, tests, "int"           ) &&
         test_type2<T, unsigned      >(name, tests, "unsigned"      ) &&
         test_type2<T, long          >(name, tests, "long"          ) &&
         test_type2<T, unsigned long >(name, tests, "unsigned long" );
  
}

int main () {
  
  // Initialize the random number generator.
  srand(time(0));
  
  if (
    test_type1<char          >("char"          ) &&
    test_type1<signed char   >("signed char"   ) &&
    test_type1<unsigned char >("unsigned char" ) &&
    test_type1<wchar_t       >("wchar_t"       ) &&
    test_type1<short         >("short"         ) &&
    test_type1<unsigned short>("unsigned short") &&
    test_type1<int           >("int"           ) &&
    test_type1<unsigned      >("unsigned"      ) &&
    test_type1<long          >("long"          ) &&
    test_type1<unsigned long >("unsigned long" )
  ) {
    return 0;
  } else {
    return 1;
  }
  
}

