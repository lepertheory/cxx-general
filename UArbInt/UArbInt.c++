/*****************************************************************************
 * UArbInt.c++
 *****************************************************************************
 * Implementation for class UArbInt.
 *****************************************************************************/

// STL includes.
#include <vector>
#include <string>
#include <limits>
#include <algorithm>

// System includes.
#include <SafeInt.h++>
#include <to_string.h++>
#include <rppower.h++>
#include <CaseConvert.h++>

// Class include.
#include "UArbInt.h++"

// Namespaces used.
using namespace std;

// Namespace wrapper.
namespace DAC {
  
  /***************************************************************************
   * Class UArbInt.
   ***************************************************************************/
  
  /***************************************************************************/
  // Data members.
  
  unsigned int   const UArbInt::s_digitbits = numeric_limits<_DigT>::digits >> 1;
  UArbInt::_DigT const UArbInt::s_digitbase = 1 << (numeric_limits<_DigT>::digits >> 1);
  UArbInt::_DigT const UArbInt::s_bitmask   = (1 << (numeric_limits<_DigT>::digits >> 1)) - 1;
  
  UArbInt::_NumChrT const UArbInt::s_numodigits = 36;
  UArbInt::_StrChrT const UArbInt::s_odigits[]  = {
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
    'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
    'U', 'V', 'W', 'X', 'Y', 'Z'
  };
  UArbInt::_NumChrT const UArbInt::s_idigits[] = {
    numeric_limits<_NumChrT>::max(), // 0x00      NUL
    numeric_limits<_NumChrT>::max(), // 0x01      SOH
    numeric_limits<_NumChrT>::max(), // 0x02      STX
    numeric_limits<_NumChrT>::max(), // 0x03      ETX
    numeric_limits<_NumChrT>::max(), // 0x04      EOT
    numeric_limits<_NumChrT>::max(), // 0x05      ENQ
    numeric_limits<_NumChrT>::max(), // 0x06      ACK
    numeric_limits<_NumChrT>::max(), // 0x07      BEL
    numeric_limits<_NumChrT>::max(), // 0x08      BS
    numeric_limits<_NumChrT>::max(), // 0x09      HT
    numeric_limits<_NumChrT>::max(), // 0x0A      LF
    numeric_limits<_NumChrT>::max(), // 0x0B      VT
    numeric_limits<_NumChrT>::max(), // 0x0C      FF
    numeric_limits<_NumChrT>::max(), // 0x0D      CR
    numeric_limits<_NumChrT>::max(), // 0x0E      SO
    numeric_limits<_NumChrT>::max(), // 0x0F      SI
    numeric_limits<_NumChrT>::max(), // 0x10      DLE
    numeric_limits<_NumChrT>::max(), // 0x11      DC1
    numeric_limits<_NumChrT>::max(), // 0x12      DC2
    numeric_limits<_NumChrT>::max(), // 0x13      DC3
    numeric_limits<_NumChrT>::max(), // 0x14      DC4
    numeric_limits<_NumChrT>::max(), // 0x15      NAK
    numeric_limits<_NumChrT>::max(), // 0x16      SYN
    numeric_limits<_NumChrT>::max(), // 0x17      ETB
    numeric_limits<_NumChrT>::max(), // 0x18      CAN
    numeric_limits<_NumChrT>::max(), // 0x19      EM
    numeric_limits<_NumChrT>::max(), // 0x1A      SUB
    numeric_limits<_NumChrT>::max(), // 0x1B      ESC
    numeric_limits<_NumChrT>::max(), // 0x1C      FS
    numeric_limits<_NumChrT>::max(), // 0x1D      GS
    numeric_limits<_NumChrT>::max(), // 0x1E      RS
    numeric_limits<_NumChrT>::max(), // 0x1F      US
    numeric_limits<_NumChrT>::max(), // 0x20      SP
    numeric_limits<_NumChrT>::max(), // 0x21      !
    numeric_limits<_NumChrT>::max(), // 0x22      "
    numeric_limits<_NumChrT>::max(), // 0x23      #
    numeric_limits<_NumChrT>::max(), // 0x24      $
    numeric_limits<_NumChrT>::max(), // 0x25      %
    numeric_limits<_NumChrT>::max(), // 0x26      &
    numeric_limits<_NumChrT>::max(), // 0x27      '
    numeric_limits<_NumChrT>::max(), // 0x28      (
    numeric_limits<_NumChrT>::max(), // 0x29      )
    numeric_limits<_NumChrT>::max(), // 0x2A      *
    numeric_limits<_NumChrT>::max(), // 0x2B      +
    numeric_limits<_NumChrT>::max(), // 0x2C      ,
    numeric_limits<_NumChrT>::max(), // 0x2D      -
    numeric_limits<_NumChrT>::max(), // 0x2E      .
    numeric_limits<_NumChrT>::max(), // 0x2F      /
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9,    // 0x30-0x39 0-9
    numeric_limits<_NumChrT>::max(), // 0x3A      :
    numeric_limits<_NumChrT>::max(), // 0x3B      ;
    numeric_limits<_NumChrT>::max(), // 0x3C      <
    numeric_limits<_NumChrT>::max(), // 0x3D      =
    numeric_limits<_NumChrT>::max(), // 0x3E      >
    numeric_limits<_NumChrT>::max(), // 0x3F      ?
    numeric_limits<_NumChrT>::max(), // 0x40      @
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
    20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
    30, 31, 32, 33, 34, 35,          // 0x41-0x5A A-Z
    numeric_limits<_NumChrT>::max(), // 0x5B      [
    numeric_limits<_NumChrT>::max(), // 0x5C      BACKSLASH
    numeric_limits<_NumChrT>::max(), // 0x5D      ]
    numeric_limits<_NumChrT>::max(), // 0x5E      ^
    numeric_limits<_NumChrT>::max(), // 0x5F      _
    numeric_limits<_NumChrT>::max(), // 0x60      `
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
    20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
    30, 31, 32, 33, 34, 35,          // 0x61-0x7A a-z
    numeric_limits<_NumChrT>::max(), // 0x7B      {
    numeric_limits<_NumChrT>::max(), // 0x7C      |
    numeric_limits<_NumChrT>::max(), // 0x7D      }
    numeric_limits<_NumChrT>::max(), // 0x7E      ~
    numeric_limits<_NumChrT>::max()  // 0x7F      DEL
  };
  UArbInt::value_type const UArbInt::max_input_base = 36;
  
