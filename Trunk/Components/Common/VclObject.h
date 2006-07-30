//===========================================================================
// Copyright © 2003 Ivan Johansen
// VclObject.h
//===========================================================================
#ifndef VclObjectH
#define VclObjectH

template<typename T> class TVclObject
{
  T *const Object;
public:
  TVclObject() : Object(new T) {}
  ~TVclObject()   {delete Object;}
  TVclObject(const TVclObject &V) : Object(new T) {Object->Assign(V.Object);}
  const TVclObject& operator=(const TVclObject<T> &V) {Object->Assign(V.Object); return *this;}
  operator T*() const {return Object;}
  T* operator->() {return Object;}
};

#endif
 