/*****************************************************************************
 * text.cxx
 *****************************************************************************
 * Tests the class SafeInteger.
 *****************************************************************************/

// STL includes.
#include <iostream>

// Internal includes.
#include "SafeInteger.hxx"

// Namespace declarations.
using namespace std;
using namespace DAC;

// This is where it all happens.
int main (int argc, char** argv, char** envp) {
  
  try {
    
    SafeInteger<unsigned char>      test1;
    SafeInteger<signed   char>      test2;
    SafeInteger<unsigned short int> test3;
    SafeInteger<signed   short int> test4;
    SafeInteger<unsigned int>       test5;
    SafeInteger<signed   int>       test6;
    SafeInteger<unsigned long int>  test7;
    SafeInteger<signed   long int>  test8;
    
    cout << "test1: "; cin  >> test1; cout << "test1: " << test1 << endl;
    cout << "test2: "; cin  >> test2; cout << "test2: " << test2 << endl;
    cout << "test3: "; cin  >> test3; cout << "test3: " << test3 << endl;
    cout << "test4: "; cin  >> test4; cout << "test4: " << test4 << endl;
    cout << "test5: "; cin  >> test5; cout << "test5: " << test5 << endl;
    cout << "test6: "; cin  >> test6; cout << "test6: " << test6 << endl;
    cout << "test7: "; cin  >> test7; cout << "test7: " << test7 << endl;
    cout << "test8: "; cin  >> test8; cout << "test8: " << test8 << endl;
    
    cout << "test1(" << test1 << ") + test1(" << test1 << ") = " << (test1 + test1.Value()) << endl;
    cout << "test1(" << test1 << ") + test2(" << test2 << ") = " << (test1 + test2.Value()) << endl;
    cout << "test1(" << test1 << ") + test3(" << test3 << ") = " << (test1 + test3.Value()) << endl;
    cout << "test1(" << test1 << ") + test4(" << test4 << ") = " << (test1 + test4.Value()) << endl;
    cout << "test1(" << test1 << ") + test5(" << test5 << ") = " << (test1 + test5.Value()) << endl;
    cout << "test1(" << test1 << ") + test6(" << test6 << ") = " << (test1 + test6.Value()) << endl;
    cout << "test1(" << test1 << ") + test7(" << test7 << ") = " << (test1 + test7.Value()) << endl;
    cout << "test1(" << test1 << ") + test8(" << test8 << ") = " << (test1 + test8.Value()) << endl;
    cout << "test2(" << test2 << ") + test1(" << test1 << ") = " << (test2 + test1.Value()) << endl;
    cout << "test2(" << test2 << ") + test2(" << test2 << ") = " << (test2 + test2.Value()) << endl;
    cout << "test2(" << test2 << ") + test3(" << test3 << ") = " << (test2 + test3.Value()) << endl;
    cout << "test2(" << test2 << ") + test4(" << test4 << ") = " << (test2 + test4.Value()) << endl;
    cout << "test2(" << test2 << ") + test5(" << test5 << ") = " << (test2 + test5.Value()) << endl;
    cout << "test2(" << test2 << ") + test6(" << test6 << ") = " << (test2 + test6.Value()) << endl;
    cout << "test2(" << test2 << ") + test7(" << test7 << ") = " << (test2 + test7.Value()) << endl;
    cout << "test2(" << test2 << ") + test8(" << test8 << ") = " << (test2 + test8.Value()) << endl;
    cout << "test3(" << test3 << ") + test1(" << test1 << ") = " << (test3 + test1.Value()) << endl;
    cout << "test3(" << test3 << ") + test2(" << test2 << ") = " << (test3 + test2.Value()) << endl;
    cout << "test3(" << test3 << ") + test3(" << test3 << ") = " << (test3 + test3.Value()) << endl;
    cout << "test3(" << test3 << ") + test4(" << test4 << ") = " << (test3 + test4.Value()) << endl;
    cout << "test3(" << test3 << ") + test5(" << test5 << ") = " << (test3 + test5.Value()) << endl;
    cout << "test3(" << test3 << ") + test6(" << test6 << ") = " << (test3 + test6.Value()) << endl;
    cout << "test3(" << test3 << ") + test7(" << test7 << ") = " << (test3 + test7.Value()) << endl;
    cout << "test3(" << test3 << ") + test8(" << test8 << ") = " << (test3 + test8.Value()) << endl;
    cout << "test4(" << test4 << ") + test1(" << test1 << ") = " << (test4 + test1.Value()) << endl;
    cout << "test4(" << test4 << ") + test2(" << test2 << ") = " << (test4 + test2.Value()) << endl;
    cout << "test4(" << test4 << ") + test3(" << test3 << ") = " << (test4 + test3.Value()) << endl;
    cout << "test4(" << test4 << ") + test4(" << test4 << ") = " << (test4 + test4.Value()) << endl;
    cout << "test4(" << test4 << ") + test5(" << test5 << ") = " << (test4 + test5.Value()) << endl;
    cout << "test4(" << test4 << ") + test6(" << test6 << ") = " << (test4 + test6.Value()) << endl;
    cout << "test4(" << test4 << ") + test7(" << test7 << ") = " << (test4 + test7.Value()) << endl;
    cout << "test4(" << test4 << ") + test8(" << test8 << ") = " << (test4 + test8.Value()) << endl;
    cout << "test5(" << test5 << ") + test1(" << test1 << ") = " << (test5 + test1.Value()) << endl;
    cout << "test5(" << test5 << ") + test2(" << test2 << ") = " << (test5 + test2.Value()) << endl;
    cout << "test5(" << test5 << ") + test3(" << test3 << ") = " << (test5 + test3.Value()) << endl;
    cout << "test5(" << test5 << ") + test4(" << test4 << ") = " << (test5 + test4.Value()) << endl;
    cout << "test5(" << test5 << ") + test5(" << test5 << ") = " << (test5 + test5.Value()) << endl;
    cout << "test5(" << test5 << ") + test6(" << test6 << ") = " << (test5 + test6.Value()) << endl;
    cout << "test5(" << test5 << ") + test7(" << test7 << ") = " << (test5 + test7.Value()) << endl;
    cout << "test5(" << test5 << ") + test8(" << test8 << ") = " << (test5 + test8.Value()) << endl;
    cout << "test6(" << test6 << ") + test1(" << test1 << ") = " << (test6 + test1.Value()) << endl;
    cout << "test6(" << test6 << ") + test2(" << test2 << ") = " << (test6 + test2.Value()) << endl;
    cout << "test6(" << test6 << ") + test3(" << test3 << ") = " << (test6 + test3.Value()) << endl;
    cout << "test6(" << test6 << ") + test4(" << test4 << ") = " << (test6 + test4.Value()) << endl;
    cout << "test6(" << test6 << ") + test5(" << test5 << ") = " << (test6 + test5.Value()) << endl;
    cout << "test6(" << test6 << ") + test6(" << test6 << ") = " << (test6 + test6.Value()) << endl;
    cout << "test6(" << test6 << ") + test7(" << test7 << ") = " << (test6 + test7.Value()) << endl;
    cout << "test6(" << test6 << ") + test8(" << test8 << ") = " << (test6 + test8.Value()) << endl;
    cout << "test7(" << test7 << ") + test1(" << test1 << ") = " << (test7 + test1.Value()) << endl;
    cout << "test7(" << test7 << ") + test2(" << test2 << ") = " << (test7 + test2.Value()) << endl;
    cout << "test7(" << test7 << ") + test3(" << test3 << ") = " << (test7 + test3.Value()) << endl;
    cout << "test7(" << test7 << ") + test4(" << test4 << ") = " << (test7 + test4.Value()) << endl;
    cout << "test7(" << test7 << ") + test5(" << test5 << ") = " << (test7 + test5.Value()) << endl;
    cout << "test7(" << test7 << ") + test6(" << test6 << ") = " << (test7 + test6.Value()) << endl;
    cout << "test7(" << test7 << ") + test7(" << test7 << ") = " << (test7 + test7.Value()) << endl;
    cout << "test7(" << test7 << ") + test8(" << test8 << ") = " << (test7 + test8.Value()) << endl;
    cout << "test8(" << test8 << ") + test1(" << test1 << ") = " << (test8 + test1.Value()) << endl;
    cout << "test8(" << test8 << ") + test2(" << test2 << ") = " << (test8 + test2.Value()) << endl;
    cout << "test8(" << test8 << ") + test3(" << test3 << ") = " << (test8 + test3.Value()) << endl;
    cout << "test8(" << test8 << ") + test4(" << test4 << ") = " << (test8 + test4.Value()) << endl;
    cout << "test8(" << test8 << ") + test5(" << test5 << ") = " << (test8 + test5.Value()) << endl;
    cout << "test8(" << test8 << ") + test6(" << test6 << ") = " << (test8 + test6.Value()) << endl;
    cout << "test8(" << test8 << ") + test7(" << test7 << ") = " << (test8 + test7.Value()) << endl;
    cout << "test8(" << test8 << ") + test8(" << test8 << ") = " << (test8 + test8.Value()) << endl;
    
  } catch (Exception e) {
    
    cout << "Exception: " << e << endl;
    exit(1);
    
  }
  
};
