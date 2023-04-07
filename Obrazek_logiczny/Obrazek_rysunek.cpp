#include "Obrazek_rysunek.h"
#include<iostream>

FT_Library Obrazek_rysunek::ft = FT_Library();
FT_Face Obrazek_rysunek::face = FT_Face();
Obrazek_rysunek::Liczba Obrazek_rysunek::cyfry[] = {};

void Obrazek_rysunek::init_ft()
{

	auto error = FT_Init_FreeType(&ft);
	if (error)
	{
		std::cerr << "FREETYPE ERROR" << std::endl;
	}

	error = FT_New_Face(ft, "consola.ttf", 0, &face);

	if (error == FT_Err_Unknown_File_Format)
	{
		std::cerr << "FREETYPE Nie znany fromat" << std::endl;
	}
	else if (error)
	{
		std::cerr << "FREETYPE nie udalo sie zaladowac czcionki" << std::endl;
	}
	FT_Set_Pixel_Sizes(face, 0, rozmiar_pola);
	for (unsigned char c = '0'; c <= '9'; c++)
	{
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "FREETYPE: Failed to load Glyph" << std::endl;
			continue;
		}
		auto& cyf = cyfry[c - '0'];
		cyf.init(face->glyph->bitmap.width, face->glyph->bitmap.rows, face->glyph->bitmap.buffer);
	}
}

void Obrazek_rysunek::free_ft()
{
	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}

void Obrazek_rysunek::gen_obrazek(const Obrazek& obr)
{
	auto k = max_k = obr.max_k;
	auto w = max_w = obr.max_w;
	auto x5 = obr.wym_x / 5u;
	auto y5 = obr.wym_y / 5u;
	auto dx = (1 + rozmiar_pola) * w;
	auto dy = (1 + rozmiar_pola) * k;
	// TODO wyj¹tki
	gen_bufor(
		dx + 2 + x5 * 6 + rozmiar_pola * x5 * 5,
		dy + 2 + y5 * 6 + rozmiar_pola * y5 * 5
	);
	gen_plansza(dx,dy,x5, y5);

	for (int i = 0; i < obr.wym_x; ++i)
	{
		for (int j = 0; j < obr.wym_y; ++j)
		{
			rysuj_w_polu(i, j, obr.pobierz_dane(i, j));
			
		}
	}

	for (int w = 0; w < obr.wym_y; ++w)
	{
		auto& wiersz = obr.wiersze[w];
		for (int i = 0; i < wiersz.size(); ++i)
		{
			rysuj_liczbe_w_wierszu(w, i, wiersz[i]);
		}
	}
	for (int k = 0; k < obr.wym_x; ++k)
	{
		auto& kolumna = obr.kolumny[k];
		for (int i = 0; i < kolumna.size(); ++i)
		{
			rysuj_liczbe_w_kolumnie(k, i, kolumna[i]);
		}
	}
}

void Obrazek_rysunek::gen_bufor(int x, int y)
{
	if (bufor)
		delete[] bufor;
	bufor = new char[x * y];
	b_wym_x = x;
	b_wym_y = y;
	for (int i = 0; i < x * y; ++i)
	{
		bufor[i] = 0xff;
	}
}

void Obrazek_rysunek::gen_plansza(int dx, int dy, int x5, int y5)
{
	this->start_x = dx + 2;
	this->start_y = dy + 2;
	//gen_bufor(2 + x5 * 6 + rozmiar_pola * x5 * 5, 2 + y5 * 6 + rozmiar_pola * y5 * 5);
	int x = dx;
	rysuj_linie_y(dx);
	for (int i = 0; i < x5; ++i)
	{
		rysuj_linie_y(x += 1);
		rysuj_linie_y(x += rozmiar_pola + 1);
		rysuj_linie_y(x += rozmiar_pola + 1);
		rysuj_linie_y(x += rozmiar_pola + 1);
		rysuj_linie_y(x += rozmiar_pola + 1);
		rysuj_linie_y(x += rozmiar_pola + 1);
	}
	rysuj_linie_y(x += 1);

	int y = dy;
	rysuj_linie_x(dy);
	for (int i = 0; i < y5; ++i)
	{
		rysuj_linie_x(y += 1);
		rysuj_linie_x(y += rozmiar_pola + 1);
		rysuj_linie_x(y += rozmiar_pola + 1);
		rysuj_linie_x(y += rozmiar_pola + 1);
		rysuj_linie_x(y += rozmiar_pola + 1);
		rysuj_linie_x(y += rozmiar_pola + 1);
	}
	rysuj_linie_x(y += 1);
}

