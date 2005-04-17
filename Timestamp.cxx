/*****************************************************************************
 * Timestamp.cxx
 *****************************************************************************
 * Implementation for class Timestamp.
 *****************************************************************************/

// System includes.
#if defined(PLAT_WIN32)
  #include "windows.h"
#endif

// Class include.
#include "Timestamp.hxx"

// Namespaces used.
using namespace std;

// Namespace wrapper.
namespace DAC {
  
  /***************************************************************************
   * Class Timestamp::I_Month.
   ***************************************************************************/
  
  /***************************************************************************/
  // Function members.
  
  // Get the number of days in this month.
  Timestamp::I_Day Timestamp::I_Month::daysInMonth (I_Year const& year) const {
    
    // Work area.
    Timestamp::I_Day days;
    
    // Easy.
    try {
      switch (Value<int>()) {
        
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
          days = 31;
        break;
        
        case 2:
          days = ((year % 4) && (!(year % 100) || (year % 400))) ? 29 : 28;
        break;
        
        case 4:
        case 6:
        case 9:
        case 11:
          days = 30;
        break;
        
      }
    } catch (ArbIntErrors::ScalarOverflow) {
      days = 0;
    }
    
    // We done.
    return days;
    
  }
  
  /***************************************************************************
   * Class Timestamp::Interval.
   ***************************************************************************/
  
  /***************************************************************************/
  // Function members.
  
  // Verify that this is a valid time.
  bool Timestamp::Interval::isValid () const {
    
    // All segments of the time must be set for it to be valid.
    if (!_set_millisecond ||
        !_set_second      ||
        !_set_minute      ||
        !_set_hour        ||
        !_set_day         ||
        !_set_month       ||
        !_set_year) {
      return false;
    }
    
    // Year must be a whole number, not zero.
    if (!_year || !_year.isInteger()) {
      return false;
    }
    
    // Month must be a whole number, 1-12.
    if (!_month.isInteger() || (_month < I_Month(1)) || (_month > I_Month(12))) {
      return false;
    }
    
    // Day must be a whole number, 1-days in month.
    if (!_day.isInteger() || (_day < I_Month(1)) || (_day > _month.daysInMonth(_year))) {
      return false;
    }
    
    // Hour must be a whole number, < 24.
    if (!_hour.isInteger() || (_hour > I_Hour(23))) {
      return false;
    }
    
    // Minute must be a whole number, < 60.
    if (!_minute.isInteger() || (_minute > I_Minute(59))) {
      return false;
    }
    
    // Second must be a whole number, < 60, or on leap second days, < 59 or < 61.
    if (!_second.isInteger() || (_second > (I_Second(59) + _day.leapSecond(_year, _month)))) {
      return false;
    }
    
    // Millisecond must be a whole number, < 1000.
    if (!_millisecond.isInteger() || (_millisecond > I_Millisecond(1000))) {
      return false;
    }
    
  }
  
  /***************************************************************************
   * Class Timestamp.
   ***************************************************************************/
  
  /***************************************************************************/
  // Function members.
  
  // Default constructor.
  Timestamp::Timestamp () {
    
    // Call common init.
    _init();
    
  }
  
  // Copy constructor.
  Timestamp::Timestamp (Timestamp const& ts) {
    
    // Call common init.
    _init();
    
    // Set the timestamp.
    copy(ts);
    
  }
  
  // Set this timestamp.
  Timestamp& Timestamp::set (Interval const& time) {
    
    // Work area.
    Timestamp newtime(*this);
    
    // Verify that the new time is a valid time.
    if (!time.isValid()) {
      throw TimestampErrors::InvalidTime();
    }
    
    
    
    // We done, return.
    return *this;
    
  }
  
  // Get the current system time.
  Timestamp& Timestamp::getSystemTime () {
    
    // Win32 platform implementation.
    #if defined(PLAT_WIN32)
      
      // Work area.
      Timestamp   newtime(*this);
      _SYSTEMTIME systime;
      
      // Make the system call. Appearantly this call cannot fail on Windows.
      GetSystemTime(&systime);
      
      // Set the new time.
      newtime.set(Interval().Millisecond(systime.wMilliseconds)
                            .Second(systime.wSecond)
                            .Minute(systime.wMinute)
                            .Hour(systime.wHour)
                            .Day(systime.wDay)
                            .Month(systime.wMonth)
                            .Year(systime.wYear));
      
    // Unknown platform.
    #else
      
      // Throw an error, nothing else we can do.
      throw TimestampErrors::UnknownPlatform();
      
    #endif
    
    // We done, return.
    return *this;
    
  }
  
  // Reset to just-constructed state.
  Timestamp& Timestamp::clear () {
    
    // We done.
    return *this;
    
  }
  
  // Copy another timestamp.
  Timestamp& Timestamp::copy (Timestamp const& ts) {
    
    // We done.
    return *this;
    
  }
  
  // Convert this timestamp to a string.
  string Timestamp::toString () const {
    
    // This is the string we will be returning.
    string retval;
    
    // We done, return the string.
    return retval;
    
  }
  
  // Common initialization tasks.
  void Timestamp::_init () {
    
    // Construct this object fully.
    clear();
    
  }
  
};
