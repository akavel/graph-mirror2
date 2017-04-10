/* This is a wrapper around Python.h
 * Python.h is interpretting the define _DEBUG as if we are using a debug version of Python, though we are not.
 * So we undef _DEBUG while including Python.h and redefine it afterwards if necessary. */
#ifdef _DEBUG
#define OLD_DEBUG _DEBUG
#undef _DEBUG
#endif

#ifdef _Windows
#define HAVE_ROUND
#define __STDC__
#define HAVE_SSIZE_T
#endif

#pragma warn -8105
#include "Python.h"
#include <structmember.h>
#pragma warn .8105

#ifdef _Windows
#undef __STDC__
#endif

#ifdef OLD_DEBUG
#define _DEBUG OLD_DEBUG
#endif
