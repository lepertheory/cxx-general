/*****************************************************************************
 * ibc.cxx
 *****************************************************************************
 * Converts any integer from any integer base to any integer base.
 *****************************************************************************/

// STL includes.
#include <iostream>

// Internal includes.
#include "ArbInt.hxx"
#include "demangle.hxx"
#include "Exception.hxx"

// Namespace declarations.
using namespace std;
using namespace DAC;

// Get a number base.
template <class T> T getBase (char const* const prompt) {
  
  // Input.
  ArbInt<T> base;
  T         retval = 0;
  
  // Get the base.
  do {
    
    cout << prompt;
    try {
      cin  >> base;
      try {
        // Check out the syntax of this call. Nutty shit, huh?
        retval = base.ArbInt<T>::template Value<T>();
      } catch (ArbIntErrors::ScalarOverflow) {
        cout << "Specified base is too large, enter a base of " << numeric_limits<T>::max() << " or less." << endl;
      }
    } catch (ArbIntErrors::BadFormat& e) {
      cout << e << endl;
    }
    
  } while (retval == 0);
  
  return retval;
  
};

// This is where it all happens.
int main (int argc, char** argv, char** envp) {
  
  // Catch any errors.
  try {
    
    // Input.
    ArbInt<unsigned int> number;
    ArbInt<unsigned int> base;
    unsigned int         ibase = 0;
    unsigned int         obase = 0;
    
    // Get and set the input base.
    do {
      ibase = getBase<unsigned int>("Input base (in base 10): ");
      if (ibase > ArbInt<unsigned int>::max_input_base()) {
        cout << "Specified base is too large, enter a base of " << ArbInt<unsigned int>::max_input_base() << " or less." << endl;
        ibase = 0;
      } else {
        try {
          number.Base(ibase);
        } catch (ArbIntErrors::BaseOutOfRange) {
          cout << "Base must be greater than 2. Please reenter." << endl;
        }
      }
    } while ((ibase == 0) || (number.Base() != ibase));
    
    // Set the number.
    do {
      cout << "Enter the number in the input base: ";
      try {
        cin  >> number;
      } catch (ArbIntErrors::BadFormat e) {
        cout << e.what() << endl;
      }
    } while (!number);
    
    // Get and set the output base:
    do {
      obase = getBase<unsigned int>("Output base (in base 10): ");
      try {
        number.Base(obase);
      } catch (ArbIntErrors::BaseOutOfRange) {
        cout << "Base must be between 2 and 65536. Please reenter." << endl;
      }
    } while (number.Base() != obase);
    
    // Output the new number.
    cout << number;
    
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
  
  // Success.
  exit(0);
  
};
