/*****************************************************************************
 * Arb.cxx
 *****************************************************************************
 * Implementation for class Arb.
 *****************************************************************************/

// STL includes.
#include <string>
#include <algorithm>
#include <iostream> // FIXME: For debugging only.

// Internal includes.
#include "rppower.hxx"
#include "toString.hxx"
#include "reduce.hxx"

// Class include.
#include "Arb.hxx"

// Namespaces used.
using namespace std;

// Namespace wrapper.
namespace DAC {
  
  /***************************************************************************
   * Class Arb.
   ***************************************************************************/
  
  /***************************************************************************/
  // Function members.
  
  // Default constructor.
  Arb::Arb () {
    
    // Call common init.
    _init();
    
  }
  
  // Destructor.
  Arb::~Arb () {
    
    // Nothing here yet.
    
  }
  
  // Reset to just-constructed state.
  Arb& Arb::clear() {
    
    // Clear this number's data. Instead of calling the clear() method of
    // _Data, we are simply creating a new structure and letting the
    // ReferencePointer take care of cleaning up any no-longer referenced
    // data. This is to allow COW.
    _data = new _Data;
    
    // We done.
    return *this;
    
  }
  
  // Convert this number to a string.
  string Arb::toString () const {
    
    // This is the string we will be returning.
    string retval;
    
    // We done.
    return retval;
    
  }
  
  // Set the number from a string.
  Arb& Arb::set (string const& number, PointPosT const decimal, bool const fix) {
    
    // Load the number into this for exception safety.
    Arb new_num;
    
    // Hold the number in case an error needs to be throw.
    ConstReferencePointer<string> tmp_number(new std::string(number));
    
    // Parser will load data into here.
    string            num;
    string            rad;
    string            exp;
    bool              p_num = true;
    bool              p_exp = true;
    _DigT             base  = 10;
    string::size_type nexp  = 0;
    
    // Parse the number, scoped for temp variables.
    {
      
      // Data for initial pass.
      enum Mode { NUM, RAD, EXP } mode = NUM;
      string::size_type length   = number.length();
      bool              s_num    = false;
      bool              s_exp    = false;
      bool              diggiven = false;
      
      for (string::size_type i = 0; i != length; ++i) {
        
        switch (number[i]) {
          
          // Decimal point.
          case '.':
            switch (mode) {
              case NUM: mode = RAD;                                                                                                 break;
              case RAD: throw ArbErrors::BadFormat().Problem("Radix point given for a second time").Position(i).Number(tmp_number); break;
              case EXP: throw ArbErrors::BadFormat().Problem("Radix point given in exponent").Position(i).Number(tmp_number);       break;
            }
          break;
          
          // Exponent symbol.
          case 'e':
          case 'E':
            switch (mode) {
              case NUM: mode = EXP;                                                                                                     break;
              case RAD: mode = EXP;                                                                                                     break;
              case EXP: throw ArbErrors::BadFormat().Problem("Exponent symbol given for a second time").Position(i).Number(tmp_number); break;
            }
            diggiven = false;
          break;
          
          // Sign symbol.
          case '+':
          case '-':
            if (diggiven) {
              throw ArbErrors::BadFormat().Problem("Sign given after digits").Position(i).Number(tmp_number);
            }
            switch (mode) {
              case NUM:
                if (s_num) {
                  throw ArbErrors::BadFormat().Problem("Sign of number given for a second time").Position(i).Number(tmp_number);
                }
                p_num = (number[i] == '+');
                s_num = true;
              break;
              case RAD:
                throw ArbErrors::BadFormat().Problem("Sign given after radix point").Position(i).Number(tmp_number);
              break;
              case EXP:
                if (s_exp) {
                  throw ArbErrors::BadFormat().Problem("Sign of exponent given for a second time").Position(i).Number(tmp_number);
                }
                p_exp = (number[i] == '+');
                s_exp = true;
              break;
            }
          break;
          
          // All other characters, most likely digits.
          default:
            
            // Add the digit to its proper place.
            switch (mode) {
              case NUM: num += number[i]; break;
              case RAD: rad += number[i]; break;
              case EXP: exp += number[i]; break;
            }
            diggiven = true;
            
          break;
          
        }
        
      }
      
    }
    
    // Trim insignificant zeros from gathered digits.
    num.erase(0, num.find_first_not_of('0'));
    if (rad.find_last_not_of('0') != string::npos) {
      rad.erase(rad.find_last_not_of('0') + 1);
    }
    exp.erase(0, exp.find_first_not_of('0'));
    
    // Count the number of decimal positions.
    nexp = rad.size();
    
    // Concatenate numeric and radix digits.
    num += rad;
    
    // Load the number.
    new_num._data->p.Base(base) = num;
    
    // Adjust the number based on the exponent. Negative exponent increases
    // radix digits, positive exponent ups order of magnitude.
    {
      
      // Decide if this will be a fixed-point number.
      if (fix) {
        new_num._data->pointpos = decimal;
        new_num._data->fix      = true;
      } else {
        new_num._data->pointpos = _data->pointpos;
        new_num._data->fix      = _data->fix;
      }
      
      // Determine the exponent given.
      _DigsT expn(exp);
      _DigsT expr(nexp);
      if (p_exp) {
        if (expn >= expr) {
          expn -= expr;
        } else {
          p_exp = false;
          expn  = expr - expn;
        }
      } else {
        expn += expr;
      }
      
      // Convert to a fully-represented number (no positive exponent).
      if (!expn.isZero()) {
        
        // This calculation will be needed whether exponent is positive or
        // negative. If positive, multiply p by it and clear, thus bringing p
        // down to 1s. If negative, leave as it is, it is already accurate.
        new_num._data->q = _DigsT(base).pow(expn);
        if (p_exp) {
          new_num._data->p *= new_num._data->q;
          new_num._data->q  = 1;
          expn              = 0;
        } 
      }
      
      // If a fixed-point number is requested, increase or decrease the
      // radix digits as needed.
      if (new_num._data->fix) {
        
        // If the exponent is positive, it is a simple case of increasing the
        // number of radix digits, this is a number represented by 1s by now.
        if (p_exp) {
          new_num._data->q  = _DigsT(base).pow(_DigsT(new_num._data->pointpos));
          new_num._data->p *= new_num._data->q;
        
        // If the exponent is negative, we must add or subtract the difference
        // in digits.
        } else {
          if (_DigsT(new_num._data->pointpos) >= expn) {
            _DigsT mod        = _DigsT(base).pow(_DigsT(new_num._data->pointpos) - expn);
            new_num._data->p *= mod;
            new_num._data->q *= mod;
          } else {
            _DigsT mod        = _DigsT(base).pow(expn - _DigsT(new_num._data->pointpos));
            new_num._data->p /= mod;
            new_num._data->q /= mod;
          }
        }
        
        // This is the q that we always need to fix at.
        new_num._data->fixq = new_num._data->q;
        
      }
      
    }
    
    // Set the original base.
    new_num._data->origbase = base;
    
    // Set the sign.
    new_num._data->positive = p_num;
    
    // Reduce the number.
    new_num._reduce();
    
    // The number has been loaded, swap it in. COW is preserved.
    _data = new_num._data;
    
    // We done.
    return *this;
    
  }
  
