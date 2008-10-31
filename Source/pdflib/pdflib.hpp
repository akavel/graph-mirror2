/*---------------------------------------------------------------------------*
 |              PDFlib - A library for generating PDF on the fly             |
 +---------------------------------------------------------------------------+
 | Copyright (c) 1997-2006 Thomas Merz and PDFlib GmbH. All rights reserved. |
 +---------------------------------------------------------------------------+
 |                                                                           |
 |    This software is subject to the PDFlib license. It is NOT in the       |
 |    public domain. Extended versions and commercial licenses are           |
 |    available, please check http://www.pdflib.com.                         |
 |                                                                           |
 *---------------------------------------------------------------------------*/

// $Id: pdflib.hpp,v 1.55 2006/09/16 20:01:57 tm Exp $
//
// in sync with pdflib.h 1.151.2.22
//
// C++ wrapper for PDFlib
//
//

#ifndef PDFLIB_HPP
#define PDFLIB_HPP

#if _MSC_VER == 1310    /* VS .NET 2003 */
/* Visual Studio 2003 is not yet 100% C++ Standard compatible, so we ignore
 * this */
#pragma warning(disable: 4290)
#endif
#include <string>

// We use PDF as a C++ class name, therefore hide the actual C struct
// name for PDFlib usage with C++.

#include "pdflib.h"

// The C++ class wrapper for PDFlib

class PDFlib {
public:
    class Exception
    {
    public:
	Exception(std::string errmsg, int errnum, std::string apiname, 
	    void *opaque);
	std::string get_errmsg();
	int get_errnum();
	std::string get_apiname();
	const void *get_opaque();
    private:
	std::string m_errmsg;
	int m_errnum;
	std::string m_apiname;
	void * m_opaque;
    }; // Exception

    PDFlib(allocproc_t allocproc = NULL,
	reallocproc_t reallocproc = NULL,
	freeproc_t freeproc = NULL,
	void *opaque = NULL) throw(Exception);

    ~PDFlib() throw();

