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

// This is where it all happens.
int main (int argc, char** argv, char** envp) {
  
  try {
    
    string testin;
    Arb test;
    Arb test0;
    Arb test1;
    Arb test2;
    Arb test3;
    Arb test5;
    
    cout << "test: ";
    cin  >> testin;
    
    test.set(testin);
    test0.set(testin, 0, true);
    test1.set(testin, 1, true);
    test2.set(testin, 2, true);
    test3.set(testin, 3, true);
    test5.set(testin, 5, true);
    
    cout << "test: "  << test  << endl;
    cout << "test0: " << test0 << endl;
    cout << "test1: " << test1 << endl;
    cout << "test2: " << test2 << endl;
    cout << "test3: " << test3 << endl;
    cout << "test5: " << test5 << endl;
    
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
