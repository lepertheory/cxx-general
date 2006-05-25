/*****************************************************************************
 * SafeInt.c++
 *****************************************************************************
 * Tests the class SafeInt.
 *****************************************************************************/

// STL includes.
#include <iostream>
#include <limits>

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

// Type 1 tester.
template <class T, RelType> class T1Tester;
template <class T> class T1Tester<T, SE_SE> {
  public:
    static bool op (string const& name) {
      
      cout << "Testing " << name << ":\n";
      
      cout << "  Unaray operations:\n";
      
      cout << "    Default constructor: ";
      {
        SafeInt<T> test;
        if (test) {
          cout << "Default constructor creates value other than 0.\n";
          return false;
        }
      }
      {
        SafeInt<T> test(numeric_limits<T>::min());
        if (test != numeric_limits<T>::min()) {
          cout << "Default constructor does not set min value properly. Min value for type is " << to_string(numeric_limits<T>::min()) << ", set as " << to_string(test) << ".\n";
          return false;
        }
      }
      {
        SafeInt<T> test(numeric_limits<T>::min() + 1);
        if (test != numeric_limits<T>::min() + 1) {
          cout << "Default constructor does not set min + 1 properly. Min + 1 for type is " << to_string(numeric_limits<T>::min() + 1) << ", set as " << to_string(test) << ".\n";
          return false;
        }
      }
      cout << "OK\n";
      
      return true;
      
    };
};
template <class T> class T1Tester<T, UE_UE> {
  public:
    static bool op (string const& name) {
      
      if (name == name) {}
      
      return true;
      
    };
};

int main () {
  
  T1Tester<bool          , Relationship<bool          , bool          >::value>::op("bool"          );
  if (
    T1Tester<bool          , Relationship<bool          , bool          >::value>::op("bool"          ) &&
    T1Tester<char          , Relationship<char          , char          >::value>::op("char"          ) &&
    T1Tester<signed char   , Relationship<signed char   , signed char   >::value>::op("signed char"   ) &&
    T1Tester<unsigned char , Relationship<unsigned char , unsigned char >::value>::op("unsigned char" ) &&
    T1Tester<wchar_t       , Relationship<wchar_t       , wchar_t       >::value>::op("wchar_t"       ) &&
    T1Tester<short         , Relationship<short         , short         >::value>::op("short"         ) &&
    T1Tester<unsigned short, Relationship<unsigned short, unsigned short>::value>::op("unsigned short") &&
    T1Tester<int           , Relationship<int           , int           >::value>::op("int"           ) &&
    T1Tester<unsigned      , Relationship<unsigned      , unsigned      >::value>::op("unsigned"      ) &&
    T1Tester<long          , Relationship<long          , long          >::value>::op("long"          ) &&
    T1Tester<unsigned long , Relationship<unsigned long , unsigned long >::value>::op("unsigned long" )
  ) {
    return 0;
  } else {
    return 1;
  }
  
}

