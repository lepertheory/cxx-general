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
  
  /***************************************************************************
   * Timestamp
   ***************************************************************************
   * This is a timestamp class.
   ***************************************************************************/
  class Timestamp {
    
    /*
     * Public members.
     */
    public:
      
      /***********************************************************************/
      // Forward declarations.
      
      class LeapSecondDay;
      
      /***********************************************************************/
      // Enums.
      
      // Calendar system.
      enum CalendarType { CT_DEFAULT, CT_JULIAN, CT_GREGORIAN };
      
      // Days of the week.
      enum DayOfWeek {
        DOW_SUNDAY    = 0,
        DOW_MONDAY    = 1,
        DOW_TUESDAY   = 2,
        DOW_WEDNESDAY = 3,
        DOW_THURSDAY  = 4,
        DOW_FRIDAY    = 5,
        DOW_SATURDAY  = 6
      };
      
      // Months.
      enum MonthName {
        MON_JANUARY   =  1,
        MON_FEBRUARY  =  2,
        MON_MARCH     =  3,
        MON_APRIL     =  4,
        MON_MAY       =  5,
        MON_JUNE      =  6,
        MON_JULY      =  7,
        MON_AUGUST    =  8,
        MON_SEPTEMBER =  9,
        MON_OCTOBER   = 10,
        MON_NOVEMBER  = 11,
        MON_DECEMBER  = 12
      };
      
      /***********************************************************************/
      // Typedefs.
      
      // Type used for all time values.
      typedef Arb TimeVal;
      
      // List of leap seconds.
      typedef std::vector<LeapSecondDay> LeapSecondList;
      
      // Format string.
      typedef std::string Format;
      
      // Pointer to a list of leap seconds.
      typedef ReferencePointer<LeapSecondList> LSLptr;
      
      // Pointer to a format.
      typedef ReferencePointer<Format> Formatptr;
      
      /***********************************************************************/
      // Classes.
      
      // Year, month, and day grouped.
      class YMD {
        public:
          YMD () {};
          YMD (YMD const& ymd) : Year(ymd.Year), Month(ymd.Month), Day(ymd.Day) {};
          YMD (TimeVal const& year, TimeVal const& month, TimeVal const& day) : Year(year), Month(month), Day(day) {};
          template <class T, class U, class V> YMD (T const year, U const month, V const day) : Year(year), Month(month), Day(day) {}
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
          
          /*******************************************************************/
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
      
      /***********************************************************************/
      // Constants.
      
      // 3-letter weekday abbreviations, Sun-Sat, 0 indexed.
      static char const* const SHORT_WEEKDAY_NAME[];
      
      // Full weekday names, Sunday-Saturday, 0 indexed.
      static char const* const LONG_WEEKDAY_NAME[];
      
      // 3-letter month abbreviation, Jan-Dec, 1 indexed.
      static char const* const SHORT_MONTH_NAME[];
      
      // Full month names, January-December, 1 indexed.
      static char const* const LONG_MONTH_NAME[];
      
      /***********************************************************************/
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
      
      // Follows the formatting rules at the following URL:
      // http://www.opengroup.org/onlinepubs/009695399/functions/strftime.html
      // FIXME: This does not, for now, include any locale functionality.
      std::string toString (std::string const& format = "") const;
      
      // Get the day of the week.
      TimeVal dow () const;
      
      // Get the ISO day of the week.
      TimeVal dowISO () const;
      
      // Get the day of the year.
      TimeVal doy () const;
      
      // Get the ISO week.
      TimeVal getISOWeek () const;
      
      // Get the ISO year.
      TimeVal getISOYear () const;
      
      // Get the ISO week and year, as a number year * 100 + week.
      TimeVal getISOWeekAndYear () const;
      
    /*
     * Private members.
     */
    private:
      
      /***********************************************************************/
      // Data members.
      
      // List of leap seconds, must always be sorted.
      LSLptr _leapseconds;
      
      // This is the time.
      TimeVal _jd;
      
      // Last julian date.
      YMD     _lastjulianymd;
      TimeVal _lastjulianjd;
      
      // Format string for conversion to string.
      Formatptr _format;
      
      // Calendar type of this timestamp.
      CalendarType _caltype;
      
      /***********************************************************************/
      // Static data members.
      
      // True when this class has been initialzed.
      static bool s_initialized;
      
      // Default list of leap seconds, must always be sorted.
      static LSLptr s_defaultleapseconds;
      
      // Default format string.
      static Formatptr s_defaultformat;
      
      /***********************************************************************/
      // Constants.
      
      // Number of days per month in a leap or non-leap year.
      static int const _DAYS_OF_YEAR_NY[];
      static int const _DAYS_OF_YEAR_LY[];
      
      /***********************************************************************/
      // Function members.
      
      // Common initialization routines.
      void _init ();
      
      // Return whether a given year is a leap year.
      bool _isLeapYear (TimeVal const& year) const;
      
      // Return true if the given date is Gregorian.
      bool _isGregorian (TimeVal const& year, TimeVal const& month, TimeVal const& day) const;
      bool _isGregorian (TimeVal const& jd)                                             const;
      
      // Get the calendar type.
      CalendarType _getCalendarType (TimeVal const& year, TimeVal const& month, TimeVal const& day) const;
      CalendarType _getCalendarType (TimeVal const& jd)                                             const;
      
      // Return the number of days in a given month.
      TimeVal _daysInMonth (TimeVal const& year, TimeVal const& month) const;
      
      // Return the leap seconds of a given day.
      TimeVal _leapSecond (TimeVal const& year, TimeVal const& month, TimeVal const& day) const;
      
      // Get the first day of ISO week one.
      Timestamp _getISOWeekOneStart (TimeVal const& year) const;
      
      /***********************************************************************/
      // Static function members.
      
      // Class initialization.
      static void s_classInit ();
      
  };
  
}

