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
  #include <vector>
  
  // Internal includes.
  #include "SafeInteger/SafeInteger.hxx"
  #include "ReferencePointer.hxx"
  
  // Forward declarations.
  namespace DAC {
    class Arb;
  };
  
  // Stream I/O operators.
  std::ostream& operator << (std::ostream& left, DAC::Arb const& right);
  std::istream& operator >> (std::istream& left, DAC::Arb&       right);
  
  // Namespace wrapping.
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
    
    /*************************************************************************
     * Arb
     *************************************************************************
     * This is an arbitrary precision number class.
     *************************************************************************/
    class Arb {
      
      /*
       * Public members.
       */
      public:
        
        /*********************************************************************/
        // Typedefs.
        
        // This sets the exponent type for the number. This must be a signed
        // type.
        typedef signed int ExponentT;
        
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
        Arb& set (std::string const& number, ExponentT const decimal = 0, bool const fix = false);
      
      /*
       * Private members.
       */
      private:
        
        /*********************************************************************/
        // Typedefs.
        
        // Forward declarations.
        class _Data;
        
        typedef SafeInteger<ExponentT>     _ExpT;    // Exponent type.
        typedef SafeInteger<unsigned int>  _DigT;    // Native digit.
        typedef SafeInteger<signed int>    _BaseT;   // Base type.
        typedef SafeInteger<unsigned char> _NumChrT; // Numeric character.
        typedef SafeInteger<char>          _StrChrT; // String character.
        typedef _NumChrT                   _DigChrT; // Digit character.
        
        typedef std::vector<_DigChrT> _DigStrT; // String of character digits.
        typedef std::vector<_DigT>    _DigsT;   // String of native digits.
        
        typedef ReferencePointer<_Data> _DataPT; // Pointer to number data.
        
        /*********************************************************************/
        // Data types.
        
        // This holds all data for this number, copying this data structure to
        // another number will make a perfect copy of this number.
        class _Data {
          public:
            bool   positive; // True if the number is positive.
            _DigsT p;        // Numerator.
            _DigsT q;        // Denominator.
            _ExpT  exponent; // Exponent.
            _ExpT  fixexp;   // Fix the exponent at this level.
            bool   fix;      // If true, fix the exponent at fixexp.
            _BaseT origbase; // Original base of this number.
            _Data ();
            _Data& clear ();
            void _init ();
        };
        
        /*********************************************************************/
        // Data members.
        
        // The number.
        _DataPT _data;
        
        /*********************************************************************/
        // Static data members.
        static bool s_initialized; // True if this class has been successfully
                                   // initialized.
        
        static SafeInteger<int> s_digitbits; // The number of bits in a digit.
        static _BaseT           s_digitbase; // The base of native digits.
        
        static _NumChrT              s_numdigits; // Number of output digits.
        static _StrChrT              s_odigits[]; // Output digits.
        static std::vector<_NumChrT> s_idigits;   // Input digits.
        
        /*********************************************************************/
        // Function members.
        
        // Common initialization tasks.
        void _init ();
        
        /*********************************************************************/
        // Static function members.
        
        // Class initialization.
        static bool s_classInit () throw();
        
        // Trim zeros from a given container.
        template <class T> static typename T::size_type s_trimZerosB (T& c);
        template <class T> static typename T::size_type s_trimZerosE (T& c);
        template <class T> static typename T::size_type s_trimZeros  (T& c);
        
        // Do long division on a given container. Container must be little-
        // endian.
        template <class DivndT, class DivorT> static DivorT s_longDiv (DivndT& divnd, DivorT const divor, _BaseT const base);
        
        // Convert a container from one base to another base. Container must
        // be little-endian, result is big-endian.
        template <class FromT, class ToT> static ReferencePointer<ToT> s_baseConv (FromT const& from, _BaseT const frombase, _BaseT const tobase);

        // Perform integer division on two big-endian containers. Returns the
        // remainder.
        template <class T> static ReferencePointer<T> s_intDiv (T const& divor, T const& divnd, T& quotient);
        
        // Comparison operators on containers.
        template <class T> static bool s_lt (T const& l, T const& r);
        template <class T> static bool s_le (T const& l, T const& r);
        template <class T> static bool s_gt (T const& l, T const& r);
        template <class T> static bool s_ge (T const& l, T const& r);
        template <class T> static bool s_eq (T const& l, T const& r);
        template <class T> static bool s_ne (T const& l, T const& r);
      
    };
    
    /*************************************************************************
     * Inline and template definitions.
     *************************************************************************/
    
    // Errors.
    namespace ArbErrors {
      inline char const* Base::what          () const throw() { return "Undefined error in Arb.";                                                                                                                                          }
      inline char const* BadFormat::what     () const throw() { return (std::string(_problem) + " at position " + DAC::toString((SafeInteger<std::string::size_type>(_position) + 1).Value()) + " in number \"" + *_number + "\".").c_str(); }
      inline BadFormat&  BadFormat::Problem  (char const*                   const problem)  throw() { _problem  = problem;  return *this; }
      inline BadFormat&  BadFormat::Position (std::string::size_type        const position) throw() { _position = position; return *this; }
      inline BadFormat&  BadFormat::Number   (ConstReferencePointer<std::string>& number)   throw() { _number   = number;   return *this; }
    };
    
    /*************************************************************************
     * Class Arb.
     *************************************************************************/
    
    // Trim leading zeros from a given container.
    template <class T> typename T::size_type Arb::s_trimZerosB (T& c) {
      
      // Nothing to do if empty.
      if (c.empty()) {
        return 0;
      }
      
      // Work data.
      typename T::iterator               pos;
      SafeInteger<typename T::size_type> retval;
      
      // Trim leading zeros.
      for (pos = c.begin(); (pos != c.end()) && (*pos == 0); ++pos) {}
      if (pos >= c.begin()) {
        retval = pos - c.begin();
        c.erase(c.begin(), pos);
      }
      
      // We done.
      return retval.Value();
      
    }
    
    // Trim trailing zeros from a given container.
    template <class T> typename T::size_type Arb::s_trimZerosE (T& c) {
      
      // Nothing to do if empty.
      if (c.empty()) {
        return 0;
      }
      
      // Work data.
      typename T::iterator               pos;
      SafeInteger<typename T::size_type> retval;
      
      // Trim trailing zeros.
      for (pos = (c.end() - 1); (pos != (c.begin() - 1)) && (*pos == 0); --pos) {}
      if (pos++ < (c.end() - 1)) {
        retval = c.end() - pos;
        c.erase(pos, c.end());
      }
      
      // We done.
      return retval.Value();
      
    }
    
    // Trim leading and trailing zeros from a given container.
    template <class T> inline typename T::size_type Arb::s_trimZeros (T& c) { return (SafeInteger<typename T::size_type>(s_trimZerosB(c)) + SafeInteger<typename T::size_type>(s_trimZerosE(c))).Value(); }
    
    // Do long division on a given container. Container must be little-
    // endian.
    template <class DivndT, class DivorT> DivorT Arb::s_longDiv (DivndT& divnd, DivorT const divor, _BaseT const base) {
      
      // Group of digits to divide.
      DivorT dgroup = 0;
      DivorT dquot  = 0;
      
      // Quotient.
      DivndT quotient;
      
      // Long division steps through each digit of the dividend.
      for (typename DivndT::iterator i = divnd.begin(); i != divnd.end(); ++i) {
        
        // Add this digit to the group.
        dgroup += *i;
        
        // Divide the group and add the result to the quotient.
        dquot = dgroup / divor;
        quotient.push_back(dquot);
        
        // Take out what we've divided.
        dgroup -= dquot * divor;
        
        // Move the remainder up to the next order of magnitude.
        dgroup *= base.Value();
        
      }
      
      // Trim insignificant zeros from the quotient.
      s_trimZerosB(quotient);
      
      // Set the result in place.
      divnd.swap(quotient);
      
      // Undo the last base shift, this is the remainder. Return it.
      return (dgroup / base.Value());
      
    }
    
    // Convert a container to a given base. Container must be little-endian,
    // result is big-endian.
    template <class FromT, class ToT> ReferencePointer<ToT> Arb::s_baseConv (FromT const& from, _BaseT const frombase, _BaseT const tobase) {
      
      // Return this container, using a reference counting pointer to get around
      // reference scoping rules.
      ReferencePointer<ToT> retval(new ToT);
      
      // Temporary, since we will be doing work in place.
      FromT tempfrom(from);
      
      // Trim insignificant zeros.
      s_trimZerosB(tempfrom);
      
      // Convert base by storing the remainder of repeated division.
      while (tempfrom.size() > 0) {
        retval->push_back(s_longDiv(tempfrom, tobase, frombase));
      }
      
      // Return.
      return retval;
      
    }
    
    // Perform integer division on two big-endian containers. Returns the
    // remainder.
    template <class T> ReferencePointer<T> Arb::s_intDiv (T const& divor, T const& divnd, T& quotient) {
      
      // Return this container.
      ReferencePointer<T> retval(new T);
      
      // If divor < divnd, result is zero, remainder is divnd.
      if (s_lt(divor, divnd)) {
        retval = divnd;
        quotient.clear();
        return retval;
      }
      
      // First, seed the group of digits we will be dividing.
      T diggroup(divor.rbegin(), divor.rbegin() + divnd.size());
      
      // Iterate through the divisor.
      for (typename T::reverse_iterator i = divor.rbegin() + divnd.size(); i != divor.rend(); ++i) {
      }
      
      // Return.
      return retval;
      
    }
    
    // Comparison operators on containers.
    template <class T> bool Arb::s_lt (T const& l, T const& r) {
      
      // Don't get fooled by zero size.
      if ((l.size() == 0) && (r.size() == 0)) {
        return false;
      }
      
      // If the size of one container is larger, that is the larger container.
      if (l.size() < r.size()) {
        return true;
      }
      if (l.size() > r.size()) {
        return false;
      }
      
      // Examine individual digits, starting from most significant. Return a
      // result as soon as possible.
      for (typename T::size_type i = l.size() - 1; i != 0; --i) {
        if (l[i] < r[i]) {
          return true;
        }
        if (l[i] > r[i]) {
          return false;
        }
      }
      
      // All digits are equal, l is not less than r.
      return false;
      
    }
    template <class T> bool Arb::s_le (T const& l, T const& r) { return (s_lt(l, r) || !s_lt(r, l));  }
    template <class T> bool Arb::s_gt (T const& l, T const& r) { return s_lt(r, l);                   }
    template <class T> bool Arb::s_ge (T const& l, T const& r) { return (s_gt(l, r) || !s_gt(r, l));  }
    template <class T> bool Arb::s_eq (T const& l, T const& r) { return (!s_lt(l, r) && !s_gt(l, r)); }
    template <class T> bool Arb::s_ne (T const& l, T const& r) { return (s_lt(l, r) || s_gt(l, r));   }
    
  };
  
  /***************************************************************************
   * Inline and template definitions.
   ***************************************************************************/
  
  inline std::ostream& operator << (std::ostream& left, DAC::Arb const& right) { left << right.toString();                               return left; }
  inline std::istream& operator >> (std::istream& left, DAC::Arb&       right) { std::string input; std::cin >> input; right.set(input); return left; }
  
#endif
