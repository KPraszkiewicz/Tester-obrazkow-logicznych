#pragma once
#include "Obrazek.h"
#include "Obrazek_baza2.h"
#define  FREEIMAGE_LIB
#include "FreeImage.h"

#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftoutln.h>

#include <SFML/Graphics.hpp>

using uchar = unsigned char;

class Obrazek_rysunek
{
	struct Liczba
	{
		Liczba() = default;
		Liczba(int x, int y, uchar* bufor);
		
		void init(int x, int y, uchar* bufor);

		int wym_x{}, wym_y{};
		sf::Image dane;
	};
public:
	const static int rozmiar_pola = 20;
	const static int rozmiar_stanu = 5;
	static FT_Library  ft;   /* handle to library     */
	static FT_Face     face;      /* handle to face object */
	static Liczba		cyfry[10];
	static sf::Image   grafiki[Obrazek::POZA_POLEM];
	static void init_ft();
	static void free_ft();
	static void init();

	static int tryb;
	//const Obrazek* src;
	const Obrazek_baza2* src;
	int b_wym_x, b_wym_y;
	int start_x, start_y;
	int max_k, k_start_x, k_start_y;
	int max_w, w_start_x, w_start_y;
	sf::Image bufor;

	void gen_obrazek(const Obrazek_baza2& obr);
	void gen_bufor(int x, int y);
	void gen_plansza(int dx, int dy, int x5, int y5);

	int wyznacz_delte(int i);

	void rysuj_linie_x(int y);
	void rysuj_linie_y(int x);
	void rysuj_w_polu(int x, int y, uchar dane);
	void rysuj_cyfre(int x, int y, uchar cyfra);
	void rysuj_liczbe(int x, int y, uchar liczba);
	void rysuj_liczbe_w_wierszu(int w, int i, uchar liczba);
	void rysuj_stan_w_wierszu(int w, sf::Color kolor);
	void rysuj_liczbe_w_kolumnie(int k, int i, uchar liczba);
	void rysuj_stan_w_kolumnie(int k, sf::Color kolor);

	void odwroc_y();
	bool export_obr(std::string nazwa);
	void reset();
};