int Obrazek_rysunek::wyznacz_delte(int i)
{
	int i5 = i / 5;
	int ri = i % 5;
	int di = (6 + 5 * rozmiar_pola) * i5 + (rozmiar_pola + 1) * ri;
	return di;
}

void Obrazek_rysunek::rysuj_linie_x(int y)
{
	int min = y * b_wym_x;
	int max = (y + 1) * b_wym_x;
	int krok = 1;
	rysuj_linie(min, max, krok);
}

void Obrazek_rysunek::rysuj_linie_y(int x)
{
	int min = x;
	int max = b_wym_y * b_wym_x + x;
	int krok = b_wym_x;
	rysuj_linie(min, max, krok);
}

void Obrazek_rysunek::rysuj_linie(int min, int max, int krok)
{
	for (int i = min; i < max; i += krok)
	{
		bufor[i] = 0;
	}
}

void Obrazek_rysunek::rysuj_w_polu(int x, int y, uchar dane)
{
	int x5 = x / 5;
	int y5 = y / 5;
	int rx = x % 5;
	int ry = y % 5;
	int dx = start_x + (6 + 5 * rozmiar_pola) * x5 + (rozmiar_pola + 1) * rx;
	int dy = start_y + (6 + 5 * rozmiar_pola) * y5 + (rozmiar_pola + 1) * ry;
	if (dane == Obrazek::WYPELNIONE)
	{
		for (int i = 0; i < rozmiar_pola; ++i)
		{
			for (int j = 0; j < rozmiar_pola; ++j)
			{
				bufor[dx + i + (dy + j) * b_wym_x] = 127;
			}
		}
	}
	else if(dane == Obrazek::KROPKA)
	{
		int s = rozmiar_pola / 2;
		int r = rozmiar_pola / 4;
		for (int i = 0; i < rozmiar_pola; ++i)
		{
			for (int j = 0; j < rozmiar_pola; ++j)
			{
				if((i-s)*(i-s) + (j-s)*(j-s) < r)
					bufor[dx + i + (dy + j) * b_wym_x] = 64;
			}
		}
	}
	
}

void Obrazek_rysunek::rysuj_prost(int x, int y, int wym_x, int wym_y, unsigned char* dane)
{
	for (int i = 0; i < wym_y; ++i)
	{
		for (int j = 0; j < wym_x; ++j)
		{
			bufor[x + j + (i + y) * b_wym_x] = dane[j + i*wym_x];
		}
	}
}

void Obrazek_rysunek::rysuj_cyfre(int x, int y, uchar cyfra)
{
	auto& c = cyfry[cyfra];
	for (int i = 0; i < c.wym_y; ++i)
	{
		for (int j = 0; j < c.wym_x; ++j)
		{
			bufor[x + j + (i + y) * b_wym_x] = 255u - c.dane[j + i * c.wym_x];
		}
	}
}

void Obrazek_rysunek::rysuj_liczbe(int x, int y, uchar liczba)
{
	if (liczba < 10)
	{
		rysuj_cyfre(x + rozmiar_pola / 4, y, liczba);
	}
	else
	{
		rysuj_cyfre(x, y, liczba / 10);
		rysuj_cyfre(x + rozmiar_pola / 2, y, liczba % 10);
	}
}

