#pragma once
#include "Obrazek.h"
#define  FREEIMAGE_LIB
#include "FreeImage.h"

#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftoutln.h>

using uchar = unsigned char;

class Obrazek_rysunek
{
	struct Liczba
	{
		Liczba() = default;
		Liczba(int x, int y, uchar* bufor);
		Liczba(const Liczba& l);
		~Liczba();

		Liczba& operator=(const Liczba& l);

		void init(int x, int y, uchar* bufor);

		int wym_x, wym_y;
		uchar* dane = nullptr;
	};
public:
	const static int rozmiar_pola = 20;
	static FT_Library  ft;   /* handle to library     */
	static FT_Face     face;      /* handle to face object */
	static Liczba		cyfry[10];
	static void init_ft();
	static void free_ft();

	Obrazek* src;
	int b_wym_x, b_wym_y;
	int start_x, start_y;
	int max_k, k_start_x, k_start_y;
	int max_w, w_start_x, w_start_y;
	char* bufor = nullptr;

	void gen_obrazek(const Obrazek& obr);
	void gen_bufor(int x, int y);
	void gen_plansza(int dx, int dy, int x5, int y5);

	int wyznacz_delte(int i);

	void rysuj_linie_x(int y);
	void rysuj_linie_y(int x);
	void rysuj_linie(int min, int max, int krok);
	void rysuj_w_polu(int x, int y, uchar dane);
	void rysuj_prost(int x, int y, int wym_x, int wym_y, unsigned char* dane);
	void rysuj_cyfre(int x, int y, uchar cyfra);
	void rysuj_liczbe(int x, int y, uchar liczba);
	void rysuj_liczbe_w_wierszu(int w, int i, uchar liczba);
	void rysuj_liczbe_w_kolumnie(int k, int i, uchar liczba);

	void odwroc_y();
	void export_obr(std::string nazwa);
	void reset();
};

