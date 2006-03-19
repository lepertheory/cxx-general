/*****************************************************************************
 * ValReader
 *****************************************************************************
 * Blah.
 *****************************************************************************/

// Standard includes.
#include <string>

// System includes.
#include <CaseConvert.h++>
#include <Arb.h++>

// Class incude.
#include "ValReader.h++"

// Bring in namespaces.
using namespace std;

// Namespace wrapper.
namespace DAC {
  
  /***************************************************************************
   * ValReader
   ***************************************************************************/
  
  /***************************************************************************/
  // Constants.
  
  // Empty string.
  string const ValReader::_BLANK;
  
  /***************************************************************************/
  // Function members.
  
  /*
   * Convert to boolean.
   */
  bool ValReader::to_boolean () const {
    
    // An empty string is false.
    if (_value.empty()) {
      return false;
    }
    
    // Work area.
    string work(uppercase(_value));
    
    // Check for negative text.
    if (work == "F" || work == "N" || work == "FALSE" || work == "NO" || work == "OFF") {
      return false;
    }
    
    // Check for positive text.
    if (work == "T" || work == "Y" || work == "TRUE" || work == "YES" || work == "ON") {
      return true;
    }
    
    // Check for numeric 0. If text can't be converted to a number, any text
    // is true.
    try {
      return Arb(work);
    } catch (Arb::Errors::Base&) {
      return true;
    }
    
  }
  
}