  /***************************************************************************/
  // Function members.
  
  /*
   * Reset to just-constructed state.
   */
  void UArbInt::clear () {
    
    // Clear the digits.
    _digits.clear();
    
    // Reset the default base.
    _base = 10;
    
  }
  
  /*
   * Make a copy of another number.
   */
  void UArbInt::copy (UArbInt const& number) {
    
    // Set the new digits.
    _digits = number._digits;
    
    // Set the default base of number.
    _base = number._base;
    
  }
  
  /*
   * Swap this number with another number.
   */
  void UArbInt::swap (UArbInt& number) {
    
    // Nothing to it.
    _digits.swap(number._digits);
    _DigT tmpbase(_base);
    _base        = number._base;
    number._base = tmpbase     ;
    
  }
  
  /*
   * Set this number with a string.
   */
  UArbInt& UArbInt::set (string const& number, bool const autobase) {
    
    // Load the number into this for exception safety.
    _DigsT new_digits;
    
    // Parser will load data into here.
    _DigStrT num;
    
    // Determine the number base.
    string::size_type num_start =     0;
    _DigT             num_base  = _base;
    if (autobase) {
      string baseprefix(number.substr(0, 2));
      uppercase(baseprefix);
      if (number.length() > 2 && baseprefix == "0X") {
        num_base  = 16;
        num_start =  2;
      } else if (number.length() > 2 && baseprefix == "0B") {
        num_base  = 2;
        num_start = 2;
      } else if (number.length() > 1 && number[0] == '0') {
        num_base  = 8;
        num_start = 1;
      }
    }
    
    // Parse the number.
    for (string::size_type i = num_start; i != number.length(); ++i) {
      
      // Get the value of this digit.
      SafeInt<_NumChrT> digval(s_idigits[SafeInt<size_t>(number[i])]);
      
      // Make sure this digit is within the number base.
      if ((digval >= num_base || (digval == numeric_limits<_NumChrT>::max()))) {
        throw Errors::BadFormat().Problem("Unrecognized character").Position(i);
      }
      
      // Add the digit to the digit string.
      num.insert(num.begin(), digval);
      
    }
    
    // Trim insignificant zeros. This number is little-endian at this point.
    s_trimZerosE(num);
    
    // Convert to the native number base.
    s_baseConv(num, num_base, new_digits, s_digitbase);
    
    // The new number has been loaded successfully. Swap it in.
    _digits = new_digits;
    
    // Done.
    return *this;
    
  }
  
