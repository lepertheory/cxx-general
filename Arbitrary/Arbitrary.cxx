/*****************************************************************************
 * Arbitrary.cxx
 *****************************************************************************
 * Implementation for class Arbitrary.
 *****************************************************************************/

// Class include.
#include "Arbitrary.hxx"

// STL includes.
#include <cmath>
#include <iostream> // FIXME: This is for debugging only, get rid of it.

// Internal includes.
#include "toString.hxx"
#include "rppower.hxx"
#include "SafeInteger/SafeInteger.hxx"

// Namespaces used.
using namespace std;

// Namespace wrapper.
namespace DAC {
  
  /*************************************************************************
   * Initialize static members.
   *************************************************************************/
  
  // Static data members.
  SafeInteger<int>  Arbitrary::s_digitbits = 0;
  Arbitrary::_BaseT Arbitrary::s_digitbase = 0;
  
  // The characters that make up the digits.
  Arbitrary::_NumChrT   Arbitrary::s_numdigits = 36;
  Arbitrary::_StrChrT   Arbitrary::s_odigits[] = {
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
    'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
    'U', 'V', 'W', 'X', 'Y', 'Z'
  };
  vector<Arbitrary::_NumChrT> Arbitrary::s_idigits;
  
  // Call class constructor.
  bool Arbitrary::s_initialized = s_classInit();
  
  /***************************************************************************
   * class Arbitrary
   ***************************************************************************/
  
  // Default constructor. By definition of the clear() function, this must do
  // nothing but call clear().
  Arbitrary::Arbitrary () {
    
    // Make sure the class initialized properly.
    if (!s_initialized) {
      ArbitraryErrors::throwClassInitFailed("Class initialization failed, cannot construct new objects.");
    }
    
    // Construct object fully.
    clear();
    
  }
  
  // Copy constructor. Makes a perfect copy of a given number.
  Arbitrary::Arbitrary (Arbitrary const& number) {
    
    // Make sure the class initialized properly.
    if (!s_initialized) {
      ArbitraryErrors::throwClassInitFailed("Class initialization failed, cannot construct new objects.");
    }
    
    // Copy the given number.
    copy(number);
    
  }
  
  // String conversion constructor. Special case.
  Arbitrary::Arbitrary (string const& number) {
    
    // Make sure the class initialized properly.
    if (!s_initialized) {
      ArbitraryErrors::throwClassInitFailed("Class initialization failed, cannot construct new objects.");
    }
    
    // Construct fully.
    clear();
    
    // Set the number.  
    set(number);
    
  }
  
  // Add a number to this number.
  Arbitrary& Arbitrary::op_add (Arbitrary const& right) {
    
    // If signs are different, redirect this to a subtract.
    if (_data->positive != right._data->positive) {
      return op_sub(-right);
    }
    
    // Reduce typing.
    typedef _DigsT::size_type DST;
    
    // Work area.
    Arbitrary tmp_left(*this);
    Arbitrary tmp_right(right);
    
    // Convert to two numbers that can be calculated against each other.
    tmp_left._normalizeExponent(tmp_right);
    
    // Add like 1st grade.
    for (DST i = 0; i != tmp_right._data->digits.size(); ++i) {
      
      // Add another digit if needed.
      if (i >= tmp_left._data->digits.size()) {
        tmp_left._data->digits.push_back(0);
      }
      
      // Add the number and carry.
      tmp_left._data->digits[i] += tmp_right._data->digits[i];
      s_carry<_DigsT, DST>(tmp_left._data->digits, i);
      
    }
    
    // Move the new data into place.
    _data = tmp_left._data;
    
    // Done.
    return *this;
    
  };
  
