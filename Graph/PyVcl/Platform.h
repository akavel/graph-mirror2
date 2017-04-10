#ifdef FIREMONKEY
//#define private public
//#define protected public
#include <fmx.h>
#define GUI_NAME fmx
#define INIT_FUNC PyInit_fmx
#else
#include <vcl.h>
#include <ActnList.hpp>
#include <ActnMan.hpp>
#include <OleCtrls.hpp>
#include <clipbrd.hpp>
#define GUI_NAME vcl
#define INIT_FUNC  PyInit_vcl
#endif


