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
  
}

// End include guard.
#endif