  // Subtract a number from this number.
  Arbitrary& Arbitrary::op_sub (Arbitrary const& right) {
    
    // If signs are different, redirect this to an add.
    if (_data->positive != right._data->positive) {
      return op_add(-right);
    }
    
    // Reduce typing.
    typedef _DigsT::size_type DST;
    
    // Work area.
    Arbitrary tmp_left(*this);
    Arbitrary tmp_right(right);
    
    // Convert to two numbers that can be calculated against each other.
    tmp_left._normalizeExponent(tmp_right);
    
    // Make sure that we are subtracting the smaller (absolute) number from
    // the larger number. If we need to swap to achieve this, also swap the
    // sign.
    if (tmp_right > *this) {
      tmp_left._data.swap(tmp_right._data);
      tmp_left._data->positive = !(tmp_left._data->positive);
    }
    
    // Subtract like 1st grade.
    for (DST i = 0; i != tmp_right._data->digits.size(); ++i) {
      
      // Subtract with borrowing if needed.
      if (tmp_left._data->digits[i] < tmp_right._data->digits[i]) {
        s_borrow<_DigsT, DST>(tmp_left._data->digits, i);
      }
      tmp_left._data->digits[i] -= tmp_right._data->digits[i];
      
    }
    
    // Move the new data into place.
    _data = tmp_left._data;
    
    // Done.
    return *this;
    
  }
  
  // Multiply this number by another number.
  Arbitrary& Arbitrary::op_mul (Arbitrary const& right) {
    
    // Reduce typing.
    typedef _DigsT::size_type DST;
    
    // Work area.
    Arbitrary tmp_left(*this);
    Arbitrary tmp_right(right);
    _DataPT retval(new _Data);
    
    // Convert to two numbers that can be calculated against each other.
    tmp_left._normalizeRadix(tmp_right);
    
    // Multiply like 3rd grade. Outer loop is the multiplicand.
    for (DST i = 0; i != _data->digits.size(); ++i) {
      
      // Temporary product for this digit.
      _DigsT digproduct;
      
      // Inner loop is the multiplicator.
      for (DST j = 0; j != tmp_right._data->digits.size(); ++j) {
        
        // Add a new digit to the temporary product if needed.
        if (digproduct.size() == j) {
          digproduct.push_back(0);
        }
        
        // Add the product of these two digits to the appropriate digit of the
        // temporary product.
        digproduct[j] += tmp_left._data->digits[i] * tmp_right._data->digits[j];
        
        // Do any carry needed.
        s_carry<_DigsT, DST>(digproduct, j);
        
      }
      
      // Add the single digit product to the final product, offset by the
      // multiplicand digit we are on now.
      for (DST j = 0; j != digproduct.size(); ++j) {
        
        // Digit to add to.
        DST tdig = j + i;
        
        // Add a new digit if needed.
        if (retval->digits.size() == tdig) {
          retval->digits.push_back(0);
        }
        
        // Add this digit.
        retval->digits[tdig] += digproduct[j];
        
        // Do any carry needed.
        s_carry<_DigsT, DST>(retval->digits, tdig);
        
      }
      
    }
    
    // Put the decimal point in the proper place.
    retval->exponent = tmp_left._data->exponent + tmp_right._data->exponent;
    
    // Set the sign.
    retval->positive = (tmp_left._data->positive == tmp_right._data->positive);
    
    // Swap in the new data.
    _data = retval;
    
    // Done.
    return *this;
    
  }
  
  // Divide this number by another number.
  Arbitrary& Arbitrary::op_div (Arbitrary const& right) {
    
    return *this;
    
  }
  
  // Bitwise shift right.
  Arbitrary& Arbitrary::op_shr (Arbitrary const& right) {
    
    // Save typing.
    typedef _DigsT::size_type DS;
    
    // Operate on temporary data.
    _DataPT new_data(new _Data);
    *new_data = *_data;
    
    // Change shift amount to the digit vector's size_type.
    DS shift = 0;
    try {
      shift = right;
    } catch (ArbitraryErrors::Overflow error) {
      ArbitraryErrors::throwOverflow("Shift amount overflows digit vector size type.", &error);
    }
    
    // Only shift if it is needed.
    if (shift != 0) {
      
      // If shift amount will leave no bits in the number, just zero it out.
      if (shift >= new_data->digits.size() * s_digitbits) {
        
        new_data->digits.clear();
        new_data->digits.push_back(0);
        
      } else {
        
        // Do any whole-digit shifting needed. Whole digit shift right is easy,
        // just cut off the needed number of low-order digits.
        DS digshift = shift / s_digitbits.Value();
        if (digshift > 0) {
          new_data->digits.erase(new_data->digits.begin(), new_data->digits.begin() + digshift);
        }
        
        // Now do any fine-grained shifting needed. First calculate how much
        // will be needed, then shift if necessary.
        if ((shift -= digshift * s_digitbits.Value()) > 0) {
          _DigT bitmask  = rppower(2, shift) - 1;
          _DigT carry    = 0;
          _DigT oldcarry = 0;
          for (_DigsT::reverse_iterator i = new_data->digits.rbegin(); i != new_data->digits.rend(); ++i) {
            carry      = *i & bitmask;
            *i       >>= shift;
            *i        |= oldcarry;
            oldcarry   = (carry << (s_digitbits - shift).Value());
          }
        }
        
      }
      
    }
    
    // Clean up.
    s_trimZeros<_DigsT, _DigsT::iterator>(new_data->digits, _END);
    
    // Move the new data into place.
    _data = new_data;
    
    // Shifted, yo.
    return *this;
    
  }
  
