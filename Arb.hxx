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
        
        // Use this type when specifying a base.
        typedef _DigT BaseT;
        
        /*********************************************************************/
        // Enums
        
        // Format of number when output.
        enum OutputFormat { FMT_RADIX, FMT_FRACTION, FMT_BOTH, FMT_DEFAULT };
        
        // Rounding method.
        enum RoundMethod { ROUND_NORMAL, ROUND_UP, ROUND_DOWN, ROUND_TOWARD_ZERO, ROUND_FROM_ZERO, ROUND_DEFAULT };
        
        /*********************************************************************/
        // Function members.
        
        // Default constructor.
        Arb ();
        
        // Copy constructor.
        Arb (Arb const& number, bool const copynow = false);
        
        // Conversion constructor.
                           Arb (std::string const& number);
        template <class T> Arb (T           const  number);
        
        // Boolean conversion operator.
        operator bool () const;
        
        // Unary sign operators.
        Arb operator - () const;
        
        // Increment / decrement operators.
        Arb& operator ++ ();
        Arb  operator ++ (int);
        Arb& operator -- ();
        Arb  operator -- (int);
        
        // Not operator.
        bool operator ! () const;
        
        // Assignment operator.
                           Arb& operator = (std::string const& number);
                           Arb& operator = (Arb         const& number);
        template <class T> Arb& operator = (T           const  number);
        
        // Arithmetic assignment operators.
        Arb& operator *= (Arb const& number);
        Arb& operator /= (Arb const& number);
        Arb& operator %= (Arb const& number);
        Arb& operator += (Arb const& number);
        Arb& operator -= (Arb const& number);
        
        // Accessors.
                           Arb&                   Base     (BaseT const base);
                           BaseT                  Base     ()                                      const;
                           Arb&                   MaxRadix (std::string::size_type const maxradix);
                           std::string::size_type MaxRadix ()                                      const;
                           Arb&                   PointPos (PointPosT const pointpos);
                           PointPosT              PointPos ()                                      const;
                           Arb&                   Fixed    (bool const fixed);
                           bool                   Fixed    ()                                      const;
                           Arb&                   Format   (OutputFormat const format);
                           OutputFormat           Format   ()                                      const;
                           Arb&                   PropCopy (bool const propcopy);
                           bool                   PropCopy ()                                      const;
                           Arb&                   Round    (RoundMethod const round);
                           RoundMethod            Round    ()                                      const;
        template <class T> Arb&                   Value    (T const number);
        template <class T> T                      Value    ()                                      const;
        
        // Reset to just-constructed defaults.
        Arb& clear ();
        
        // Copy another number.
        Arb& copy (Arb const& number, bool const copynow = false);
        
        // Convert to string.
        std::string toString (OutputFormat const format = FMT_DEFAULT) const;
        
        // Set the number.
                           Arb& set      (std::string const& number);
                           Arb& set      (Arb         const& number);
        template <class T> Arb& set      (T           const  number);
        template <class T> Arb& setInt   (T           const  number);
        template <class T> Arb& setFloat (T           const  number);
        
        // Arithmetic operator backends.
        Arb& op_mul (Arb const& number);
        Arb& op_div (Arb const& number);
        Arb& op_mod (Arb const& number);
        Arb& op_add (Arb const& number);
        Arb& op_sub (Arb const& number);
        
        // Comparison operator backends.
        bool op_gt (Arb const& number) const;
        bool op_ge (Arb const& number) const;
        bool op_lt (Arb const& number) const;
        bool op_le (Arb const& number) const;
        bool op_eq (Arb const& number) const;
        bool op_ne (Arb const& number) const;
        
        // Return information about this number.
        bool isInteger  () const;
        bool isPositive () const;
        bool isZero     () const;
        bool isEven     () const;
        bool isOdd      () const;
        
        // Get the floor of this fractional number.
        Arb floor () const;
        
        // Raise this number to a power.
        Arb pow (Arb const& exp) const;
        
        // Find the nth root of this number.
        Arb root (Arb const& n) const;
        
        // Return the absolute value of this number.
        Arb abs () const;
        
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
            
            _DigsT p;    // Numerator.
            _DigsT q;    // Denominator.
            _DigsT fixq; // Fixed denominator.
            
            bool      positive; // True if the number is positive.
            PointPosT pointpos; // Radix point position.
            bool      fix;      // If true, fix the radix point.
            BaseT     base;     // The base of the number.
            
            /*****************************************************************/
            // Function members.
            
            // Default constructor.
            _Data ();
            
            // Copy constructor.
            _Data (_Data const& data);
            
            // Assignment operator.
            _Data& operator = (_Data const& data);
            
            // Reset to just-constructed state.
            _Data& clear ();
            
            // Copy a given _Data object.
            _Data& copy (_Data const& data);
            
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
        
        // Properties.
        std::string::size_type _maxradix; // Radix digits to output.
        OutputFormat           _format;   // Format to output this number.
        bool                   _propcopy; // Copy the properties of other
                                          // numbers when setting with =.
        RoundMethod            _round;    // Rounding method.
        
        /*********************************************************************/
        // Function members.
        
        // Common initialization tasks.
        void _init ();
        
        // Reduce the number to its most compact representation.
        Arb& _reduce      ();
        Arb& _forcereduce (_DigsT const& q);
        
        // Normalize this number to another number.
        Arb& _normalize (Arb& number);
        
        // Find a whole-number root of this number.
        Arb& _root (_DigsT const& root);
        
        // Root finding function, x^y - z.
        Arb _f (Arb const& x, Arb const& y) const;
        
    };
    
  };
  
  /***************************************************************************
   * Global operators.
   ***************************************************************************/
  
  // Stream I/O operators.
  std::ostream& operator << (std::ostream& l, DAC::Arb  const& r);
  std::istream& operator >> (std::istream& l, DAC::Arb&        r);
  
  // Arithmetic operators.
  DAC::Arb operator * (DAC::Arb const& l, DAC::Arb const& r);
  DAC::Arb operator / (DAC::Arb const& l, DAC::Arb const& r);
  DAC::Arb operator % (DAC::Arb const& l, DAC::Arb const& r);
  DAC::Arb operator + (DAC::Arb const& l, DAC::Arb const& r);
  DAC::Arb operator - (DAC::Arb const& l, DAC::Arb const& r);
  
  // Comparison operators.
  bool operator >  (DAC::Arb const& l, DAC::Arb const& r);
  bool operator >= (DAC::Arb const& l, DAC::Arb const& r);
  bool operator <  (DAC::Arb const& l, DAC::Arb const& r);
  bool operator <= (DAC::Arb const& l, DAC::Arb const& r);
  bool operator == (DAC::Arb const& l, DAC::Arb const& r);
  bool operator != (DAC::Arb const& l, DAC::Arb const& r);
  
  /***************************************************************************
   * Error declarations.
   ***************************************************************************/
  namespace DAC {
    
    // Errors.
    namespace ArbErrors {
      class Base       : public Exception { public: virtual char const* what () const throw(); };
      class BadFormat  : public Base      {
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
      class DivByZero  : public Base      { public: virtual char const* what () const throw(); };
      class Complex    : public Base      { public: virtual char const* what () const throw(); };
      class NonInteger : public Base      { public: virtual char const* what () const throw(); };
    };
    
  };
  
  /***************************************************************************
   * Declare template specializations.
   ***************************************************************************/
  
  namespace DAC {
    
    /*************************************************************************
     * Class Arb.
     *************************************************************************/
    
    // Set from a built-in type.
    template <> inline Arb& Arb::set (bool               const number);
    template <> inline Arb& Arb::set (unsigned char      const number);
    template <> inline Arb& Arb::set (signed   char      const number);
    template <> inline Arb& Arb::set (unsigned short int const number);
    template <> inline Arb& Arb::set (signed   short int const number);
    template <> inline Arb& Arb::set (unsigned int       const number);
    template <> inline Arb& Arb::set (signed   int       const number);
    template <> inline Arb& Arb::set (unsigned long int  const number);
    template <> inline Arb& Arb::set (signed   long int  const number);
    template <> inline Arb& Arb::set (float              const number);
    template <> inline Arb& Arb::set (double             const number);
    template <> inline Arb& Arb::set (long double        const number);
    
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
      inline char const* DivByZero::what     () const throw() { return "Divide by zero.";                                                                                                                                                    }
      inline char const* Complex::what       () const throw() { return "Even roots of negative numbers can only be complex numbers.";                                                                                                        }
      inline char const* NonInteger::what    () const throw() { return "An integer operation was attempted on non-integer numbers.";                                                                                                         }
    };
    
    /*************************************************************************
     * Class Arb.
     *************************************************************************/
    
    // Conversion constructor.
    template <class T> Arb::Arb (T const number) {
      
      // Call common init.
      _init();
      
      // Set the number.
      set<T>(number);
      
    }
    
    // Boolean conversion operator.
    inline Arb::operator bool () const { return _data->p; }
    
    // Negation operator.
    inline Arb Arb::operator - () const { Arb retval(*this, true); retval._data->positive = !retval._data->positive; return retval; }
    
    // Increment / decrement operators.
    inline Arb& Arb::operator ++ ()    { return op_add(1);                         }
    inline Arb  Arb::operator ++ (int) { Arb retval(*this); op_add(1); return retval; }
    inline Arb& Arb::operator -- ()    { return op_sub(1);                         }
    inline Arb  Arb::operator -- (int) { Arb retval(*this); op_sub(1); return retval; }
    
    // Not operator.
    inline bool Arb::operator ! () const { return !(_data->p); }
    
    // Assignment operator.
                       inline Arb& Arb::operator = (std::string const& number) { return set(number);                                                  }
                       inline Arb& Arb::operator = (Arb         const& number) { if (_propcopy) { return copy(number); } else { return set(number); } }
    template <class T> inline Arb& Arb::operator = (T           const  number) { return set<T>(number);                                               }
    
    // Arithmetic assignment operators.
    inline Arb& Arb::operator *= (Arb const& number) { return op_mul(number); }
    inline Arb& Arb::operator /= (Arb const& number) { return op_div(number); }
    inline Arb& Arb::operator %= (Arb const& number) { return op_mod(number); }
    inline Arb& Arb::operator += (Arb const& number) { return op_add(number); }
    inline Arb& Arb::operator -= (Arb const& number) { return op_sub(number); }
    
    // Accessors.
                       inline Arb::BaseT             Arb::Base     ()                                      const { return _data->base;                                                               }
                       inline Arb&                   Arb::MaxRadix (std::string::size_type const maxradix)       { _maxradix = maxradix; return *this;                                               }
                       inline std::string::size_type Arb::MaxRadix ()                                      const { return _maxradix;                                                                 }
                       inline Arb::PointPosT         Arb::PointPos ()                                      const { return _data->pointpos;                                                           }
                       inline bool                   Arb::Fixed    ()                                      const { return _data->fix;                                                                }
                       inline Arb&                   Arb::Format   (OutputFormat const format)                   { _format = (format == FMT_DEFAULT) ? _format : format; return *this;               }
                       inline Arb::OutputFormat      Arb::Format   ()                                      const { return _format;                                                                   }
                       inline Arb&                   Arb::PropCopy (bool const propcopy)                         { _propcopy = propcopy; return *this;                                               }
                       inline bool                   Arb::PropCopy ()                                      const { return _propcopy;                                                                 }
                       inline Arb&                   Arb::Round    (RoundMethod const round)                     { _round = (round == ROUND_DEFAULT) ? _round : round; return *this;                 }
                       inline Arb::RoundMethod       Arb::Round    ()                                      const { return _round;                                                                    }
    template <class T> inline Arb&                   Arb::Value    (T const number)                              { return set<T>(number);                                                            }
    template <class T> inline T                      Arb::Value    ()                                      const { if (!isInteger()) { throw "Unimplemented"; } else { return _data->p.Value<T>(); } }
    
    // Set from a built-in type.
    template <> inline Arb& Arb::set (bool               const number) { setInt(number);   }
    template <> inline Arb& Arb::set (unsigned char      const number) { setInt(number);   }
    template <> inline Arb& Arb::set (signed   char      const number) { setInt(number);   }
    template <> inline Arb& Arb::set (unsigned short int const number) { setInt(number);   }
    template <> inline Arb& Arb::set (signed   short int const number) { setInt(number);   }
    template <> inline Arb& Arb::set (unsigned int       const number) { setInt(number);   }
    template <> inline Arb& Arb::set (signed   int       const number) { setInt(number);   }
    template <> inline Arb& Arb::set (unsigned long int  const number) { setInt(number);   }
    template <> inline Arb& Arb::set (signed   long int  const number) { setInt(number);   }
    template <> inline Arb& Arb::set (float              const number) { setFloat(number); }
    template <> inline Arb& Arb::set (double             const number) { setFloat(number); }
    template <> inline Arb& Arb::set (long double        const number) { setFloat(number); }
    
    // Set from an integer type.
    template <class T> Arb& Arb::setInt (T const number) {
      
      // Work area.
      Arb new_num;
      
      // Carry over old fixed-point properties.
      new_num._data->fix      = _data->fix;
      new_num._data->pointpos = _data->pointpos;
      new_num._data->base     = _data->base;
      new_num._data->fixq     = _data->fixq;
      
      // This number is 1s.
      new_num._data->p = number;
      new_num._data->q = 1;
      
      // Reduce the fraction.
      new_num._reduce();
      
      // Move the new data into place and return.
      _data = new_num._data;
      return *this;
      
    }
    
    // Set from a floating-point type.
    template <class T> inline Arb& Arb::setFloat (T const number) { return set(DAC::toString(number)); }
    
    // Comparison operator backends.
    inline bool Arb::op_ge (Arb const& number) const { return (op_gt(number) || op_eq(number));  }
    inline bool Arb::op_lt (Arb const& number) const { return number.op_gt(*this);               }
    inline bool Arb::op_le (Arb const& number) const { return (op_lt(number) || op_eq(number));  }
    inline bool Arb::op_eq (Arb const& number) const { return !(op_gt(number) || op_lt(number)); }
    inline bool Arb::op_ne (Arb const& number) const { return (op_gt(number) || op_lt(number));  }
    
    // Return whether this number is an integer.
    inline bool Arb::isInteger () const { return (_data->q == _DigsT(1)); }
    
    // Return whether this number is positive.
    inline bool Arb::isPositive () const { return _data->positive; }
    
    // Return whether this number is equal to zero.
    inline bool Arb::isZero () const { return _data->p.isZero(); }
    
    // Return whether this number is even or odd.
    inline bool Arb::isOdd  () const { return (isInteger() && _data->p.isOdd());  }
    inline bool Arb::isEven () const { return (isInteger() && _data->p.isEven()); }
    
    // Return the absolute value of this number.
    inline Arb Arb::abs () const { Arb retval(*this, true); retval._data->positive = true; return retval; }
    
    // Root finding function, x^y - z.
    inline Arb Arb::_f (Arb const& x, Arb const& y) const { return x.pow(y) - *this; }
    
  };
  
  /***************************************************************************
   * Inline and template definitions.
   ***************************************************************************/
  
  // Stream I/O operators.
  inline std::ostream& operator << (std::ostream& l, DAC::Arb  const& r) { l << r.toString();                                  return l; }
  inline std::istream& operator >> (std::istream& l, DAC::Arb&        r) { std::string input; std::cin >> input; r.set(input); return l; }
  
  // Arithmetic operators.
  inline DAC::Arb operator * (DAC::Arb const& l, DAC::Arb const& r) { return DAC::Arb(l).op_mul(r); }
  inline DAC::Arb operator / (DAC::Arb const& l, DAC::Arb const& r) { return DAC::Arb(l).op_div(r); }
  inline DAC::Arb operator % (DAC::Arb const& l, DAC::Arb const& r) { return DAC::Arb(l).op_mod(r); }
  inline DAC::Arb operator + (DAC::Arb const& l, DAC::Arb const& r) { return DAC::Arb(l).op_add(r); }
  inline DAC::Arb operator - (DAC::Arb const& l, DAC::Arb const& r) { return DAC::Arb(l).op_sub(r); }
  
  // Comparison operators.
  inline bool operator >  (DAC::Arb const& l, DAC::Arb const& r) { return l.op_gt(r); }
  inline bool operator >= (DAC::Arb const& l, DAC::Arb const& r) { return l.op_ge(r); }
  inline bool operator <  (DAC::Arb const& l, DAC::Arb const& r) { return l.op_lt(r); }
  inline bool operator <= (DAC::Arb const& l, DAC::Arb const& r) { return l.op_le(r); }
  inline bool operator == (DAC::Arb const& l, DAC::Arb const& r) { return l.op_eq(r); }
  inline bool operator != (DAC::Arb const& l, DAC::Arb const& r) { return l.op_ne(r); }
  
#endif
