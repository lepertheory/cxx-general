/*****************************************************************************
 * ReferencePointer.hxx
 *****************************************************************************
 * Interface and implementation for class ReferencePointer.
 *****************************************************************************/

// Include guard.
#if !defined(REFERENCEPOINTER_huhoeu89o8e)
  #define REFERENCEPOINTER_huhoeu89o8e
  
  // Internal includes.
  #include "xorswap.hxx"
  
  // Namespace wrapper.
  namespace DAC {
    
    /*************************************************************************
     * ReferencePointer
     *************************************************************************
     * Smart pointer type that counts the number of times it is referenced and
     * only deletes itself when the reference count falls to 0.
     *************************************************************************/
    template <class T> class ReferencePointer {
      
      /***********************************************************************
       * Public members.
       ***********************************************************************/
      public:
        
        // Typedefs.
        typedef int RefCountT;
        
        // Copy constructor.
        ReferencePointer (ReferencePointer<T> const& p) throw();
        
        // Conversion constructor, also default constructor.
        explicit ReferencePointer (T* const p = 0) throw();
        
        // Destructor.
        ~ReferencePointer () throw();
        
        // Assignment operator.
        ReferencePointer& operator = (ReferencePointer<T> const& right) throw();
        ReferencePointer& operator = (T* const                   right) throw();
        
        // Dereference operator.
        T& operator *  () const throw();
        T* operator -> () const throw();
        
        // Get the number of references to this object.
        RefCountT refs () const throw();
        
        // Return true if this is a null pointer.
        bool isNull () const throw();
        
        // Get the pointer without creating an additional reference.
        T* get () const throw();
        
        // Get the pointer and release this reference.
        T* release () throw();
        
        // Release the current pointer and set a new one.
        void reset (ReferencePointer<T> const& p)     throw();
        void reset (T* const                   p = 0) throw();
        
        // Swap this pointer with another one.
        void swap (ReferencePointer<T>& p) throw();
        
        // Reset to just-constructed by default constructor state.
        void clear () throw();
      
      /***********************************************************************
       * Private members.
       ***********************************************************************/
      private:
        
        // The pointer.
        T* _p;
        
        // Number of references to this pointer.
        RefCountT* _refs;
        
        // Decrement the reference count.
        RefCountT _decrementRefs() throw();
        
    };
    
    /*************************************************************************
     * ConstReferencePointer
     *************************************************************************
     * Smart pointer type that counts the number of times it is referenced and
     * only deletes itself when the reference count falls to 0.
     *************************************************************************/
    template <class T> class ConstReferencePointer {
      
      /***********************************************************************
       * Public members.
       ***********************************************************************/
      public:
        
        // Typedefs.
        typedef int RefCountT;
        
        // Copy constructor.
        ConstReferencePointer (ReferencePointer<T>      const& p) throw();
        ConstReferencePointer (ConstReferencePointer<T> const& p) throw();
        
        // Conversion constructor, also default constructor.
        explicit ConstReferencePointer (T*       const p = 0) throw();
        explicit ConstReferencePointer (T const* const p)     throw();
        
        // Destructor.
        ~ConstReferencePointer () throw();
        
        // Assignment operator.
        ConstReferencePointer& operator = (ReferencePointer<T>      const& right) throw();
        ConstReferencePointer& operator = (ConstReferencePointer<T> const& right) throw();
        ConstReferencePointer& operator = (T*                       const  right) throw();
        ConstReferencePointer& operator = (T const*                 const  right) throw();
        
        // Dereference operator.
        T const& operator *  () const throw();
        T const* operator -> () const throw();
        
        // Get the number of references to this object.
        RefCountT refs () const throw();
        
        // Return true if this is a null pointer.
        bool isNull () const throw();
        
        // Get the pointer without creating an additional reference.
        T const* get () const throw();
        
        // Get the pointer and release this reference.
        T const* release () throw();
        
        // Release the current pointer and set a new one.
        void reset (ReferencePointer<T>      const& p)     throw();
        void reset (ConstReferencePointer<T> const& p)     throw();
        void reset (T*                       const  p)     throw();
        void reset (T const*                 const  p = 0) throw();
        
        // Swap this pointer with another one.
        void swap (ConstReferencePointer<T>& p) throw();
        
        // Reset to just-constructed by default constructor state.
        void clear () throw();
      
      /***********************************************************************
       * Private members.
       ***********************************************************************/
      private:
        
        // The pointer.
        T const* _p;
        
        // Number of references to this pointer.
        RefCountT* _refs;
        
        // Decrement the reference count.
        RefCountT _decrementRefs() throw();
        
    };
    
    /*************************************************************************
     * Inline and template definitions.
     *************************************************************************/
    
    // Copy constructor.
    template <class T> ReferencePointer<T>::ReferencePointer (ReferencePointer<T> const& p) throw() {
      
      // Just constructed.
      clear();
      
      // Set the pointer & increment references.
      reset(p);
      
    }
    
    // Conversion and default constructor.
    template <class T> ReferencePointer<T>::ReferencePointer (T* const p) throw() {
      
      // Just constructed.
      clear();
      
      // Set the pointer & start reference counting.
      reset(p);
      
    }
    
    // Destructor.
    template <class T> ReferencePointer<T>::~ReferencePointer () throw() {
      
      // Release the current pointer.
      reset();
      
    }
    
    // Assignment operator (copy).
    template <class T> inline ReferencePointer<T>& ReferencePointer<T>::operator = (ReferencePointer<T> const& right) throw() {
      
      // Release the current pointer and set the new pointer.
      reset(right);
      
      // Return this because that's how the assignment operator works.
      return *this;
      
    }
    
    // Assignment operator (conversion).
    template <class T> inline ReferencePointer<T>& ReferencePointer<T>::operator = (T* const right) throw() {
      
      // Release the current pointer and set the new pointer.
      reset(right);
      
      // Return this because that's how the assignment operator works.
      return *this;
      
    }
    
    // Dereference operator.
    template <class T> inline T& ReferencePointer<T>::operator * () const throw() {
      
      // Return a reference to the pointer's target.
      return *_p;
      
    }
    
    // Dereference member call operator.
    template <class T> inline T* ReferencePointer<T>::operator -> () const throw() {
      
      // Return the pointer itself, the -> operator takes care of
      // dereferencing automatically.
      return _p;
      
    }
    
    // Return the number of references to the pointer.
    template <class T> inline typename ReferencePointer<T>::RefCountT ReferencePointer<T>::refs () const throw() {
      
      // Duh.
      if (_refs == 0) {
        return 0;
      } else {
        return *_refs;
      }
      
    }
    
    // Return true if the pointer is null.
    template <class T> inline bool ReferencePointer<T>::isNull () const throw() {
      
      // Duh.
      return (_p == 0);
      
    }
    
    // Get the pointer without creating an additional reference.
    template <class T> inline T* ReferencePointer<T>::get () const throw() {
      
      // Duh.
      return _p;
      
    }
    
    // Get the pointer and release this reference.
    template <class T> T* ReferencePointer<T>::release () throw() {
      
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
    
    // Set this pointer to point to a new pointer. (pointer)
    template <class T> void ReferencePointer<T>::reset (T* const p) throw() {
      
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
    
    // Set this pointer to point to an existing pointer. (ReferencePointer)
    template <class T> void ReferencePointer<T>::reset (ReferencePointer<T> const& p) throw() {
      
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
    
    // Swap this pointer with another one.
    template <class T> void ReferencePointer<T>::swap (ReferencePointer<T>& p) throw() {
      
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
    
    // Reset to just-constructed by default constructor state.
    template <class T> void ReferencePointer<T>::clear () throw() {
      
      // Pointing to nothing.
      _p = 0;
      
      // No counter.
      _refs = 0;
      
    }
    
    // Decrement the number of references to the pointer.
    template <class T> typename ReferencePointer<T>::RefCountT ReferencePointer<T>::_decrementRefs () throw() {
      
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
    
    // Copy constructor.
    template <class T> ConstReferencePointer<T>::ConstReferencePointer (ReferencePointer<T>      const& p) throw() { clear(); reset(p); }
    template <class T> ConstReferencePointer<T>::ConstReferencePointer (ConstReferencePointer<T> const& p) throw() { clear(); reset(p); }
    
    // Conversion and default constructor.
    template <class T> ConstReferencePointer<T>::ConstReferencePointer (T*       const p) throw() { clear(); reset(p); }
    template <class T> ConstReferencePointer<T>::ConstReferencePointer (T const* const p) throw() { clear(); reset(p); }
    
    // Destructor.
    template <class T> ConstReferencePointer<T>::~ConstReferencePointer () throw() { reset(); }
    
    // Assignment operator (copy).
    template <class T> inline ConstReferencePointer<T>& ConstReferencePointer<T>::operator = (ReferencePointer<T>      const& right) throw() { reset(right); return *this; }
    template <class T> inline ConstReferencePointer<T>& ConstReferencePointer<T>::operator = (ConstReferencePointer<T> const& right) throw() { reset(right); return *this; }
    
    // Assignment operator (conversion).
    template <class T> inline ConstReferencePointer<T>& ConstReferencePointer<T>::operator = (T*       const right) throw() { reset(right); return *this; }
    template <class T> inline ConstReferencePointer<T>& ConstReferencePointer<T>::operator = (T const* const right) throw() { reset(right); return *this; }
    
    // Dereference operator.
    template <class T> inline T const& ConstReferencePointer<T>::operator * () const throw() { return *_p; }
    
    // Dereference member call operator.
    template <class T> inline T const* ConstReferencePointer<T>::operator -> () const throw() { return _p; }
    
    // Return the number of references to the pointer.
    template <class T> inline typename ConstReferencePointer<T>::RefCountT ConstReferencePointer<T>::refs () const throw() { if (_refs == 0) { return 0; } else { return *_refs; } }
    
    // Return true if the pointer is null.
    template <class T> inline bool ConstReferencePointer<T>::isNull () const throw() { return (_p == 0); }
    
    // Get the pointer without creating an additional reference.
    template <class T> inline T const* ConstReferencePointer<T>::get () const throw() { return _p; }
    
    // Get the pointer and release this reference.
    template <class T> T const* ConstReferencePointer<T>::release () throw() { T const* tp = _p; _p = 0; _decrementRefs(); _refs = 0; return tp; }
    
    // Set this pointer to point to a new pointer. (pointer)
    template <class T> void ConstReferencePointer<T>::reset (T*       const p) throw() { if (_p != p) { if (_decrementRefs() == 0) { delete _p; } _p = p; if (_p == 0) { _refs = 0; } else { _refs = new int; *_refs = 1; } } }
    template <class T> void ConstReferencePointer<T>::reset (T const* const p) throw() { if (_p != p) { if (_decrementRefs() == 0) { delete _p; } _p = p; if (_p == 0) { _refs = 0; } else { _refs = new int; *_refs = 1; } } }
    
    // Set this pointer to point to an existing pointer. (ReferencePointer)
    template <class T> void ConstReferencePointer<T>::reset (ReferencePointer<T>      const& p) throw() { if (_p != p._p) { if (_decrementRefs() == 0) { delete _p; } _p = p._p; if (_p == 0) { _refs = 0; } else { _refs = p._refs; if (_refs != 0) { ++(*_refs); } } } }
    template <class T> void ConstReferencePointer<T>::reset (ConstReferencePointer<T> const& p) throw() { if (_p != p._p) { if (_decrementRefs() == 0) { delete _p; } _p = p._p; if (_p == 0) { _refs = 0; } else { _refs = p._refs; if (_refs != 0) { ++(*_refs); } } } }
    
    // Swap this pointer with another one.
    template <class T> void ConstReferencePointer<T>::swap (ConstReferencePointer<T>& p) throw() { if (_p != p._p) { T* tmpp = _p; _p = p._p; p._p = tmpp; int* tmprefs = _refs; _refs = p._refs; p._refs = tmprefs; } }
    
    // Reset to just-constructed by default constructor state.
    template <class T> void ConstReferencePointer<T>::clear () throw() { _p = 0; _refs = 0; }
    
    // Decrement the number of references to the pointer.
    template <class T> typename ConstReferencePointer<T>::RefCountT ConstReferencePointer<T>::_decrementRefs () throw() { if (_refs != 0) { --(*_refs); if (*_refs == 0) { delete _refs; _refs = 0; } } return (_refs == 0) ? 0 : *_refs; }
    
  };
  
#endif
