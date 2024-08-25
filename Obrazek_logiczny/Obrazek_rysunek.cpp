#include "Obrazek_rysunek.h"
#include<iostream>

FT_Library Obrazek_rysunek::ft = FT_Library();
FT_Face Obrazek_rysunek::face = FT_Face();
Obrazek_rysunek::Liczba Obrazek_rysunek::cyfry[]{};
sf::Image Obrazek_rysunek::grafiki[]{};
int Obrazek_rysunek::tryb{ 0 };

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

void Obrazek_rysunek::init()
{
	init_ft();
	grafiki[Obrazek::PUSTE].loadFromFile("gfx/iks.png");
	grafiki[Obrazek::WYPELNIONE].loadFromFile("gfx/wypelnione.png");
	grafiki[Obrazek::NIEZNANE].loadFromFile("gfx/puste.png");

}

void Obrazek_rysunek::gen_obrazek(const Obrazek_baza2& obr)
{
	src = &obr;
	auto k = max_k = obr.max_k;
	auto w = max_w = obr.max_w;
	auto x5 = obr.wym_x / 5u;
	auto y5 = obr.wym_y / 5u;
	auto dx = (1 + rozmiar_pola) * w;
	auto dy = (1 + rozmiar_pola) * k;
	// TODO wyj¹tki
	gen_bufor(
		rozmiar_stanu + dx + 2 + x5 * 6 + rozmiar_pola * x5 * 5,
		rozmiar_stanu + dy + 2 + y5 * 6 + rozmiar_pola * y5 * 5
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
		for (int i = 0; i < wiersz.liczby.size(); ++i)
		{
			rysuj_liczbe_w_wierszu(w, i, wiersz.liczby[i]);
		}
		switch (wiersz.stan)
		{
		case Linia::Stan::ZMIENIONY:
			rysuj_stan_w_wierszu(w, sf::Color::Yellow);
			break;
		case Linia::Stan::SKONCZONY:
			rysuj_stan_w_wierszu(w, sf::Color::Green);
			break;
		default:
			break;
		}
		
	}
	for (int k = 0; k < obr.wym_x; ++k)
	{
		auto& kolumna = obr.kolumny[k];
		for (int i = 0; i < kolumna.liczby.size(); ++i)
		{
			rysuj_liczbe_w_kolumnie(k, i, kolumna.liczby[i]);
		}
		
		switch (kolumna.stan)
		{
		case Linia::Stan::ZMIENIONY:
			rysuj_stan_w_kolumnie(k, sf::Color::Yellow);
			break;
		case Linia::Stan::SKONCZONY:
			rysuj_stan_w_kolumnie(k, sf::Color::Green);
			break;
		default:
			break;
		}
	}
}

void Obrazek_rysunek::gen_bufor(int x, int y)
{
	bufor.create(x, y, sf::Color::White);
	b_wym_x = x;
	b_wym_y = y;
}

void Obrazek_rysunek::gen_plansza(int dx, int dy, int x5, int y5)
{
	start_x = rozmiar_stanu + dx + 2;
	start_y = rozmiar_stanu + dy + 2;
	
	int x = start_x - 2;
	rysuj_linie_y(x);
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

	int y = start_y - 2;
	rysuj_linie_x(y);
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
	for (int i = 0; i < b_wym_x; ++i)
		bufor.setPixel(i, y, sf::Color::Black);
}

void Obrazek_rysunek::rysuj_linie_y(int x)
{
	for (int i = 0; i < b_wym_y; ++i)
		bufor.setPixel(x, i, sf::Color::Black);
}

void Obrazek_rysunek::rysuj_w_polu(int x, int y, uchar dane)
{
	int x5 = x / 5;
	int y5 = y / 5;
	int rx = x % 5;
	int ry = y % 5;
	int dx = start_x + (6 + 5 * rozmiar_pola) * x5 + (rozmiar_pola + 1) * rx;
	int dy = start_y + (6 + 5 * rozmiar_pola) * y5 + (rozmiar_pola + 1) * ry;

	if (tryb == 0)
	{
		bufor.copy(grafiki[dane], dx, dy);
	}
	else if (tryb == 1)
	{
		rysuj_liczbe(dx, dy + 4, src->pobierz_etap(x, y));
	}
}

void Obrazek_rysunek::rysuj_cyfre(int x, int y, uchar cyfra)
{
	bufor.copy(cyfry[cyfra].dane, x, y);
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

	int dx = (rozmiar_pola + 1) * (max_w - 1 - i) + 1 + rozmiar_stanu;
	int dy = start_y + wyznacz_delte(w) + rozmiar_pola / 5;
	rysuj_liczbe(dx, dy, liczba);
}

void Obrazek_rysunek::rysuj_stan_w_wierszu(int w, sf::Color kolor)
{
	int dx = 0;
	int dy = start_y + wyznacz_delte(w);
	for (int x = 0; x < rozmiar_stanu; ++x)
	{
		for (int y = 0; y < rozmiar_pola; ++y)
		{
			bufor.setPixel(dx + x, dy + y, kolor);
		}
	}
}

void Obrazek_rysunek::rysuj_liczbe_w_kolumnie(int k, int i, uchar liczba)
{
	if (liczba == 0)
		return;

	int dx = start_x + wyznacz_delte(k);
	int dy = (rozmiar_pola + 1) * (max_k - 1 - i) + 1 + rozmiar_pola / 5 + rozmiar_stanu;
	
	rysuj_liczbe(dx, dy, liczba);
}

void Obrazek_rysunek::rysuj_stan_w_kolumnie(int k, sf::Color kolor)
{
	int dx = start_x + wyznacz_delte(k);
	int dy = 0;

	for (int x = 0; x < rozmiar_pola; ++x)
	{
		for (int y = 0; y < rozmiar_stanu; ++y)
		{
			bufor.setPixel(dx + x, dy + y, kolor);
		}
	}
}

void Obrazek_rysunek::odwroc_y()
{
	bufor.flipVertically();
}

bool Obrazek_rysunek::export_obr(std::string nazwa)
{
	return bufor.saveToFile(nazwa);
}

void Obrazek_rysunek::reset()
{

}

Obrazek_rysunek::Liczba::Liczba(int x, int y, uchar* bufor)
{
	init(x, y, bufor);
}

void Obrazek_rysunek::Liczba::init(int x, int y, uchar* bufor)
{
	wym_x = x;
	wym_y = y;
	dane.create(x, y);
	for (int j = 0; j < y; ++j)
	{
		for (int i = 0; i < x; ++i)
		{
			const uint8_t odcien{ (uint8_t)(255 - *bufor) };
			++bufor;
			const sf::Color kolor{ odcien, odcien, odcien };
			dane.setPixel(i, j, kolor);
		}
	}
}