void Obrazek_rysunek::rysuj_liczbe_w_wierszu(int w, int i, uchar liczba)
{
	if (liczba == 0)
		return;

	int dx = (rozmiar_pola + 1) * (max_w - 1 - i) + 1;
	int dy = start_y + wyznacz_delte(w) + rozmiar_pola / 5;
	rysuj_liczbe(dx, dy, liczba);
}

void Obrazek_rysunek::rysuj_liczbe_w_kolumnie(int k, int i, uchar liczba)
{
	if (liczba == 0)
		return;

	int dx = start_x + wyznacz_delte(k);
	int dy = (rozmiar_pola + 1) * (max_k - 1 - i) + 1 + rozmiar_pola / 5;
	
	rysuj_liczbe(dx, dy, liczba);
}

void Obrazek_rysunek::odwroc_y()
{
	if (!bufor)
		return;
	char* tmp = new char[b_wym_x];
	char* start = bufor;
	char* koniec = bufor + (b_wym_y - 1) * b_wym_x;

	while (koniec > start)
	{
		std::memcpy(tmp, start, b_wym_x);
		std::memcpy(start, koniec, b_wym_x);
		std::memcpy(koniec, tmp, b_wym_x);
		koniec -= b_wym_x;
		start += b_wym_x;
	}
	delete[] tmp;
}

void Obrazek_rysunek::export_obr(std::string nazwa)
{
	//FIBITMAP* obr = FreeImage_Allocate(b_wym_x, b_wym_y, 8);
	FIBITMAP* obr = FreeImage_ConvertFromRawBits((BYTE*)bufor, b_wym_x, b_wym_y, b_wym_x, 8, 0, 0, 0);
	if (obr) {
		/*for (int y = 0; y < b_wym_y; ++y)
		{
			for (int x = 0; x < b_wym_x; ++x)
			{
				RGBQUAD kolor;
				kolor.rgbRed = bufor[y * b_wym_x + x];
				kolor.rgbBlue = bufor[y * b_wym_x + x];
				kolor.rgbGreen = bufor[y * b_wym_x + x];
				kolor.rgbReserved = bufor[y * b_wym_x + x];
				FreeImage_SetPixelColor(obr, x, y, &kolor);
			}
		}*/
		// bitmap successfully created!
		if (FreeImage_Save(FIF_BMP, obr, nazwa.c_str(), 0)) {
			// bitmap successfully saved!
		}

		FreeImage_Unload(obr);
	}

	
	

	
}

void Obrazek_rysunek::reset()
{
	if (bufor)
		delete[] bufor;
}

Obrazek_rysunek::Liczba::Liczba(int x, int y, uchar* bufor)
{
	wym_x = x;
	wym_y = y;
	dane = new uchar[x * y];
	std::memcpy(dane, bufor, x * y);
}

Obrazek_rysunek::Liczba::Liczba(const Liczba& l)
{
	wym_x = l.wym_x;
	wym_x = l.wym_y;
	dane = new uchar[wym_x * wym_y];
	std::memcpy(dane, l.dane, wym_x * wym_y);
}

Obrazek_rysunek::Liczba::~Liczba()
{
	std::free(dane);
}

Obrazek_rysunek::Liczba& Obrazek_rysunek::Liczba::operator=(const Liczba& l)
{
	if (this == &l)
		return *this;
	if(dane)
		std::free(dane);

	wym_x = l.wym_x;
	wym_x = l.wym_y;
	dane = new uchar[wym_x * wym_y];
	std::memcpy(dane, l.dane, wym_x * wym_y);
	return *this;
	// TODO: tu wstawiæ instrukcjê return
}

void Obrazek_rysunek::Liczba::init(int x, int y, uchar* bufor)
{
	wym_x = x;
	wym_y = y;
	dane = new uchar[wym_x * wym_y];
	std::memcpy(dane, bufor, x * y);
}