  // Bitwise and operator.
  Arbitrary& Arbitrary::op_bit_and (Arbitrary const& right) {
    
    // Allow right to be const.
    Arbitrary tmp_right(right);
    
    // Convert to two numbers that can be calculated against each other.
    _normalizeRadix(tmp_right);
    _toWhole();
    tmp_right._toWhole();
    
    // Only shared digits need to be checked. resize() cuts off high-order
    // digits.
    if (_data->digits.size() > tmp_right._data->digits.size()) { _data->digits.resize(tmp_right._data->digits.size()); }
    if (tmp_right._data->digits.size() > _data->digits.size()) { tmp_right._data->digits.resize(_data->digits.size()); }
    
    // Do the and.
    for (_DigsT::size_type i = 0; i != _data->digits.size(); ++i) {
      _data->digits[i] &= tmp_right._data->digits[i];
    }
    
    // Clean up.
    s_trimZeros<_DigsT, _DigsT::iterator>(_data->digits, _END);
    
    // Return, we done.
    return *this;
    
  }
  
  // Comparison operators. Only op_gt until optimization time, all others are
  // built off of this one.
  bool Arbitrary::op_gt (Arbitrary const& right) const {
    
    // Temp variables allow const.
    Arbitrary tmp_left(*this);
    Arbitrary tmp_right(right);
    
    // Convert to two numbers that can be calculated against each other.
    tmp_left._normalizeExponent(tmp_right);
    
    // Test.
    if (tmp_left._data->positive && !(tmp_right._data->positive)) {
      return true;
    }
    if (!(tmp_left._data->positive) && tmp_right._data->positive) {
      return false;
    }
    if (tmp_left._data->digits.size() > tmp_right._data->digits.size()) {
      return true;
    }
    if (tmp_left._data->digits.size() < tmp_right._data->digits.size()) {
      return false;
    }
    {
      _DigsT::reverse_iterator li = tmp_left._data->digits.rbegin();
      _DigsT::reverse_iterator ri = tmp_right._data->digits.rbegin();
      for (; (li != tmp_left._data->digits.rend()) && (ri != tmp_right._data->digits.rend()); li++, ri++) {
        if (*li > *ri) {
          return true;
        }
        if (*li < *ri) {
          return false;
        }
      }
    }
    
    // No differences found. Not greater.
    return false;
    
  }
  
  // Convert to a bool.
  bool Arbitrary::op_bool () const {
    
    // This one is easy.
    return (_data->digits.size() > 0);
    
  }
  
  // Raise to a power.
  Arbitrary& Arbitrary::pow (Arbitrary const& power) {
    
    // Allow const.
    Arbitrary tmp_left(*this);
    Arbitrary tmp_right(power);
    Arbitrary retval(1);
    
    // Convert to two numbers that can be calculated against each other.
    _normalizeRadix(tmp_right._toWhole());
    
    // Russian peasant power.
    if (tmp_right >= Arbitrary(0)) {
      while (tmp_right) {
        if (tmp_right & Arbitrary(1)) {
          retval *= tmp_left;
        }
        tmp_left   *= tmp_left;
        tmp_right >>= Arbitrary(1);
      }
    }
    
    // Set the result.
    _data = retval._data;
    
    return *this;
    
  }
  
