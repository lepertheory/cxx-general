/*****************************************************************************
 * Arb.cxx
 *****************************************************************************
 * Implementation for class Arb.
 *****************************************************************************/

// STL includes.
#include <string>
#include <algorithm>

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
  // Static member initialization.
  
  SafeInteger<int> Arb::s_digitbits = 0;
  Arb::_BaseT      Arb::s_digitbase = 0;
  
  Arb::_NumChrT Arb::s_numdigits = 36;
  Arb::_StrChrT Arb::s_odigits[] = {
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
    'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
    'U', 'V', 'W', 'X', 'Y', 'Z'
  };
  vector<Arb::_NumChrT> Arb::s_idigits;
  
  // Call class constructor.
  bool Arb::s_initialized = s_classInit();
  
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
    
    // Easy case outputting 0.
    if (_data->p.size() == 0) {
      
      retval += "0";
      
    } else {
      
      // Output the sign.
      if (!(_data->positive)) {
        retval += "-";
      }
      
      // Convert p to the output base.
      _DigsT   convert(_data->p);
      _DigStrT num;
      _BaseT   base(_data->origbase);
      reverse(convert.begin(), convert.end());
      num.swap(*(s_baseConv<_DigsT, _DigStrT>(convert, s_digitbase, base)));
      reverse(num.begin(), num.end());
      
      // Output the number character by character. If the output base is
      // greater than the number of output digits defined, output the raw
      // numeric values for each character.
      for (_DigStrT::iterator i = num.begin(); i != num.end(); ++i) {
        if (base > s_numdigits.Value()) {
          retval += "'" + DAC::toString((*i).Value()) + "'";
          if (i != (num.end() - 1)) {
            retval += ",";
          }
        } else {
          retval += s_odigits[i->Value()].Value();
        }
      }
      
      // Insert the radix point.
      if (_data->exponent != 0) {
        if (_data->exponent < 0) {
          retval.append((-(_data->exponent)).Value(), '0');
        } else {
          if (_data->exponent >= retval.size()) {
            retval.insert(0, ((_data->exponent - retval.size()) + 1).Value(), '0');
          }
          retval.insert((retval.size() - _data->exponent).Value(), ".");
        }
      }
      
    }
    
    // We done.
    return retval;
    
  }
  
  // Set the number from a string.
  Arb& Arb::set (string const& number, ExponentT const decimal, bool const fix) {
    
    // Load the number into this for exception safety.
    _DataPT new_data(new _Data);
    
    // Hold the number in case an error needs to be throw.
    ConstReferencePointer<string> tmp_number(new std::string(number));
    
    // Parser will load data into here.
    _DigStrT num;
    _DigStrT rad;
    _DigStrT exp;
    bool     p_num = true;
    bool     p_exp = true;
    _BaseT   base  = 10;
    
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
            
            // Scoped for temp variables.
            {
              
              // Get the value of this digit.
              _NumChrT digval = s_idigits[number[i]];
              
              // Make sure this digit is within he number base.
              if ((digval >= base.Value()) || (digval == numeric_limits<_NumChrT>::max())) {
                throw ArbErrors::BadFormat().Problem("Unrecognized character").Position(i).Number(tmp_number);
              }
              
              // Add the digit to its proper place.
              switch (mode) {
                case NUM: num.push_back(digval); break;
                case RAD: num.push_back(digval); break;
                case EXP: num.push_back(digval); break;
              }
              diggiven = true;
              
            }
            
          break;
          
        }
        
      }
      
    }
    
    // Trim insignificant zeros from gathered digits.
    s_trimZerosB(num);
    s_trimZerosE(rad);
    
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
    
    // Set the quotient. Special case a 0 exponent to prevent infinite
    // recursion since we are using Arb to create q.
    if (new_data->exponent != 0) {
    }
    
    // Set the original base.
    new_data->origbase = base;
    
    // Set the sign.
    new_data->positive = p_num;
    
    // The number has been loaded, swap it in. COW is preserved.
    _data = new_data;
    
    // We done.
    return *this;
    
  }
  
  // Common initialization tasks.
  void Arb::_init () {
    
    // Construct this object fully. By definition of the clear() function,
    // the default constructor must do nothing more than call clear().
    clear();
    
  }
  
  // Class constructor.
  bool Arb::s_classInit () throw() {
    
    // This function cannot throw.
    try {
      
      // Get the maximum number that can be held in a single digit.
      s_digitbits = numeric_limits<_DigT>::digits >> 1;
      s_digitbase = rppower(_BaseT(2), s_digitbits);
      
      // Get the input digits.
      for (_NumChrT i = 0; i != numeric_limits<_NumChrT>::max(); ++i) {
        _NumChrT digit = 0;
        if      ((i >= '0') && (i <= '9')) { digit = i - _NumChrT('0');               }
        else if ((i >= 'A') && (i <= 'Z')) { digit = i - _NumChrT('A') + 10;          }
        else if ((i >= 'a') && (i <= 'z')) { digit = i - _NumChrT('a') + 10;          }
        else                               { digit = numeric_limits<_NumChrT>::max(); }
        s_idigits.push_back(digit);
      }
      
    } catch (...) {
      
      // If any exception was caught, we failed. Have fun debugging.
      return false;
      
    }
    
    // Happy joy.
    return true;
    
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
    new_q.push_back(1);
    
    // These operations will never throw.
    positive = true;
    exponent = 0;
    fixexp   = 0;
    fix      = false;
    origbase = 10;
    p.swap(new_p);
    q.swap(new_q);
    
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
