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
        
        // Reset to just-constructed defaults.
        Arb& clear ();
        
        // Copy another number.
        Arb& copy (Arb const& number, bool const copynow = false);
        
        // Convert to string.
        std::string toString () const;
        
        // Set the number.
                           Arb& set (std::string const& number);
                           Arb& set (Arb         const& number);
        template <class T> Arb& set (T           const  number);
        
        // Arithmetic operator backends.
        Arb& op_mul (Arb const& number);
        Arb& op_div (Arb const& number);
        Arb& op_add (Arb const& number);
        Arb& op_sub (Arb const& number);
        
        // Comparison operator backends.
        bool op_gt (Arb const& number) const;
        bool op_ge (Arb const& number) const;
        bool op_lt (Arb const& number) const;
        bool op_le (Arb const& number) const;
        bool op_eq (Arb const& number) const;
        bool op_ne (Arb const& number) const;
        
        // Raise this number to a power.
        Arb& pow (Arb const& exp);
        
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
            
            _DigsT p;    // Numerator.
            _DigsT q;    // Denominator.
            _DigsT fixq; // Fixed denominator.
            
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
        
        /*********************************************************************/
        // Function members.
        
        // Common initialization tasks.
        void _init ();
        
        // Reduce the number to its most compact representation.
        Arb& _reduce ();
        
        // Normalize this number to another number.
        Arb& _normalize (Arb& number);
        
        // Find a whole-number root of this number.
        Arb& _root (_DigsT const& root);
        
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
      class DivByZero : public Base      { public: virtual char const* what () const throw(); };
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
      inline char const* DivByZero::what     () const throw() { return "Divide by zero.";                                                                                                                                                    }
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
                       inline Arb& Arb::operator = (std::string const& number) { return set(number);    }
                       inline Arb& Arb::operator = (Arb         const& number) { return copy(number);   }
    template <class T> inline Arb& Arb::operator = (T           const  number) { return set<T>(number); }
    
    // Accessors.
    inline Arb::BaseT             Arb::Base     ()                                      const { return _data->base;                 }
    inline Arb&                   Arb::MaxRadix (std::string::size_type const maxradix)       { _maxradix = maxradix; return *this; }
    inline std::string::size_type Arb::MaxRadix ()                                      const { return _maxradix;                   }
    inline Arb::PointPosT         Arb::PointPos ()                                      const { return _data->pointpos;             }
    inline bool                   Arb::Fixed    ()                                      const { return _data->fix;                  }
    
    // Set from a built-in type.
    template <class T> Arb& Arb::set (T const number) {
      
      // If this is an integer type, set is easy.
      if (std::numeric_limits<T>::is_integer) {
        
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
        
      // For now be lazy and convert to string.
      } else {
        
        return set(DAC::toString(number));
        
      }
      
    }
    
    // Comparison operator backends.
    inline bool Arb::op_ge (Arb const& number) const { return (op_gt(number) || op_eq(number));  }
    inline bool Arb::op_lt (Arb const& number) const { return number.op_gt(*this);               }
    inline bool Arb::op_le (Arb const& number) const { return (op_lt(number) || op_eq(number));  }
    inline bool Arb::op_eq (Arb const& number) const { return !(op_gt(number) || op_lt(number)); }
    inline bool Arb::op_ne (Arb const& number) const { return (op_gt(number) || op_lt(number));  }
    
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
