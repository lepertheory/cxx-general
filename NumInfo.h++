/*****************************************************************************
 * NumInfo.h++
 *****************************************************************************
 * Classes that retrieve information about numbers.
 *****************************************************************************/

// Include guard.
#if !defined(NUMINFO_8989hu89h)
  #define NUMINFO_8989hu89h

// Standard includes.
  #include <limits>

// Namespace wrapper.
namespace DAC {
  
  /***************************************************************************/
  // Types.
  
  // Different possible relationships.
  namespace RelTypes {
    enum Type {
      SE_SE, SE_UE, SS_SL, SS_UL, SL_SS, SL_US,
      UE_UE, UE_SE, US_SL, US_UL, UL_SS, UL_US,
      ERROR_INTEGER_ONLY
    };
  }
  
  // Different types of numbers.
  namespace NumTypes {
    enum Type {
      UINT, SINT, FLPT, ERROR_UNKNOWN_TYPE
    };
  }
  
  /***************************************************************************
   * IntRel
   ***************************************************************************
   * Returns relationship between two integers. Compiles down to a constant,
   * do not instantiate.
   ***************************************************************************/
  template <class T, class U> class IntRel {
    
    /*
     * Public members.
     */
    public:
      
      /***********************************************************************/
      // Constants.
      static RelTypes::Type const type;
      
    /*
     * Private members.
     */
    private:
      
      // Instantiation not allowed.
      IntRel ();
      IntRel (IntRel const&);
      IntRel& operator = (IntRel const&);
      
  };
  
  /***************************************************************************
   * NumType
   ***************************************************************************
   * Returns the type of number. Compiles down to a constant, do not
   * instantiate.
   ***************************************************************************/
  template <class T> class NumType {
    
    /*
     * Public members.
     */
    public:
      
      /***********************************************************************/
      // Constants.
      static NumTypes::Type const type;
      
    /*
     * Private members.
     */
    private:
      
      // Instantiation not allowed.
      NumType ();
      NumType (NumType const&);
      NumType& operator = (NumType const&);
      
  };
  
  /***************************************************************************/
  // Functions.
  
  // Returns true if a number is negative. Avoids compiler warnings, and
  // should compile down to a no-op on unsigned types.
  template <class T> bool is_negative (T const number);
  
  /***************************************************************************/
  // Utilities.
  namespace NumInfoUtil {
    
    // Decides if a number is negative, no-op for unsigned types.
    template <class T, bool is_signed> class Is_Negative;
    template <class T> class Is_Negative<T, false> { public: static bool op (T const number); };
    template <class T> class Is_Negative<T, true > { public: static bool op (T const number); };
    
  }
  
  /***************************************************************************
   * Inline and template definitions.
   ***************************************************************************/
  
  /***************************************************************************
   * IntRel
   ***************************************************************************/
  
  /***************************************************************************/
  // Constants.
  
  // The relationship itself.
  template <class T, class U> RelTypes::Type const IntRel<T, U>::type =
    (std::numeric_limits<T>::is_integer && std::numeric_limits<U>::is_integer) ? (
      (std::numeric_limits<T>::digits > std::numeric_limits<U>::digits) ? (
        std::numeric_limits<T>::is_signed ? (
          std::numeric_limits<U>::is_signed ? (
            RelTypes::SL_SS
          ) : (
            RelTypes::SL_US
          )
        ) : (
          std::numeric_limits<U>::is_signed ? (
            RelTypes::UL_SS
          ) : (
            RelTypes::UL_US
          )
        )
      ) : (
        (std::numeric_limits<T>::digits < std::numeric_limits<U>::digits) ? (
          std::numeric_limits<T>::is_signed ? (
            std::numeric_limits<U>::is_signed ? (
              RelTypes::SS_SL
            ) : (
              RelTypes::SS_UL
            )
          ) : (
            std::numeric_limits<U>::is_signed ? (
              RelTypes::US_SL
            ) : (
              RelTypes::US_UL
            )
          )
        ) : (
          std::numeric_limits<T>::is_signed ? (
            std::numeric_limits<U>::is_signed ? (
              RelTypes::SE_SE
            ) : (
              RelTypes::SE_UE
            )
          ) : (
            std::numeric_limits<U>::is_signed ? (
              RelTypes::UE_SE
            ) : (
              RelTypes::UE_UE
            )
          )
        )
      )
    ) : (
      RelTypes::ERROR_INTEGER_ONLY
    )
  ;
  
  /***************************************************************************
   * NumType
   ***************************************************************************/

  /***************************************************************************/
  // Constants.

  // The number type.
  template <class T> NumTypes::Type const NumType<T>::type =
    std::numeric_limits<T>::is_specialized ? (
      std::numeric_limits<T>::is_integer ? (
        std::numeric_limits<T>::is_signed ? (
          NumTypes::SINT
        ) : (
          NumTypes::UINT
        )
      ) : (
        NumTypes::FLPT
      )
    ) : (
      NumTypes::ERROR_UNKNOWN_TYPE
    )
  ;
  
  /*
   * Check if a number is negative.
   */
  template <class T> inline bool is_negative (T const number) {
    return NumInfoUtil::Is_Negative<T, std::numeric_limits<T>::is_signed>::op(number);
  }
  
  /***************************************************************************
   * NumInfoUtil::Is_Negative
   ***************************************************************************/
  
  /*
   * Check if an unsigned type is negative. No-op.
   */
  template <class T> inline bool NumInfoUtil::Is_Negative<T, false>::op (T const number) {
    // Shut up compiler.
    if (number) {}
    return false;
  }
  
  /*
   * Check if a signed type is negative.
   */
  template <class T> inline bool NumInfoUtil::Is_Negative<T, true>::op (T const number) {
    return number < static_cast<T>(0);
  }
  
}

// End include guard.
#endif