  // Convert to a string.
  string Arbitrary::toString () const {
    
    // Base conversion data.
    _DigStrT num;
    _BaseT   base = _data->originalbase;
    
    // Construct the string here.
    string construct;
    
    // Output the sign if negative.
    if (!(_data->positive)) {
      construct += "-";
    }
    
    // Get the string of digits. Wow. This one's a doozie. Here goes, from the
    // innermost level out:
    // 1.) Reverse data->digits for the base conversion.
    // 2.) Convert data->digits to base whatever from the internal digit base.
    // 3.) Reverse the converted number.
    // 4.) Swap that into num.
    // Each step is, by itself, ridiculous, but it should be possible to
    // follow if you're good at templates and have six independent eyes.
    num.swap(
      *(s_reverse<_DigStrT, _DigStrT::reverse_iterator>(
        *(s_baseconv<_DigStrT, _DigChrT, _DigsT, _DigT, _DigsT::iterator>(
          *(s_reverse<_DigsT, _DigsT::reverse_iterator>(_data->digits)),
          s_digitbase,
          base
        ))
      ))
    );
    
    // Output the number character by character. If a character is not found
    // in the digit table, then output the raw number in single quotes. If num
    // is empty, then output a 0.
    if (num.size() == 0) {
      construct += "0";
    } else {
      for (_DigStrT::iterator i = num.begin(); i != num.end(); ++i) {
        if (*i > s_numdigits) {
          construct += "'" + DAC::toStringChr((*i).Value()) + "'";
        } else {
          construct += s_odigits[i->Value()].Value();
        }
      }
    }
    
    // Insert the radix point.
    if (_data->exponent != 0) {
      if (_data->exponent < 0) {
        construct.append(-(_data->exponent.Value()), '0');
      } else {
        if (_data->exponent >= construct.size()) {
          construct.insert(0, ((_data->exponent - construct.size()) + 1).Value(), '0');
        }
        construct.insert((construct.size() - _data->exponent).Value(), ".");
      }
    }
    
    // Return the string we've made.
    return construct;
    
  }
  
