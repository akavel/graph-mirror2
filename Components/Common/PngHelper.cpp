/* Graph (http://sourceforge.net/projects/graph)
 * Copyright 2006 Ivan Johansen
 *
 * Graph is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 */
//---------------------------------------------------------------------------
#pragma hdrstop
#include "Config.h"
#include "BMGLibPNG.h"
#include "PngHelper.h"
#include "png.h"
#include <vector>
extern "C"
{
#include "BMGUtils.h"
}
//---------------------------------------------------------------------------
struct TPngResources
{
  png_struct *png_ptr;
  png_info *info_ptr;

  TPngResources() : png_ptr(NULL), info_ptr(NULL) {}
  ~TPngResources()
  {
    png_destroy_info_struct(png_ptr, &info_ptr);
    png_destroy_write_struct(&png_ptr, NULL);
  }
};
//---------------------------------------------------------------------------
void FlushStream(png_structp png_ptr)
{
  std::ostream &Stream = *static_cast<std::ostream*>(png_ptr->io_ptr);
  Stream.flush();
}
//---------------------------------------------------------------------------
void WriteToStream(png_structp png_ptr, png_bytep Data, png_size_t Size)
{
  std::ostream &Stream = *static_cast<std::ostream*>(png_ptr->io_ptr);
  Stream.write(reinterpret_cast<char*>(Data), Size);
  if(!Stream)
    png_error(png_ptr, "Error writing to stream!");
}
//---------------------------------------------------------------------------
BMGError WriteStreamPNG(std::ostream &Stream, BMGImageStruct img)
{
	SetLastBMGError(BMG_OK);

	/* 16 BPP DIBS do not have palettes.  libPNG expects 16 BPP images to have
	   a palette.  To correct this situation we must convert 16 BPP images to
	   24 BPP images before saving the data to the file */
	if(img.bits_per_pixel == 16)
	{
    BMGError tmp = Convert16to24(&img);
    SetLastBMGError(tmp);
		if(tmp != BMG_OK )
      return tmp;
	}

  int HasPalette = img.bits_per_pixel <= 8;
  int GrayScale = 0;
  int NumColors = 0;
  if(HasPalette)
  {
    NumColors = img.palette_size;
    /* if this is a grayscale image then set the flag and delete the
          palette*/
    int i = 0;
    unsigned char *bits = img.palette;
    while(i < NumColors && bits[0] == bits[1] && bits[0] == bits[2])
    {
      i++;
      bits += img.bytes_per_palette_entry;
    }
    GrayScale = i == NumColors;
  }

  /* dimensions */
  int DIBScanWidth = (img.width * img.bits_per_pixel + 7) / 8;

  /* create the png pointer */
  TPngResources PngResources;
  PngResources.png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if(!PngResources.png_ptr)
  {
    SetLastBMGError(errMemoryAllocation);
    return errMemoryAllocation;
  }

  /* create the info pointer */
  PngResources.info_ptr = png_create_info_struct(PngResources.png_ptr);
  if(!PngResources.info_ptr)
  {
    SetLastBMGError(errMemoryAllocation);
    return errMemoryAllocation;
  }

  /* setup the output control */
  png_set_write_fn(PngResources.png_ptr, &Stream, WriteToStream, FlushStream);

  /* prepare variables needed to create PNG header */
  int BitDepth = img.bits_per_pixel < 8 ? img.bits_per_pixel : 8;

  /* determine color type */
  int ColorType;
  if(GrayScale)
    ColorType = PNG_COLOR_TYPE_GRAY;
  else if(img.bits_per_pixel == 32)
    ColorType = PNG_COLOR_TYPE_RGB_ALPHA;
  else if(img.bits_per_pixel == 24)
    ColorType = PNG_COLOR_TYPE_RGB;
  else
    ColorType = PNG_COLOR_TYPE_PALETTE;

  /* create the PNG header */
  png_set_IHDR(PngResources.png_ptr, PngResources.info_ptr, img.width, img.height, BitDepth,
              ColorType, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE,
              PNG_FILTER_TYPE_BASE );

  /* store the palette information if there is any */
  if(img.palette != NULL && !GrayScale)
  {
    std::vector<png_color> PNGPalette(NumColors);
    unsigned char *bits = img.palette;
    for(int i = 0; i < NumColors; i++, bits += img.bytes_per_palette_entry)
    {
      PNGPalette[i].red   = bits[2];
      PNGPalette[i].green = bits[1];
      PNGPalette[i].blue  = bits[0];
    }
    png_set_PLTE(PngResources.png_ptr, PngResources.info_ptr, &PNGPalette[0], NumColors);
  }

  /* write the file header information */
  png_write_info(PngResources.png_ptr, PngResources.info_ptr);

  /* create array to store data in */
  std::vector<unsigned char> rows(DIBScanWidth);

  /* point to the bottom row of the DIB data.  DIBs are stored bottom-to-top,
      PNGs are stored top-to-bottom. */
  unsigned char *bits = img.bits + img.height * img.scan_width;

  /* store bits */
  for(int i = 0; i < (int)img.height; i++)
  {
    bits -= img.scan_width;
    switch (img.bits_per_pixel)
    {
      case 1:
      case 4:
      case 8:
        memcpy(&rows[0], bits, DIBScanWidth);
        break;
      case 24:
      {
        unsigned char *q = bits;
        for(unsigned char *p = &rows[0]; p < &rows[0] + DIBScanWidth; p += 3, q += 3)
        {
          p[0] = q[2];
          p[1] = q[1];
          p[2] = q[0];
        }
        break;
      }
      case 32:
        unsigned char *q = bits;
        for(unsigned char *p = &rows[0]; p < &rows[0] + DIBScanWidth; p += 4, q += 4)
        {
          p[3] = q[3];
          p[0] = q[2];
          p[1] = q[1];
          p[2] = q[0];
        }
        break;
    }

    unsigned char *rows_ptr = &rows[0];
    png_write_rows(PngResources.png_ptr, &rows_ptr, 1);
  }

  /* finish writing the rest of the file */
  png_write_end(PngResources.png_ptr, PngResources.info_ptr);
  return BMG_OK;
}
//---------------------------------------------------------------------------
//This function converts a bitmap to PNG data which are stored in Stream
//This is a workaround and replaces SaveBitmapToPNGFile() in BMGlib which does not support storing PNG files in memory.
bool SaveBitmapToPngStream(HBITMAP hBitmap, std::ostream &Stream)
{
  /* extract data from the bitmap.  We assume that 32 bit images have been
  // blended with the background (unless this is a DDB - see GetDataFromBitmap
	// for more details) */
  BMGImageStruct img;
  InitBMGImage(&img);
  BMGError out = GetDataFromBitmap(hBitmap, &img, 1);
  if(out == BMG_OK)
    out = WriteStreamPNG(Stream, img);
 	FreeBMGImage(&img);

	SetLastBMGError( out );
  return out == BMG_OK;
}
//---------------------------------------------------------------------------


