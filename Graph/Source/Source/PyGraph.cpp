//---------------------------------------------------------------------------
#include "Graph.h"
#pragma hdrstop
//This creates the definition of Python variables when PythonBind.h is included.
//Warning: This has been seen to cause access violations. It may help to move the define to another file.
#define PYTHON_WRAP(type,name) type& name = GetPythonAddress<type>(#name);
#include "PythonBind.h"
#include "Python.hpp"
#include "PyGraph.h"
#include "Unit1.h"
#include "Unit22.h"
#include "VersionInfo.h"
#include <fstream>
#include "Images.h"
#include <boost/variant/get.hpp>
#include "PyVcl.h"
#include "ConfigRegistry.h"
#include "PyVclObject.h"
//---------------------------------------------------------------------------
PyObject* DownCastSharedPtr(const boost::shared_ptr<TGraphElem> &Elem);
namespace Python
{
PyObject *PyEFuncError = NULL;
PyObject *PyEGraphError = NULL;
HINSTANCE PythonInstance = NULL;
static bool PythonInitialized = false;
//---------------------------------------------------------------------------
template<typename T>
T& GetPythonAddress(const char *Name)
{
  static T Dummy;
  if(IsPythonInstalled())
    return *reinterpret_cast<T*>(GetProcAddress(PythonInstance, Name));
  return Dummy;
}
//---------------------------------------------------------------------------
bool IsPythonInstalled()
{
static bool PythonInstalledChecked = false; //Warning: There might be an initialization order issue.

#define STRING(x) #x
#define XSTRING(x) STRING(x)
#define PYTHON_DLL_NAME "Python" XSTRING(PY_MAJOR_VERSION) XSTRING(PY_MINOR_VERSION) ".dll";
#define PYTHON_VERSION_STRING XSTRING(PY_MAJOR_VERSION) "." XSTRING(PY_MINOR_VERSION)
  if(!PythonInstalledChecked)
  {
		std::wstring BaseDir = GetRegValue(REGISTRY_KEY, L"BaseDir", HKEY_CURRENT_USER, ExtractFileDir(Application->ExeName).c_str());
    std::wstring PythonDll = BaseDir + L"\\Python\\" PYTHON_DLL_NAME;
    PythonInstance = LoadLibrary(PythonDll.c_str());

    if(PythonInstance == NULL)
    {
      //On 64-bit Windows this will actually access Software\Wow6432Node\Python\PythonCore\3.6-32\InstallPath
      //under HKCU because of redirection of some registry keys for 32-bit applications.
      wchar_t *RegKey = L"Software\\Python\\PythonCore\\" PYTHON_VERSION_STRING "-32\\InstallPath";
		  BaseDir = GetRegValue(RegKey, L"", HKEY_CURRENT_USER, L"").c_str();
      if(BaseDir.empty())
  		  BaseDir = GetRegValue(RegKey, L"", HKEY_LOCAL_MACHINE, L"").c_str();
      if(!BaseDir.empty())
      {
        PythonDll = BaseDir + L"\\" PYTHON_DLL_NAME;
        PythonInstance = LoadLibrary(PythonDll.c_str());
      }
    }
    PythonInstalledChecked = true;
  }
  return PythonInstance != NULL;
}
//---------------------------------------------------------------------------
void PrintException(bool ShowTraceback=true, bool ThrowException=false)
{
	if(PyErr_ExceptionMatches(PyExc_SystemExit))
  {
    PyErr_Clear();
    Form1->Close();
    return;
  }

  String Str;
  if(!ShowTraceback || ThrowException)
  {
    PyObject *Type, *Value, *Traceback;
    PyErr_Fetch(&Type, &Value, &Traceback);
    if(!ShowTraceback)
    {
      Py_XDECREF(Traceback);
      Traceback = NULL;
    }
    if(ThrowException)
    {
      TPyObjectPtr StrObject(PyObject_Str(Value), false);
      Str = FromPyObject<String>(StrObject.get());
    }
    PyErr_Restore(Type, Value, Traceback);
  }
  PyErr_Print();
  if(ThrowException)
    throw Python::EPyVclError(Str);
}
//---------------------------------------------------------------------------
bool ExecutePythonCommand(const String &Command)
{
	TLockGIL Dummy;
  PyErr_Clear();
  PyObject *Module = PyImport_ImportModule("code");
  bool Result = true;
  if(Module)
  {
    PyObject *Function = PyObject_GetAttrString(Module, "compile_command");
    if(Function)
    {
      PyObject *Code = PyObject_CallFunction(Function, "us", Command.c_str(), "<console>");
      if(Code == NULL)
        PrintException(false);
      else if(Code == Py_None)
        Result = false;
      else
      {
	      PyObject *MainModule = PyImport_AddModule("__main__");
	      if(MainModule)
        {
      	  PyObject *Dict = PyModule_GetDict(MainModule);
          PyObject *Temp = PyEval_EvalCode(Code, Dict, Dict);
          if(Temp == NULL)
            PrintException();
          Py_XDECREF(Temp);
        }
      }
      Py_XDECREF(Code);
      Py_DECREF(Function);
    }
    Py_DECREF(Module);
  }
  else
    PrintException();
  return Result;
}
//---------------------------------------------------------------------------
PyObject* ToPyObject(const TPyVariant &Variant)
{
  if(const int *Value = boost::get<int>(&Variant  ))
    return ToPyObject(*Value);
  if(const double *Value = boost::get<double>(&Variant))
    return ToPyObject(*Value);
  if(const std::wstring *Value = boost::get<std::wstring>(&Variant))
    return ToPyObject(*Value);
	if(PyObject *const*Value = boost::get<PyObject*>(&Variant))
	{
	  Py_INCREF(*Value);
		return *Value;
	}
  if(TObject *const*Object = boost::get<TObject*>(&Variant))
  {
    return VclObject_Create(*Object, false);
  }
  return NULL;
}
//---------------------------------------------------------------------------
bool ExecutePluginEvent(TPluginEvent PluginEvent, const TGraphElemPtr &Elem)
{
  if(!PythonInitialized)
    return false;
  if(IsPythonInstalled())
  {
    TLockGIL Dummy;
    return ExecutePluginEvent(PluginEvent, Py_BuildValue("(N)", DownCastSharedPtr(Elem)));
  }
  return false;
}
//---------------------------------------------------------------------------
bool ExecutePluginEvent(TPluginEvent PluginEvent, PyObject *Param)
{
  if(IsPythonInstalled() && PythonInitialized)
  {
    TLockGIL Dummy;
    PyObject *Module = PyImport_AddModule("Graph");
    char *MethodName = "ExecuteEvent";
    char *Format = Param ? (char*)"(iN)" : (char*)"(i())";
    PyObject *ResultObj = PyObject_CallMethod(Module, MethodName, Format, PluginEvent, Param, NULL);
    bool Result = ResultObj && PyObject_IsTrue(ResultObj);
		Py_XDECREF(ResultObj);
    return Result;
	}
  return false;
}
//---------------------------------------------------------------------------
bool ExecutePluginEvent(TPluginEvent PluginEvent, TPyVariant V1)
{
  if(IsPythonInstalled())
  {
    TLockGIL Dummy;
    return ExecutePluginEvent(PluginEvent, Py_BuildValue("(N)", ToPyObject(V1)));
  }
  return false;
}
//---------------------------------------------------------------------------
bool ExecutePluginEvent(TPluginEvent PluginEvent, TPyVariant V1, TPyVariant V2)
{
  if(IsPythonInstalled())
  {
    TLockGIL Dummy;
    return ExecutePluginEvent(PluginEvent, Py_BuildValue("(NN)", ToPyObject(V1), ToPyObject(V2)));
  }
  return false;
}
//---------------------------------------------------------------------------
bool ExecutePluginEvent(TPluginEvent PluginEvent, TPyVariant V1, TPyVariant V2, TPyVariant V3)
{
  if(IsPythonInstalled())
	{
		TLockGIL Dummy;
    return ExecutePluginEvent(PluginEvent, Py_BuildValue("(NNN)", ToPyObject(V1), ToPyObject(V2), ToPyObject(V3)));
  }
  return false;
}
//---------------------------------------------------------------------------
TPyObjectPtr ExecutePluginFunc(const char *MethodName, PyObject *Param)
{
  //Python must be installed and the we must have the GIL
  PyObject *Module = PyImport_AddModule("Graph");
  TPyObjectPtr Function(PyObject_GetAttrString(Module, MethodName), false);
  if(!Function)
    PrintException(true, true);
  TPyObjectPtr ResultObj(PyObject_CallObject(Function.get(), Param), false);
  if(!ResultObj)
    PrintException(true, true);
  return ResultObj;
}
//---------------------------------------------------------------------------
PyObject* ConvertException()
{
  try
  {
    throw;
  }
  catch(Func32::EFuncError &E)
  {
    PyErr_SetString(PyEFuncError, AnsiString(GetErrorMsg(E)).c_str());
  }
  catch(Exception &E)
  {
    PyErr_SetString(PyVclException, AnsiString(E.Message).c_str());
  }
  catch(...)
  {
    PyErr_SetString(PyExc_RuntimeError, "Unknown exception");
	}
  return NULL;
}
//---------------------------------------------------------------------------
template<> Func32::TComplex FromPyObject<Func32::TComplex>(PyObject *O)
{
  Py_complex V = PyComplex_AsCComplex(O);
	return Func32::TComplex(V.real, V.imag);
}
//---------------------------------------------------------------------------
PyObject* ToPyObject(const Func32::TComplex &Value)
{
	return Value.imag() ? PyComplex_FromDoubles(Value.real(), Value.imag()) : PyFloat_FromDouble(Value.real());
}
//---------------------------------------------------------------------------
static int WriteToConsole(int Arg)
{
  std::pair<String, TColor> *Pair = (std::pair<String, TColor>*)Arg;
  if(Form22)
    Form22->WriteText(Pair->first, Pair->second);
  delete Pair;
  return 0;
}
//---------------------------------------------------------------------------
static PyObject* PluginWriteToConsole(PyObject *Self, PyObject *Args)
{
  const wchar_t *Str;
  TColor Color = clBlack;
  if(!PyArg_ParseTuple(Args, "u|i", &Str, &Color))
    return NULL;

  Py_BEGIN_ALLOW_THREADS

  if(GetCurrentThreadId() != MainThreadID)
  {
    std::pair<String,TColor> *Pair = new std::pair<String, TColor>(Str, Color);
		PostMessage(Form1->Handle, WM_USER+1, (DWORD)WriteToConsole, (DWORD)Pair);
  }
  else if(Form22)
    Form22->WriteText(Str, Color);
#ifdef _DEBUG
  static std::ofstream Log((ExtractFileDir(Application->ExeName) + "\\PyGraph.log").c_str());
  if(Log)
    Log << AnsiString(Str);
#endif
  Py_END_ALLOW_THREADS
  //This creates problems when used from OLE
//  Application->ProcessMessages();
  Py_RETURN_NONE;
}
//---------------------------------------------------------------------------
static PyObject* PluginClearConsole(PyObject *Self, PyObject *Args)
{
  Form22->Clear();
  Py_RETURN_NONE;
}
//---------------------------------------------------------------------------
static PyObject* PluginInputQuery(PyObject *Self, PyObject *Args)
{
  const wchar_t *Caption = L"Python input";
  const wchar_t *Prompt = L"";
  if(!PyArg_ParseTuple(Args, "|uu", &Caption, &Prompt))
    return NULL;
  String Value;
	if(InputQuery(Caption, Prompt, Value))
    return PyUnicode_FromWideChar(Value.c_str(), Value.Length());

  Py_RETURN_NONE;
}
//---------------------------------------------------------------------------
class TPluginFunc : public Func32::TBaseCustomFunc
{
  PyObject *Func;
  unsigned Arguments;
  std::wstring FuncName;
public:
  TPluginFunc(PyObject *AFunc, unsigned AArguments, const std::wstring &AFuncName)
    : Func(AFunc), Arguments(AArguments), FuncName(AFuncName) {Py_INCREF(Func);}
  ~TPluginFunc() {Py_XDECREF(Func);}
  unsigned ArgumentCount() const {return Arguments;}
  long double Call(const long double *Args, Func32::TTrigonometry Trig, Func32::TErrorCode &ErrorCode, std::wstring &ErrorStr) const
  {
    return CallCustomFunction(Args, Trig, ErrorCode, ErrorStr);
  }
  Func32::TComplex Call(const Func32::TComplex *Args, Func32::TTrigonometry Trig, Func32::TErrorCode &ErrorCode, std::wstring &ErrorStr) const
  {
    return CallCustomFunction(Args, Trig, ErrorCode, ErrorStr);
  }
  template<typename T>
  T TPluginFunc::CallCustomFunction(const T *Args, Func32::TTrigonometry Trig, Func32::TErrorCode &ErrorCode, std::wstring &ErrorStr) const;
  PyObject* GetFunc() {return Func;}
};
//---------------------------------------------------------------------------
template<typename T>
T TPluginFunc::CallCustomFunction(const T *Args, Func32::TTrigonometry Trig, Func32::TErrorCode &ErrorCode, std::wstring &ErrorStr) const
{
  TLockGIL Dummy;
  PyObject *Tuple = PyTuple_New(Arguments);
  for(unsigned I = 0; I < Arguments; I++)
    PyTuple_SET_ITEM(Tuple, I, ToPyObject(Args[I]));
  PyObject *CallResult = PyObject_CallObject(Func, Tuple);
  T Result = T();
  if(CallResult == Py_None)
    ErrorCode = Func32::ecExtFuncError;
  else
  {
    if(CallResult != NULL)
      Result = FromPyObject<T>(CallResult);
    if(PyErr_Occurred())
    {
      PyObject *Type, *Value, *Traceback;
      PyErr_Fetch(&Type, &Value, &Traceback);
      if(PyUnicode_Check(Value))
        ErrorStr = FuncName + L": " + PyUnicode_AsUnicode(Value);
      ErrorCode = Func32::ecExtFuncError;

      Py_XDECREF(Type);
      Py_XDECREF(Value);
      Py_XDECREF(Traceback);
    }
	}
  Py_XDECREF(CallResult);
  Py_XDECREF(Tuple);
  return Result;
}
//---------------------------------------------------------------------------
int GetFunctionArgumentCount(PyObject *Function)
{
  int Arguments = -1;
  PyObject *FuncCode = PyObject_GetAttrString(Function, "__code__");
  if(FuncCode)
  {
    PyObject *ArgCount = PyObject_GetAttrString(FuncCode, "co_argcount");
    if(ArgCount)
    {
      Arguments = PyLong_AsLong(ArgCount);
      Py_XDECREF(ArgCount);
    }
    Py_XDECREF(FuncCode);
  }
  return Arguments;
}
//---------------------------------------------------------------------------
static PyObject* PluginSetCustomFunction(PyObject *Self, PyObject *Args)
{
  try
  {
    //The number of arguments are in func_code.co_argcount
    const wchar_t *Name;
    PyObject *Value;
    if(!PyArg_ParseTuple(Args, "uO", &Name, &Value))
      return NULL;

    Func32::TComplex ComplexValue(PyComplex_RealAsDouble(Value), PyComplex_ImagAsDouble(Value));
    if(!PyErr_Occurred())
    {
      Form1->Data.CustomFunctions.SymbolList.Add(Name, ComplexValue);
      Form1->Data.CustomFunctions.GlobalSymbolList.Add(Name, ComplexValue);
    }
    else
    {
      PyErr_Clear();
      int Arguments = GetFunctionArgumentCount(Value);
      if(Arguments != -1)
      {
        boost::shared_ptr<TPluginFunc> Func(new TPluginFunc(Value, Arguments, Name));
        Form1->Data.CustomFunctions.SymbolList.Add(Name, Func);
        Form1->Data.CustomFunctions.GlobalSymbolList.Add(Name, Func);
      }
    }
    Py_RETURN_NONE;
  }
  catch(Func32::EFuncError &E)
  {
    PyErr_SetString(PyEFuncError, ToString(GetErrorMsg(E)).c_str());
    return NULL;
  }
}
//---------------------------------------------------------------------------
static PyObject* PluginGetCustomFunction(PyObject *Self, PyObject *Args)
{
  const wchar_t *Name = PyUnicode_AsUnicode(Args);
  if(Name == NULL)
    return NULL;

  if(!Form1->Data.CustomFunctions.GlobalSymbolList.Exists(Name))
  {
    PyErr_SetString(PyExc_KeyError, AnsiString(Name).c_str());
    return NULL;
  }

  Func32::TBaseCustomFuncPtr Func = Form1->Data.CustomFunctions.GlobalSymbolList.Get(Name);
  boost::shared_ptr<TPluginFunc> PluginFunc = boost::dynamic_pointer_cast<TPluginFunc>(Func);
  if(PluginFunc)
  {
    PyObject *Obj = PluginFunc->GetFunc();
    Py_INCREF(Obj);
    return Obj;
  }
  //We assume it is a constant if it is not a plugin function
  boost::shared_ptr<Func32::TCustomFunc> CustomFunc = boost::dynamic_pointer_cast<Func32::TCustomFunc>(Func);
  if(CustomFunc)
  {
    std::vector<Func32::TComplex> Args;
    Func32::TComplex Value = CustomFunc->Calc(Args);
    return imag(Value) ? PyComplex_FromDoubles(real(Value), imag(Value)) : PyFloat_FromDouble(real(Value));
  }
  Py_RETURN_NONE;
}
//---------------------------------------------------------------------------
static PyObject* PluginDelCustomFunction(PyObject *Self, PyObject *Args)
{
  const wchar_t *Name = PyUnicode_AsUnicode(Args);
  if(Name == NULL)
		return NULL;

  if(!Form1->Data.CustomFunctions.GlobalSymbolList.Exists(Name))
  {
    PyErr_SetString(PyExc_KeyError, AnsiString(Name).c_str());
    return NULL;
  }

  Form1->Data.CustomFunctions.SymbolList.Erase(Name);
  Form1->Data.CustomFunctions.GlobalSymbolList.Erase(Name);
  Py_RETURN_NONE;
}
//---------------------------------------------------------------------------
static PyObject* PluginGetCustomFunctionNames(PyObject *Self, PyObject *Args)
{
  const Func32::TSymbolList &SymbolList = Form1->Data.CustomFunctions.GlobalSymbolList;
  Func32::TSymbolList::TConstIterator Begin = SymbolList.Begin();
  Func32::TSymbolList::TConstIterator End = SymbolList.End();
  PyObject *Names = PyList_New(SymbolList.Size());

  int Index = 0;
  for(Func32::TSymbolList::TConstIterator Iter = Begin; Iter != End; ++Iter, ++Index)
    PyList_SetItem(Names, Index, PyUnicode_FromWideChar(Iter->first.c_str(), Iter->first.size()));
  return Names;
}
//---------------------------------------------------------------------------
static PyObject* PluginEval(PyObject *Self, PyObject *Args, PyObject *Kwds)
{
  try
  {
		static char *kwlist[] = {"Expression", "Trigonometry", "Locals", NULL};
    int Degrees = Form1->Data.Axes.Trigonometry == Func32::Degree;
    PyObject *Locals = NULL;
    const wchar_t *Expression;
    if(!PyArg_ParseTupleAndKeywords(Args, Kwds, "u|iO!", kwlist, &Expression, &Degrees, &PyDict_Type, &Locals))
      return NULL;

    if(Locals != NULL)
    {
      Func32::TSymbolList SymbolList = Form1->Data.CustomFunctions.SymbolList;
      Py_ssize_t Pos = 0;
      PyObject *Key, *Value;
      while(PyDict_Next(Locals, &Pos, &Key, &Value))
      {
        std::wstring Name = PyUnicode_AsUnicode(Key);
        double FloatValue = PyFloat_AsDouble(Value);
        if(!PyErr_Occurred())
          SymbolList.Add(Name, FloatValue);
        else
        {
          PyErr_Clear();
          int Arguments = GetFunctionArgumentCount(Value);
          if(Arguments == -1)
            return NULL;

          boost::shared_ptr<TPluginFunc> Func(new TPluginFunc(Value, Arguments, Name));
          SymbolList.Add(Name, Func);
        }
      }
      return PyFloat_FromDouble(Func32::Eval(Expression, SymbolList, Degrees ? Func32::Degree : Func32::Radian));
    }

    return PyFloat_FromDouble(Func32::Eval(Expression, Form1->Data.CustomFunctions.SymbolList, Degrees ? Func32::Degree : Func32::Radian));
  }
  catch(Func32::EFuncError &E)
  {
    PyErr_SetString(PyEFuncError, ToString(GetErrorMsg(E)).c_str());
    return NULL;
  }
}
//---------------------------------------------------------------------------
static PyObject* PluginEvalComplex(PyObject *Self, PyObject *Args, PyObject *Kwds)
{
  try
  {
		static char *kwlist[] = {"Expression", "Trigonometry", "Locals", NULL};
    int Degrees = Form1->Data.Axes.Trigonometry == Func32::Degree;
    PyObject *Locals = NULL;
    const wchar_t *Expression;
    if(!PyArg_ParseTupleAndKeywords(Args, Kwds, "u|iO!", kwlist, &Expression, &Degrees, &PyDict_Type, &Locals))
      return NULL;

    Func32::TComplex Result;
    if(Locals != NULL)
    {
      Func32::TSymbolList SymbolList = Form1->Data.CustomFunctions.SymbolList;
      Py_ssize_t Pos = 0;
      PyObject *Key, *Value;
      while(PyDict_Next(Locals, &Pos, &Key, &Value))
      {
        std::wstring Name = PyUnicode_AsUnicode(Key);
        Func32::TComplex ComplexValue(PyComplex_RealAsDouble(Value), PyComplex_ImagAsDouble(Value));
        if(!PyErr_Occurred())
          SymbolList.Add(Name, ComplexValue);
        else
        {
          PyErr_Clear();
          int Arguments = GetFunctionArgumentCount(Value);
          if(Arguments == -1)
            return NULL;

          boost::shared_ptr<TPluginFunc> Func(new TPluginFunc(Value, Arguments, Name));
          SymbolList.Add(Name, Func);
        }
      }

      Result = Func32::EvalComplex(Expression, SymbolList, Degrees ? Func32::Degree : Func32::Radian);
    }
    else
      Result = Func32::EvalComplex(Expression, Form1->Data.CustomFunctions.SymbolList, Degrees ? Func32::Degree : Func32::Radian);
    return PyComplex_FromDoubles(real(Result), imag(Result));
  }
  catch(Func32::EFuncError &E)
  {
    PyErr_SetString(PyEFuncError, AnsiString(GetErrorMsg(E)).c_str());
    return NULL;
  }
}
//---------------------------------------------------------------------------
static PyObject* PluginUpdate(PyObject *Self, PyObject *Args)
{
  TUnlockGIL Dummy;
  Form1->Draw.AbortUpdate();
  Form1->Data.ClearCache();
  Form1->Data.Update();
  Form1->UpdateTreeView();
  Form1->Data.SetModified();
  Form1->Redraw(); //Activates thread; must be done after OLE update
  Py_RETURN_NONE;
}
//---------------------------------------------------------------------------
static PyObject* PluginGetConstantNames(PyObject *Self, PyObject *Args)
{
  const TCustomFunctions &Functions = Form1->Data.CustomFunctions;
  TCustomFunctions::TConstIterator Begin = Functions.Begin();
  TCustomFunctions::TConstIterator End = Functions.End();
  PyObject *ConstantNames = PyList_New(End - Begin);

  int Index = 0;
  for(TCustomFunctions::TConstIterator Iter = Begin; Iter != End; ++Iter, ++Index)
    PyList_SetItem(ConstantNames, Index, PyUnicode_FromWideChar(Iter->Name.c_str(), Iter->Name.size()));
  return ConstantNames;
}
//---------------------------------------------------------------------------
static PyObject* PluginGetConstant(PyObject *Self, PyObject *Args)
{
	const wchar_t *Name = PyUnicode_AsUnicode(Args);
  if(Name == NULL)
    return NULL;

  try
  {
    const TCustomFunction &Function = Form1->Data.CustomFunctions.GetValue(Name);
    PyObject *Args = PyTuple_New(Function.Arguments.size() + 1);
    PyTuple_SetItem(Args, 0, PyUnicode_FromWideChar(Function.Text.c_str(), Function.Text.size()));
    for(unsigned I = 0; I < Function.Arguments.size(); I++)
      PyTuple_SetItem(Args, I+1, PyUnicode_FromWideChar(Function.Arguments[I].c_str(), Function.Arguments[I].size()));
    return Args;
  }
  catch(ECustomFunctionError &E)
  {
    PyErr_SetString(PyExc_KeyError, AnsiString(Name).c_str());
    return NULL;
  }
}
//---------------------------------------------------------------------------
static PyObject* PluginSetConstant(PyObject *Self, PyObject *Args)
{
  try
  {
    PyObject *Arguments;
    const wchar_t *Text, *Name;
    if(!PyArg_ParseTuple(Args, "uOu", &Name, &Arguments, &Text))
      return NULL;
		TCustomFunctions &Functions = Form1->Data.CustomFunctions;
    std::vector<std::wstring> ArgList;
    if(Arguments != Py_None)
    {
      if(!PyTuple_Check(Arguments))
      {
        PyErr_SetString(PyExc_TypeError, "Function arguments must be a tuple or None");
        return NULL;
      }

      int Size = PyTuple_Size(Arguments);
      if(Size == -1)
        return NULL;
      for(int I = 0; I < Size; I++)
      {
        const wchar_t *Str = PyUnicode_AsUnicode(PyTuple_GetItem(Arguments, I));
        if(Str == NULL)
          return NULL;
        ArgList.push_back(Str);
      }
    }
    Functions.Add(Name, ArgList, Text);
    Py_RETURN_NONE;
  }
  catch(Func32::EFuncError &E)
  {
    PyErr_SetString(PyEFuncError, AnsiString(GetErrorMsg(E)).c_str());
    return NULL;
	}
}
//---------------------------------------------------------------------------
static PyObject* PluginDelConstant(PyObject *Self, PyObject *Args)
{
  const wchar_t *Name = PyUnicode_AsUnicode(Args);
  if(Name == NULL)
    return NULL;

  try
  {
    TCustomFunctions &Functions = Form1->Data.CustomFunctions;
    Functions.Delete(Name);
    Py_RETURN_NONE;
  }
  catch(ECustomFunctionError &E)
  {
    PyErr_SetString(PyExc_KeyError, AnsiString(Name).c_str());
    return NULL;
  }
}
//---------------------------------------------------------------------------
static PyObject* PluginSaveAsImage(PyObject *Self, PyObject *Args, PyObject *Kwds)
{
  try
  {
    TImageOptions Options(Form1->Image1->Width, Form1->Image1->Height);
		Options.LoadSettings();
		static char *kwlist[] = {"FileName", "FileType", "Width", "Height", NULL};

		const wchar_t *FileName = NULL;
		int FileType = -1, Width = -1, Height = -1;
		if(!PyArg_ParseTupleAndKeywords(Args, Kwds, "u|iii", kwlist, &FileName, &FileType, &Width, &Height))
      return NULL;

    if(Width == 0 || Height == 0)
      Options.UseCustomSize = false;
    else if(Width != -1 || Height != -1)
    {
      Options.UseCustomSize = true;
      Options.CustomWidth = Width;
      Options.CustomHeight = Height;
    }

		if(FileType != -1)
      SaveAsImage(FileName, FileType, Options);
    else
      SaveAsImage(FileName, Options);
    Py_RETURN_NONE;
  }
  catch(...)
  {
    ConvertException();
    return NULL;
  }
}
//---------------------------------------------------------------------------
static PyObject* PluginGetLanguageList(PyObject *Self, PyObject *Args)
{
  try
  {
    TStringList *List = new TStringList;
    GetLanguageList(List);
    return VclObject_Create(List, true);
  }
  catch(...)
  {
    return ConvertException();
  }
}
//---------------------------------------------------------------------------
PyMethodDef GraphMethods[] = {
	{"SetCustomFunction",         PluginSetCustomFunction, METH_VARARGS, ""},
	{"GetCustomFunction",         PluginGetCustomFunction, METH_O, ""},
	{"DelCustomFunction",         PluginDelCustomFunction, METH_O, ""},
	{"GetCustomFunctionNames",    PluginGetCustomFunctionNames, METH_NOARGS, ""},
	{"WriteToConsole",            PluginWriteToConsole, METH_VARARGS, ""},
	{"ClearConsole",              PluginClearConsole, METH_NOARGS, "Clear the Python console."},
	{"InputQuery",                PluginInputQuery, METH_VARARGS, ""},
	{"Eval",                      (PyCFunction)PluginEval, METH_VARARGS | METH_KEYWORDS, "Evaluates an expression and returns the result as a real number."},
	{"EvalComplex",               (PyCFunction)PluginEvalComplex, METH_VARARGS | METH_KEYWORDS, "Evaluates an expression and returns the result as a complex number."},
	{"Update",                    PluginUpdate, METH_NOARGS, "Updated the graphing area."},
	{"SetConstant",               PluginSetConstant, METH_VARARGS, ""},
	{"GetConstant",               PluginGetConstant, METH_O, ""},
	{"DelConstant",               PluginDelConstant, METH_O, ""},
	{"GetConstantNames",          PluginGetConstantNames, METH_NOARGS, ""},
	{"SaveAsImage",               (PyCFunction)PluginSaveAsImage, METH_VARARGS | METH_KEYWORDS, "Saves the graphing area to an image file."},
  {"GetLanguageList",           PluginGetLanguageList, METH_NOARGS, "Get list of detected languages."},
	{NULL, NULL, 0, NULL}
};
//---------------------------------------------------------------------------
static PyModuleDef GraphModuleDef =
{
  PyModuleDef_HEAD_INIT,
  "GraphImpl",
  NULL,
  -1,
  GraphMethods,
  NULL,
  NULL,
	NULL,
	NULL,
};
//---------------------------------------------------------------------------
void ShowPythonConsole(bool Visible)
{
	if(Form22)
		Form22->Visible = Visible;
}
//---------------------------------------------------------------------------
PyObject* InitGraphImpl()
{
	PyObject *GraphImpl = PyModule_Create(&GraphModuleDef);
	PyModule_AddObject(GraphImpl, "Form1", VclObject_Create(Form1, false));
	PyModule_AddObject(GraphImpl, "Form22", VclObject_Create(Form22, false));
	PyEFuncError = PyErr_NewException("Graph.EFuncError", NULL, NULL);
	PyEGraphError = PyErr_NewException("Graph.EGraphError", NULL, NULL);
	PyObject_SetAttrString(GraphImpl, "EFuncError", PyEFuncError);
	PyObject_SetAttrString(GraphImpl, "EGraphError", PyEGraphError);
	return GraphImpl;
}
//---------------------------------------------------------------------------
extern "C" PyObject* PyInit__Settings();
extern "C" PyObject* PyInit__Data();
extern "C" PyObject* PyInit__Utility();
_inittab Modules[] =
{
	{"GraphImpl", InitGraphImpl},
	{"vcl", InitPyVcl},
	{"_Settings", PyInit__Settings},
	{"_Data", PyInit__Data},
	{"_Utility", PyInit__Utility},
  {NULL, NULL}
};
//---------------------------------------------------------------------------
void InitPlugins()
{
  using namespace Python;
  SET_DEFAULT_FPU_MASK();
	if(IsPythonInstalled())
	{
		RegisterClass(__classid(TForm));

		Form22 = new TForm22(Application);
		Form1->ScriptDocAction->Visible = true;
 		SET_PYTHON_FPU_MASK(); //Set the FPU Control Word to what Python expects
		PyImport_ExtendInittab(Modules);
		static String ExeName = Application->ExeName; //Py_SetProgramName() requires variable to be static
		Py_SetProgramName(ExeName.c_str());
		Py_Initialize();
		PyEval_InitThreads();
		int argc;
		wchar_t **argv = CommandLineToArgvW(GetCommandLine(), &argc);
		PySys_SetArgv(argc, argv);
		LocalFree(argv);

		TVersionInfo Info;
		TVersion Version = Info.FileVersion();
		const char *BetaFinal = Info.FileFlags() & ffDebug ? "beta" : "final";
		AnsiString BaseDir = GetRegValue(REGISTRY_KEY, L"BaseDir", HKEY_CURRENT_USER, ExtractFileDir(Application->ExeName).c_str()).c_str();
    BaseDir = ReplaceStr(BaseDir, '\\', "\\\\");
		AnsiString PythonCommands = AnsiString().sprintf(
			"import sys\n"
			"import GraphImpl\n"
			"class ConsoleWriter:\n"
			"  def __init__(self, color):\n"
			"    self._color = color\n"
			"  def write(self, str):\n"
			"    GraphImpl.WriteToConsole(str, self._color)\n"
			"  def readline(self):\n"
			"    value = GraphImpl.InputQuery()\n"
			"    if value == None: raise KeyboardInterrupt('operation cancelled')\n"
			"    return value + '\\n'\n"
      "  def flush(self):\n"
      "    pass\n"

			"sys.stdout = ConsoleWriter(0)\n"
			"sys.stderr = ConsoleWriter(0xFF)\n"
			"sys.stdin = sys.stdout\n"

			"GraphImpl.version_info = (%d,%d,%d,%d,'%s')\n"
#ifdef _DEBUG
      "GraphImpl.Debug = True\n"
#else
      "GraphImpl.Debug = False\n"
#endif
			"sys.path.append('%s\\Lib')\n"
			"import vcl\n"
			"import Graph\n"
      "from imp import reload\n"
			"Graph.InitPlugins('%s')\n"
			"sys.stdin = sys.stdout\n"
			, Version.Major, Version.Minor, Version.Release, Version.Build, BetaFinal
			, BaseDir.c_str()
			, BaseDir.c_str()
		);

		int Result = PyRun_SimpleString(PythonCommands.c_str());
		PyEval_SaveThread();
    SET_DEFAULT_FPU_MASK();
		PythonInitialized = true;
	}
	else
	{
		Form1->Panel6->Height = 0;
		Form1->Splitter2->Visible = false;
	}
}
//---------------------------------------------------------------------------
void UnloadPlugin()
{
	if(PythonInitialized)
	{
    SET_PYTHON_FPU_MASK();
		PyGILState_Ensure();
		Py_Finalize();
    SET_DEFAULT_FPU_MASK();
	}
}
//---------------------------------------------------------------------------

}

