/*****************************************************************************
 * Timestamp.hxx
 *****************************************************************************
 * I will finish this class.
 *****************************************************************************/

// Include guard.
#if !defined(TIMESTAMP_khu83mbu932hiu94)
  #define TIMESTAMP_khu83mbu932hiu94
  
  // STL includes.
  #include <string>
  
  // Internal includes.
  #include "Exception.hxx"
  #include "Arbitrary.hxx"
  
  // Contain in namespace.
  namespace DAC {
    
    // The class.
    class Timestamp {
      
      // Public members.
      public:
        
        // Errors.
        class Errors {
          public:
            class Base : public Exception {
              public:
                Main (std::string const& text) :
                  Exception(text)
                {};
            };
          private:
            Errors ();
        };
        
        // Default constructor.
        Timestamp ();
        
      // Private members.
      private:
        
        
        
    };
    
  };
  
#endif
