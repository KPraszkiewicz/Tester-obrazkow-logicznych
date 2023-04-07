#pragma once

#define  FREEIMAGE_LIB
#include "FreeImage.h"
#include <assert.h>
#include <stdio.h>
#include <io.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <cstring>
#include <filesystem>

#ifndef MAX_PATH
#define MAX_PATH	260
#endif

#include "Obrazek.h"

/** Generic image loader
	@param lpszPathName Pointer to the full file name
	@param flag Optional load flag constant
	@return Returns the loaded dib if successful, returns NULL otherwise
*/
FIBITMAP* GenericLoader(const char* lpszPathName, int flag) {
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;

	// check the file signature and deduce its format
	// (the second argument is currently not used by FreeImage)
	fif = FreeImage_GetFileType(lpszPathName, 0);
	if (fif == FIF_UNKNOWN) {
		// no signature ?
		// try to guess the file format from the file extension
		fif = FreeImage_GetFIFFromFilename(lpszPathName);
	}
	// check that the plugin has reading capabilities ...
	if ((fif != FIF_UNKNOWN) && FreeImage_FIFSupportsReading(fif)) {
		// ok, let's load the file
		FIBITMAP* dib = FreeImage_Load(fif, lpszPathName, flag);
		// unless a bad file format, we are done !
		return dib;
	}
	return NULL;
}


/** Generic image writer
	@param dib Pointer to the dib to be saved
	@param lpszPathName Pointer to the full file name
	@param flag Optional save flag constant
	@return Returns true if successful, returns false otherwise
*/
bool GenericWriter(FIBITMAP* dib, const char* lpszPathName, int flag) {
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	BOOL bSuccess = FALSE;

	if (dib) {
		// try to guess the file format from the file extension
		fif = FreeImage_GetFIFFromFilename(lpszPathName);
		if (fif != FIF_UNKNOWN) {
			// check that the plugin has sufficient writing and export capabilities ...
			WORD bpp = FreeImage_GetBPP(dib);
			if (FreeImage_FIFSupportsWriting(fif) && FreeImage_FIFSupportsExportBPP(fif, bpp)) {
				// ok, we can save the file
				bSuccess = FreeImage_Save(fif, dib, lpszPathName, flag);
				// unless an abnormal bug, we are done !
			}
		}
	}
	return (bSuccess == TRUE) ? true : false;
}


/**
	FreeImage error handler
	@param fif Format / Plugin responsible for the error
	@param message Error message
*/
void FreeImageErrorHandler(FREE_IMAGE_FORMAT fif, const char* message) {
	printf("\n*** ");
	if (fif != FIF_UNKNOWN) {
		printf("%s Format\n", FreeImage_GetFormatFromFIF(fif));
	}
	printf(message);
	printf(" ***\n");
}


void rysuj_obrazek(const Obrazek& obrazek)
{
	// kolumny
	for (int i = obrazek.max_k - 1; i >= 0; --i)
	{
		size_t rrr = (size_t)obrazek.max_w * 3ull;
		std::cout << std::string(rrr, ' ');
		for (int j = 0; j < obrazek.wym_x; ++j)
		{
			if (obrazek.kolumny[j].size() > i)
			{
				std::cout << std::setw(3) << (int)obrazek.kolumny[j][i];
			}
			else
			{
				std::cout << std::string(3, ' ');
			}

		}
		std::cout << std::endl;
	}
	// wiersze
	for (int i = 0; i < obrazek.wym_y; ++i)
	{
		int s = obrazek.wiersze[i].size();
		//int sr = s - obrazek.max_w ;
		for (int j = obrazek.max_w - 1; j >= 0; --j)
		{
			/*
			if (sr >= 0)
			{
				std::cout << std::setw(3) << (int)obrazek.wiersze[i][sr];
			}
			else
			{
				std::cout << std::string(3, ' ');
			}
			*/
			if (j < s)
			{
				std::cout << std::setw(3) << (int)obrazek.wiersze[i][j];
			}
			else
			{
				std::cout << std::string(3, ' ');
			}
		}
		// dane
		for (int j = 0; j < obrazek.wym_x; ++j)
		{
			std::cout << std::setw(3) << obrazek.pobierz_dane_string(j, i);
		}
		std::cout << std::endl;
	}


}

Obrazek wczytaj_obrazek(const std::string& nazwa)
{
	FIBITMAP* obr = GenericLoader(nazwa.c_str(), 0);
	obr = FreeImage_ConvertTo8Bits(obr);
	std::cout << FreeImage_GetBPP(obr) << std::endl;

	unsigned int wym_y = FreeImage_GetHeight(obr);
	unsigned int wym_x = FreeImage_GetWidth(obr);
	Obrazek obrazek{ wym_x,wym_y };

	if (FreeImage_GetBPP(obr) == 8) {
		for (unsigned int y = 0; y < wym_y; y++) {
			BYTE* bits = FreeImage_GetScanLine(obr, y);
			obrazek.wczytaj_dane(y, bits);
		}
	}
	return obrazek;
}

void gen_obr_test()
{
	int w = 100;
	int h = 100;

	FIBITMAP* obr = FreeImage_Allocate(w, h, 8);

	if (obr) {
		// bitmap successfully created!
		if (FreeImage_Save(FIF_BMP, obr, "test.bmp", 0)) {
			// bitmap successfully saved!
		}

		FreeImage_Unload(obr);
	}

}

void init_FreeImage()
{

	// call this ONLY when linking with FreeImage as a static library
#ifdef FREEIMAGE_LIB
	FreeImage_Initialise();
#endif // FREEIMAGE_LIB

	FreeImage_SetOutputMessage(FreeImageErrorHandler);

	// print version & copyright infos
	std::cout << FreeImage_GetVersion() << std::endl;
	std::cout << FreeImage_GetCopyrightMessage() << std::endl;
}

void free_FreeImage()
{
#ifdef FREEIMAGE_LIB
	FreeImage_DeInitialise();
#endif // FREEIMAGE_LIB
}