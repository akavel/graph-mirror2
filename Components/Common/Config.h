//---------------------------------------------------------------------------
#ifndef ConfigH
#define ConfigH
//---------------------------------------------------------------------------
#ifdef _DEBUG
  #define BOOST_ENABLE_ASSERT_HANDLER

//  #define _STLP_DEBUG  //Debugging version of STLport
//  #define _STLP_NO_FORCE_INSTANTIATE
//  #define _STLP_USE_EXCEPTIONS
//  #undef _STLP_NO_DEBUG_EXCEPTIONS

//  #include <stdlib.h>
//  using std::abort;

//Don't call external functions when build as package
#if !defined(USEPACKAGES) && defined(CUSTOM_DEBUG_HANDLER)
  #define _STLP_DEBUG_TERMINATE 1  //Call __stl_debug_terminate() to terminate (eg. on assertion failed and similar)
  #define _STLP_DEBUG_MESSAGE 1    //Call __stl_debug_message() to write debug text
#else
  #include <cstdio>
  using std::vsnprintf;
#endif

#else
  #define NDEBUG

  //Used with COM in utilcls.h; Disables assert checks and prevent linker warnings in release build
  #define _INC_CRTDBG
  #define NO_PROMPT_ON_ASSERTE_FAILURE
  #define NO_PROMPT_ON_HRCHECK_FAILURE
  //#undef _ASSERTE
	#undef _ASSERTE_
	#undef OLECHECK
	#undef PROMPT_ON_ASSERTE_FAILURE
	#undef PROMPT_ON_HRCHECK_FAILURE
// #define _ASSERTE(expr) expr
	#define _ASSERTE_(expr) expr
	#define OLECHECK(hrexpr) hrexpr
	#define PROMPT_ON_ASSERTE_FAILURE 0
	#define PROMPT_ON_HRCHECK_FAILURE 0
#endif

//---------------------------------------------------------------------------
#endif
