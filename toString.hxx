/*****************************************************************************
 * toString.hxx
 *****************************************************************************
 * Convert a number to a string.
 *****************************************************************************/

// Include guard.
#if !defined(TOSTRING_buhu84hahasetkbx842)
  #define TOSTRING_buhu84hahasetkbx842
  
  // STL includes.
  #include <sstream>
  
  // Contain in namespace.
  namespace DAC {
    
    /*************************************************************************/
    // Convert the number to a string.
    template <class T> std::string toString (T const& from) {
     
      std::ostringstream os;
      os << from;
      return os.str();
      
    }
    /*************************************************************************/
    
  };
  
#endif
