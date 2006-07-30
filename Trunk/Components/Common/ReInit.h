//===========================================================================
// Copyright © 2004 Ivan Johansen
// VclObject.h
//===========================================================================
#ifndef ReInitH
#define ReInitH
//---------------------------------------------------------------------------
HINSTANCE LoadNewResourceModule(LCID Locale);
HINSTANCE LoadNewResourceModule(const AnsiString &Ext);
HINSTANCE SetResourceHInstance(HINSTANCE NewInstance, bool FreeOldInstance = true);
void ReinitializeForms();
TComponent* ReloadComponent(TForm *Form, TComponent *Comp);
template<typename T> void RecreateComponent(TForm *Form, T *&Comp)
{
  Comp = static_cast<T*>(ReloadComponent(Form, Comp));
}
#endif
