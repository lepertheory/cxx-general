/*****************************************************************************
 * test.cxx
 *****************************************************************************
 * Tests the class Arb.
 *****************************************************************************/

// STL includes.
#include <iostream>

// Internal includes.
#include "Arb.hxx"
#include "demangle.hxx"
#include "Exception.hxx"

// Namespace declarations.
using namespace std;
using namespace DAC;

// This is where it all happens.
int main (int argc, char** argv, char** envp) {
  
  try {
    
    Arb test;
    
    cout << "test: ";
    cin  >> test;
    cout << "test: " << test << endl;
    
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
