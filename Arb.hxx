/*****************************************************************************
 * Arb.hxx
 *****************************************************************************
 * Interface for class Arb.
 *****************************************************************************/

// Include guard.
#if !defined(ARB_43693478di8u7do)
  #define ARB_43693478di8u7do
  
  // STL includes.
  #include <iostream>
  #include <string>
  
  // Internal includes.
  #include "SafeInteger.hxx"
  #include "ReferencePointer.hxx"
  #include "toString.hxx"
  #include "ArbInt.hxx"
  
  // Namespace wrapping.
  namespace DAC {
    
    /*************************************************************************
     * Arb
     *************************************************************************
     * This is an arbitrary precision number class.
     *************************************************************************/
    class Arb {
      
      /*
       * Private members.
       */
      private:
        
        /*********************************************************************/
        // Typedefs.
        
        // Native digit type. Setting this to a larger type will allow larger
        // numbers to be stored, however they will be slower and take up more
        // memory. Setting this to a smaller type will restrict Arb to smaller
        // numbers and use less memory, but not likely have an effect on
        // speed. This must be an unsigned integer type, that is, char, short,
        // int, long, and sometimes long long.
        typedef unsigned int _DigT;
        
      /*
       * Public members.
       */
      public:
        
        /*********************************************************************/
        // Typedefs.
        
        // This is the type that controls where the radix point is in a fixed
        // decimal number.
        typedef _DigT PointPosT;
        
        /*********************************************************************/
        // Function members.
        
        // Default constructor.
        Arb ();
        
        // Destructor.
        ~Arb ();
        
        // Reset to just-constructed defaults.
        Arb& clear ();
        
        // Convert to string.
        std::string toString () const;
        
        // Set the number.
        Arb& set (std::string const& number, PointPosT const decimal = 0, bool const fix = false);
      
      /*
       * Private members.
       */
      private:
        
        /*********************************************************************/
        // Typedefs.
        
        // Forward declarations.
        class _Data;
        
        typedef ArbInt<_DigT> _DigsT; // Native digits type.
        
        typedef ReferencePointer<_Data> _DataPT; // Pointer to number data.
        
        /*********************************************************************/
        // Data types.
        
        /*********************************************************************
         * _Data
         *********************************************************************
         * This holds all data for this number, copying this data structure to
         * another number will make a perfect copy of this number.
         *********************************************************************/
        class _Data {
          
          /*
           * Public members.
           */
          public:
            
            /*****************************************************************/
            // Data members.
            
            bool positive; // True if the number is positive.
            
            _DigsT p;        // Numerator.
            _DigsT q;        // Denominator.
            
            PointPosT pointpos; // Radix point position.
            bool      fix;      // If true, fix the radix point.
            
            _DigT origbase; // Original base of this number.
            
            /*****************************************************************/
            // Function members.
            
            // Default constructor.
            _Data ();
            
            // Reset to just-constructed state.
            _Data& clear ();
            
          /*
           * Private members.
           */
          private:
            
            /*****************************************************************/
            // Function members.
            
            // Common initialization routines.
            void _init ();
          
        };
        
        /*********************************************************************/
        // Data members.
        
        // The number.
        _DataPT _data;
        
        /*********************************************************************/
        // Function members.
        
        // Common initialization tasks.
        void _init ();
        
        /*********************************************************************/
        // Static function members.
        
        /*
        // Get the greatest common divisor and least common multiple of two
        // big-endian containers. Base is 2^(bits/2).
        template <class T> static ReferencePointer<T> s_gcd (T const& c1, T const& c2);
        template <class T> static ReferencePointer<T> s_lcm (T const& c1, T const& c2);
        */
      
    };
    
  };
  
  /***************************************************************************
   * Global operators.
   ***************************************************************************/
  
  // Stream I/O operators.
  std::ostream& operator << (std::ostream& l, DAC::Arb  const& r);
  std::istream& operator >> (std::istream& l, DAC::Arb&        r);
  
  /***************************************************************************
   * Error declarations.
   ***************************************************************************/
  namespace DAC {
    
    // Errors.
    namespace ArbErrors {
      class Base      : public Exception { public: virtual char const* what () const throw(); };
      class BadFormat : public Base      {
        public:
          virtual char const* what () const throw();
          virtual BadFormat& Problem  (char const*                   const problem)  throw();
          virtual BadFormat& Position (std::string::size_type        const position) throw();
          virtual BadFormat& Number   (ConstReferencePointer<std::string>& number)   throw();
        protected:
          char const* _problem;
          std::string::size_type _position;
          ConstReferencePointer<std::string> _number;
      };
    };
    
  };
    
  /***************************************************************************
   * Inline and template definitions.
   ***************************************************************************/
  namespace DAC {
    
    // Errors.
    namespace ArbErrors {
      inline char const* Base::what          () const throw() { return "Undefined error in Arb.";                                                                                                                                            }
      inline char const* BadFormat::what     () const throw() { return (std::string(_problem) + " at position " + DAC::toString((SafeInteger<std::string::size_type>(_position) + 1).Value()) + " in number \"" + *_number + "\".").c_str(); }
      inline BadFormat&  BadFormat::Problem  (char const*                   const problem)  throw() { _problem  = problem;  return *this; }
      inline BadFormat&  BadFormat::Position (std::string::size_type        const position) throw() { _position = position; return *this; }
      inline BadFormat&  BadFormat::Number   (ConstReferencePointer<std::string>& number)   throw() { _number   = number;   return *this; }
    };
    
    /*************************************************************************
     * Class Arb.
     *************************************************************************/
    
    /*
    // Get the greatest common divisor of two big-endian containers. Base
    // is 2^(bits/2).
    template <class T> ReferencePointer<T> Arb::s_gcd (T const& c1, T const& c2) {
      
      // Return value.
      ReferencePointer<T> retval(new T);
      
      // Work area.
      T tmp_c1(c1);
      T tmp_c2(c2);
      if (s_lt(tmp_c1, tmp_c2)) {
        tmp_c1.swap(tmp_c2);
      }
      
      // Euclid's algorithm.
      while (tmp_c2 != 0) {
        T                   tmp = tmp_c2;
        ReferencePointer<T> remainder(new T);
        s_intDiv(tmp_c1, tmp_c2, remainder);
        tmp_c2.swap(*remainder);
        tmp_c1.swap(tmp);
      }
      
      // Swap in result and return.
      retval->swap(tmp_c1);
      return retval;
      
    }
    
    // Get the least common multiple of two big-endian containers. Base is
    // 2^(bits/2).
    template <class T> ReferencePointer<T> Arb::s_lcm (T const& c1, T const& c2) {
      
      // Return value.
      ReferencePointer<T> retval(new T);
      
      // Work area.
      T tmp_c1(c1);
      T tmp_c2(c2);
      if (s_lt(tmp_c1, tmp_c2)) {
        tmp_c1.swap(tmp_c2);
      }
      
      // LCM is (c1 * c2) / gcd(c1, c2). c1 is always the largest of the two,
      // save the multiplication by c1 until the end when we have the smallest
      // number possible.
      return s_intMul(*s_intDiv(tmp_c2, s_gcd(tmp_c1, tmp_c2)), tmp_c1);
      
    }
    */
    
  };
  
  /***************************************************************************
   * Inline and template definitions.
   ***************************************************************************/
  
  // Stream I/O operators.
  inline std::ostream& operator << (std::ostream& l, DAC::Arb  const& r) { l << r.toString();                                  return l; }
  inline std::istream& operator >> (std::istream& l, DAC::Arb&        r) { std::string input; std::cin >> input; r.set(input); return l; }
  
#endif
