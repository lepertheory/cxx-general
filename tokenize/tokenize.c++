/*****************************************************************************
 * tokenize.c++
 *****************************************************************************
 * Implementation for function tokenize().
 *****************************************************************************/

// Standard includes.
#include <string>
#include <vector>

// Function include.
#include "tokenize.h++"

// Bring in namespaces.
using namespace std;

// Namespace wrapper.
namespace DAC {

  /*****************************************************************************/
  // Functions.

  /*
  * Tokenize a string.
  */
  void tokenize (string const& strtotok, vector<string>& retval, string const& delim) {
    
    // Work area.
    vector<string> workarr;
    
    // Loop through each delimiter in the string.
    string::size_type spos = 0;
    string::size_type epos = 0;
    for (; epos != string::npos; ) {
      
      // Find the next delimiter.
      epos = strtotok.find_first_of(delim, spos);
      
      // Add another element to the array and set the new search start pos.
      // Even though epos and spos are both offsets, do not add one since we
      // don't want to include the delimiter.
      workarr.push_back(strtotok.substr(spos, epos - spos));
      
    }
    
    // Move the results into place.
    retval.swap(workarr);
    
  }
  
}