  /*
   * Push a string onto the back of this number.
   */
  void UArbInt::push_back (string const& number) {
    
    // Work area.
    UArbInt newnum;
    
    // Convert the new number.
    newnum.Base(_base).set(number);
    
    // Move digits up to accomidate the new digits.
    op_mul(UArbInt(_base).pow(number.length()));
    
    // Add the new number to this.
    op_add(newnum);
    
  }
  
  /*
   * Return a string of this number.
   */
  string& UArbInt::to_string (string& buffer, value_type const base) const {
    
    // This is the string we will return.
    string retval;
    
    // Easy case of 0.
    if (isZero()) {
      retval = "0";
      
    // Otherwise we have work to do.
    } else {
      
      // Get the base we will be converting to.
      _DigT tobase(base ? base : _base);
      
      // Convert to the output base.
      _DigsT num;
      s_baseConv(_digits, s_digitbase, num, tobase);
      
      // Load this into the string. If the base is greater than the number
      // of digits defined, output the raw numbers of each digit.
      if (SafeInt<_DigT>(tobase) > s_numodigits) {
        for (_DigsT::reverse_iterator i = num.rbegin(); i != num.rend(); ++i) {
          retval += "'" + DAC::to_string(*i) + "'";
          if (i != (num.rend() - 1)) {
            retval += ",";
          }
        }
      } else {
        for (_DigsT::reverse_iterator i = num.rbegin(); i != num.rend(); ++i) {
          retval += s_odigits[*i];
        }
      }
      
    }
    
    // String constructed, return it.
    buffer.swap(retval);
    return buffer;
    
  }
  
  /*
   * Multiply.
   */
  UArbInt& UArbInt::op_mul (UArbInt const& number) {
    
    // Work area.
    UArbInt retval;
    
    // If either number is zero, no multiplying to be done.
    if (!isZero() && !number.isZero()) {
      
      // Create a digit product and make a reference its digits to reduce
      // typing.
      UArbInt digproduct;
      
      // Multiply like 3rd grade.
      for (_DigsT::size_type i = 0; i != number._digits.size(); ++i) {
        
        // Get the product for a single digit.
        for (_DigsT::size_type j = 0; j != _digits.size(); ++j) {
          
          // Create a new digit in the digit product if necessary.
          if (digproduct._digits.size() == j) {
            digproduct._digits.push_back(0);
          }
          
          // Multiply into the digit product and carry.
          digproduct._digits[j] += SafeInt<_DigT>(number._digits[i]) * _digits[j];
          digproduct._carry(j);
          
        }
        
        // Offset this digit product and add it to the final product.
        digproduct._digits.insert(digproduct._digits.begin(), i, 0);
        retval.op_add(digproduct);
        digproduct.clear();
        
      }
      
    }
    
    // Swap in the result and return.
    _digits.swap(retval._digits);
    return *this;
    
  }
  
