%typemap(in) TPoint  {
  if(!PyArg_ParseTuple($input, "ii", &($1.x), &($1.y)))
    SWIG_fail;
}

%typemap(out) TPoint {
  $result = Py_BuildValue("ii", $1.x, $1.y);
}

%typemap(in) const TPoint& {
  if(!PyArg_ParseTuple($input, "ii", &($1->x), &($1->y)))
    SWIG_fail;
}

%typemap(out) const TPoint& {
  $result = Py_BuildValue("ii", $1->x, $1->y);
}

%typemap(out) const TRect& {
  $result = Py_BuildValue("iiii", $1->Left, $1->Top, $1->Right, $1->Bottom);
}

%typemap(in) TTextValue  {
  $1 = TTextValue(PyFloat_AsDouble($input));
  if(PyErr_Occurred() != NULL)
    SWIG_fail;
}

%typemap(out) TTextValue {
  $result = PyFloat_FromDouble($1.Value);
}

%typemap(out) TFont* {
  $result = PyInt_FromLong((long)$1);
}

%typemap(out) boost::shared_ptr<TGraphElem>
{
  $result = DownCastSharedPtr($1);
}

%typemap(out) const boost::shared_ptr<TGraphElem>&
{
  $result = DownCastSharedPtr(*$1);
}
%typemap(out) boost::shared_ptr<TBaseFuncType> = boost::shared_ptr<TGraphElem>;

%typemap(out) Func32::TCoord<long double> {
  $result = Py_BuildValue("dd", (double)$1.x, (double)$1.y);
}

%typemap(out) const Func32::TCoordSet& {
  $result = Py_BuildValue("ddd", (double)$1->t, (double)$1->x, (double)$1->y);
}

%typemap(out) std::pair<double,double>* {
  $result = Py_BuildValue("dd", $1->first, $1->second);
}

%define TUPLE(StructName, First, Second)
%typemap(out) StructName
{
  $result = Py_BuildValue("NN", Python::ToPyObject($1.First), Python::ToPyObject($1.Second));
}

%typemap(out) const StructName&
{
  $result = Py_BuildValue("NN", Python::ToPyObject($1->First), Python::ToPyObject($1->Second));
}

%typemap(in) StructName
{
  PyObject *O1=NULL, *O2=NULL;
  if(!PyArg_ParseTuple($input, "OO", &O1, &O2))
    SWIG_fail;
  if(!Python::FromPyObject(O1, $1.First) || !Python::FromPyObject(O2, $1.Second))
      SWIG_fail;
}
%typemap(in) const StructName&
{
  PyObject *O1=NULL, *O2=NULL;
  if(!PyArg_ParseTuple($input, "OO", &O1, &O2))
    SWIG_fail;
  $*1_ltype Temp;
  if(!Python::FromPyObject(O1, Temp.First) || !Python::FromPyObject(O2, Temp.Second))
      SWIG_fail;
  $1 = &Temp;
}
%enddef


TUPLE(TPointSeriesPoint, First, Second)
TUPLE(Func32::TDblPoint, x, y)

%apply double {long double};
typedef unsigned TColor;
typedef unsigned TBrushStyle;

