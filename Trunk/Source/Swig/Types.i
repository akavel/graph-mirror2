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

%typemap(in) Func32::TDblPoint  {
  if(!PyArg_ParseTuple($input, "dd", &($1.x), &($1.y)))
    SWIG_fail;
}

%typemap(out) Func32::TDblPoint {
  $result = Py_BuildValue("dd", $1.x, $1.y);
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

%typemap(out) const boost::shared_ptr<TGraphElem>&
{
  $result = DownCastSharedPtr(*$1);
}

%typemap(out) Func32::TCoord<long double> {
  $result = Py_BuildValue("dd", (double)$1.x, (double)$1.y);
}

%typemap(out) std::pair<double,double> {
  $result = Py_BuildValue("dd", $1.first, $1.second);
}

%apply double {long double};
typedef unsigned TColor;

