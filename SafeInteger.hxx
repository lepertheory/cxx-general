/*****************************************************************************
 * SafeInteger.hxx
 *****************************************************************************
 * Overflow safe integer class.
 *****************************************************************************/

// Include guard.
#if !defined(SAFEINTEGER_0234ykb209384jbyp08u)
  #define SAFEINTEGER_0234ykb209384jbyp08u
  
  // Namespace wrapper.
  namespace DAC {
    
    /*************************************************************************
     * Class SafeInteger
     *************************************************************************
     * This is an overflow safe replacement for any integer type. Should
     * preserve all semantics of using a normal native integer type with a
     * minimum of syntatic changes needed.
     *************************************************************************/
    template <class T> class SafeInteger {
      
      // Public members.
      public:
        
        SafeInteger (T const number);
        
        T Value ();
      
      // Private members.
      private:
        
        T _number;
      
    };
    
    /*************************************************************************
     * Inline and template definitions.
     *************************************************************************/
    
    template <class T> SafeInteger<T>::SafeInteger (T const number) {
      _number = number;
    }
    
    template <class T> T SafeInteger<T>::Value () {
      return _number;
    }
    
  };
  
#endif
