/* Copyright (C) 2003-2015 Runtime Revolution Ltd.
 
 This file is part of LiveCode.
 
 LiveCode is free software; you can redistribute it and/or modify it under
 the terms of the GNU General Public License v3 as published by the Free
 Software Foundation.
 
 LiveCode is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or
 FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 for more details.
 
 You should have received a copy of the GNU General Public License
 along with LiveCode.  If not see <http://www.gnu.org/licenses/>.  */


#include "prefix.h"
#include <windows.h>
#include <uxtheme.h>
#include <vssym32.h>

#include "foundation.h"


extern "C"
{


MC_DLLEXPORT bool MCWin32ThemeDrawThemeBackgroundToData(HTHEME hTheme, HWND hWnd, int iPartId, int iStateId, int nX, int nY, int nWidth, int nHeight, MCDataRef& r_pixels)
{
    // Work-around for inability to pass rectangles
	MCRectangle p_rect = { nX, nY, nWidth, nHeight };
	
	// Create a memory DC compatible with the window's DC
    HDC hWindowDC = GetDC(hWnd);
    HDC hMemoryDC = CreateCompatibleDC(hWindowDC);
    
    // We need to create one or more device-independent bitmaps to draw into.
    // Before creating them, we need to define the pixel format to be used.
    BITMAPINFO bmi = {0};
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = p_rect.width;
    bmi.bmiHeader.biHeight = -p_rect.height;	// Want a top-down bitmap
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;
    //bmi.bmiColors[0] = NULL;
    
    // Widget rectangle
    RECT p_winrect = { p_rect.x, p_rect.y, p_rect.width, p_rect.height };
    
    // Allocate space for the output image
    uint32_t t_data_length = p_rect.width * p_rect.height * sizeof(uint32_t);
    void *t_pixels;
	/* UNCHECKED */ MCMemoryAllocate(t_data_length, t_pixels);
    
    // If the control is partially-transparent, we need to render twice in order
    // to calculate the alpha values for the pixels. Otherwise, only one pass
    // is needed.
    if (IsThemeBackgroundPartiallyTransparent(hTheme, iPartId, iStateId))
    {
        // Create one white bitmap and one black bitmap
        void *pvWhiteBits, *pvBlackBits;
        HBITMAP hWhiteBitmap = CreateDIBSection(hMemoryDC, &bmi, DIB_RGB_COLORS, &pvWhiteBits, NULL, 0);
        HBITMAP hBlackBitmap = CreateDIBSection(hMemoryDC, &bmi, DIB_RGB_COLORS, &pvBlackBits, NULL, 0);
        
        // Draw the bitmap twice
        SelectObject(hMemoryDC, hWhiteBitmap);
        FillRect(hMemoryDC, &p_winrect, (HBRUSH)GetStockObject(WHITE_BRUSH));
        DrawThemeBackground(hTheme, hMemoryDC, iPartId, iStateId, &p_winrect, &p_winrect);
        SelectObject(hMemoryDC, hBlackBitmap);
        FillRect(hMemoryDC, &p_winrect, (HBRUSH)GetStockObject(BLACK_BRUSH));
        DrawThemeBackground(hTheme, hMemoryDC, iPartId, iStateId, &p_winrect, &p_winrect);
        
        // Ensure all GDI operations are complete
        GdiFlush();
        
        // Calculate the alpha values for each pixel
        uint32_t *t_white_pixel = reinterpret_cast<uint32_t*>(pvWhiteBits);
        uint32_t *t_black_pixel = reinterpret_cast<uint32_t*>(pvBlackBits);
        uint32_t *t_alpha_pixel = reinterpret_cast<uint32_t*>(t_pixels);
        for (int y = 0; y < p_rect.height; y++)
        {
            for (int x = 0; x < p_rect.width; x++)
            {
                // Calculate the alpha value for this pixel. Note that we only
                // do this for one channel (because the alpha value should be
                // applied equally to all channels anyway).
                uint8_t t_alpha = 255 - (*t_white_pixel & 0xFF) + (*t_black_pixel & 0xFF);
                *t_alpha_pixel = t_alpha | _byteswap_ulong(*t_black_pixel);
  
                t_white_pixel++;
                t_black_pixel++;
                t_alpha_pixel++;
            }
        }

        // Clean up
        DeleteDC(hMemoryDC);
        DeleteObject(hWhiteBitmap);
        DeleteObject(hBlackBitmap);
    }
    else
    {
        // Even when the background isn't transparent, the drawing operation
		// may only touch some of the pixels. Initialise them to a distinctive
		// value so that we can tell which have been drawn (GDI operations
		// clear the upper 8 bits of the buffer).
        void *pvBitmapBits;
        HBITMAP hBitmap = CreateDIBSection(hMemoryDC, &bmi, DIB_RGB_COLORS, &pvBitmapBits, NULL, 0);
		memset(pvBitmapBits, 0xFF, p_rect.width * p_rect.height * sizeof(uint32_t));
        SelectObject(hMemoryDC, hBitmap);
        DrawThemeBackground(hTheme, hMemoryDC, iPartId, iStateId, &p_winrect, &p_winrect);
        
        // Just copy the image to the output
		uint32_t *t_in_pixel = reinterpret_cast<uint32_t*>(pvBitmapBits);
		uint32_t *t_out_pixel = reinterpret_cast<uint32_t*>(t_pixels);
		for (int y = 0; y < p_rect.height; y++)
		{
			for (int x = 0; x < p_rect.width; x++)
			{
				uint32_t t_pixel;
				t_pixel = *t_in_pixel;

				if (t_pixel == 0xFFFFFFFF)
					*t_out_pixel = 0;	// Transparent black
				else
					*t_out_pixel = _byteswap_ulong(0xFF000000 | t_pixel);

				t_in_pixel++;
				t_out_pixel++;
			}
		}
        
        // Clean up
        DeleteDC(hMemoryDC);
        DeleteObject(hBitmap);
    }
    
    // Clean up
    ReleaseDC(hWnd, hWindowDC);
    
    // All done
    return MCDataCreateWithBytesAndRelease((byte_t*)t_pixels, t_data_length, r_pixels);
}

MC_DLLEXPORT HWND MCWin32ThemeCreateDummyWindow()
{
	// Create a new window. Just use the built-in "BUTTON" class
	HWND hWnd;
	hWnd = CreateWindowA("BUTTON", "LCTheme", WS_OVERLAPPEDWINDOW, 0, 0, CW_USEDEFAULT, CW_USEDEFAULT, (HWND)NULL, (HMENU)NULL, (HINSTANCE)NULL, NULL);
	if (hWnd == NULL)
	{
		abort();
	}
	return hWnd;
}

MC_DLLEXPORT bool MCWin32ThemeGetContentRect(HTHEME hTheme, int iPartId, int iStateId, int pWidth, int pHeight, int& rX, int& rY, int& rWidth, int& rHeight)
{
	HRESULT t_result;
	RECT t_background = { 0, 0, pWidth, pHeight };
	RECT t_contents;
	t_result = GetThemeBackgroundContentRect(hTheme, NULL, iPartId, iStateId, &t_background, &t_contents);

	if (t_result == S_OK)
	{
		rX = t_contents.left;
		rY = t_contents.top;
		rWidth = t_contents.right - t_contents.left;
		rHeight = t_contents.bottom - t_contents.top;
	}

	return t_result == S_OK;
}

MC_DLLEXPORT bool MCWin32ThemeGetFont(HTHEME hTheme, int iPartId, int iStateId, int iWhichFont, MCStringRef& r_fontname, int& r_size)
{
	LOGFONTW lfwFontInfo;
	HRESULT hResult = GetThemeFont(hTheme, NULL, iPartId, iStateId, iWhichFont, &lfwFontInfo);

	// If it failed to get it for the theme, try to get the system equivalent
	if (hResult != S_OK)
	{
		hResult = GetThemeSysFont(hTheme, iWhichFont, &lfwFontInfo);
	}

	if (hResult == S_OK)
	{
		MCStringCreateWithChars(lfwFontInfo.lfFaceName, wcslen(lfwFontInfo.lfFaceName), r_fontname);
		r_size = -MulDiv(lfwFontInfo.lfHeight, 72, /*GetDeviceCaps(hDC, LOGPIXELSY)*/72);
	}

	return hResult == S_OK;
}

}	// extern "C"

