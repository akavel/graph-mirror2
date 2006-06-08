Graph is an appliation used for drawing functions in a coordinate system. The program is licensed under the GNU Gneral Public License. This pacakge contains the following parts necesarry to rebuild the application:

Graph
The program itself written by Ivan Johansen in Borland C++ Builder. The GPL license (see COPYING) covers all files found in the Graph catalog and subcatalogs. 

Graph\Help
The help files in English and translations of the English version.

Graph\Locale
Language translations in po format.

Components
C++ Builder components written by Ivan Johansen specific for use in Graph but can be used in any application. See COPYING for license.

Func32
Library for handling mathematical functions written by Ivan Johansen specific for use in Graph, though the library should be portable to any C++ compiler. Distributed under GNU GPL (See COPYING).

Thumbnail Handler
Shell extension for showing thumbnails of grf files. This is written in Delphi and uses the EasyNSE library. It is basically just an example that follows EasyNSE modified to show grf files.

The following third party libraries are needed to build Graph:

BMGLib
BMGLib (http://members.cox.net/scottheiman/bmglib.htm) is used for writing PNG files. See PNG.H for license.

Boost
Boost (http://boost.org) is used as a general purpose library collection.

Spirit
Spirit (http://spirit.sf.net) is a paraser library that is part of Boost (http://boost.org). It is necesarry to compiler Func32. Notice that Spirit 1.6.1 is used because newer versions doesn't work with bcc.

dxgettext
dxgettext (http://dybdahl.dk/dxgettext), which is an improved version of GNU gettext for Delphi/BCB, is used as translation library.

EasyNSE
EasyNSE (http://www.mustangpeak.net/) is used for shell extensions. 

PDFlib-Lite
PDFlib-Lite (http://www.pdflib.com) is used for generating PDF documents.

TNT Unicode Controls
The Tnt Delphi Unicode Controls (http://www.tntware.com/delphicontrols/unicode/) are used for unicode support in the user interface.


The program can be compiled with Borland C++ Builder 6 Pro/Enterprise. To compile with other versions some changes to the code is probably requeried.
 