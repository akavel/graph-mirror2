/* Graph (http://sourceforge.net/projects/graph)
 * Copyright 2007 Ivan Johansen
 *
 * Graph is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 */
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "CompilerWorkaround.h"
#include <fstream>
#include <stdio.h>
//---------------------------------------------------------------------------
/* Workaround for problem with support for Unicode filenames in std::fstream in CB2009.
 * It looks like the file fiopen.cpp is not compiled for Unicode support.
 * The following code is copied from Source\cpprtl\Source\Dinkumware\source\fiopen.cpp for wchar_t* instead of char*
 */
#undef _SYSCH
#define _SYSCH(x) L##x
namespace std
{
FILE *_Fiopen(const wchar_t *filename,
	ios_base::openmode mode, int)	// protection currently unused
	{	// open a file with native name
	static const wchar_t *mods[] =
		{	// fopen mode strings corresponding to valid[i]
		_SYSCH("r"), _SYSCH("w"), _SYSCH("w"), _SYSCH("a"),
		_SYSCH("rb"), _SYSCH("wb"), _SYSCH("wb"), _SYSCH("ab"),
		_SYSCH("r+"), _SYSCH("w+"), _SYSCH("a+"),
		_SYSCH("r+b"), _SYSCH("w+b"), _SYSCH("a+b"),
		0};

	static const int valid[] =
		{	// valid combinations of open flags
		ios_base::in,
		ios_base::out,
		ios_base::out | ios_base::trunc,
		ios_base::out | ios_base::app,
		ios_base::in | ios_base::binary,
		ios_base::out | ios_base::binary,
		ios_base::out | ios_base::trunc | ios_base::binary,
		ios_base::out | ios_base::app | ios_base::binary,
		ios_base::in | ios_base::out,
		ios_base::in | ios_base::out | ios_base::trunc,
		ios_base::in | ios_base::out | ios_base::app,
		ios_base::in | ios_base::out | ios_base::binary,
		ios_base::in | ios_base::out | ios_base::trunc
			| ios_base::binary,
		ios_base::in | ios_base::out | ios_base::app
			| ios_base::binary,
		0};

	FILE *fp = 0;
	int n;
	ios_base::openmode atendflag = mode & ios_base::ate;
	ios_base::openmode norepflag = mode & ios_base::_Noreplace;

	if (mode & ios_base::_Nocreate)
		mode |= ios_base::in;	// file must exist
	if (mode & ios_base::app)
		mode |= ios_base::out;	// extension -- app implies out

	mode &= ~(ios_base::ate | ios_base::_Nocreate | ios_base::_Noreplace);
	for (n = 0; valid[n] != 0 && valid[n] != mode; ++n)
		;	// look for a valid mode

	if (valid[n] == 0)
		return (0);	// no valid mode
	else if (norepflag && mode & (ios_base::out || ios_base::app)
		&& (fp = _wfopen(filename, _SYSCH("r"))) != 0)
		{	// file must not exist, close and fail
		fclose(fp);
		return (0);
		}
	else if (fp != 0 && fclose(fp) != 0)
		return (0);	// can't close after test open
// should open with protection here, if other than default
	else if ((fp = _wfopen(filename, mods[n])) == 0)
		return (0);	// open failed

	if (!atendflag || fseek(fp, 0, SEEK_END) == 0)
		return (fp);	// no need to seek to end, or seek succeeded

	fclose(fp);	// can't position at end
	return (0);
		}
}
//---------------------------------------------------------------------------
//Workaround for bug in C++ Builder 2009 where TPen::SetHandle() is broken
namespace Graphics
{
  void __fastcall TPen::SetHandle(HPEN Value)
  {
    TPenData PenData = {
      Value,
      clBlack,
      1,
      psSolid};
    SetData(PenData);
  }
}
//---------------------------------------------------------------------------

