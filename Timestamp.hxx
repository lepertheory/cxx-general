/*****************************************************************************
 * Timestamp.hxx
 *****************************************************************************
 * Interface for class Timestamp.
 *****************************************************************************/

// Include guard.
#if !defined(TIMESTAMP_982yk298huh98u)
  #define TIMESTAMP_982yk298huh98u
  
  // STL includes.
  #include <iostream>
  #include <string>
  #include <map>
  
  // Internal includes.
  #include "Exception.hxx"
  #include "Arb.hxx"
  #include "ReferencePointer.hxx"
  
  // Namespace wrapping.
  namespace DAC {
    
    /*************************************************************************
     * Timestamp
     *************************************************************************
     * This is a timestamp class.
     *************************************************************************/
    class Timestamp {
      
      /*
       * Public members.
       */
      public:
        
        /*********************************************************************/
        // Classes.
        
        // Forward declarations.
        class I_Millisecond;
        class I_Second;
        class I_Minute;
        class I_Hour;
        class I_Day;
        class I_Month;
        class I_Year;
        
        // Interval classes, for function overloading.
        class I_Millisecond : public Arb {
          public:
                               I_Millisecond ();
            template <class T> I_Millisecond (T const value);
        };
        class I_Second : public Arb {
          public:
                               I_Second ();
            template <class T> I_Second (T const value);
        };
        class I_Minute : public Arb {
          public:
                               I_Minute ();
            template <class T> I_Minute (T const value);
        };
        class I_Hour : public Arb {
          public:
                               I_Hour ();
            template <class T> I_Hour (T const value);
        };
        class I_Day : public Arb {
          public:
                               I_Day ();
            template <class T> I_Day (T const value);
        };
        class I_Month : public Arb {
          public:
                               I_Month ();
            template <class T> I_Month (T const value);
            I_Day daysInMonth (I_Year const& year) const;
        };
        class I_Year : public Arb {
          public:
                               I_Year ();
            template <class T> I_Year (T const value);
        };
        
        // Interval container, for passing multiple intervals at once.
        class Interval {
          
          /*
           * Public members.
           */
          public:
            
            /*******************************************************************/
            // Function members.
            
            // Default constructor.
            Interval ();
            
            // Set properties.
            Interval& Millisecond (I_Millisecond const& millisecond);
            Interval& Second      (I_Second      const& second);
            Interval& Minute      (I_Minute      const& minute);
            Interval& Hour        (I_Hour        const& hour);
            Interval& Day         (I_Day         const& day);
            Interval& Month       (I_Month       const& month);
            Interval& Year        (I_Year        const& year);
            
            // Get properties.
            I_Millisecond Millisecond () const;
            I_Second      Second      () const;
            I_Minute      Minute      () const;
            I_Hour        Hour        () const;
            I_Day         Day         () const;
            I_Month       Month       () const;
            I_Year        Year        () const;
            
            // Check if properties are set.
            bool isSet_Millisecond () const;
            bool isSet_Second      () const;
            bool isSet_Minute      () const;
            bool isSet_Hour        () const;
            bool isSet_Day         () const;
            bool isSet_Month       () const;
            bool isSet_Year        () const;
            
            // Verify that this is a valid time.
            bool isValid () const;
            
          /*
           * Private members.
           */
          private:
            
            /*****************************************************************/
            // Data members.
            
            // Properties.
            I_Millisecond _millisecond;
            I_Second      _second;
            I_Minute      _minute;
            I_Hour        _hour;
            I_Day         _day;
            I_Month       _month;
            I_Year        _year;
            bool          _set_millisecond;
            bool          _set_second;
            bool          _set_minute;
            bool          _set_hour;
            bool          _set_day;
            bool          _set_month;
            bool          _set_year;
          
        };
        
        /*********************************************************************/
        // Function members.
        
        // Default constructor.
        Timestamp ();
        
        // Copy constructor.
        Timestamp (Timestamp const& ts);
        
        // Reset to just-constructed defaults.
        Timestamp& clear ();
        
        // Copy another timestamp.
        Timestamp& copy (Timestamp const& ts);
        
        // Set this timestamp.
        Timestamp& set (Interval const& time);
        
        // Get the current system time.
        Timestamp& getSystemTime ();
        
        // Convert this timestamp to a string.
        std::string toString () const;
        
        /*********************************************************************/
        // Static function members.
        
        // Return the leap second of a given date. Usually 0, may be 1 or -1.
        I_Second leapSecond (I_Year const& year, I_Month const& month, I_Day const& day);
        
      /*
       * Private members.
       */
      private:
        
        /*********************************************************************/
        // Class members.
        
        // Year, month, day key for map.
        class _YMD {
          public:
            I_Year  year;
            I_Month month;
            I_Day   day;
            static less (_YMD const& 
        };
        
        /*********************************************************************/
        // Static data members.
        
        ReferencePointer< map<
        
        /*********************************************************************/
        // Function members.
        
        // Common initialization routines.
        void _init ();
        
    };
    
  };
  
  /***************************************************************************
   * Global operators.
   ***************************************************************************/
  
  // Stream I/O operators.
  std::ostream& operator << (std::ostream& l, DAC::Timestamp const& r);
  
  /***************************************************************************
   * Error declarations.
   ***************************************************************************/
  namespace DAC {
    
    // Errors.
    namespace TimestampErrors {
      class Base            : public Exception { public: virtual char const* what () const throw(); };
      class UnknownPlatform : public Base      { public: virtual char const* what () const throw(); };
      class InvalidTime     : public Base      { public: virtual char const* what () const throw(); };
    };
    
  };
  
  /***************************************************************************
   * Inline and template definitions.
   ***************************************************************************/
  
  namespace DAC {
    
    // Errors
    namespace TimestampErrors {
      inline char const* Base::what            () const throw() { return "Undefined error in class Timestamp.";                                            }
      inline char const* UnknownPlatform::what () const throw() { return "Cannot perform function on unknown platform, requires platform-specific calls."; }
      inline char const* InvalidTime::what     () const throw() { return "The specified time is invalid.";                                                 }
    };
    
    /*************************************************************************
     * Class Timestamp.
     *************************************************************************/
    
    // Interval default constructors.
    inline Timestamp::I_Millisecond::I_Millisecond () : Arb() {}
    inline Timestamp::I_Second::I_Second           () : Arb() {}
    inline Timestamp::I_Minute::I_Minute           () : Arb() {}
    inline Timestamp::I_Hour::I_Hour               () : Arb() {}
    inline Timestamp::I_Day::I_Day                 () : Arb() {}
    inline Timestamp::I_Month::I_Month             () : Arb() {}
    inline Timestamp::I_Year::I_Year               () : Arb() {}
    
    // Interval conversion constructors.
    template <class T> inline Timestamp::I_Millisecond::I_Millisecond (T const value) : Arb(value) {}
    template <class T> inline Timestamp::I_Second::I_Second           (T const value) : Arb(value) {}
    template <class T> inline Timestamp::I_Minute::I_Minute           (T const value) : Arb(value) {}
    template <class T> inline Timestamp::I_Hour::I_Hour               (T const value) : Arb(value) {}
    template <class T> inline Timestamp::I_Day::I_Day                 (T const value) : Arb(value) {}
    template <class T> inline Timestamp::I_Month::I_Month             (T const value) : Arb(value) {}
    template <class T> inline Timestamp::I_Year::I_Year               (T const value) : Arb(value) {}
    
    /*************************************************************************
     * Class Timestamp::Interval.
     *************************************************************************/
    
    // Default constructor.
    inline Timestamp::Interval::Interval () {};
    
    // Set properties.
    inline Timestamp::Interval& Timestamp::Interval::Millisecond (I_Millisecond const& millisecond) { _millisecond = millisecond; _set_millisecond = true; return *this; }
    inline Timestamp::Interval& Timestamp::Interval::Second      (I_Second      const& second)      { _second      = second;      _set_second      = true; return *this; }
    inline Timestamp::Interval& Timestamp::Interval::Minute      (I_Minute      const& minute)      { _minute      = minute;      _set_minute      = true; return *this; }
    inline Timestamp::Interval& Timestamp::Interval::Hour        (I_Hour        const& hour)        { _hour        = hour;        _set_hour        = true; return *this; }
    inline Timestamp::Interval& Timestamp::Interval::Day         (I_Day         const& day)         { _day         = day;         _set_day         = true; return *this; }
    inline Timestamp::Interval& Timestamp::Interval::Month       (I_Month       const& month)       { _month       = month;       _set_month       = true; return *this; }
    inline Timestamp::Interval& Timestamp::Interval::Year        (I_Year        const& year)        { _year        = year;        _set_year        = true; return *this; }
    
    // Get properties.
    inline Timestamp::I_Millisecond Timestamp::Interval::Millisecond () const { return _millisecond; }
    inline Timestamp::I_Second      Timestamp::Interval::Second      () const { return _second;      }
    inline Timestamp::I_Minute      Timestamp::Interval::Minute      () const { return _minute;      }
    inline Timestamp::I_Hour        Timestamp::Interval::Hour        () const { return _hour;        }
    inline Timestamp::I_Day         Timestamp::Interval::Day         () const { return _day;         }
    inline Timestamp::I_Month       Timestamp::Interval::Month       () const { return _month;       }
    inline Timestamp::I_Year        Timestamp::Interval::Year        () const { return _year;        }
    
    // Check if properties are set.
    inline bool Timestamp::Interval::isSet_Millisecond () const { return _set_millisecond; }
    inline bool Timestamp::Interval::isSet_Second      () const { return _set_second;      }
    inline bool Timestamp::Interval::isSet_Minute      () const { return _set_minute;      }
    inline bool Timestamp::Interval::isSet_Hour        () const { return _set_hour;        }
    inline bool Timestamp::Interval::isSet_Day         () const { return _set_day;         }
    inline bool Timestamp::Interval::isSet_Month       () const { return _set_month;       }
    inline bool Timestamp::Interval::isSet_Year        () const { return _set_year;        }
    
  };
  
  // Stream I/O operators.
  inline std::ostream& operator << (std::ostream& l, DAC::Timestamp const& r) { l << r.toString(); return l; }
  
#endif
