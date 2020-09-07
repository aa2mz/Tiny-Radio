#ifndef __SS_OLED_H__
#define __SS_OLED_H__
/*
 * Copyright 2019, 2020, Edward L. Taychert, AA2MZ
 * 
 * Forked from SS_OLED routines written by Larry Bank. (Give Larry the beer for this one.)
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.
    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

// Make the Linux library interface C instead of C++
#if defined(_LINUX_) && defined(__cplusplus)
extern "C" {
#endif

// These are defined the same in my SPI_LCD library
#ifndef SPI_LCD_H

// 4 possible font sizes: 8x8, 16x32, 6x8, 16x16 (stretched from 8x8)
enum {
   FONT_NORMAL = 0,
   FONT_LARGE,
   FONT_SMALL,
   FONT_STRETCHED
};
#endif

// OLED type for init function
enum {
  OLED_128x128 = 1,
  OLED_128x32,
  OLED_128x64,
  OLED_132x64,
  OLED_64x32,
  OLED_96x16,
  OLED_72x40
};

// Rotation and flip angles to draw tiles
enum {
  ANGLE_0=0,
  ANGLE_90,
  ANGLE_180,
  ANGLE_270,
  ANGLE_FLIPX,
  ANGLE_FLIPY
};

// Return value from oledInit()
enum {
  OLED_NOT_FOUND = -1, // no display found
  OLED_SSD1306_3C,  // SSD1306 found at 0x3C
  OLED_SSD1306_3D,  // SSD1306 found at 0x3D
  OLED_SH1106_3C,   // SH1106 found at 0x3C
  OLED_SH1106_3D,   // SH1106 found at 0x3D
  OLED_SH1107_3C,  // SH1107
  OLED_SH1107_3D
};
//
// Initializes the OLED controller into "page mode" on I2C
// If SDAPin and SCLPin are not -1, then bit bang I2C on those pins
// Otherwise use the Wire library
//
int oledInit(int iType, int bFlip, int bInvert, int iSDAPin, int iSCLPin, int32_t iSpeed);
//
// Initialize an SPI version of the display
//
void oledSPIInit(int iType, int iDC, int iCS, int iReset, int bFlip, int bInvert, int32_t iSpeed);

//
// Sends a command to turn off the OLED display
//
void oledShutdown();
//
// Sets the brightness (0=off, 255=brightest)
//
void oledSetContrast(unsigned char ucContrast);
//
// Load a 128x64 1-bpp Windows bitmap
// Pass the pointer to the beginning of the BMP file
// First pass version assumes a full screen bitmap
//
int oledLoadBMP(uint8_t *pBMP, int bInvert, int bRender);
//
// Power up/down the display
// useful for low power situations
//
void oledPower(uint8_t bOn);
//
// Set the current cursor position
// The column represents the pixel column (0-127)
// The row represents the text row (0-7)
//
void oledSetCursor(int x, int y);

//
// Turn text wrap on or off for the oldWriteString() function
//
void oledSetTextWrap(int bWrap);
//
// Draw a string of normal (8x8), small (6x8) or large (16x32) characters
// At the given col+row with the given scroll offset. The scroll offset allows you to
// horizontally scroll text which does not fit on the width of the display. The offset
// represents the pixels to skip when drawing the text. An offset of 0 starts at the beginning
// of the text.
// The system remembers where the last text was written (the cursor position)
// To continue writing from the last position, set the x,y values to -1
// The text can optionally wrap around to the next line by calling oledSetTextWrap(true);
// otherwise text which would go off the right edge will not be drawn and the cursor will
// be left "off screen" until set to a new position explicitly

// Calling one of the four named oledWriteStrings[size] routines may save 700 to 2000 bytes of
// Flash memory if you only use one or two font sizes in your program.
//
//  Returns 0 for success, -1 for invalid parameter
//
int oledWriteString(int iScrollX, int x, int y, char *szMsg, int iSize, int bInvert, int bRender);
int oledWriteStringNormal(int iScrollX, int x, int y, char *szMsg, int bInvert, int bRender);
int oledWriteStringLarge(int iScrollX, int x, int y, char *szMsg, int bInvert, int bRender);
int oledWriteStringStretched(int iScrollX, int x, int y, char *szMsg, int bInvert, int bRender);
int oledWriteStringSmall(int iScrollX, int x, int y, char *szMsg, int bInvert, int bRender);

//
// Fill the frame buffer with a byte pattern
// e.g. all off (0x00) or all on (0xff)
//
void oledFill(unsigned char ucData, int bRender);
//
// Set (or clear) an individual pixel
// The local copy of the frame buffer is used to avoid
// reading data from the display controller
// (which isn't possible in most configurations)
// This function needs the USE_BACKBUFFER macro to be defined
// otherwise, new pixels will erase old pixels within the same byte
//
int oledSetPixel(int x, int y, unsigned char ucColor, int bRender);
//
// Dump an entire custom buffer to the display
// useful for custom animation effects
//
void oledDumpBuffer(uint8_t *pBuffer);
//
// Render a window of pixels from a provided buffer or the library's internal buffer
// to the display. The row values refer to byte rows, not pixel rows due to the memory
// layout of OLEDs. Pass a src pointer of NULL to use the internal backing buffer
// returns 0 for success, -1 for invalid parameter
//
int oledDrawGFX(uint8_t *pSrc, int iSrcCol, int iSrcRow, int iDestCol, int iDestRow, int iWidth, int iHeight, int iSrcPitch);

//
// Draw a line between 2 points
//
void oledDrawLine(int x1, int y1, int x2, int y2, int bRender);
//
// Get the buffer pointer for direct manipulation of the pixels
//
uint8_t * oledGetBuffer(void);
//
// Play a frame of animation data
// The animation data is assumed to be encoded for a full frame of the display
// Given the pointer to the start of the compressed data,
// it returns the pointer to the start of the next frame
// Frame rate control is up to the calling program to manage
// When it finishes the last frame, it will start again from the beginning
//
uint8_t * oledPlayAnimFrame(uint8_t *pAnimation, uint8_t *pCurrent, int iLen);

//
// Scroll the internal buffer by 1 scanline (up/down)
// width is in pixels, lines is group of 8 rows
// Returns 0 for success, -1 for invalid parameter
//
int oledScrollBuffer(int iStartCol, int iEndCol, int iStartRow, int iEndRow, int bUp);
//
// Draw a 16x16 tile in any of 4 rotated positions
// Assumes input image is laid out like "normal" graphics with
// the MSB on the left and 2 bytes per line
// On AVR, the source image is assumed to be in FLASH memory
// The function can draw the tile on byte boundaries, so the x value
// can be from 0 to 112 and y can be from 0 to 6
//
void oledDrawTile(const uint8_t *pTile, int x, int y, int iRotation, int bInvert, int bRender);

#if defined(_LINUX_) && defined(__cplusplus)
}
#endif // _LINUX_

#endif // __SS_OLED_H__
