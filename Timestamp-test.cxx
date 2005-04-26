/*****************************************************************************
 * Timestamp-test.cxx
 *****************************************************************************
 * Tests the class Timestamp.
 *****************************************************************************/

// STL includes.
#include <iostream>

// Internal includes.
#include "demangle.hxx"
#include "Exception.hxx"

// Testing include.
#include "Timestamp.hxx"

// Namespace declarations.
using namespace std;
using namespace DAC;

// This is where it all happens.
int main (int argc, char** argv, char** envp) {
  
  try {
    
    Timestamp test1;
    
    test1.getSystemTime();
    
    cout << "Current:" << endl
         << "  Julian: " << test1.Julian() << endl;
    
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
  
};
