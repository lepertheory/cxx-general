/*****************************************************************************
 * ReferencePointer.h++
 *****************************************************************************
 * Interface and implementation for class ReferencePointer.
 *****************************************************************************/

// Include guard.
#if !defined(REFERENCEPOINTER_huhoeu89o8e)
  #define REFERENCEPOINTER_huhoeu89o8e

// Namespace wrapper.
namespace DAC {
  
  /***************************************************************************
   * ReferencePointer
   ***************************************************************************
   * Smart pointer type that counts the number of times it is referenced and
   * only deletes itself when the reference count falls to 0.
   ***************************************************************************/
  template <class T> class ReferencePointer {
    
    /*
     * Public members.
     */
    public:
      
      // Typedefs.
      typedef int RefCountT;
      typedef T   value_type;
      
      // Copy constructor.
      ReferencePointer (ReferencePointer<T> const& p);
      
      // Conversion constructor, also default constructor.
      explicit ReferencePointer (T* const p = 0);
      
      // Destructor.
      ~ReferencePointer ();
      
      // Assignment operator.
      ReferencePointer& operator = (ReferencePointer<T> const& right);
      ReferencePointer& operator = (T* const                   right);
      
      // Dereference operator.
      T& operator *  () const;
      T* operator -> () const;
      
      // Get the number of references to this object.
      RefCountT refs () const;
      
      // Return true if this is a null pointer.
      bool isNull () const;
      
      // Get the pointer without creating an additional reference.
      T* get () const;
      
      // Get the pointer and release this reference.
      T* release ();
      
      // Release the current pointer and set a new one.
      void reset (ReferencePointer<T> const& p)    ;
      void reset (T* const                   p = 0);
      
      // Swap this pointer with another one.
      void swap (ReferencePointer<T>& p);
      
      // Reset to just-constructed by default constructor state.
      void clear ();
    
    /*
     * Private members.
     */
    private:
      
      // Friend classes.
      template <class U> friend class ConstReferencePointer;
      
      // The pointer.
      T* _p;
      
      // Number of references to this pointer.
      RefCountT* _refs;
      
      // Decrement the reference count.
      RefCountT _decrementRefs();
      
  };
  
  /***************************************************************************
   * ConstReferencePointer
   ***************************************************************************
   * Smart pointer type that counts the number of times it is referenced and
   * only deletes itself when the reference count falls to 0.
   ***************************************************************************/
  template <class T> class ConstReferencePointer {
    
    /*
     * Public members.
     */
    public:
      
      // Typedefs.
      typedef int RefCountT;
      
      // Copy constructor.
      ConstReferencePointer (ReferencePointer<T>      const& p);
      ConstReferencePointer (ConstReferencePointer<T> const& p);
      
      // Conversion constructor, also default constructor.
      explicit ConstReferencePointer (T*       const p = 0);
      explicit ConstReferencePointer (T const* const p)    ;
      
      // Destructor.
      ~ConstReferencePointer ();
      
      // Assignment operator.
      ConstReferencePointer& operator = (ReferencePointer<T>      const& right);
      ConstReferencePointer& operator = (ConstReferencePointer<T> const& right);
      ConstReferencePointer& operator = (T*                       const  right);
      ConstReferencePointer& operator = (T const*                 const  right);
      
      // Dereference operator.
      T const& operator *  () const;
      T const* operator -> () const;
      
      // Get the number of references to this object.
      RefCountT refs () const;
      
      // Return true if this is a null pointer.
      bool isNull () const;
      
      // Get the pointer without creating an additional reference.
      T const* get () const;
      
      // Get the pointer and release this reference.
      T const* release ();
      
      // Release the current pointer and set a new one.
      void reset (ReferencePointer<T>      const& p)    ;
      void reset (ConstReferencePointer<T> const& p)    ;
      void reset (T*                       const  p)    ;
      void reset (T const*                 const  p = 0);
      
      // Swap this pointer with another one.
      void swap (ConstReferencePointer<T>& p);
      
      // Reset to just-constructed by default constructor state.
      void clear ();
      
    /*
     * Private members.
     */
    private:
      
      // The pointer.
      T const* _p;
      
      // Number of references to this pointer.
      RefCountT* _refs;
      
      // Decrement the reference count.
      RefCountT _decrementRefs();
      
  };
  
  /***************************************************************************
   * Inline and template definitions.
   ***************************************************************************/
  
  /*
   * Copy constructor.
   */
  template <class T> inline ReferencePointer<T>::ReferencePointer (ReferencePointer<T> const& p) {
    
    // Just constructed.
    clear();
    
    // Set the pointer & increment references.
    reset(p);
    
  }
  
  /*
   * Conversion and default constructor.
   */
  template <class T> inline ReferencePointer<T>::ReferencePointer (T* const p) {
    
    // Just constructed.
    clear();
    
    // Set the pointer & start reference counting.
    reset(p);
    
  }
  