  /*
   * Divide.
   */
  UArbInt& UArbInt::op_div (UArbInt const& number, UArbInt* const remainder) {
    
    // If number is zero, throw error.
    if (number.isZero()) {
      throw Errors::DivByZero();
    }
    
    // Work area.
    UArbInt retval;
    
    // If number > this, result is zero, remainder is this.
    if (number > *this) {
      
      // Set the remainder.
      if (remainder != 0) {
        *remainder = *this;
      }
      
    // Otherwise we have work to do.
    } else {
      
      // Cache a copy of the high-order digit of the dividend.
      SafeInt<_DigT> roughdivnd(number._digits.back());
      
      // Seed the group of digits we will be dividing, then iterate through
      // the divisor. diggroup is set to *this's last number._digits's size
      // digits. Since *this is at least as big as number, this will always
      // succeed. The loop iterates backwards from the least significant
      // digit added to diggroup to the end of _digits, although the
      // iterator is only ever referenced to add one to it.
      UArbInt diggroup;
      diggroup._digits = _DigsT(_digits.end() - number._digits.size(), _digits.end());
      for (_DigsT::reverse_iterator i = _digits.rbegin() + (number._digits.size() - 1); i != _digits.rend(); ++i) {
        
        // Guess and test. Guess is a single native digit so we can use the
        // computer to do some of our division for us. These need to be set
        // to zero at each iteration, possible cause of bug if they are not.
        SafeInt<_DigT> guess;
        UArbInt        test;
        
        // Make sure that the digit group is greater than or equal to the
        // dividend. If not, the quotient for this digit is zero, move on.
        if (diggroup >= number) {
          
          // Make a guess at the quotient of this digit by dividing the
          // high-order digits. Initially the rough divisor is the high-
          // order digit of the digit group, but our guess needs to be based
          // on the number of digits the digit group has over our dividend.
          // The high-order digits of diggroup are added one at a time, and
          // moved up 1/2 of the native type's precision. Since the rough
          // dividend will never be over 1/2 of the native type's precision,
          // roughdivor can never overflow, since it will always spend at
          // least one iteration greater than roughdivnd but cannot be
          // greater than the type maximum.
          SafeInt<_DigT> roughdivor(diggroup._digits.back());
          _DigsT::size_type j;
          for (j = 0; j != (diggroup._digits.size() - number._digits.size()); ++j) {
            roughdivor *= s_digitbase;
            roughdivor += diggroup._digits[diggroup._digits.size() - 1 - (j + 1)];
          }
          guess = roughdivor / roughdivnd;
            
          // Correct the guess. First create a test. Also create a floor
          // and ceiling for future guesses. Error in guesses is based on
          // the fact that we are only dividing the high order digits of
          // each number, so the worst-case scenario is that the low-order
          // digits are all at maximum or minimum. For the floor divisor,
          // assume that all untested digits are at the minimum by lowering
          // the divisor by s_digitbase^addl digits (necessary because carry
          // would cause high-order digit to change), and raise the dividend
          // by 1. For the ceiling divisor, raise it by s_digitbase^addl
          // and lower the dividend by 1.
          // *** CORRECTION ***
          // Now starting floor at 1 (guess must be at least 1 or we
          // wouldn't be here) and ceil at max(). The performance penalty
          // will be negligable since the first time we go over or under the
          // floor and ceil are corrected, those two possible missed guesses
          // should not take significantly more time than it would take to
          // ensure that floor and ceil do not overflow, and even with those
          // guarantees the better guesses may not even help significantly
          // since they will still almost always be wrong.
          test = number * guess;
          //SafeInteger<_DigT> guessfloor = (roughdivor - rppower(SafeInteger<_DigT>(s_digitbase), j)) / (roughdivnd + 1);
          //SafeInteger<_DigT> guessceil  = (roughdivor + rppower(SafeInteger<_DigT>(s_digitbase), j)) / ((roughdivnd == 1) ? 1 : (roughdivnd - 1));
          SafeInt<_DigT> guessfloor(1);
          SafeInt<_DigT> guessceil(numeric_limits<_DigT>::max());
          
          // Loop until the test is within the correct range.
          while ((test > diggroup) || ((test + number) <= diggroup)) {
            
            // If the test is greater than the digit group, we need to lower
            // our guess. Lower it by moving halfway toward the floor. Also,
            // update the ceiling. We know that we are already over it, so
            // make the ceiling one less than the current guess.
            if (test > diggroup) {
              guessceil = guess - 1;
              guess     = guess - (((guess - guessfloor) >> 1) + 1);
              test      = number * guess;
            
            // If there is room for more dividends, we need to raise our
            // guess. Raise it halfway toward the ceiling. Update the floor,
            // it is at least one higher than our current guess.
            } else if ((test + number) <= diggroup) {
              guessfloor = guess + 1;
              guess      = guess + (((guessceil - guess) >> 1) + 1);
              test       = number * guess;
            }
            
          }
          
        }
        
        // The guess must be correct by this point. Add it to the quotient
        // and prepare for the next iteration by removing the current digit
        // from the digit group, shift the digit group up one digit, and add
        // the next digit of the divisor.
        retval._digits.insert(retval._digits.begin(), guess);
        diggroup -= test;
        if (i != (_digits.rend() - 1)) {
          diggroup._digits.insert(diggroup._digits.begin(), *(i + 1));
        }
        
      }
      
      // Set the remainder if a container was given.
      if (remainder != 0) {
        *remainder = diggroup;
      }
      
    }
    
    // Clean up, move the result into place and return.
    retval._trimZeros();
    _digits.swap(retval._digits);
    return *this;
    
  }
  