  // Set the number from a string.
  Arbitrary& Arbitrary::set (string const& number) {
    
    // Create the number in this.
    _DataPT new_data(new _Data);
    
    // Data for parser.
    _DigStrT num;
    _DigStrT rad;
    _DigStrT exp;
    bool     p_num = true;
    bool     p_exp = true;
    _BaseT   base  = 10;
    
    // Parse the number character by character. Scoped because there are a
    // lot of temp variables that won't be needed later.
    {
      
      // Data for intial parse pass.
      enum Mode { NUM, RAD, EXP } mode = NUM;
      string::size_type length   = number.length();
      bool              s_num    = false;
      bool              s_exp    = false;
      bool              diggiven = false;
      
      for (string::size_type i = 0; i != length; ++i) {
        
        switch(number[i]) {
          
          // Decimal point.
          case '.':
            switch (mode) {
              case NUM:
                mode = RAD;
                break;
              case RAD: ArbitraryErrors::throwBadFormat("Radix point given for a second time", i, number);
              case EXP: ArbitraryErrors::throwBadFormat("Radix point given in exponent",       i, number);
            }
            diggiven = false;
          break;
          
          // Exponent symbol.
          case 'e':
          case 'E':
            switch (mode) {
              case NUM: mode = EXP; break;
              case RAD: mode = EXP; break;
              case EXP: ArbitraryErrors::throwBadFormat("Exponent symbol given for a second time", i, number);
            }
            diggiven = false;
          break;
          
          // Sign symbol.
          case '+':
          case '-':
            if (diggiven) {
              ArbitraryErrors::throwBadFormat("Sign given after digits", i, number);
            }
            switch (mode) {
              case NUM:
                if (s_num) {
                  ArbitraryErrors::throwBadFormat("Sign of number given for a second time", i, number);
                }
                p_num = (number[i] == '+');
                s_num = true;
              break;
              case RAD:
                ArbitraryErrors::throwBadFormat("Sign given after radix point", i, number);
              break;
              case EXP:
                if (s_exp) {
                  ArbitraryErrors::throwBadFormat("Sign of exponent given for a second time", i, number);
                }
                p_exp = (number[i] == '+');
                s_exp = true;
              break;
            }
          break;
          
          // All other characters. Most likely digits.
          default:
            
            // Scoped for temp variables.
            {
              
              // Get the value of this digit. No need do bounds checking
              // unless someone does something stupid like changing number
              // to be something other than a string of char.
              _NumChrT digval = s_idigits[number[i]];
              
              // Make sure this digit is within the number base.
              if ((digval >= base.Value()) || (digval == numeric_limits<_NumChrT>::max())) {
                ArbitraryErrors::throwBadFormat("Unrecognized character '" + DAC::toString(number[i]) + "'", i, number);
              }
              
              // Add the digit to its proper place.
              switch (mode) {
                case NUM: num.push_back(digval); break;
                case RAD: rad.push_back(digval); break;
                case EXP: exp.push_back(digval); break;
              }
              diggiven = true;
              
            }
            
          break;
          
        }
        
      }
      
    }
    
    // Set the original base for fraction issues.
    new_data->originalbase = base;
    
    // Trim leading and trailing zeros from gathered digits.
    s_trimZeros<_DigStrT, _DigStrT::iterator>(num, _BEGIN);
    s_trimZeros<_DigStrT, _DigStrT::iterator>(rad, _END);
    
    // Load the exponent.
    {
      
      _ExpT digexp = 1;
      for (_DigStrT::reverse_iterator i = exp.rbegin(); i != exp.rend(); ++i) {
        
        // Get the single digit value.
        _ExpT digval = digexp * (*i).Value();
        
        // Set the digit value.
        new_data->exponent = digval;
        
        // Up the order of magnitude.
        digexp *= base;
        
      }
      
      if (p_exp) {
        new_data->exponent *= -1;
      }
      
    }
    
    // Combine the numeric and radix digits, taking note of the original
    // number of decimal places.
    new_data->exponent += rad.size();
    num.insert(num.end(), rad.begin(), rad.end());
    
    // Load the numeric digits. Convert from the given base to the target
    // base. Digits come out in reverse order, no need for a temp.
    new_data->digits.swap(*(s_baseconv<_DigsT, _DigT, _DigStrT, _DigChrT, _DigStrT::iterator>(num, base, s_digitbase)));
    
    // Set the sign.
    new_data->positive = p_num;
    
    // Number loaded succesfully, swap it in.
    _data = new_data;
    
    // Return self.
    return *this;
    
  }
  
  // Copy the given number (constant).
  Arbitrary& Arbitrary::copy (Arbitrary const& number) {
    
    // Start from scratch.
    clear();
    
    // Do the copy.
    *_data = *(number._data);
    
    // Return self.
    return *this;
    
  }
  
  // Copy the given number.
  Arbitrary& Arbitrary::copy (Arbitrary& number) {
    
    // Start from scratch.
    clear();
    
    // Set the data.
    _data = number._data;
    
    // Return self.
    return *this;
    
  }
  
  // Reset to just-constructed by default constructor state.
  Arbitrary& Arbitrary::clear () {
    
    // Create a new data structure.
    _data = new _Data;
    
    // Return self.
    return *this;
    
  }
  
  // Initialize this class.
  bool Arbitrary::s_classInit () throw() {
    
    // This cannot throw.
    try {
      
      // Get the maximum number that can be held in a single digit.
      s_digitbits = numeric_limits<_DigT>::digits >> 1;
      s_digitbase = static_cast<_BaseT>(rppower(2, s_digitbits));
      
      // Get the input digits.
      for (_NumChrT i = 0; i != numeric_limits<_NumChrT>::max(); ++i) {
        _NumChrT digit = 0;
        if      ((i >= '0') && (i <= '9')) { digit = i - static_cast<_NumChrT>('0'); }
        else if ((i >= 'A') && (i <= 'Z')) { digit = (i - static_cast<_NumChrT>('A')) + 10; }
        else if ((i >= 'a') && (i <= 'z')) { digit = (i - static_cast<_NumChrT>('a')) + 10; }
        else                                                                             { digit = numeric_limits<_NumChrT>::max(); }
        s_idigits.push_back(digit);
      }
      
    } catch (...) {
      
      // If any exception was caught, we failed.
      return false;
      
    }
    
    // Happy joy.
    return true;
    
  }
  