  /*
   * Destructor.
   */
  template <class T> inline ReferencePointer<T>::~ReferencePointer () {
    
    // Release the current pointer.
    reset();
    
  }
  
  /*
   * Assignment operator (copy).
   */
  template <class T> inline ReferencePointer<T>& ReferencePointer<T>::operator = (ReferencePointer<T> const& right) {
    
    // Release the current pointer and set the new pointer.
    reset(right);
    
    // Return this because that's how the assignment operator works.
    return *this;
    
  }
  
  /*
   * Assignment operator (conversion).
   */
  template <class T> inline ReferencePointer<T>& ReferencePointer<T>::operator = (T* const right) {
    
    // Release the current pointer and set the new pointer.
    reset(right);
    
    // Return this because that's how the assignment operator works.
    return *this;
    
  }
  
  /*
   * Dereference operator.
   */
  template <class T> inline T& ReferencePointer<T>::operator * () const {
    
    // Return a reference to the pointer's target.
    return *_p;
    
  }
  
  /*
   * Dereference member call operator.
   */
  template <class T> inline T* ReferencePointer<T>::operator -> () const {
    
    // Return the pointer itself, the -> operator takes care of
    // dereferencing automatically.
    return _p;
    
  }
  
  /*
   * Return the number of references to the pointer.
   */
  template <class T> inline typename ReferencePointer<T>::RefCountT ReferencePointer<T>::refs () const {
    
    // Duh.
    if (_refs == 0) {
      return 0;
    } else {
      return *_refs;
    }
    
  }
  
  /*
   * Return true if the pointer is null.
   */
  template <class T> inline bool ReferencePointer<T>::isNull () const {
    
    // Duh.
    return (_p == 0);
    
  }
  
  /*
   * Get the pointer without creating an additional reference.
   */
  template <class T> inline T* ReferencePointer<T>::get () const {
    
    // Duh.
    return _p;
    
  }
  
  /*
   * Get the pointer and release this reference.
   */
  template <class T> inline T* ReferencePointer<T>::release () {
    
    // Hold the current pointer.
    T* tp = _p;
    
    // Set this pointer to nothing, decrement the reference count, and
    // release the reference counter.
    _p    = 0;
    _decrementRefs();
    _refs = 0;
    
    // Return the old pointer.
    return tp;
    
  }
  
  /*
   * Set this pointer to point to a new pointer. (pointer)
   */
  template <class T> inline void ReferencePointer<T>::reset (T* const p) {
    
    // Make sure this is a new pointer.
    if (_p != p) {
      
      // Decrement references to the old pointer. Delete it if references
      // fall to zero.
      if (_decrementRefs() == 0) {
        delete _p;
      }
      
      // Set the new pointer and create a reference counter. If _p is a
      // null pointer, there is no counter.
      _p = p;
      if (_p == 0) {
        _refs = 0;
      } else {
        _refs  = new int;
        *_refs = 1;
      }
      
    }
    
  }
  
  /*
   * Set this pointer to point to an existing pointer. (ReferencePointer)
   */
  template <class T> inline void ReferencePointer<T>::reset (ReferencePointer<T> const& p) {
    
    // Make sure this is a new pointer.
    if (_p != p._p) {
      
      // Decrement references to the old pointer. Delete it if references
      // fall to zero.
      if (_decrementRefs() == 0) {
        delete _p;
      }
      
      // Set the new pointer and link to and increment the current reference
      // counter. If _p is a null pointer, there is no counter.
      _p = p._p;
      if (_p == 0) {
        _refs = 0;
      } else {
        _refs = p._refs;
        if (_refs != 0) {
          ++(*_refs);
        }
      }
      
    }
    
  }
  
  /*
   * Swap this pointer with another one.
   */
  template <class T> inline void ReferencePointer<T>::swap (ReferencePointer<T>& p) {
    
    // Make sure this is a new pointer.
    if (_p != p._p) {
      
      // Swap.
      T* tmpp = _p;
      _p      = p._p;
      p._p    = tmpp;
      
      int* tmprefs = _refs;
      _refs        = p._refs;
      p._refs      = tmprefs;
      
    }
    
  }
  
  /*
   * Reset to just-constructed by default constructor state.
   */
  template <class T> inline void ReferencePointer<T>::clear () {
    
    // Pointing to nothing.
    _p = 0;
    
    // No counter.
    _refs = 0;
    
  }
  
  /*
   * Decrement the number of references to the pointer.
   */
  template <class T> inline typename ReferencePointer<T>::RefCountT ReferencePointer<T>::_decrementRefs () {
    
    // Only decrement if the counter exists.
    if (_refs != 0) {
      
      // Decrement the refs and release the reference counter if it reaches
      // zero.
      --(*_refs);
      if (*_refs == 0) {
        delete _refs;
        _refs = 0;
      }
      
    }
    
    // Return the number of references to this pointer.
    return (_refs == 0) ? 0 : *_refs;
    
  }
  
