/*****************************************************************************
 * ArbInt.cxx
 *****************************************************************************
 * Implementation for class ArbInt.
 *****************************************************************************/

// STL includes.
#include <vector>
#include <string>
#include <limits>
#include <algorithm>

// Internal includes.
#include "SafeInt.hxx"
#include "ReferencePointer.hxx"
#include "toString.hxx"
#include "rppower.hxx"
#include "logBase.hxx"

// Class include.
#include "ArbInt.hxx"

// Namespaces used.
using namespace std;

// Namespace wrapper.
namespace DAC {
  
  /***************************************************************************
   * Class ArbInt.
   ***************************************************************************/
  
  /***************************************************************************/
  // Data members.
  
  int           const ArbInt::s_digitbits = numeric_limits<_DigT>::digits >> 1;
  ArbInt::_DigT const ArbInt::s_digitbase = 1 << (numeric_limits<_DigT>::digits >> 1);
  ArbInt::_DigT const ArbInt::s_bitmask   = (1 << (numeric_limits<_DigT>::digits >> 1)) - 1;
  
  ArbInt::_NumChrT ArbInt::s_numodigits = 36;
  ArbInt::_StrChrT ArbInt::s_odigits[]  = {
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
    'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
    'U', 'V', 'W', 'X', 'Y', 'Z'
  };
  ArbInt::_NumChrT         ArbInt::s_numidigits = 36;
  vector<ArbInt::_NumChrT> ArbInt::s_idigits;
  
  bool ArbInt::s_initialized = false;
  
  /***************************************************************************/
  // Function members.
  
  // Default constructor.
  ArbInt::ArbInt () {
    
    // Call common init.
    _init();
    
  }
  
  // Copy constructor.
  ArbInt::ArbInt (ArbInt const& number, bool const copynow) {
    
    // Call common init.
    _init();
    
    // Set the number.
    copy(number, copynow);
    
  }
  
  // Conversion constructor.
  ArbInt::ArbInt (string const& number) {
    
    // Call common init.
    _init();
    
    // Set the number.
    set(number);
    
  }
  
  // Reset to just-constructed state.
  ArbInt& ArbInt::clear () {
    
    // Create a new vector, this preserves COW behavior.
    _digits = new _DigsT;
    
    // Reset the default base.
    _base = 10;
    
    // Return self.
    return *this;
    
  }
  
  // Make a copy of another number.
  ArbInt& ArbInt::copy (ArbInt const& number, bool const copynow) {
    
    // Set the new digits. Make a copy if instructed to do so, otherwise
    // just make a reference and use COW.
    _DataPT new_digits;
    if (copynow) {
      new_digits = new _DigsT(*(number._digits));
    } else {
      new_digits = number._digits;
    }
    
    // Set the default base of number.
    _base = number._base;
    
    // Swap in the new digits.
    _digits = new_digits;
    
    // Return self.
    return *this;
    
  }
  
  // Set this number with a string.
  ArbInt& ArbInt::set (string const& number) {
    
    // Load the number into this for exception safety and COW.
    _DataPT new_digits(new _DigsT);
    
    // Hold the number in case an error needs to be thrown.
    ConstReferencePointer<string> tmp_number(new string(number));
    
    // Parser will load data into here.
    _DigStrT num;
    
    // Parse the number.
    for (string::size_type i = 0; i != number.length(); ++i) {
      
      // Get the value of this digit.
      SafeInt<_NumChrT> digval(s_idigits[number[i]]);
      
      // Make sure this digit is within the number base.
      if ((digval >= _base || (digval == numeric_limits<_NumChrT>::max()))) {
        throw ArbIntErrors::BadFormat().Problem("Unrecognized character").Position(i).Number(tmp_number);
      }
      
      // Add the digit to the digit string.
      num.insert(num.begin(), digval);
      
    }
    
    // Trim insignificant zeros. This number is little-endian at this point.
    s_trimZerosE(num);
    
    // Convert to the native number base.
    s_baseConv(num, _base, *new_digits, s_digitbase);
    
    // The new number has been loaded successfully. Swap it in.
    _digits = new_digits;
    
    // We done, return the new number.
    return *this;
    
  }
  
