/*****************************************************************************
 * Arbitrary-test.cxx
 *****************************************************************************
 * Tests the class Arbitrary.
 *****************************************************************************/

// STL includes.
#include <iostream>

// Internal includes.
#include "Arbitrary.hxx"

// Namespace declarations.
using namespace std;
using namespace DAC;

// This is where it all happens.
int main (int argc, char** argv, char** envp) {
  
  try {
    
    Arbitrary test1;
    Arbitrary test2;
    
    /*
    string from;
    
    from = "01";                            test1 = from; cout << "original: " << from << "  now: " << test1 << endl;
    from = "001";                           test1 = from; cout << "original: " << from << "  now: " << test1 << endl;
    from = "0001";                          test1 = from; cout << "original: " << from << "  now: " << test1 << endl;
    from = "10";                            test1 = from; cout << "original: " << from << "  now: " << test1 << endl;
    from = "100";                           test1 = from; cout << "original: " << from << "  now: " << test1 << endl;
    from = "1000";                          test1 = from; cout << "original: " << from << "  now: " << test1 << endl;
    from = "010";                           test1 = from; cout << "original: " << from << "  now: " << test1 << endl;
    from = "00100";                         test1 = from; cout << "original: " << from << "  now: " << test1 << endl;
    from = "0001000";                       test1 = from; cout << "original: " << from << "  now: " << test1 << endl;
    from = "68468468178465416841293723915"; test1 = from; cout << "original: " << from << "  now: " << test1 << endl;
    from = "0.123456789";                   test1 = from; cout << "original: " << from << "  now: " << test1 << endl;
    from = "1234.56789";                    test1 = from; cout << "original: " << from << "  now: " << test1 << endl;
    from = "0";                             test1 = from; cout << "original: " << from << "  now: " << test1 << endl;
    from = "0.00001";                       test1 = from; cout << "original: " << from << "  now: " << test1 << endl;
    from = "12340000";                      test1 = from; cout << "original: " << from << "  now: " << test1 << endl;
    from = "123000.000";                    test1 = from; cout << "original: " << from << "  now: " << test1 << endl;
    
    test1 = 1;
    */
    
    cout << "test1: ";
    cin  >> test1;
    cout << "test1: " << test1 << endl;
    cout << "test2: ";
    cin  >> test2;
    cout << "test2: " << test2 << endl;
    cout << endl;
    cout << test1 << "  + " << test2 << " = " << (test1  + test2) << endl;
    cout << test1 << "  - " << test2 << " = " << (test1  - test2) << endl;
    cout << test1 << "  * " << test2 << " = " << (test1  * test2) << endl;
    cout << test1 << "  / " << test2 << " = " << (test1  / test2) << endl;
    cout << test1 << " >> " << test2 << " = " << (test1 >> test2) << endl;
    
    exit(0);
    
  } catch (Exception& e) {
    
    cout << "Exception: " << e << endl;
    exit(1);
    
  } catch (...) {
    
    cout << "Unexpected exception caught." << endl;
    exit(1);
    
  }
  
};