/*****************************************************************************
 * Global operators.
 *****************************************************************************/

// Stream I/O operators.
std::ostream& operator << (std::ostream& l, DAC::Timestamp const& r);

/*****************************************************************************
 * Error declarations.
 *****************************************************************************/
namespace DAC {
  
  // Errors.
  namespace TimestampErrors {
    class Base              : public Exception   { public: virtual char const* what () const throw(); };
    class BadFormat  : public Base      {
      public:
        virtual char const* what () const throw();
        virtual BadFormat& Problem  (char const*                   const problem)  throw();
        virtual BadFormat& Position (std::string::size_type        const position) throw();
        virtual BadFormat& Format   (ConstReferencePointer<std::string>& number)   throw();
      protected:
        char const*                        _problem;
        std::string::size_type             _position;
        ConstReferencePointer<std::string> _format;
    };
    class UnknownPlatform   : public Base        { public: virtual char const* what () const throw(); };
    class InvalidTime       : public Base        { public: virtual char const* what () const throw(); };
    class NoYearZero        : public InvalidTime { public: virtual char const* what () const throw(); };
    class SysCallError      : public Base        { public: virtual char const* what () const throw(); };
    class MissingSysSupport : public Base        { public: virtual char const* what () const throw(); };
  }
  
}

/*****************************************************************************
 * Inline and template definitions.
 *****************************************************************************/

namespace DAC {
  
  // Errors
  namespace TimestampErrors {
    inline char const* Base::what              () const throw() { return "Undefined error in class Timestamp.";                                                                                                                  }
    inline char const* BadFormat::what         () const throw() { return (std::string(_problem) + " at position " + DAC::toString(SafeInt<std::string::size_type>(_position) + 1) + " in format \"" + *_format + "\".").c_str(); }
    inline BadFormat&  BadFormat::Problem      (char const*                   const problem)  throw() { _problem  = problem;  return *this; }
    inline BadFormat&  BadFormat::Position     (std::string::size_type        const position) throw() { _position = position; return *this; }
    inline BadFormat&  BadFormat::Format       (ConstReferencePointer<std::string>& format)   throw() { _format   = format;   return *this; }
    inline char const* UnknownPlatform::what   () const throw() { return "Cannot perform function on unknown platform, requires platform-specific calls.";                                                                       }
    inline char const* InvalidTime::what       () const throw() { return "The specified time is invalid.";                                                                                                                       }
    inline char const* NoYearZero::what        () const throw() { return "There is no year 0.";                                                                                                                                  }
    inline char const* SysCallError::what      () const throw() { return "Error making the requested system call.";                                                                                                              }
    inline char const* MissingSysSupport::what () const throw() { return "Missing necessary system-provided support.";                                                                                                           }
  }
  
  /***************************************************************************
   * Class Timestamp.
   ***************************************************************************/
  
  // Properties.
  inline Timestamp& Timestamp::LastJulianDate (YMD const& lastjulian) {
    Timestamp tmp;
    tmp.set(
      Interval().Year(lastjulian.Year)
                .Month(lastjulian.Month)
                .Day(lastjulian.Day)
                .Hour(TimeVal(12))
                .Minute(TimeVal(0))
                .Second(TimeVal(0)),
      CT_JULIAN
    );
    _lastjulianymd = lastjulian;
    _lastjulianjd  = tmp.Julian().floor();
    return *this;
  }
  inline Timestamp::YMD Timestamp::LastJulianDate () const { return _lastjulianymd; }
  
  inline Timestamp&         Timestamp::Julian (TimeVal const& jd)       { _jd = jd; return *this; }
  inline Timestamp::TimeVal Timestamp::Julian ()                  const { return _jd;             }
  
  // Return true if the given date is Gregorian.
  inline bool Timestamp::_isGregorian (TimeVal const& year, TimeVal const& month, TimeVal const& day) const {
    return year >  _lastjulianymd.Year || (
           year == _lastjulianymd.Year && (month >  _lastjulianymd.Month || (
                                           month == _lastjulianymd.Month && day > _lastjulianymd.Day
                                          ))
           );
  }
  inline bool Timestamp::_isGregorian (TimeVal const& jd) const { return jd > _lastjulianjd; }
  
  // Get the calendar type. _isLeapYear depends on this function counting 3/0
  // as 2/28 or 2/29.
  inline Timestamp::CalendarType Timestamp::_getCalendarType (TimeVal const& year, TimeVal const& month, TimeVal const& day) const { return (_caltype == CT_DEFAULT) ? (_isGregorian(year, month, day) ? CT_GREGORIAN : CT_JULIAN) : _caltype; }
  inline Timestamp::CalendarType Timestamp::_getCalendarType (TimeVal const& jd                                            ) const { return (_caltype == CT_DEFAULT) ? (_isGregorian(jd)               ? CT_GREGORIAN : CT_JULIAN) : _caltype; }
  
  /***************************************************************************
   * Class Timestamp::Interval.
   ***************************************************************************/
  
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