    void activate_item(int id) throw(PDFlib::Exception);
    int add_bookmark(std::string text, int parent, int p_open)
    	throw(PDFlib::Exception);
    void add_launchlink(double llx, double lly, double urx, double ury,
	std::string filename) throw(PDFlib::Exception);
    void add_locallink(double llx, double lly, double urx, double ury, int page,
	std::string optlist) throw(PDFlib::Exception);
    void add_nameddest(std::string name, std::string optlist) 
	throw(PDFlib::Exception);
    void add_note(double llx, double lly, double urx, double ury,
    	std::string contents,
	std::string title, std::string icon, int p_open)
	throw(PDFlib::Exception);
    void add_pdflink(double llx, double lly, double urx, double ury,
	std::string filename, int page, std::string optlist)
	    throw(PDFlib::Exception);
    int add_table_cell(int table, int column, int row, std::string text,
	std::string optlist) throw(PDFlib::Exception);
    int add_textflow(int textflow, std::string text, std::string optlist)
	throw(PDFlib::Exception);
    void add_thumbnail(int image) throw(PDFlib::Exception);
    void add_weblink(double llx, double lly, double urx, double ury,
	std::string url) throw(PDFlib::Exception);
    void arc(double x, double y, double r, double alpha, double beta)
	throw(PDFlib::Exception);
    void arcn(double x, double y, double r, double alpha, double beta)
	throw(PDFlib::Exception);
    void attach_file(double llx, double lly, double urx, double ury,
	std::string filename, std::string description, std::string author,
	std::string mimetype, std::string icon) throw(PDFlib::Exception);
    int begin_document(std::string filename, std::string optlist)
	throw(PDFlib::Exception);
    void begin_document_callback(writeproc_t writeproc, std::string optlist)
	throw(PDFlib::Exception);
    void begin_font(std::string fontname, double a, double b,
	double c, double d, double e, double f, std::string optlist)
	throw(PDFlib::Exception);
    void begin_glyph(std::string glyphname, double wx, double llx, double lly,
	double urx, double ury) throw(PDFlib::Exception);
    int begin_item(std::string tag, std::string optlist)
	throw(PDFlib::Exception);
    void begin_layer(int layer) throw(PDFlib::Exception);
    void begin_page(double width, double height) throw(PDFlib::Exception);
    void begin_page_ext(double width, double height, std::string optlist)
	throw(PDFlib::Exception);
    int begin_pattern(double width, double height, double xstep, double ystep,
	int painttype) throw(PDFlib::Exception);
    int begin_template(double width, double height) throw(PDFlib::Exception);
    int begin_template_ext(double width, double height, std::string optlist)
	throw(PDFlib::Exception);
    void circle(double x, double y, double r) throw(PDFlib::Exception);
    void clip() throw(PDFlib::Exception);
    void close() throw(PDFlib::Exception);
    void close_image(int image) throw(PDFlib::Exception);
    void close_pdi(int doc) throw(PDFlib::Exception);
    void close_pdi_document(int doc) throw(PDFlib::Exception);
    void close_pdi_page(int page) throw(PDFlib::Exception);
    void closepath() throw(PDFlib::Exception);
    void closepath_fill_stroke() throw(PDFlib::Exception);
    void closepath_stroke() throw(PDFlib::Exception);
    void concat(double a, double b, double c, double d, double e, double f)
	throw(PDFlib::Exception);
    void continue_text(std::string text) throw(PDFlib::Exception);
    int create_3dview(std::string username, std::string optlist)
	throw(PDFlib::Exception);
    int create_action(std::string type, std::string optlist)
	throw(PDFlib::Exception);
    void create_annotation(double llx, double lly, double urx, double ury,
	std::string type, std::string optlist) throw(PDFlib::Exception);
    int create_bookmark(std::string text, std::string optlist)
	throw(PDFlib::Exception);
    void create_field(double llx, double lly, double urx, double ury,
	std::string name, std::string type, std::string optlist)
	throw(PDFlib::Exception);
    void create_fieldgroup(std::string name, std::string optlist)
    	throw(PDFlib::Exception);
    int create_gstate (std::string optlist) throw(PDFlib::Exception);
    void create_pvf(std::string filename, const void *data, size_t size,
    	std::string optlist) throw(PDFlib::Exception);
    int create_textflow(std::string text, std::string optlist)
	throw(PDFlib::Exception);
    void curveto(double x1, double y1, double x2, double y2, double x3,
    	double y3) throw(PDFlib::Exception);
    int define_layer(std::string name, std::string optlist)
	throw(PDFlib::Exception);
    int delete_pvf(std::string filename) throw(PDFlib::Exception);
    void delete_table(int table, std::string optlist) throw(PDFlib::Exception);
    void delete_textflow(int textflow) throw(PDFlib::Exception);
    void encoding_set_char(std::string encoding, int slot,
	std::string glyphname, int uv) throw(PDFlib::Exception);
    void end_document(std::string optlist) throw(PDFlib::Exception);
    void end_font() throw(PDFlib::Exception);
    void end_glyph() throw(PDFlib::Exception);
    void end_item(int id) throw(PDFlib::Exception);
    void end_layer() throw(PDFlib::Exception);
    void end_page() throw(PDFlib::Exception);
    void end_page_ext(std::string optlist) throw(PDFlib::Exception);
    void end_pattern() throw(PDFlib::Exception);
    void end_template() throw(PDFlib::Exception);
    void endpath() throw(PDFlib::Exception);
    void fill() throw(PDFlib::Exception);
    int fill_imageblock(int page, std::string blockname, int image,
	std::string optlist) throw(PDFlib::Exception);
    int fill_pdfblock(int page, std::string blockname, int contents,
	std::string optlist)
	throw(PDFlib::Exception);
    int fill_textblock(int page, std::string blockname, std::string text,
	std::string optlist) throw(PDFlib::Exception);
    void fill_stroke() throw(PDFlib::Exception);
    int findfont(std::string fontname, std::string encoding, int embed)
    	throw(PDFlib::Exception);
    void fit_image (int image, double x, double y, std::string optlist)
	throw(PDFlib::Exception);
    void fit_pdi_page (int page, double x, double y, std::string optlist)
	throw(PDFlib::Exception);
    std::string fit_table(int table, double llx, double lly, double urx,
	double ury, std::string optlist) throw(PDFlib::Exception);
    std::string fit_textflow(int textflow, double llx, double lly, double urx,
	double ury, std::string optlist) throw(PDFlib::Exception);
    void fit_textline(std::string text, double x, double y, std::string optlist)
	throw(PDFlib::Exception);
    std::string get_apiname() throw(PDFlib::Exception);
    const char * get_buffer(long *size) throw(PDFlib::Exception);
    std::string get_errmsg() throw(PDFlib::Exception);
    int get_errnum() throw(PDFlib::Exception);
    void * get_opaque() throw(PDFlib::Exception);
    std::string get_parameter(std::string key, double modifier)
	throw(PDFlib::Exception);
    double get_pdi_value(std::string key, int doc, int page, int reserved)
	throw(PDFlib::Exception);
    std::string get_pdi_parameter(std::string key, int doc, int page,
	int reserved, int *len = NULL) throw(PDFlib::Exception);
    double get_value(std::string key, double modifier) throw(PDFlib::Exception);
    double info_font(int font, std::string keyword, std::string optlist)
	throw(PDFlib::Exception);
    double info_matchbox(std::string boxname, int num, std::string keyword)
	throw(PDFlib::Exception);
    double info_table(int table, std::string keyword) throw(PDFlib::Exception);
    double info_textflow(int textflow, std::string keyword)
	throw(PDFlib::Exception);
    double info_textline(std::string text, std::string keyword,
	std::string optlist) throw(PDFlib::Exception);
    void initgraphics() throw(PDFlib::Exception);
    void lineto(double x, double y) throw(PDFlib::Exception);
    int load_3ddata(std::string filename, std::string optlist)
	throw(PDFlib::Exception);
    int load_font(std::string fontname, std::string encoding,
	std::string optlist) throw(PDFlib::Exception);
    int load_iccprofile(std::string profilename, std::string optlist)
	throw(PDFlib::Exception);
    int load_image (std::string imagetype, std::string filename,
	std::string optlist) throw(PDFlib::Exception);
    int makespotcolor(std::string spotname) throw(PDFlib::Exception);
    void moveto(double x, double y) throw(PDFlib::Exception);
    int open_CCITT(std::string filename, int width, int height, int BitReverse,
	int K, int BlackIs1) throw(PDFlib::Exception);
    int open_file(std::string filename) throw(PDFlib::Exception);
    int open_image(std::string imagetype, std::string source, const char *data,
	long len, int width, int height, int components, int bpc,
	std::string params) throw(PDFlib::Exception);
    int open_image_file(std::string imagetype, std::string filename,
	std::string stringparam, int intparam) throw(PDFlib::Exception);
    void open_mem(writeproc_t writeproc) throw(PDFlib::Exception);
    int open_pdi(std::string filename, std::string optlist, int reserved)
    	throw(PDFlib::Exception);
    int open_pdi_document(std::string filename, std::string optlist)
    	throw(PDFlib::Exception);
    int open_pdi_page(int doc, int pagenumber, std::string optlist)
    	throw(PDFlib::Exception);
    double pcos_get_number(int doc, std::string path) throw(PDFlib::Exception);
    const std::string pcos_get_string(int doc, std::string path)
	throw(PDFlib::Exception);
    const unsigned char * pcos_get_stream(int doc, int *length,
	std::string optlist, std::string path) throw(PDFlib::Exception);
    void place_image(int image, double x, double y, double p_scale)
	throw(PDFlib::Exception);
    void place_pdi_page(int page, double x, double y, double sx, double sy)
	throw(PDFlib::Exception);
    int process_pdi(int doc, int page, std::string optlist)
	throw(PDFlib::Exception);
    void rect(double x, double y, double width, double height)
    	throw(PDFlib::Exception);
    void restore() throw(PDFlib::Exception);
    void resume_page(std::string optlist) throw(PDFlib::Exception);
    void rotate(double phi) throw(PDFlib::Exception);
    void save() throw(PDFlib::Exception);
    void scale(double sx, double sy) throw(PDFlib::Exception);
    void set_border_color(double red, double green, double blue)
    	throw(PDFlib::Exception);
    void set_border_dash(double b, double w) throw(PDFlib::Exception);
    void set_border_style(std::string style, double width)
	throw(PDFlib::Exception);
    void setfont(int font, double fontsize) throw(PDFlib::Exception);
    void set_gstate(int gstate) throw(PDFlib::Exception);
    void set_info(std::string key, std::string value) throw(PDFlib::Exception);
    void set_layer_dependency(std::string type, std::string optlist)
    	throw(PDFlib::Exception);
    void set_parameter(std::string key, std::string value)
	throw(PDFlib::Exception);
    void set_text_pos(double x, double y) throw(PDFlib::Exception);
    void set_value(std::string key, double value) throw(PDFlib::Exception);
    void setcolor(std::string fstype, std::string colorspace,
	double c1, double c2, double c3, double c4) throw(PDFlib::Exception);
    void setdash(double b, double w) throw(PDFlib::Exception);
    void setdashpattern(std::string optlist) throw(PDFlib::Exception);
    void setflat(double flatness) throw(PDFlib::Exception);
    void setlinecap(int linecap) throw(PDFlib::Exception);
    void setlinejoin(int linejoin) throw(PDFlib::Exception);
    void setlinewidth(double width) throw(PDFlib::Exception);
    void setmatrix( double a, double b, double c, double d, double e, double f)
	throw(PDFlib::Exception);
    void setmiterlimit(double miter) throw(PDFlib::Exception);
    void setpolydash(float *darray, int length) throw(PDFlib::Exception);
    int shading (std::string shtype, double x0, double y0, double x1, double y1,
	double c1, double c2, double c3, double c4, std::string optlist)
	throw(PDFlib::Exception);
    int shading_pattern (int shade, std::string optlist)
	throw(PDFlib::Exception);
    void shfill (int shade) throw(PDFlib::Exception);
    void show(std::string text) throw(PDFlib::Exception);
    int show_boxed(std::string text, double left, double top,
	double width, double height, std::string hmode, std::string feature)
	throw(PDFlib::Exception);
    void show_xy(std::string text, double x, double y) throw(PDFlib::Exception);
    void skew(double alpha, double beta) throw(PDFlib::Exception);
    double stringwidth(std::string text, int font, double fontsize)
    	throw(PDFlib::Exception);
    void stroke() throw(PDFlib::Exception);
    void suspend_page(std::string optlist) throw(PDFlib::Exception);
    void translate(double tx, double ty) throw(PDFlib::Exception);
    std::string utf16_to_utf8(std::string utf16string) throw(PDFlib::Exception);
    std::string utf8_to_utf16(std::string utf8string, std::string format)
    	throw(PDFlib::Exception);
    std::string utf32_to_utf16(std::string utf32string, std::string ordering)
	throw(PDFlib::Exception);

    void xshow(std::string text, const double *xadvancelist)
    	throw(PDFlib::Exception);

private:
    PDF *p;
    const PDFlib_api *m_PDFlib_api;
};

#endif	// PDFLIB_HPP