  /*
   * Modulo division.
   */
  UArbInt& UArbInt::op_mod (UArbInt const& number) {
    
    // Cannot divide by zero. check is redundant, but needed for exception to
    // report proper operator.
    if (number.isZero()) {
      throw UArbInt::Errors::DivByZero();
    }
    
    // Work area.
    UArbInt retval;
    UArbInt quotient(*this);
    
    // Do division and return the remainder.
    quotient.op_div(number, &retval);
    _digits.swap(retval._digits);
    return *this;
    
  }
  
  /*
   * Add.
   */
  UArbInt& UArbInt::op_add (UArbInt const& number) {
    
    // Add like kindergarten.
    for (_DigsT::size_type i = 0; i != number._digits.size(); ++i) {
      
      // Create a new digit if necessary.
      if (i == _digits.size()) {
        _digits.push_back(0);
      }
      
      // Add this digit and carry.
      _digits[i] += SafeInt<_DigT>(number._digits[i]);
      _carry(i);
      
    }
    
    // Done.
    return *this;
    
  }
  
  /*
   * Subtract.
   */
  UArbInt& UArbInt::op_sub (UArbInt const& number) {
    
    // These are unsigned numbers, so if number > this, throw an error.
    if (op_lt(number)) {
      throw Errors::Negative();
    }
    
    // Subtract like kindergarten.
    for (_DigsT::size_type i = 0; i != number._digits.size(); ++i) {
      
      // Borrow if necessary and subtract.
      if (_digits[i] < number._digits[i]) {
        _borrow(i);
      }
      _digits[i] = SafeInt<_DigT>(_digits[i]) - number._digits[i];
      
    }
    
    // Trim zeros.
    _trimZeros();
    
    // Done.
    return *this;
    
  }
  
  /*
   * Greater than.
   */
  bool UArbInt::op_gt (UArbInt const& number) const {
    
    // Check zeros.
    if (isZero()) {
      return false;
    } else {
      if (number.isZero()) {
        return true;
      }
    }
    
    // Check sizes of containers.
    if (_digits.size() > number._digits.size()) {
      return true;
    }
    if (_digits.size() < number._digits.size()) {
      return false;
    }
    
    // Step through each element looking for a difference, start at high-
    // order and work down.
    for (_DigsT::size_type i = 0; i != _digits.size(); ++i) {
      _DigsT::size_type j = _digits.size() - 1 - i;
      if (_digits[j] > number._digits[j]) {
        return true;
      }
      if (_digits[j] < number._digits[j]) {
        return false;
      }
    }
    
    // No difference was found, is not greater.
    return false;
    
  }
  