  // Normalize this number to another number.
  Arbitrary& Arbitrary::_normalizeRadix (Arbitrary& number) {
    
    // Check for problems that will be caused by base differences.
    if (_data->originalbase != number._data->originalbase) {
      
      // Both numbers have fractional digits.
      if ((_data->exponent > 0) && (number._data->exponent > 0)) {
        ArbitraryErrors::throwFractionalConflict("Cannot normalize two fractional numbers of different radix.");
      }
      
      // Convert this number to the target number's base unless this number
      // has fractional digits, in that case convert the target number.
      if (_data->exponent > 0) {
        number._toWhole();
        number._data->originalbase = _data->originalbase;
      } else {
        _toWhole();
        _data->originalbase = number._data->originalbase;
      }
      
    }
    
    // We done.
    return *this;
    
  }
  
  // Set the exponents of two numbers to be the same.
  Arbitrary& Arbitrary::_normalizeExponent (Arbitrary& number) {
    
    // Both numbers need to be of the same radix.
    _normalizeRadix(number);
    
    // Drop the larger exponent down to the smaller one.
    if (_data->exponent > number._data->exponent) {
      number._setExponent(_data->exponent);
    } else {
      _setExponent(number._data->exponent);
    }
    
    return *this;
    
  }
  
  // Convert this number to a whole number.
  Arbitrary& Arbitrary::_toWhole () {
    
    // If this number is not fully represented, un-exponentize it. I'm a
    // programmer, not a playwright.
    if (_data->exponent < 0) {
      op_mul(Arbitrary(_data->originalbase).pow(-_data->exponent));
    }
    
    return *this;
    
  }
 
  // Set the exponent of this number.
  Arbitrary& Arbitrary::_setExponent (_ExpT const exponent, bool const force) {
    
    // Make sure there's a need to do this work.
    if (exponent != _data->exponent) {
      
      // Make sure that we won't be losing precision.
      if (!force && (exponent < _data->exponent)) {
        _cleanup();
        if (exponent < _data->exponent) {
          ArbitraryErrors::throwPrecisionLoss("Precision will be lost lowering the precision of '" + toString() + "' to " + DAC::toString(exponent.Value()) + " radix digits.");
        }
      }
      
      // Do the exponent shift.
      if (exponent > _data->exponent) {
        op_mul(Arbitrary(_data->originalbase).pow(exponent - _data->exponent));
      } else {
        op_div(Arbitrary(_data->originalbase).pow(_data->exponent - exponent));
      }
      
      // Update the exponent.
      _data->exponent = exponent;
      
    }
    
    return *this;
    
  }
  
  /***************************************************************************
   * class Arbitrary::_Data
   ***************************************************************************/
  
  // Default constructor.
  Arbitrary::_Data::_Data () {
    
    // Construct fully.
    clear();
    
  }
  
  // Copy constructor.
  Arbitrary::_Data::_Data (Arbitrary::_Data const& data) {
    
    // Construct fully.
    clear();
    
    // Do the copy.
    copy(data);
    
  }
  
  // Copy another _Data structure.
  Arbitrary::_Data& Arbitrary::_Data::copy (Arbitrary::_Data const& data) {
    
    // Copying digits may throw, so do it first. It's the only one that may
    // throw, so just let it throw if it throws.
    digits   = data.digits;
    positive = data.positive;
    exponent = data.exponent;
    
    //  Return this.
    return *this;
    
  }
  
  // Reset to just-constructed by default constructor state.
  Arbitrary::_Data& Arbitrary::_Data::clear () {
    
    // Exception safety.
    _DigsT temp;
    temp.push_back(0);
    originalbase = 10;
    
    // These won't throw.
    positive     = true;
    exponent     = 0;
    
    // Swap in the new data.
    digits.swap(temp);
    
    // Return this.
    return *this;
    
  }
  
};
