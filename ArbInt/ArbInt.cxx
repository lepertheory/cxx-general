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
  
  int           ArbInt::s_digitbits = 0;
  ArbInt::_DigT ArbInt::s_digitbase = 0;
  ArbInt::_DigT ArbInt::s_bitmask   = 0;
  
  ArbInt::_NumChrT ArbInt::s_numodigits = 36;
  ArbInt::_StrChrT ArbInt::s_odigits[]  = {
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
    'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
    'U', 'V', 'W', 'X', 'Y', 'Z'
  };
  ArbInt::_NumChrT         ArbInt::s_numidigits = 0;
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
      _NumChrT digval = s_idigits[number[i]];
      
      // Make sure this digit is within the number base.
      if ((digval >= SafeInt<_DigT>(_base) || (digval == numeric_limits<_NumChrT>::max()))) {
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
    if (_digits->empty()) {
      
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
    if ((_digits->size() != 0) && (number._digits->size() != 0)) {
      
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
          (*(digproduct._digits))[j] = SafeInt<_DigT>((*(digproduct._digits))[j]) + (SafeInt<_DigT>((*(number._digits))[i]) * SafeInt<_DigT>((*_digits)[j]));
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
  ArbInt& ArbInt::op_div (ArbInt const& number, ArbInt* remainder) {
    
    // If number is zero, throw error.
    if (number.isZero()) {
      throw ArbIntErrors::DivByZero();
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
      (*(retval._digits))[i] = SafeInt<_DigT>((*(retval._digits))[i]) + SafeInt<_DigT>((*(number._digits))[i]);
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
      throw ArbIntErrors::Negative();
    }
    
    // Subtract like kindergarten.
    for (_DigsT::size_type i = 0; i != number._digits->size(); ++i) {
      
      // Borrow if necessary and subtract.
      if ((*(retval._digits))[i] < (*(number._digits))[i]) {
        retval._borrow(i);
      }
      (*(retval._digits))[i] = SafeInt<_DigT>((*(retval._digits))[i]) - SafeInt<_DigT>((*(number._digits))[i]);
      
    }
    
    // Trim zeros.
    retval._trimZeros();
    
    // Move the result into place and return.
    _digits = retval._digits;
    return *this;
    
  }
  
  // Greater than.
  bool ArbInt::op_gt (ArbInt const& number) const {
    
    // Check sizes of containers first.
    if (_digits->size() > number._digits->size()) {
      return true;
    }
    if (_digits->size() < number._digits->size()) {
      return false;
    }
    if (_digits->empty() && number._digits->empty()) {
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
    
    // Check sizes of containers first.
    if (_digits->size() < number._digits->size()) {
      return true;
    }
    if (_digits->size() > number._digits->size()) {
      return false;
    }
    if (_digits->empty() && number._digits->empty()) {
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
    
    // Check sizes of containers first.
    if (_digits->empty() && number._digits->empty()) {
      return true;
    }
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
    while (!tmp_expn._digits->empty()) {
      if (tmp_expn._digits->front() & 1) {
        retval *= tmp_base;
      }
      tmp_base  *= tmp_base;
      tmp_expn >>= one;
    }
    
    // Return the result.
    return retval;
    
  }
  
  // Find a root of this number.
  ArbInt ArbInt::root (ArbInt const& root, ArbInt& divisor, ArbInt& remainder) {
    
    // Work area.
    ArbInt eroot;
    ArbInt edivor(1);
    ArbInt erem;
    
    // 0 and 1 are special cases, always themselves, also root 1.
    if (!_digits->empty() && !((_digits->size() == 1) && ((*_digits)[0] == 1)) && (root != ArbInt(1))) {
      
      // Root 0 is a different problem.
      if (root.isZero()) {
        throw ArbIntErrors::DivByZero();
      }
      
      // FIXME: This should be picked.
      SafeInt<_DigsT::size_type> maxextra(10);
      
      // Get the root in integral form.
      SafeInt<_DigsT::size_type> iroot;
      try {
        iroot = root.Value<_DigsT::size_type>();
      } catch (ArbIntErrors::ScalarOverflow()) {
        throw ArbIntErrors::RootTooLarge();
      }
      
      // Get the number of aligned digit groups in the number before
      // expansion.
      SafeInt<_DigsT::size_type> groups = (_digits->size() - 1) / iroot + 1;
      
      // Cache type conversions.
      ArbInt abase(s_digitbase);
      
      // Iterate through the number, stop when desired precision is reached
      // or a perfect root is found.
      SafeInt<_DigsT::size_type> group;
      SafeInt<_DigsT::size_type> extra;
      do {
        
        // Get the next aligned block of digits from the radicand.
        ArbInt diggroup;
        if (group < groups) {
          SafeInt<_DigsT::size_type> spos = (groups - 1 - group) * iroot;
          SafeInt<_DigsT::size_type> epos = spos + iroot - 1;
          if (epos >= _digits->size()) {
            epos = _digits->size() - 1;
          }
          *(diggroup._digits) = _DigsT(_digits->begin() + spos.Value(), _digits->begin() + (epos + 1).Value());
          ++group;
        } else {
          *(diggroup._digits) = _DigsT(iroot, 0);
          edivor *= abase;
          ++extra;
        }
        
        // Find the next digit of the root with a binary search.
        SafeInt<_DigT> guess;
        SafeInt<_DigT> min;
        SafeInt<_DigT> max(SafeInt<_DigT>(s_digitbase) - 1);
        ArbInt         arbguess;
        ArbInt         abpr  = ArbInt(abase).pow(root);
        while (min <= max) {
          guess    = (min + max) / 2;
          arbguess = guess;
          if ((abase * eroot + arbguess).pow(root) - abpr * ArbInt(eroot).pow(root) <= abpr * erem + diggroup) {
            min = guess + 1;
          } else {
            max = guess - 1;
          }
        }
        if (guess == min) {
          guess -= 1;
        }
        arbguess = guess;
        
        // Get the next iteration's values.
        erem  = abpr * erem + diggroup - ((abase * eroot + arbguess).pow(root) - abpr * ArbInt(eroot).pow(root));
        eroot = abase * eroot + arbguess;
        /*
        ArbInt<T> new_eroot = abase * eroot + arbguess;
        ArbInt<T> new_erem  = abpr * erem + diggroup - ((abase * eroot + arbguess).pow(root) - abpr * ArbInt<T>(eroot).pow(root));
        eroot = new_eroot;
        erem  = new_erem;
        */
        
      } while ((group < groups) || ((extra < maxextra) && erem));
      
      _digits   = eroot._digits;
      remainder = erem;
      
    }
    
    // We done. Move results into place and return.
    remainder = erem;
    divisor   = edivor;
    return eroot;
    
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
      throw ArbIntErrors::Overrun();
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
        (*_digits)[i + 1]                    = SafeInt<_DigsT::value_type>((*_digits)[i + 1]) + overflow;
        (*_digits)[i]                        = SafeInt<_DigsT::value_type>((*_digits)[i]) - (overflow * s_digitbase);
        
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
      throw ArbIntErrors::Overrun();
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
    throw ArbIntErrors::Overrun();
    
  }
  
  // Bitwise shift.
  ArbInt& ArbInt::_shift (ArbInt const& number, _Dir const dir) {
    
    // Only shift if it is needed.
    if (!this->isZero() && !number.isZero()) {
      
      // Cache the number of bits in a digit as an ArbInt, this will be used
      // many times.
      ArbInt arbdigbits(s_digitbits);
      
      // If shift amount will leave no bits in the number, just zero it out.
      if ((dir == _DIR_R) && (number >= (ArbInt(_digits->size()) * arbdigbits))) {
        
        _digits->clear();
        
      } else {
        
        // Check if shift will overrun _DigsT::size_type. This line is:
        // if we are shifting left (up), check if number (number of bits to
        // shift) is greater than the maximum number of bits - the number of
        // bits in the current number. Unless you have over 4GB of VM,
        // you'll run out of RAM first, but this probably indicates a bug in
        // whatever is asking for a shift that big.
        if ((dir == _DIR_L) && (number > ((ArbInt(numeric_limits<_DigsT::size_type>::max()) * arbdigbits) - ((ArbInt(_digits->size()) * arbdigbits) - ArbInt(logBase(_digits->back(), 2)))))) {
          throw ArbIntErrors::Overrun();
        }
        
        // Shift with whole digits as much as possible. size_type should be
        // fine, since by now we've eliminated any shifts of more bits.
        // Shift by inserting or erasing whole digits.
        _DigsT::size_type digshift = (number / arbdigbits).Value<_DigsT::size_type>();
        if (digshift > 0) {
          if (dir == _DIR_L) {
            _digits->insert(_digits->begin(), digshift, 0);
          } else {
            _digits->erase(_digits->begin(), _digits->begin() + digshift);
          }
        }
        
        // Now do fine-grained shifting.
        int bitshift = (number - (ArbInt(digshift) * arbdigbits)).Value<int>();
        if (bitshift > 0) {
          _DigT carry    = 0;
          _DigT oldcarry = 0;
          if (dir == _DIR_L) {
            _DigT bitmask  = (rppower(SafeInt<_DigT>(2), bitshift) - 1) << (s_digitbits - bitshift);
            for (_DigsT::iterator i = _digits->begin(); i != _digits->end(); ++i) {
              carry      = *i & bitmask;
              *i       <<= bitshift;
              *i        |= oldcarry;
              oldcarry   = carry >> (s_digitbits - bitshift);
            }
          } else {
            _DigT bitmask  = rppower(SafeInt<_DigT>(2), bitshift) - 1;
            for (_DigsT::reverse_iterator i = _digits->rbegin(); i != _digits->rend(); ++i) {
              carry      = *i & bitmask;
              *i       >>= bitshift;
              *i        |= oldcarry;
              oldcarry   = carry << (s_digitbits - bitshift);
            }
          }
        }
        
        // Clean up.
        _trimZeros();
        
      }
      
    }
    
    // We done.
    return *this;
    
  }
  
  // Class constructor.
  void ArbInt::s_classInit () {
    
    // Get the maximum number that can be held in a single digit.
    s_digitbits = numeric_limits<_DigT>::digits >> 1;
    s_digitbase = rppower(SafeInt<_DigT>(2), s_digitbits);
    s_bitmask   = SafeInt<_DigT>(s_digitbase) - 1;
    
    // Get the input digits.
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
    s_numidigits = 36;
    
    // Class has successfully been initialized.
    s_initialized = true;
    
  }
  
}