  /*
   * Less than.
   */
  bool UArbInt::op_lt (UArbInt const& number) const {
    
    // Check zeros.
    if (number.isZero()) {
      return false;
    } else {
      if (isZero()) {
        return true;
      }
    }
    
    // Check sizes of containers.
    if (_digits.size() < number._digits.size()) {
      return true;
    }
    if (_digits.size() > number._digits.size()) {
      return false;
    }
    
    // Step through each element looking for a difference, start at high-order
    // and work down.
    for (_DigsT::size_type i = 0; i != _digits.size(); ++i) {
      _DigsT::size_type j = _digits.size() - 1 - i;
      if (_digits[j] < number._digits[j]) {
        return true;
      }
      if (_digits[j] > number._digits[j]) {
        return false;
      }
    }
    
    // No difference was found, is not less.
    return false;
    
  }
  
  /*
   * Equal to.
   */
  bool UArbInt::op_eq (UArbInt const& number) const {
    
    // Check zeros.
    if (isZero()) {
      return number.isZero();
    } else {
      if (number.isZero()) {
        return false;
      }
    }
    
    // Check sizes of containers.
    if (_digits.size() != number._digits.size()) {
      return false;
    }
    
    // Step through each element looking for a difference, start at low-order
    // and work up, most differences will be in low-order digits.
    for (_DigsT::size_type i = 0; i != _digits.size(); ++i) {
      if (_digits[i] != number._digits[i]) {
        return false;
      }
    }
    
    // No difference was found, is equal.
    return true;
    
  }
  
  /*
   * Bitwise AND.
   */
  UArbInt& UArbInt::op_bit_and (UArbInt const& number) {
    
    // Cut off digits longer than number, they will be 0 anyway.
    if (_digits.size() > number._digits.size()) {
      _digits.resize(number._digits.size());
    }
    
    // AND each remaining digit.
    for (_DigsT::size_type i = 0; i != min(_digits.size(), number._digits.size()); ++i) {
      _digits[i] &= number._digits[i];
    }
    
    // Clean up.
    _trimZeros();
    
    // Done.
    return *this;
    
  }
  
  /*
   * Bitwise inclusive OR.
   */
  UArbInt& UArbInt::op_bit_ior (UArbInt const& number) {
    
    // Get the original size of *this, no processing of any of the added
    // digits will be necessary b/c of how | works.
    _DigsT::size_type origsize = min(_digits.size(), number._digits.size());
    
    // Directly copy digits longer than *this.
    if (_digits.size() < number._digits.size()) {
      _digits.insert(_digits.end(), number._digits.begin() + _digits.size(), number._digits.end());
    }
    
    // Inclusive OR each digit.
    for (_DigsT::size_type i = 0; i != origsize; ++i) {
      _digits[i] |= number._digits[i];
    }
    
    // No need to trim zeros with inclusive OR. Done.
    return *this;
    
  }
  
  /*
   * Bitwise exclusive OR.
   */
  UArbInt& UArbInt::op_bit_xor (UArbInt const& number) {
    
    // Get the original size of *this, no processing of any of the added
    // digits will be necessary b/c of how ^ works.
    _DigsT::size_type origsize = min(_digits.size(), number._digits.size());
    
    // Directly copy digits longer than *this.
    if (_digits.size() < number._digits.size()) {
      _digits.insert(_digits.end(), number._digits.begin() + _digits.size(), number._digits.end());
    }
    
    // Exclusive OR each digit.
    for (_DigsT::size_type i = 0; i != origsize; ++i) {
      _digits[i] ^= number._digits[i];
    }
    
    // Clean up.
    _trimZeros();
    
    // Done.
    return *this;
    
  }
  
