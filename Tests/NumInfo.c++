/*****************************************************************************
 * NumInfo.c++
 *****************************************************************************
 * Unit test classes in the NumInfo header.
 *****************************************************************************/

// Standard includes.
#include <string>
#include <iostream>
#include <limits>

// Testing includes.
#include <NumInfo.h++>

// Bring in namespaces.
using namespace std;
using namespace DAC;

/*
 * Test Level 2.
 */
template <typename T, typename U> int test2 (string const& type_t, string const& type_u) {
  
  // Output test being run.
  cout << "  Testing type " << type_t << " with type " << type_u << "..." << endl;
  
  // Test that IntRel returns the correct relationship type.
  cout << "    Testing IntRel::type... ";
  RelTypes::Type reltype = IntRel<T, U>::type;
  switch (reltype) {
    case RelTypes::SE_SE:
      cout << "RelTypes::SE_SE ";
      if (numeric_limits<T>::is_specialized && numeric_limits<U>::is_specialized &&
          numeric_limits<T>::is_integer     && numeric_limits<U>::is_integer     &&
          numeric_limits<T>::digits         == numeric_limits<U>::digits         &&
          numeric_limits<T>::is_signed      && numeric_limits<U>::is_signed) {
        cout << "OK!" << endl;
      } else {
        cout << "FAILED!" << endl;
        return 1;
      }
    break;
    case RelTypes::SE_UE:
      cout << "RelTypes::SE_UE ";
      if (numeric_limits<T>::is_specialized &&  numeric_limits<U>::is_specialized &&
          numeric_limits<T>::is_integer     &&  numeric_limits<U>::is_integer     &&
          numeric_limits<T>::digits         ==  numeric_limits<U>::digits         &&
          numeric_limits<T>::is_signed      && !numeric_limits<U>::is_signed) {
        cout << "OK!" << endl;
      } else {
        cout << "FAILED!" << endl;
        return 1;
      }
    break;
    case RelTypes::SS_SL:
      cout << "RelTypes::SS_SL ";
      if (numeric_limits<T>::is_specialized && numeric_limits<U>::is_specialized &&
          numeric_limits<T>::is_integer     && numeric_limits<U>::is_integer     &&
          numeric_limits<T>::digits         <  numeric_limits<U>::digits         &&
          numeric_limits<T>::is_signed      && numeric_limits<U>::is_signed) {
        cout << "OK!" << endl;
      } else {
        cout << "FAILED!" << endl;
        return 1;
      }
    break;
    case RelTypes::SS_UL:
      cout << "RelTypes::SS_UL ";
      if (numeric_limits<T>::is_specialized &&  numeric_limits<U>::is_specialized &&
          numeric_limits<T>::is_integer     &&  numeric_limits<U>::is_integer     &&
          numeric_limits<T>::digits         <   numeric_limits<U>::digits         &&
          numeric_limits<T>::is_signed      && !numeric_limits<U>::is_signed) {
        cout << "OK!" << endl;
      } else {
        cout << "FAILED!" << endl;
        return 1;
      }
    break;
    case RelTypes::SL_SS:
      cout << "RelTypes::SL_SS ";
      if (numeric_limits<T>::is_specialized && numeric_limits<U>::is_specialized &&
          numeric_limits<T>::is_integer     && numeric_limits<U>::is_integer     &&
          numeric_limits<T>::digits         >  numeric_limits<U>::digits         &&
          numeric_limits<T>::is_signed      && numeric_limits<U>::is_signed) {
        cout << "OK!" << endl;
      } else {
        cout << "FAILED!" << endl;
        return 1;
      }
    break;
    case RelTypes::SL_US:
      cout << "RelTypes::SL_US ";
      if (numeric_limits<T>::is_specialized &&  numeric_limits<U>::is_specialized &&
          numeric_limits<T>::is_integer     &&  numeric_limits<U>::is_integer     &&
          numeric_limits<T>::digits         >   numeric_limits<U>::digits         &&
          numeric_limits<T>::is_signed      && !numeric_limits<U>::is_signed) {
        cout << "OK!" << endl;
      } else {
        cout << "FAILED!" << endl;
        return 1;
      }
    break;
    case RelTypes::UE_UE:
      cout << "RelTypes::UE_UE ";
      if ( numeric_limits<T>::is_specialized &&  numeric_limits<U>::is_specialized &&
           numeric_limits<T>::is_integer     &&  numeric_limits<U>::is_integer     &&
           numeric_limits<T>::digits         ==  numeric_limits<U>::digits         &&
          !numeric_limits<T>::is_signed      && !numeric_limits<U>::is_signed) {
        cout << "OK!" << endl;
      } else {
        cout << "FAILED!" << endl;
        return 1;
      }
    break;
    case RelTypes::UE_SE:
      cout << "RelTypes::UE_SE ";
      if ( numeric_limits<T>::is_specialized && numeric_limits<U>::is_specialized &&
           numeric_limits<T>::is_integer     && numeric_limits<U>::is_integer     &&
           numeric_limits<T>::digits         == numeric_limits<U>::digits         &&
          !numeric_limits<T>::is_signed      && numeric_limits<U>::is_signed) {
        cout << "OK!" << endl;
      } else {
        cout << "FAILED!" << endl;
        return 1;
      }
    break;
    case RelTypes::US_SL:
      cout << "RelTypes::US_SL ";
      if ( numeric_limits<T>::is_specialized && numeric_limits<U>::is_specialized &&
           numeric_limits<T>::is_integer     && numeric_limits<U>::is_integer     &&
           numeric_limits<T>::digits         <  numeric_limits<U>::digits         &&
          !numeric_limits<T>::is_signed      && numeric_limits<U>::is_signed) {
        cout << "OK!" << endl;
      } else {
        cout << "FAILED!" << endl;
        return 1;
      }
    break;
    case RelTypes::US_UL:
      cout << "RelTypes::US_UL ";
      if ( numeric_limits<T>::is_specialized &&  numeric_limits<U>::is_specialized &&
           numeric_limits<T>::is_integer     &&  numeric_limits<U>::is_integer     &&
           numeric_limits<T>::digits         <   numeric_limits<U>::digits         &&
          !numeric_limits<T>::is_signed      && !numeric_limits<U>::is_signed) {
        cout << "OK!" << endl;
      } else {
        cout << "FAILED!" << endl;
        return 1;
      }
    break;
    case RelTypes::UL_SS:
      cout << "RelTypes::UL_SS ";
      if ( numeric_limits<T>::is_specialized && numeric_limits<U>::is_specialized &&
           numeric_limits<T>::is_integer     && numeric_limits<U>::is_integer     &&
           numeric_limits<T>::digits         >  numeric_limits<U>::digits         &&
          !numeric_limits<T>::is_signed      && numeric_limits<U>::is_signed) {
        cout << "OK!" << endl;
      } else {
        cout << "FAILED!" << endl;
        return 1;
      }
    break;
    case RelTypes::UL_US:
      cout << "RelTypes::UL_US ";
      if ( numeric_limits<T>::is_specialized &&  numeric_limits<U>::is_specialized &&
           numeric_limits<T>::is_integer     &&  numeric_limits<U>::is_integer     &&
           numeric_limits<T>::digits         >   numeric_limits<U>::digits         &&
          !numeric_limits<T>::is_signed      && !numeric_limits<U>::is_signed) {
        cout << "OK!" << endl;
      } else {
        cout << "FAILED!" << endl;
        return 1;
      }
    break;
    case RelTypes::ERROR_INTEGER_ONLY:
      cout << "RelTypes::ERROR_INTEGER_ONLY ";
      if (!numeric_limits<T>::is_specialized || !numeric_limits<U>::is_specialized ||
          !numeric_limits<T>::is_integer     || !numeric_limits<U>::is_integer) {
        cout << "OK!" << endl;
      } else {
        cout << "FAILED!" << endl;
        return 1;
      } 
    break;
    default:
      cout << "Unknown type " << static_cast<int>(reltype) << " FAILED!" << endl;
      return 1;
    break;
  }
  
  // All tests passed.
  cout << "  OK!" << endl;
  return 0;
  
}