  // Push a string onto the back of this number.
  ArbInt& ArbInt::push_back (string const& number) {
    
    // Work area.
    ArbInt retval(*this, true);
    ArbInt newnum;
    
    // Move retval up to accomidate the new digits.
    retval *= ArbInt(_base).pow(number.length());
    
    // Convert the new number.
    newnum.Base(_base).set(number);
    
    // Add the new number to this.
    retval += newnum;
    
    // Move the return into place and we're done.
    _digits = retval._digits;
    return *this;
    
  }
  
  // Return a string of this number.
  string ArbInt::toString () const {
    
    // This is the string we will return.
    string retval;
    
    // Easy case of 0.
    if (isZero()) {
      
      retval = "0";
      
    // Otherwise we have work to do.
    } else {
      
      // Convert to the output base.
      _DigsT num;
      s_baseConv(*_digits, s_digitbase, num, _base);
      
      // Load this into the string. If the base is greater than the number
      // of digits defined, output the raw numbers of each digit.
      if (SafeInt<_DigT>(_base) > s_numodigits) {
        for (_DigsT::reverse_iterator i = num.rbegin(); i != num.rend(); ++i) {
          retval += "'" + DAC::toString(*i) + "'";
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
    return retval;
    
  }
  
  // Multiply.
  ArbInt& ArbInt::op_mul (ArbInt const& number) {
    
    // Work area.
    ArbInt retval;
    
    // If either number is zero, no multiplying to be done.
    if (!isZero() && !number.isZero()) {
      
      // Create a digit product and make a reference its digits to reduce
      // typing.
      ArbInt digproduct;
      
      // Multiply like 3rd grade.
      for (_DigsT::size_type i = 0; i != number._digits->size(); ++i) {
        
        // Get the product for a single digit.
        for (_DigsT::size_type j = 0; j != _digits->size(); ++j) {
          
          // Create a new digit in the digit product if necessary.
          if (digproduct._digits->size() == j) {
            digproduct._digits->push_back(0);
          }
          
          // Multiply into the digit product and carry.
          cout << "(*(digproduct._digits))[j]: " << (*(digproduct._digits))[j] << endl;
          cout << "(*(number._digits))[i]:     " << (*(number._digits))[i]     << endl;
          cout << "(*_digits)[j]:              " << (*_digits)[j]              << endl;
          (*(digproduct._digits))[j] += SafeInt<_DigT>((*(number._digits))[i]) * (*_digits)[j];
          cout << "(*(digproduct._digits))[j]: " << (*(digproduct._digits))[j] << endl;
          digproduct._carry(j);
          
        }
        
        // Offset this digit product and add it to the final product.
        digproduct._digits->insert(digproduct._digits->begin(), i, 0);
        retval.op_add(digproduct);
        digproduct.clear();
        
      }
      
    }
    
    // Swap in the result and return.
    _digits = retval._digits;
    return *this;
    
  }
  
  // Divide.
  ArbInt& ArbInt::op_div (ArbInt const& number, ArbInt* const remainder) {
    
    // If number is zero, throw error.
    if (number.isZero()) {
      throw ArbIntErrors::DivByZeroBinary<ArbInt, ArbInt>().Left(*this).Operator("/").Right(number);
    }
    
    // Work area.
    ArbInt retval;
    
    // If number > this, result is zero, remainder is this.
    if (number > *this) {
      
      // Set the remainder.
      if (remainder != 0) {
        *remainder = *this;
      }
      
    // Otherwise we have work to do.
    } else {
      
      // Cache a copy of the high-order digit of the dividend.
      SafeInt<_DigT> roughdivnd(number._digits->back());
      
      // Seed the group of digits we will be dividing, then iterate through
      // the divisor. diggroup is set to *this's last number._digits's size
      // digits. Since *this is at least as big as number, this will always
      // succeed. The loop iterates backwards from the least significant
      // digit added to diggroup to the end of _digits, although the
      // iterator is only ever referenced to add one to it.
      ArbInt diggroup;
      *(diggroup._digits) = _DigsT(_digits->end() - number._digits->size(), _digits->end());
      for (_DigsT::reverse_iterator i = _digits->rbegin() + (number._digits->size() - 1); i != _digits->rend(); ++i) {
        
        // Guess and test. Guess is a single native digit so we can use the
        // computer to do some of our division for us. These need to be set
        // to zero at each iteration, possible cause of bug if they are not.
        SafeInt<_DigT> guess;
        ArbInt         test;
        
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
          SafeInt<_DigT> roughdivor(diggroup._digits->back());
          _DigsT::size_type j;
          for (j = 0; j != (diggroup._digits->size() - number._digits->size()); ++j) {
            roughdivor *= s_digitbase;
            roughdivor += (*(diggroup._digits))[diggroup._digits->size() - 1 - (j + 1)];
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
        retval._digits->insert(retval._digits->begin(), guess);
        diggroup -= test;
        if (i != (_digits->rend() - 1)) {
          diggroup._digits->insert(diggroup._digits->begin(), *(i + 1));
        }
        
      }
      
      // Set the remainder if a container was given.
      if (remainder != 0) {
        *remainder = diggroup;
      }
      
    }
    
    // Clean up, move the result into place and return.
    retval._trimZeros();
    _digits = retval._digits;
    return *this;
    
  }
  
  // Modulo division.
  ArbInt& ArbInt::op_mod (ArbInt const& number) {
    
    // Cannot divide by zero. check is redundant, but needed for exception to
    // report proper operator.
    if (number.isZero()) {
      throw ArbIntErrors::DivByZeroBinary<ArbInt, ArbInt>().Left(*this).Operator("%").Right(number);
    }
    
    // Work area.
    ArbInt retval;
    ArbInt quotient(*this);
    
    // Do division and return the remainder.
    quotient.op_div(number, &retval);
    _digits = retval._digits;
    return *this;
    
  }
  
  // Add.
  ArbInt& ArbInt::op_add (ArbInt const& number) {
    
    // Work area.
    ArbInt retval(*this, true);
    
    // Add like kindergarten.
    for (_DigsT::size_type i = 0; i != number._digits->size(); ++i) {
      
      // Create a new digit if necessary.
      if (i == retval._digits->size()) {
        retval._digits->push_back(0);
      }
      
      // Add this digit and carry.
      (*(retval._digits))[i] += SafeInt<_DigT>((*(number._digits))[i]);
      retval._carry(i);
      
    }
    
    // Move the result into place and return.
    _digits = retval._digits;
    return *this;
    
  }
  
  // Subtract.
  ArbInt& ArbInt::op_sub (ArbInt const& number) {
    
    // Work area.
    ArbInt retval(*this, true);
    
    // These are unsigned numbers, so if number > this, throw an error.
    if (number > retval) {
      throw ArbIntErrors::NegativeBinary<ArbInt, ArbInt>().Left(*this).Operator("-").Right(number);
    }
    
    // Subtract like kindergarten.
    for (_DigsT::size_type i = 0; i != number._digits->size(); ++i) {
      
      // Borrow if necessary and subtract.
      if ((*(retval._digits))[i] < (*(number._digits))[i]) {
        retval._borrow(i);
      }
      (*(retval._digits))[i] = SafeInt<_DigT>((*(retval._digits))[i]) - (*(number._digits))[i];
      
    }
    
    // Trim zeros.
    retval._trimZeros();
    
    // Move the result into place and return.
    _digits = retval._digits;
    return *this;
    
  }
  
  // Greater than.
  bool ArbInt::op_gt (ArbInt const& number) const {
    
    // Check zeros.
    if (isZero()) {
      return false;
    } else {
      if (number.isZero()) {
        return true;
      }
    }
    
    // Check sizes of containers.
    if (_digits->size() > number._digits->size()) {
      return true;
    }
    if (_digits->size() < number._digits->size()) {
      return false;
    }
    
    // Step through each element looking for a difference, start at high-
    // order and work down.
    for (_DigsT::size_type i = 0; i != _digits->size(); ++i) {
      _DigsT::size_type j = _digits->size() - 1 - i;
      if ((*(_digits))[j] > (*(number._digits))[j]) {
        return true;
      }
      if ((*(_digits))[j] < (*(number._digits))[j]) {
        return false;
      }
    }
    
    // No difference was found, is not greater.
    return false;
    
  }
  
  // Less than.
  bool ArbInt::op_lt (ArbInt const& number) const {
    
    // Check zeros.
    if (number.isZero()) {
      return false;
    } else {
      if (isZero()) {
        return true;
      }
    }
    
    // Check sizes of containers.
    if (_digits->size() < number._digits->size()) {
      return true;
    }
    if (_digits->size() > number._digits->size()) {
      return false;
    }
    
    // Step through each element looking for a difference, start at high-order
    // and work down.
    for (_DigsT::size_type i = 0; i != _digits->size(); ++i) {
      _DigsT::size_type j = _digits->size() - 1 - i;
      if ((*(_digits))[j] < (*(number._digits))[j]) {
        return true;
      }
      if ((*(_digits))[j] > (*(number._digits))[j]) {
        return false;
      }
    }
    
    // No difference was found, is not less.
    return false;
    
  }
  
  bool ArbInt::op_eq (ArbInt const& number) const {
    
    // Check zeros.
    if (isZero()) {
      return number.isZero();
    } else {
      if (number.isZero()) {
        return false;
      }
    }
    
    // Check sizes of containers.
    if (_digits->size() != number._digits->size()) {
      return false;
    }
    
    // Step through each element looking for a difference, start at low-order
    // and work up, most differences will be in low-order digits.
    for (_DigsT::size_type i = 0; i != _digits->size(); ++i) {
      if ((*(_digits))[i] != (*(number._digits))[i]) {
        return false;
      }
    }
    
    // No difference was found, is equal.
    return true;
    
  }
  
  // Bitwise AND.
  ArbInt& ArbInt::op_bit_and (ArbInt const& number) {
    
    // Work area.
    ArbInt retval(*this, true);
    
    // Cut off digits longer than number, they will be 0 anyway.
    if (retval._digits->size() > number._digits->size()) {
      retval._digits->resize(number._digits->size());
    }
    
    // AND each digit.
    for (_DigsT::size_type i = 0; i != retval._digits->size(); ++i) {
      (*(retval._digits))[i] &= (*(number._digits))[i];
    }
    
    // Clean up.
    retval._trimZeros();
    
    // Move the result into place and return.
    _digits = retval._digits;
    return *this;
    
  }
  
  // Bitwise inclusive OR.
  ArbInt& ArbInt::op_bit_ior (ArbInt const& number) {
    
    // Work area.
    ArbInt retval(*this, true);
    
    // Get the original size of retval, no processing of any of the added
    // digits will be necessary b/c of how | works.
    _DigsT::size_type origsize = min(retval._digits->size(), number._digits->size());
    
    // Directly copy digits longer than *this.
    if (retval._digits->size() < number._digits->size()) {
      retval._digits->insert(retval._digits->end(), number._digits->begin() + retval._digits->size(), number._digits->end());
    }
    
    // Inclusive OR each digit.
    for (_DigsT::size_type i = 0; i != origsize; ++i) {
      (*(retval._digits))[i] |= (*(number._digits))[i];
    }
    
    // No need to trim zeros with inclusive OR. Move the result into place
    // and return.
    _digits = retval._digits;
    return *this;
    
  }
  
  // Bitwise exclusive OR.
  ArbInt& ArbInt::op_bit_xor (ArbInt const& number) {
    
    // Work area.
    ArbInt retval(*this, true);
    
    // Get the original size of retval, no processing of any of the added
    // digits will be necessary b/c of how | works.
    _DigsT::size_type origsize = min(retval._digits->size(), number._digits->size());
    
    // Directly copy digits longer than *this.
    if (retval._digits->size() < number._digits->size()) {
      retval._digits->insert(retval._digits->end(), number._digits->begin() + retval._digits->size(), number._digits->end());
    }
    
    // Exclusive OR each digit.
    for (_DigsT::size_type i = 0; i != origsize; ++i) {
      (*(retval._digits))[i] ^= (*(number._digits))[i];
    }
    
    // Clean up.
    retval._trimZeros();
    
    // Move the result into place and return.
    _digits = retval._digits;
    return *this;
    
  }
  
  // Bitwise compliment.
  ArbInt& ArbInt::op_bit_cpm () {
    
    // Work area.
    ArbInt retval(*this, true);
    
    // Apply compliment to each digit.
    for (_DigsT::size_type i = 0; i != retval._digits->size(); ++i) {
      (*(retval._digits))[i] = (~((*(retval._digits))[i])) & s_bitmask;
    }
    
    // Clean up.
    retval._trimZeros();
    
    // Move the result into place and return.
    _digits = retval._digits;
    return *this;
    
  }
  
  // Raise this number to a power.
  ArbInt ArbInt::pow (ArbInt const& exp) {
    
    // Work area.
    ArbInt tmp_base(*this);
    ArbInt tmp_expn(exp);
    ArbInt retval(1);
    ArbInt one(1);
    
    // Russian peasant power.
    while (!tmp_expn.isZero()) {
      if (tmp_expn.isOdd()) {
        retval *= tmp_base;
      }
      tmp_base *= tmp_base;
      tmp_expn >>= one;
    }
    
    // Return the result.
    return retval;
    
  }
  
  // Common initialization tasks.
  void ArbInt::_init () {
    
    // Check that the class constructor was successfully called.
    if (!s_initialized) {
      s_classInit();
    }
    
    // Clear.
    clear();
    
  }
  
  // Perform carry. Only call this on temporary objects that are not copied.
  // Copy on write is not preserved for this function.
  ArbInt& ArbInt::_carry (_DigsT::size_type start) {
    
    // Ensure that the start is not beyond the end of the container.
    if (start >= _digits->size()) {
      throw ArbIntErrors::OverrunSpecialized<_DigsT::size_type>().Problem("Carry requested beyond end of container").Offset(start).Limit(_digits->size());
    }
    
    // Loop through the container looking for base overflow.
    for (_DigsT::size_type i = start; i != _digits->size(); ++i) {
      
      // If we find overflow, push it to the next digit.
      if ((*_digits)[i] >= s_digitbase) {
        
        // Create the next digit if it does not already exist.
        if ((_digits->size() - 1) <= i) {
          _digits->push_back(0);
        }
        
        // Add any overflow to the next digit and remove it from the current
        // digit. No need to check i + 1 for overflow; i is the size_type of
        // _DigsT, and we just added an element, we would have recieved an
        // error when we did that.
        SafeInt<_DigsT::value_type> overflow = SafeInt<_DigsT::value_type>((*_digits)[i]) / s_digitbase;
        (*_digits)[i + 1]                    = (*_digits)[i + 1] + overflow;
        (*_digits)[i]                        = (*_digits)[i] - (overflow * s_digitbase);
        
      // If there is no overflow now, there will be no more overflow.
      } else {
        
        // Simply leave the loop.
        break;
        
      }
      
    }
    
    // We done.
    return *this;
    
  }
  
  // Perform borrow. Only call this on temporary objects that are not
  // copied. Copy on write is not preserved for this function.
  ArbInt& ArbInt::_borrow (_DigsT::size_type start) {
    
    // Ensure that the start is not beyond the end of the container.
    if (start >= _digits->size()) {
      throw ArbIntErrors::OverrunSpecialized<_DigsT::size_type>().Problem("Borrow requested beyond end of container").Offset(start).Limit(_digits->size());
    }
    
    // Loop through the container borrowing until we've met our borrow.
    for (_DigsT::size_type i = start; i != (_digits->size() - 1); ++i) {
      
      // Add base to this digit.
      (*_digits)[i] = SafeInt<_DigT>((*_digits)[i]) + s_digitbase;
      
      // If this is not the first digit, we are only here because we
      // borrowed for the previous digit. Subtract one for the borrow.
      if (i != start) {
        --(*_digits)[i];
      }
      
      // If the next digit is > 0, subtract 1 and we're done.
      if ((*_digits)[i + 1] > 0) {
        --(*_digits)[i + 1];
        _trimZeros();
        return *this;
      }
      
    }
    
    // If we get here, we have not paid for our borrow.
    throw ArbIntErrors::OverrunSpecialized<_DigsT::size_type>().Problem("Borrow requested ran out of digits").Offset(_digits->size()).Limit(_digits->size());
    
  }
  
  // Bitwise shift by ArbInt.
  ArbInt& ArbInt::_shift (ArbInt const& bits, _Dir const dir) {
    
    // Only shift if it is needed.
    if (*this && bits) {
      
      // Convert bits to digits if the number of bits requested meets or
      // exceeds the number of bits in a digit.
      SafeInt<unsigned int>      tmp_bits;
      SafeInt<_DigsT::size_type> tmp_digits;
      if (bits >= s_digitbits) {
        tmp_digits = static_cast<_DigsT::size_type>(bits / s_digitbits);
        tmp_bits   = static_cast<unsigned int>(bits - tmp_digits * s_digitbits);
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
  
  // Bitwise shift.
  ArbInt& ArbInt::_shift (SafeInt<_DigsT::size_type> const digits, SafeInt<unsigned int> const bits, _Dir const dir) {
    
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
  
  // Bitwise shift entire digits.
  ArbInt& ArbInt::_shiftDigits (SafeInt<_DigsT::size_type> const digits, _Dir const dir) {
    
    // Only shift if it is needed.
    if (*this && digits) {
      
      // Shift whole digits.
      if (dir == _DIR_L) {
        
        // Check if shift will overrun _DigsT::size_type.
        if (digits > numeric_limits<_DigsT::size_type>::max() - _digits->size()) {
          throw ArbIntErrors::OverrunSpecialized<_DigsT::size_type>().Problem("Shift requested will overrun maximum size of digit container").Offset(digits).Limit(numeric_limits<_DigsT::size_type>::max());
        }
        
        // Insert 0s to shift left.
        _digits->insert(_digits->begin(), digits, 0);
        
      } else {
        
        // Delete low-order digits to shift right. If the shift will leave no
        // digits, just zero it out.
        if (digits < _digits->size()) {
          _digits->erase(_digits->begin(), _digits->begin() + static_cast<_DigsT::size_type>(digits));
        } else {
          _digits->clear();
        }
        
      }
      
    }
    
    // Return
    return *this;
    
  }
  
  // Bitwise shift by bits.
  ArbInt& ArbInt::_shiftBits (SafeInt<unsigned int> const bits, _Dir const dir) {
    
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
        _DigT carry    = 0;
        _DigT oldcarry = 0;
        _DigT bitmask  = rppower(SafeInt<_DigT>(2), tmp_bits) - 1;
        int   bitdiff  = s_digitbits - tmp_bits;
        
        // Select shift direction.
        if (dir == _DIR_L) {
          bitmask <<= (s_digitbits - tmp_bits);
          for (_DigsT::iterator i = _digits->begin(); i != _digits->end(); ++i) {
            carry     = *i & bitmask;
            *i       <<= tmp_bits;
            *i        |= oldcarry;
            oldcarry   = carry >> bitdiff;
          }
        } else {
          for (_DigsT::reverse_iterator i = _digits->rbegin(); i != _digits->rend(); ++i) {
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
  
  // Class constructor.
  void ArbInt::s_classInit () {
    
    // Get the maximum number that can be held in a single digit.
    
    // Get the input digits. If this changes, you need to update s_numidigits!
    SafeInt<_NumChrT> j;
    for (_NumChrT i = 0; i != numeric_limits<_NumChrT>::max(); ++i) {
      j = i;
      SafeInt<_NumChrT> digit;
      if      ((j >= '0') && (j <= '9')) { digit = j - '0';                         }
      else if ((j >= 'A') && (j <= 'Z')) { digit = j - 'A' + 10;                    }
      else if ((j >= 'a') && (j <= 'z')) { digit = j - 'a' + 10;                    }
      else                               { digit = numeric_limits<_NumChrT>::max(); }
      s_idigits.push_back(digit);
    }
    
    // Class has successfully been initialized.
    s_initialized = true;
    
  }
  
  /***************************************************************************
   * Errors.
   ***************************************************************************/
  namespace ArbIntErrors {
    
    char const* Base::what () const throw() {
      return "Undefined error in ArbInt.";
    }
    Base::~Base () throw() {
      // Da nada.
    }
    
    char const* BadFormat::what () const throw() {
      try {
        return (std::string(_problem) + " at position " + DAC::toString(SafeInt<std::string::size_type>(_position) + 1) + " in number \"" + *_number + "\".").c_str();
      } catch (...) {
        return "Bad format. Error creating message string.";
      }
    }
    
    char const* Negative::what () const throw() {
      return "Attempt to set ArbInt to a negative number.";
    }
    
    char const* NegativeUnary::what () const throw() {
      try {
        return (std::string(_prefix ? "Prefix" : "Postfix") + " operator " + _op + " applied to " + _number.toString() + " results in a negative number.").c_str();
      } catch (...) {
        return "Unary operation results in a negative number. Error creating message string.";
      }
    }
    
    char const* Overrun::what () const throw() {
      return "Instruction overruns end of container.";
    }
    
    char const* DivByZero::what () const throw() {
      return "Divide by zero.";
    }
    
    char const* ScalarOverflow::what () const throw() {
      return "ArbInt overflows requested scalar type.";
    }
    
    char const* BaseOutOfRange::what () const throw() {
      return "Requested base is out of range.";
    }
    
    char const* RootTooLarge::what () const throw() {
      try {
        return (_root.toString() + " root of " + _number.toString() + ": Root is too large to calculate.").c_str();
      } catch (...) {
        return "Root is too large to calculate. Error creating message string.";
      }
    }
    
  }
  
}
