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
  #include <algorithm>
  
  // Internal includes.
  #include "SafeInteger/SafeInteger.hxx"
  #include "ReferencePointer.hxx"
  #include "rppower.hxx"
  
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
      class Overrun   : public Base      { public: virtual char const* what () const throw(); };
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
        
        // Do long division or multiplication on a given container. Container
        // must be little-endian for division, big-endian for multiplication.
        // These are grouped because they both involve applying one number to
        // a container, but semantics of each are very different.
        template <class DivndT, class DivorT> static DivorT               s_longDiv (DivndT& divnd, DivorT const divor, _BaseT const base);
        template <class LT,     class RT>     static ReferencePointer<LT> s_longMul (LT const& l, RT const r);
        
        // Convert a container from one base to another base. Container must
        // be little-endian, result is big-endian.
        template <class FromT, class ToT> static ReferencePointer<ToT> s_baseConv (FromT const& from, _BaseT const frombase, _BaseT const tobase);
        
        // Perform integer arithmetic on two big-endian containers.
        // Subtraction requires that minuend be greater than or equal to
        // the subtrahend. Base is 2^(bits/2).
        template <class T> static ReferencePointer<T> s_intMul (T const& mulnd, T const& mulor);
        template <class T> static ReferencePointer<T> s_intDiv (T const& divor, T const& divnd, ReferencePointer<T> remainder = 0);
        template <class T> static ReferencePointer<T> s_intAdd (T const& aug,   T const& add);
        template <class T> static ReferencePointer<T> s_intSub (T const& min,   T const& sub);
        
        // Get the greatest common divisor and least common multiple of two
        // big-endian containers. Base is 2^(bits/2).
        template <class T> static ReferencePointer<T> s_gcd (T const& c1, T const& c2);
        template <class T> static ReferencePointer<T> s_lcm (T const& c1, T const& c2);
        
        // Perform a carry on a big-endian container. Base is 2^(bits/2).
        template <class T> static void s_carry  (T& c, typename T::size_type const start);
        template <class T> static void s_borrow (T& c, typename T::size_type const start);
        
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
      inline char const* Base::what          () const throw() { return "Undefined error in Arb.";                                                                                                                                            }
      inline char const* BadFormat::what     () const throw() { return (std::string(_problem) + " at position " + DAC::toString((SafeInteger<std::string::size_type>(_position) + 1).Value()) + " in number \"" + *_number + "\".").c_str(); }
      inline BadFormat&  BadFormat::Problem  (char const*                   const problem)  throw() { _problem  = problem;  return *this; }
      inline BadFormat&  BadFormat::Position (std::string::size_type        const position) throw() { _position = position; return *this; }
      inline BadFormat&  BadFormat::Number   (ConstReferencePointer<std::string>& number)   throw() { _number   = number;   return *this; }
      inline char const* Overrun::what       () const throw() { return "Instruction overruns end of container.";                                                                                                                             }
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
    
    // Perform integer multiplication on two big-endian containers. Base is
    // 2^(bits/2).
    template <class T> ReferencePointer<T> Arb::s_intMul (T const& mulnd, T const& mulor) {
      
      // Return value.
      ReferencePointer<T> retval(new T);
      
      // Multiply like 3rd grade.
      for (typename T::size_type i = 0; i != mulor.size(); ++i) {
        
        // Get the product for a single digit.
        ReferencePointer<T> digproduct(s_longMul(mulnd, mulor[i]));
        
        // Offset this digit product and add it to the final product.
        digproduct->insert(digproduct->begin(), i, 0);
        retval = s_intAdd(*retval, *digproduct);
        
      }
      
      // Return the result.
      return retval;
      
    }
    
    // Perform integer division on two big-endian containers. Returns the
    // quotient. Base is 2^(bits/2).
    template <class T> ReferencePointer<T> Arb::s_intDiv (T const& divor, T const& divnd, ReferencePointer<T> remainder) {
      
      // Return this container.
      ReferencePointer<T> retval(new T);
      
      // If divor < divnd, result is zero, remainder is divnd.
      if (s_lt(divor, divnd)) {
        if (remainder.get() != 0) {
          remainder = divnd;
        }
        return retval;
      }
      
      // Make a copy of the high-order digit of the dividend, this will be
      // used frequently.
      typename T::value_type roughdivnd = divnd.back();
      
      // Seed the group of digits we will be dividing, then iterate through
      // the divisor.
      T diggroup(divor.rbegin(), divor.rbegin() + divnd.size());
      T test;
      for (typename T::reverse_iterator i = divor.rbegin() + divnd.size(); i != divor.rend(); ++i) {
        
        // Make a guess at the quotient of this digit by dividing the high-
        // order digits.
        typename T::value_type guess = diggroup.back() / roughdivnd;
        
        // Correct the guess.
        test.swap(*s_longMul(divnd, guess));
        if (s_gt(test, diggroup)) {
          --guess;
          test.swap(*s_longMul(divnd, guess));
        } else if (s_ge(*s_intSub(diggroup, test), divnd)) {
          ++guess;
          test.swap(*s_longMul(divnd, guess));
        }
        
        // The guess must be correct by this point. Add it to the quotient and
        // prepare for the next iteration if there will be one.
        retval->insert(retval->begin(), guess);
        diggroup.swap(*s_intSub(diggroup, test));
        
      }
      
      // Swap in the remainder if a container was given.
      if (remainder.refs != 0) {
        remainder->swap(diggroup);
      }
      
      // Return.
      return retval;
      
    }
    
    // Perform integer addition on two big-endian containers. Base is
    // 2^(bits/2).
    template <class T> ReferencePointer<T> Arb::s_intAdd (T const& aug, T const& add) {
      
      // Return value.
      ReferencePointer<T> retval(new T);
      
      // Iterate through the larger digit, adding to the return value. Cache
      // sizes.
      typename T::size_type augsz = aug.size();
      typename T::size_type addsz = add.size();
      typename T::size_type digs  = max(augsz, addsz);
      for (typename T::size_type i = 0; i != digs; ++i) {
        
        // Add a new digit if necessary.
        if (retval->size() == i) {
          retval->push_back(0);
        }
        
        // Add the augend and addend.
        if (augsz > i) {
          *retval[i]  = aug[i];
        }
        if (addsz > i) {
          *retval[i] += add[i];
        }
        
        // Do any carry needed.
        s_carry(*retval, i);
        
      }
      
      // Return.
      return retval;
      
    }
    
    // Perform integer subtraction on two big-endian containers. This is
    // an unsigned operation, so minuend must be greater than or equal to
    // subtrahend. Base is 2^(bits/2).
    template <class T> ReferencePointer<T> Arb::s_intSub (T const& min, T const& sub) {
      
      // Ensure that min >= sub.
      if (!s_ge(min, sub)) {
        throw ArbErrors::Overrun();
      }
      
      // Copy min into the return container.
      ReferencePointer<T> retval(new T(min));
      
      // Iterate through min, subtracting each digit of sub.
      for (typename T::size_type i = 0; i != sub.size(); ++i) {
        
        // Perform borrow if necessary.
        if (retval[i] < sub[i]) {
          s_borrow(retval, i);
        }
        
        // Subtract this digit.
        retval[i] -= sub[i];
        
      }
      
      // Trim zeros and return.
      s_trimZerosE(*retval);
      return retval;
      
    }
    
    // Multiply a big-endian container by a given number. Base is 2^(bits/2).
    template <class LT, class RT> static ReferencePointer<LT> s_longMul (LT const& l, RT const r) {
      
      // Reduce typing.
      typedef typename LT::value_type LDT;
      
      // This is the product.
      ReferencePointer<LT>   retval;
      typename LT::size_type curpos = 0;
      
      // Multiply like 3rd grade.
      for (typename LT::size_type i = 0; i != l.size(); ++i) {
        
        // Create the digit if necessary.
        if (curpos == retval.size()) {
          retval.push_back(0);
        }
        
        // Multiply one digit and carry.
        retval[curpos] = *i * r;
        s_carry(retval, curpos);
        
      }
      
      // We done.
      return retval;
      
    }
    
    // Get the greatest common divisor of two big-endian containers. Base
    // is 2^(bits/2).
    template <class T> ReferencePointer<T> Arb::s_gcd (T const& c1, T const& c2) {
      
      // Return value.
      ReferencePointer<T> retval;
      
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
      ReferencePointer<T> retval;
      
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
    
    // Perform a carry on a big-endian container. Base is 2^(bits/2).
    template <class T> void Arb::s_carry (T& c, typename T::size_type const start) {
      
      // Reduce typing.
      typedef typename T::value_type DT;
      
      // Calculate the number of bits in a number. This is static so that it
      // is only calculated once for a given type instead of every function
      // call.
      static const int halfbits = std::numeric_limits<DT>::digits >> 1;
      static const DT  base     = rppower(static_cast<DT>(2), halfbits);
      
      // Ensure that the start is not beyond the end of the container.
      if (start >= c.size()) {
        throw ArbErrors::Overrun();
      }
      
      // Loop through the container looking for base overflow.
      for (typename T::size_type i = start; i != c.size(); ++i) {
        
        // If we find overflow, push it to the next digit.
        if (c[i] >= base) {
          
          // Create the next digit if it does not already exist.
          if ((c.size() - 1) <= i) {
            c.push_back(0);
          }
          
          // Add any overflow to the next digit and remove it from the
          // current digit.
          DT overflow  = c[i] / base;
          c[i + 1]    += overflow;
          c[i]        -= overflow * base;
          
        // If there is no overflow, there will be no more overflow.
        } else {
          
          // Nothing to return, work is done in place.
          return;
          
        }
        
      }
      
    }
    
    // Perform a borrow on a big-endian container. Base is 2^(bits/2).
    template <class T> void Arb::s_borrow (T& c, typename T::size_type const start) {
      
      // Reduce typing.
      typedef typename T::value_type DT;
      
      // Calculate the number of bits in a number.
      static const int halfbits = std::numeric_limits<DT>::digits >> 1;
      static const DT  base     = rppower(static_cast<DT>(2), halfbits);
      
      // Ensure that the start is not beyond the end of the container.
      if (start >= c.size()) {
        throw ArbErrors::Overrun();
      }
      
      // Loop through the container borrowing until we've met our borrow.
      for (typename T::size_type i = start; i != (c.size() - 1); ++i) {
        
        // Add base to this digit.
        c[i] += base;
        
        // If the next digit is > 0, subtract 1 and we're done.
        if (c[i + 1] > 0) {
          --c[i + 1];
          s_trimZerosE(c);
          return;
        }
        
      }
      
      // If we got to here, we have not paid for our borrow.
      throw ArbErrors::Overrun();
      
    }
    
    // Comparison operators on big-endian containers.
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
    template <class T> inline bool Arb::s_le (T const& l, T const& r) { return (s_lt(l, r) || !s_lt(r, l));  }
    template <class T> inline bool Arb::s_gt (T const& l, T const& r) { return s_lt(r, l);                   }
    template <class T> inline bool Arb::s_ge (T const& l, T const& r) { return (s_gt(l, r) || !s_gt(r, l));  }
    template <class T> inline bool Arb::s_eq (T const& l, T const& r) { return (!s_lt(l, r) && !s_gt(l, r)); }
    template <class T> inline bool Arb::s_ne (T const& l, T const& r) { return (s_lt(l, r) || s_gt(l, r));   }
    
  };
  
  /***************************************************************************
   * Inline and template definitions.
   ***************************************************************************/
  
  inline std::ostream& operator << (std::ostream& left, DAC::Arb const& right) { left << right.toString();                               return left; }
  inline std::istream& operator >> (std::istream& left, DAC::Arb&       right) { std::string input; std::cin >> input; right.set(input); return left; }
  
#endif
