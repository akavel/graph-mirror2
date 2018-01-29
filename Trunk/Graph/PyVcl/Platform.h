#ifndef FIREMONKEY
#define FIREMONKEY 0
#endif

#if FIREMONKEY
#include <fmx.h>
#include <FMX.BehaviorManager.hpp>
#define GUI_NAME "fmx"
#define GUI_TYPE "fmx.Fmx"
#define PROJECT_NAME "FireMonkey"
#define INIT_FUNC PyInit_fmx
#else
#include <vcl.h>
#include <ActnList.hpp>
#include <ActnMan.hpp>
#include <OleCtrls.hpp>
#include <clipbrd.hpp>
#define GUI_NAME "vcl"
#define GUI_TYPE "vcl.Vcl"
#define PROJECT_NAME "VCL"
#define INIT_FUNC  PyInit_vcl
#endif


#define Py_LIMITED_API


