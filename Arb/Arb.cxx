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
    _DataPT new_data(new _Data);
    
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
    new_data->p.Base(base) = num;
    
    // Adjust the number based on the exponent. If a negative exponent was
    // given, up the order of magnitude of the number, otherwise set q to
    // base^exponent.
    if (p_exp) {
      new_data->q  = _DigsT(base).pow(_DigsT(exp));
    } else {
      new_data->p *= _DigsT(exp);
    }
    
    /*
    // Load the exponent.
    {
      
      _ExpT digexp(1);
      for (_DigStrT::reverse_iterator i = exp.rbegin(); i != exp.rend(); ++i) {
        
        // Get the single digit value.
        _ExpT digval(digexp * (*i).Value());
        
        // Set the digit value.
        new_data->exponent += digval;
        
        // Up the order of magnitude.
        digexp *= base;
        
      }
      
      // Set the exponent sign.
      if (p_exp) {
        new_data->exponent *= -1;
      }
      
    }
    
    // Combine the numeric and radix digits, taking note of the original
    // number of decimal places.
    new_data->exponent += rad.size();
    num.insert(num.end(), rad.begin(), rad.end());
    
    // Trim insignificant zeros and update the exponent. In the case of
    // trimming high-order radix zeros, no need to update exponent, they have
    // already been counted.
    new_data->exponent -= s_trimZerosE(num);
                          s_trimZerosB(num);
    
    // If this is a fixed-decimal number, pad or truncate as necessary.
    if (fix) {
      new_data->fixexp = decimal;
      new_data->fix    = fix;
    } else {
      new_data->fixexp = _data->fixexp;
      new_data->fix    = _data->fix;
    }
    if (new_data->fix) {
      if (new_data->exponent != new_data->fixexp) {
        if (num.size() > (new_data->exponent - new_data->fixexp)) {
          num.resize((num.size() - (new_data->exponent - new_data->fixexp)).Value());
        } else {
          num.clear();
        }
        new_data->exponent = new_data->fixexp;
      }
    }
    
    // Load the numeric digits. Convert from the given base to the target
    // base. Digits come out in big endian format, no need for a temp.
    new_data->p.swap(*(s_baseConv<_DigStrT, _DigsT>(num, base, s_digitbase)));
    
    // Set the quotient.
    if (new_data->exponent >= 0) {
      _DigsT tmp_base;
      _DigsT tmp_expn;
      tmp_base.push_back(base);
      tmp_expn.push_back(new_data->exponent);
      new_data->q.swap(*s_intPow(tmp_base, tmp_expn));
    }
    
    // Set the original base.
    new_data->origbase = base;
    
    // Set the sign.
    new_data->positive = p_num;
    
    // The number has been loaded, swap it in. COW is preserved.
    _data = new_data;
    */
    
    // We done.
    return *this;
    
  }
  
  // Common initialization tasks.
  void Arb::_init () {
    
    // Construct this object fully. By definition of the clear() function,
    // the default constructor must do nothing more than call clear().
    clear();
    
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
    new_q.set(1);
    
    // These operations will never throw.
    positive = true;
    
    p = new_p;
    q = new_q;
    
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
