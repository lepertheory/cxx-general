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
  #include <vector>
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
        // Forward declarations.
        
        class LeapSecondDay;
        
        /*********************************************************************/
        // Typedefs.
        
        typedef Arb TimeVal;
        
        typedef std::vector<LeapSecondDay> LeapSecondList;
        
        /*********************************************************************/
        // Classes.
        
        // Year, month, and day grouped.
        class YMD {
          public:
            YMD () {};
            YMD (YMD const& ymd) : Year(ymd.Year), Month(ymd.Month), Day(ymd.Day) {};
            YMD (TimeVal const& year, TimeVal const& month, TimeVal const& day) : Year(year), Month(month), Day(day) {};
            template <class T> YMD (T const year, T const month, T const day) : Year(year), Month(month), Day(day) {}
            TimeVal  Year;
            TimeVal Month;
            TimeVal   Day;
        };
        
        // Leap second day.
        class LeapSecondDay : public YMD {
          public:
            LeapSecondDay () {};
            LeapSecondDay (YMD const& ymd, TimeVal const& leap) : YMD(ymd), Leap(leap) {};
            template <class T> LeapSecondDay (YMD const& ymd, T const leap) : YMD(ymd), Leap(leap) {}
            TimeVal Leap;
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
            Interval& Millisecond (TimeVal const& millisecond);
            Interval& Second      (TimeVal const& second);
            Interval& Minute      (TimeVal const& minute);
            Interval& Hour        (TimeVal const& hour);
            Interval& Day         (TimeVal const& day);
            Interval& Month       (TimeVal const& month);
            Interval& Year        (TimeVal const& year);
            
            // Get properties.
            TimeVal Millisecond () const;
            TimeVal Second      () const;
            TimeVal Minute      () const;
            TimeVal Hour        () const;
            TimeVal Day         () const;
            TimeVal Month       () const;
            TimeVal Year        () const;
            
            // Check if properties are set.
            bool isSet_Millisecond () const;
            bool isSet_Second      () const;
            bool isSet_Minute      () const;
            bool isSet_Hour        () const;
            bool isSet_Day         () const;
            bool isSet_Month       () const;
            bool isSet_Year        () const;
            
          /*
           * Private members.
           */
          private:
            
            /*****************************************************************/
            // Data members.
            
            // Properties.
            TimeVal _millisecond;
            TimeVal _second;
            TimeVal _minute;
            TimeVal _hour;
            TimeVal _day;
            TimeVal _month;
            TimeVal _year;
            bool    _set_millisecond;
            bool    _set_second;
            bool    _set_minute;
            bool    _set_hour;
            bool    _set_day;
            bool    _set_month;
            bool    _set_year;
          
        };
        
        /*********************************************************************/
        // Function members.
        
        // Default constructor.
        Timestamp ();
        
        // Copy constructor.
        Timestamp (Timestamp const& ts);
        
        // Properties.
        Timestamp& LastJulianDate (YMD const& lastjulian);
        YMD        LastJulianDate ()                      const;
        Timestamp& Julian         (TimeVal const& jd);
        TimeVal    Julian         ()                      const;
        
        // Reset to just-constructed defaults.
        Timestamp& clear ();
        
        // Copy another timestamp.
        Timestamp& copy (Timestamp const& ts);
        
        // Set this timestamp.
        Timestamp& set (Interval const& time);
        
        // Get the individual values of this timestamp.
        Interval get () const;
        
        // Get the current system time.
        Timestamp& getSystemTime ();
        
        // Convert this timestamp to a string.
        std::string toString () const;
        
      /*
       * Private members.
       */
      private:
        
        /*********************************************************************/
        // Data members.
        
        // List of leap seconds, must always be sorted.
        ReferencePointer<LeapSecondList> _leapseconds;
        
        // This is the time.
        TimeVal _jd;
        
        // Last julian date.
        YMD _lastjulian;
        
        /*********************************************************************/
        // Static data members.
        
        // True when this class has been initialzed.
        static bool s_initialized;
        
        // Default list of leap seconds, must always be sorted.
        static ReferencePointer<LeapSecondList> s_defaultleapseconds;
        
        /*********************************************************************/
        // Function members.
        
        // Common initialization routines.
        void _init ();
        
        // Return the number of days in a given month.
        TimeVal _daysInMonth (TimeVal const& year, TimeVal const& month) const;
        
        // Return the leap seconds of a given day.
        TimeVal _leapSecond (TimeVal const& year, TimeVal const& month, TimeVal const& day) const;
        
        /*********************************************************************/
        // Static function members.
        
        // Class initialization.
        static void s_classInit ();
        
    };
    
  }
  
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
      class Base              : public Exception { public: virtual char const* what () const throw(); };
      class UnknownPlatform   : public Base      { public: virtual char const* what () const throw(); };
      class InvalidTime       : public Base      { public: virtual char const* what () const throw(); };
      class SysCallError      : public Base      { public: virtual char const* what () const throw(); };
      class MissingSysSupport : public Base      { public: virtual char const* what () const throw(); };
    }
    
  }
  
  /***************************************************************************
   * Inline and template definitions.
   ***************************************************************************/
  
  namespace DAC {
    
    // Errors
    namespace TimestampErrors {
      inline char const* Base::what              () const throw() { return "Undefined error in class Timestamp.";                                            }
      inline char const* UnknownPlatform::what   () const throw() { return "Cannot perform function on unknown platform, requires platform-specific calls."; }
      inline char const* InvalidTime::what       () const throw() { return "The specified time is invalid.";                                                 }
      inline char const* SysCallError::what      () const throw() { return "Error making the requested system call.";                                        }
      inline char const* MissingSysSupport::what () const throw() { return "Missing necessary system-provided support.";                                     }
    }
    
    /*************************************************************************
     * Class Timestamp.
     *************************************************************************/
    
    // Properties.
    inline Timestamp&         Timestamp::LastJulianDate (YMD const& lastjulian)       { _lastjulian = lastjulian; return *this; }
    inline Timestamp::YMD     Timestamp::LastJulianDate ()                      const { return _lastjulian;                     }
    inline Timestamp&         Timestamp::Julian         (TimeVal const& jd)           { _jd = jd; return *this;                 }
    inline Timestamp::TimeVal Timestamp::Julian         ()                      const { return _jd;                             }
    
    /*************************************************************************
     * Class Timestamp::Interval.
     *************************************************************************/
    
    // Default constructor.
    inline Timestamp::Interval::Interval () {}
    
    // Set properties.
    inline Timestamp::Interval& Timestamp::Interval::Millisecond (TimeVal const& millisecond) { _millisecond = millisecond; _set_millisecond = true; return *this; }
    inline Timestamp::Interval& Timestamp::Interval::Second      (TimeVal const& second)      { _second      = second;      _set_second      = true; return *this; }
    inline Timestamp::Interval& Timestamp::Interval::Minute      (TimeVal const& minute)      { _minute      = minute;      _set_minute      = true; return *this; }
    inline Timestamp::Interval& Timestamp::Interval::Hour        (TimeVal const& hour)        { _hour        = hour;        _set_hour        = true; return *this; }
    inline Timestamp::Interval& Timestamp::Interval::Day         (TimeVal const& day)         { _day         = day;         _set_day         = true; return *this; }
    inline Timestamp::Interval& Timestamp::Interval::Month       (TimeVal const& month)       { _month       = month;       _set_month       = true; return *this; }
    inline Timestamp::Interval& Timestamp::Interval::Year        (TimeVal const& year)        { _year        = year;        _set_year        = true; return *this; }
    
    // Get properties.
    inline Timestamp::TimeVal Timestamp::Interval::Millisecond () const { return _millisecond; }
    inline Timestamp::TimeVal Timestamp::Interval::Second      () const { return _second;      }
    inline Timestamp::TimeVal Timestamp::Interval::Minute      () const { return _minute;      }
    inline Timestamp::TimeVal Timestamp::Interval::Hour        () const { return _hour;        }
    inline Timestamp::TimeVal Timestamp::Interval::Day         () const { return _day;         }
    inline Timestamp::TimeVal Timestamp::Interval::Month       () const { return _month;       }
    inline Timestamp::TimeVal Timestamp::Interval::Year        () const { return _year;        }
    
    // Check if properties are set.
    inline bool Timestamp::Interval::isSet_Millisecond () const { return _set_millisecond; }
    inline bool Timestamp::Interval::isSet_Second      () const { return _set_second;      }
    inline bool Timestamp::Interval::isSet_Minute      () const { return _set_minute;      }
    inline bool Timestamp::Interval::isSet_Hour        () const { return _set_hour;        }
    inline bool Timestamp::Interval::isSet_Day         () const { return _set_day;         }
    inline bool Timestamp::Interval::isSet_Month       () const { return _set_month;       }
    inline bool Timestamp::Interval::isSet_Year        () const { return _set_year;        }
    
  }
  
  // Stream I/O operators.
  inline std::ostream& operator << (std::ostream& l, DAC::Timestamp const& r) { l << r.toString(); return l; }
  
#endif