  /*
   * Bitwise compliment.
   */
  UArbInt& UArbInt::op_bit_cpm () {
    
    // Apply compliment to each digit.
    for (_DigsT::size_type i = 0; i != _digits.size(); ++i) {
      _digits[i] = ~_digits[i] & s_bitmask;
    }
    
    // Clean up.
    _trimZeros();
    
    // Done.
    return *this;
    
  }
  
  /*
   * Raise this number to a power.
   */
  UArbInt UArbInt::pow (UArbInt const& exp) {
    
    // Work area.
    UArbInt tmp_base(*this);
    UArbInt tmp_expn(exp);
    UArbInt retval(1);
    
    // Russian peasant power.
    while (!tmp_expn.isZero()) {
      if (tmp_expn.isOdd()) {
        retval *= tmp_base;
      }
      tmp_base *= tmp_base;
      tmp_expn >>= 1;
    }
    
    // Return the result.
    return retval;
    
  }
  
  /*
   * Perform carry.
   */
  UArbInt& UArbInt::_carry (_DigsT::size_type start) {
    
    // Loop through the container looking for base overflow.
    for (_DigsT::size_type i = start; i != _digits.size(); ++i) {
      
      // If we find overflow, push it to the next digit.
      if (_digits[i] >= s_digitbase) {
        
        // Create the next digit if it does not already exist.
        if ((_digits.size() - 1) <= i) {
          _digits.push_back(0);
        }
        
        // Add any overflow to the next digit and remove it from the current
        // digit. No need to check i + 1 for overflow; i is the size_type of
        // _DigsT, and we just added an element, we would have recieved an
        // error when we did that.
        SafeInt<_DigsT::value_type> overflow = SafeInt<_DigsT::value_type>(_digits[i]) / s_digitbase;
        _digits[i + 1]                       = _digits[i + 1] + overflow;
        _digits[i]                           = _digits[i] - (overflow * s_digitbase);
        
      // If there is no overflow now, there will be no more overflow.
      } else {
        break;
      }
      
    }
    
    // We done.
    return *this;
    
  }
  
  /*
   * Perform borrow. Only call this on temporary objects that are not
   * copied. Copy on write is not preserved for this function.
   */
  UArbInt& UArbInt::_borrow (_DigsT::size_type start) {
    
    // Loop through the container borrowing until we've met our borrow.
    for (_DigsT::size_type i = start; i != (_digits.size() - 1); ++i) {
      
      // Add base to this digit.
      _digits[i] = SafeInt<_DigT>(_digits[i]) + s_digitbase;
      
      // If this is not the first digit, we are only here because we
      // borrowed for the previous digit. Subtract one for the borrow.
      if (i != start) {
        --_digits[i];
      }
      
      // If the next digit is > 0, subtract 1 and we're done.
      if (_digits[i + 1] > 0) {
        --_digits[i + 1];
        _trimZeros();
        return *this;
      }
      
    }
    
    // If we get here, we have not paid for our borrow. This should never ever
    // happen, but do not allow silent bugs to appear. Will cause an invalid
    // throw.
    throw Errors::Overrun();
    
  }
  
  /*
   * Bitwise shift by UArbInt.
   */
  UArbInt& UArbInt::_shift (UArbInt const& bits, _Dir const dir) {
    
    // Only shift if it is needed.
    if (*this && bits) {
      
      // Convert bits to digits if the number of bits requested meets or
      // exceeds the number of bits in a digit.
      SafeInt<unsigned int>      tmp_bits;
      SafeInt<_DigsT::size_type> tmp_digits;
      if (bits >= s_digitbits) {
        try {
          tmp_digits = static_cast<_DigsT::size_type>(bits / s_digitbits);
        } catch (Errors::ScalarOverflow) {
          throw Errors::Overrun();
        }
        tmp_bits = static_cast<unsigned int>(bits - tmp_digits * s_digitbits);
      } else {
        tmp_bits = static_cast<unsigned int>(bits);
      }
      
      // Shift.
      _shiftDigits(tmp_digits, dir);
      _shiftBits  (tmp_bits,   dir);
      
    }
    
    // Return.
    return *this;
    
  }
  