  /*
   * Copy constructor.
   */
  template <class T> inline ConstReferencePointer<T>::ConstReferencePointer (ReferencePointer<T>      const& p) { clear(); reset(p); }
  template <class T> inline ConstReferencePointer<T>::ConstReferencePointer (ConstReferencePointer<T> const& p) { clear(); reset(p); }
  
  /*
   * Conversion and default constructor.
   */
  template <class T> inline ConstReferencePointer<T>::ConstReferencePointer (T*       const p) { clear(); reset(p); }
  template <class T> inline ConstReferencePointer<T>::ConstReferencePointer (T const* const p) { clear(); reset(p); }
  
  /*
   * Destructor.
   */
  template <class T> inline ConstReferencePointer<T>::~ConstReferencePointer () { reset(); }
  
  /*
   * Assignment operator (copy).
   */
  template <class T> inline ConstReferencePointer<T>& ConstReferencePointer<T>::operator = (ReferencePointer<T>      const& right) { reset(right); return *this; }
  template <class T> inline ConstReferencePointer<T>& ConstReferencePointer<T>::operator = (ConstReferencePointer<T> const& right) { reset(right); return *this; }
  
  /*
   * Assignment operator (conversion).
   */
  template <class T> inline ConstReferencePointer<T>& ConstReferencePointer<T>::operator = (T*       const right) { reset(right); return *this; }
  template <class T> inline ConstReferencePointer<T>& ConstReferencePointer<T>::operator = (T const* const right) { reset(right); return *this; }
  
  /*
   * Dereference operator.
   */
  template <class T> inline T const& ConstReferencePointer<T>::operator * () const { return *_p; }
  
  /*
   * Dereference member call operator.
   */
  template <class T> inline T const* ConstReferencePointer<T>::operator -> () const { return _p; }
  
  /*
   * Return the number of references to the pointer.
   */
  template <class T> inline typename ConstReferencePointer<T>::RefCountT ConstReferencePointer<T>::refs () const { if (_refs == 0) { return 0; } else { return *_refs; } }
  
  /*
   * Return true if the pointer is null.
   */
  template <class T> inline bool ConstReferencePointer<T>::isNull () const { return (_p == 0); }
  
  /*
   * Get the pointer without creating an additional reference.
   */
  template <class T> inline T const* ConstReferencePointer<T>::get () const { return _p; }
  
  /*
   * Get the pointer and release this reference.
   */
  template <class T> inline T const* ConstReferencePointer<T>::release () { T const* tp = _p; _p = 0; _decrementRefs(); _refs = 0; return tp; }
  
  /*
   * Set this pointer to point to a new pointer. (pointer)
   */
  template <class T> inline void ConstReferencePointer<T>::reset (T*       const p) { if (_p != p) { if (_decrementRefs() == 0) { delete _p; } _p = p; if (_p == 0) { _refs = 0; } else { _refs = new int; *_refs = 1; } } }
  template <class T> inline void ConstReferencePointer<T>::reset (T const* const p) { if (_p != p) { if (_decrementRefs() == 0) { delete _p; } _p = p; if (_p == 0) { _refs = 0; } else { _refs = new int; *_refs = 1; } } }
  
  /*
   * Set this pointer to point to an existing pointer. (ReferencePointer)
   */
  template <class T> inline void ConstReferencePointer<T>::reset (ReferencePointer<T>      const& p) { if (_p != p._p) { if (_decrementRefs() == 0) { delete _p; } _p = p._p; if (_p == 0) { _refs = 0; } else { _refs = p._refs; if (_refs != 0) { ++(*_refs); } } } }
  template <class T> inline void ConstReferencePointer<T>::reset (ConstReferencePointer<T> const& p) { if (_p != p._p) { if (_decrementRefs() == 0) { delete _p; } _p = p._p; if (_p == 0) { _refs = 0; } else { _refs = p._refs; if (_refs != 0) { ++(*_refs); } } } }
  
  /*
   * Swap this pointer with another one.
   */
  template <class T> inline void ConstReferencePointer<T>::swap (ConstReferencePointer<T>& p) { if (_p != p._p) { T* tmpp = _p; _p = p._p; p._p = tmpp; int* tmprefs = _refs; _refs = p._refs; p._refs = tmprefs; } }
  
  /*
   * Reset to just-constructed by default constructor state.
   */
  template <class T> inline void ConstReferencePointer<T>::clear () { _p = 0; _refs = 0; }
  
  /*
   * Decrement the number of references to the pointer.
   */
  template <class T> inline typename ConstReferencePointer<T>::RefCountT ConstReferencePointer<T>::_decrementRefs () { if (_refs != 0) { --(*_refs); if (*_refs == 0) { delete _refs; _refs = 0; } } return (_refs == 0) ? 0 : *_refs; }
  
}

#endif