/*
 * Run a test.
 */
template <typename T> int test (string const& type) {
  
  // Output test being run.
  cout << "Testing type " << type << "..." << endl;
  
  // Test that NumType returns the correct type.
  cout << "  Testing NumType::type... ";
  NumTypes::Type numtype = NumType<T>::type;
  switch (numtype) {
    case NumTypes::SINT:
      cout << "NumTypes::SINT ";
      if (numeric_limits<T>::is_specialized && numeric_limits<T>::is_integer && numeric_limits<T>::is_signed) {
        cout << "OK!" << endl;
      } else {
        cout << "FAILED!" << endl;
        return 1;
      }
    break;
    case NumTypes::UINT:
      cout << "NumTypes::UINT ";
      if (numeric_limits<T>::is_specialized && numeric_limits<T>::is_integer && !numeric_limits<T>::is_signed) {
        cout << "OK!" << endl;
      } else {
        cout << "FAILED!" << endl;
        return 1;
      }
    break;
    case NumTypes::FLPT:
      cout << "NumTypes::FLPT ";
      if (numeric_limits<T>::is_specialized && !numeric_limits<T>::is_integer) {
        cout << "OK!" << endl;
      } else {
        cout << "FAILED!" << endl;
        return 1;
      }
    break;
    case NumTypes::ERROR_UNKNOWN_TYPE:
      cout << "NumTypes::ERROR_UNKNOWN_TYPE ";
      if (!numeric_limits<T>::is_specialized) {
        cout << "OK!" << endl;
      } else {
        cout << "FAILED!" << endl;
        return 1;
      }
    break;
    default:
      cout << "Unknown type " << static_cast<int>(numtype) << " FAILED!" << endl;
      return 1;
    break;
  }
  
  // Test that IntRel return the correct relationship.
  if (test2<T, bool          >(type, "bool"          )) { return 1; }
  if (test2<T, char          >(type, "char"          )) { return 1; }
  if (test2<T, signed char   >(type, "signed char"   )) { return 1; }
  if (test2<T, unsigned char >(type, "unsigned char" )) { return 1; }
  if (test2<T, wchar_t       >(type, "wchar_t"       )) { return 1; }
  if (test2<T, short         >(type, "short"         )) { return 1; }
  if (test2<T, unsigned short>(type, "unsigned short")) { return 1; }
  if (test2<T, int           >(type, "int"           )) { return 1; }
  if (test2<T, unsigned int  >(type, "unsigned int"  )) { return 1; }
  if (test2<T, long          >(type, "long"          )) { return 1; }
  if (test2<T, unsigned long >(type, "unsigned long" )) { return 1; }
  if (test2<T, float         >(type, "float"         )) { return 1; }
  if (test2<T, double        >(type, "double"        )) { return 1; }
  if (test2<T, long double   >(type, "long double"   )) { return 1; }
  
  // All tests passed.
  cout << "OK!" << endl;
  return 0;
  
}

/*
 * This is it.
 */
int main () {
  
  // Test each possible type.
  if (test<bool          >("bool"          )) { return 1; }
  if (test<char          >("char"          )) { return 1; }
  if (test<signed char   >("signed char"   )) { return 1; }
  if (test<unsigned char >("unsigned char" )) { return 1; }
  if (test<wchar_t       >("wchar_t"       )) { return 1; }
  if (test<short         >("short"         )) { return 1; }
  if (test<unsigned short>("unsigned short")) { return 1; }
  if (test<int           >("int"           )) { return 1; }
  if (test<unsigned int  >("unsigned int"  )) { return 1; }
  if (test<long          >("long"          )) { return 1; }
  if (test<unsigned long >("unsigned long" )) { return 1; }
  if (test<float         >("float"         )) { return 1; }
  if (test<double        >("double"        )) { return 1; }
  if (test<long double   >("long double"   )) { return 1; }
  
  // All tests passed.
  cout << "\nOK!" << endl;
  return 0;
  
}