  /*
   * Bitwise shift.
   */
  UArbInt& UArbInt::_shift (SafeInt<_DigsT::size_type> const digits, SafeInt<unsigned int> const bits, _Dir const dir) {
    
    // Only shift if it is needed.
    if (*this && (digits || bits)) {
      
      // Convert bits to digits if the number of bits requested meets or
      // exceeds the number of bits in a digit.
      SafeInt<unsigned int>      tmp_bits;
      SafeInt<_DigsT::size_type> tmp_digits;
      if (bits >= s_digitbits) {
        tmp_digits = bits / s_digitbits;
        tmp_bits   = bits - tmp_digits * s_digitbits;
      } else {
        tmp_bits = bits;
      }
      
      // Shift.
      _shiftDigits(tmp_digits, dir);
      _shiftBits  (tmp_bits,   dir);
      
    }
    
    // Return.
    return *this;
    
  }
  
  /*
   * Bitwise shift entire digits.
   */
  UArbInt& UArbInt::_shiftDigits (SafeInt<_DigsT::size_type> const digits, _Dir const dir) {
    
    // Only shift if it is needed.
    if (*this && digits) {
      
      // Shift whole digits.
      if (dir == _DIR_L) {
        
        // Check if shift will overrun _DigsT::size_type.
        if (digits > numeric_limits<_DigsT::size_type>::max() - _digits.size()) {
          throw Errors::Overrun();
        }
        
        // Insert 0s to shift left.
        _digits.insert(_digits.begin(), digits, 0);
        
      } else {
        
        // Delete low-order digits to shift right. If the shift will leave no
        // digits, just zero it out.
        if (digits < _digits.size()) {
          _digits.erase(_digits.begin(), _digits.begin() + static_cast<_DigsT::size_type>(digits));
        } else {
          _digits.clear();
        }
        
      }
      
    }
    
    // Return
    return *this;
    
  }
  
  /*
   * Bitwise shift by bits.
   */
  UArbInt& UArbInt::_shiftBits (SafeInt<unsigned int> const bits, _Dir const dir) {
    
    // Only shift if it is needed.
    if (*this && bits) {
      
      // Pull out any whole digits and shift them.
      SafeInt<_DigsT::size_type> tmp_digits = bits / s_digitbits;
      SafeInt<unsigned int>      tmp_bits   = bits - tmp_digits;
      if (tmp_digits) {
        _shiftDigits(tmp_digits, dir);
      }
      
      // Shift any remaining bits.
      if (bits) {
        
        // Work area
        _DigT        carry    = 0;
        _DigT        oldcarry = 0;
        _DigT        bitmask  = rppower(SafeInt<_DigT>(2), tmp_bits) - 1;
        unsigned int bitdiff  = s_digitbits - tmp_bits;
        
        // Select shift direction.
        if (dir == _DIR_L) {
          bitmask <<= (s_digitbits - tmp_bits);
          for (_DigsT::iterator i = _digits.begin(); i != _digits.end(); ++i) {
            carry     = *i & bitmask;
            *i       <<= tmp_bits;
            *i        &= s_bitmask;
            *i        |= oldcarry;
            oldcarry   = carry >> bitdiff;
          }
          if (oldcarry) {
            _digits.push_back(oldcarry);
          }
        } else {
          for (_DigsT::reverse_iterator i = _digits.rbegin(); i != _digits.rend(); ++i) {
            carry      = *i & bitmask;
            *i       >>= tmp_bits;
            *i        |= oldcarry;
            oldcarry   = carry << bitdiff;
          }
        }
        _trimZeros();
      }
      
    }
    
    // Return.
    return *this;
    
  }
  
}
