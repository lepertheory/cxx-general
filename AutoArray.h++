/*****************************************************************************
 * AutoArray
 *****************************************************************************
 * Like auto_ptr, except for arrays.
 *****************************************************************************/

// Include guard.
#if !defined(AUTOARRAY_9234khti934)
  #define AUTOARRAY_9234khti934

// Namespace container.
namespace DAC {
  
  /***************************************************************************
   * AutoArray
   ***************************************************************************/
  template<class T> class AutoArray {
    
    /*
     * Public members.
     */
    public:
      
      /***********************************************************************/
      // Function members.
      
      // Constructor. When constructed with a pointer, take owership of the
      // pointer. When constructed with another AutoArray, transfer ownership
      // from the original.
      explicit AutoArray (T*         p = 0) throw();
               AutoArray (AutoArray& p    ) throw();
      
      // Destructor.
      ~AutoArray () throw();
      
      // Assignment operator. See description of the constructor.
      AutoArray& operator= (T*         p) throw();
      AutoArray& operator= (AutoArray& p) throw();
      
      // Dereferencing operators. Works as if AutoArray were a native *.
      T& operator[] (int index) const throw();
      T& operator*  ()          const throw();
      T* operator-> ()          const throw();
      
      // get() returns the pointer without de-allocating. Will still de-
      // allocate on destruction of the object.
      T* get () const throw();
      
      // release() returns the pointer and releases ownership, i.e., the
      // memory pointed to will not be de-allocated on object destruction.
      T* release () throw();
      
      // reset() de-allocates memory at the current pointer and takes
      // ownership of the passed pointer.
      void reset (T* p = 0) throw();
      
    /*
     * Private members.
     */
    private:
      
      /***********************************************************************/
      // Data members.

      // Pointer to the array.
      T* _p;
    
  };
  
  /***************************************************************************
   * Inline and template definitions.
   ***************************************************************************/
  
  /*
   * Default and conversion constructor.
   */
  template<class T> inline AutoArray<T>::AutoArray (T* p) throw() {
    
    // Set the pointer to the given pointer.
    _p = p;
    
  }
  
  /*
   * Copy constructor. Will take pointer from source.
   */
  template<class T> inline AutoArray<T>::AutoArray (AutoArray& p) throw() {
    
    // Take the pointer, make other release.
    _p = p.release();
    
  }
  
  /*
   * Destructor.
   */
  template<class T> inline AutoArray<T>::~AutoArray () throw() {
    
    // Free pointed-to memory.
    delete[] _p;
    
  }
  
  /*
   * Assignment operator, assigning pointer to this.
   */
  template<class T> inline AutoArray<T>& AutoArray<T>::operator= (T* p) throw() {
    
    // Free current pointer, set new pointer.
    reset(p);
    
    // Return.
    return *this;
    
  }
  
  /*
   * Assignment operator, assigning another AutoArray to this. Will take
   * pointer from source.
   */
  template<class T> inline AutoArray<T>& AutoArray<T>::operator= (AutoArray<T>& p) throw() {
    
    // Make other release the pointer, free current pointer, set new pointer.
    reset(p.release());
    
    // Done.
    return *this;
    
  }
  
  /*
   * Subscript operator. Return the nth element of the array.
   */
  template<class T> inline T& AutoArray<T>::operator[] (int index) const throw() {
    return _p[index];
  }
  
  /*
   * Dereference operator. Return a reference to the first item of the array.
   */
  template<class T> inline T& AutoArray<T>::operator* () const throw() {
    return *_p;
  }
  
  /*
   * Dereference function call operator.
   */
  template<class T> inline T* AutoArray<T>::operator-> () const throw() {
    return _p;
  }
  
  /*
   * Get the pointer.
   */
  template<class T> inline T* AutoArray<T>::get () const throw() {
    return _p;
  }
  
  /*
   * Release the pointer without freeing memory.
   */
  template<class T> inline T* AutoArray<T>::release () throw() {
    T* tp = _p;
    _p    = 0;
    return tp;
  }
  
  /*
   * Free memory used by pointer and set new pointer.
   */
  template<class T> inline void AutoArray<T>::reset (T* p) throw() {
    if (_p != p) {
      delete[] _p;
      _p = p;
    }
  }
  
}

#endif