  // Common initialization tasks.
  void Arb::_init () {
    
    // Construct this object fully. By definition of the clear() function,
    // the default constructor must do nothing more than call clear().
    clear();
    
  }
  
  // Reduce the number to its most compact representation.
  Arb& Arb::_reduce () {
    
    cout << "p: " << _data->p << "  q: " << _data->q << endl;
    // Fixed-point numbers are forced to their dividend.
    if (_data->fix) {
      
      // Only work if we have to.
      if (_data->q != _data->fixq) {
        
        //  p       x
        // --- == ------
        //  q      fixq
        // Solve for x.
        _data->p = _data->p * _data->fixq / _data->q;
        _data->q = _data->fixq;
        
      }
      
    // Floating-point numbers are simply reduced.
    } else {
      reduce(_data->p, _data->q);
    }
    cout << "p: " << _data->p << "  q: " << _data->q << endl;
    
  }
  
  /***************************************************************************
   * Class Arb::_Data.
   ***************************************************************************/
  
  // Default constructor.
  Arb::_Data::_Data () {
    
    // Call common init.
    _init();

  }
  
  // Reset to just constructed state.
  Arb::_Data& Arb::_Data::clear () {
    
    // These operations may throw, do them first so any error will be thrown
    // before any changes are made.
    _DigsT new_p;
    _DigsT new_q;
    _DigsT new_fixq;
    new_q.set(1);
    
    // These operations will never throw.
    positive = true;
    
    p    = new_p;
    q    = new_q;
    fixq = new_fixq;
    
    pointpos = 0;
    fix      = false;
    
    origbase = 10;
    
    // We done.
    return *this;
    
  }
  
  // Common initialization tasks.
  void Arb::_Data::_init () {
    
    // Construct this object fully. By definition of the clear() function, the
    // default constructor must do nothing more than call clear().
    clear();

  }
  
};
