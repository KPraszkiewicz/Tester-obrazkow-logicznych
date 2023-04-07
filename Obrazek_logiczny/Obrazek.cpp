#include "Obrazek.h"

void Obrazek::ustaw_wymiary(uint x, uint y)
{
	wym_x = x;
	wym_y = y;
	max_k = 0;
	max_w = 0;
	wiersze.resize(y);
	kolumny.resize(x);
	dane.resize(y);
}

void Obrazek::wczytaj_dane(int i, uchar* wiersz, int n)
{
	if (i < 0 || i >= wym_y)
		return;
	if (n == 0)
		n = wym_x;
	int index = wym_y - i - 1;
	dane[index].resize(n);
	for (int j = 0; j < n; ++j)
	{
		int w = uchar(255u) - wiersz[j];
		if (w < 128)
			dane[index][j] = KROPKA;
		else
			dane[index][j] = WYPELNIONE;
		//dane[index][j] = w;
	}
}

void Obrazek::wyczysc_dane()
{
	for (int i = 0; i < wym_y; ++i)
	{
		for (int j = 0; j < wym_x; ++j)
		{
			dane[i][j] = PUSTE;
		}
	}
}

void Obrazek::akt_liczby()
{
	uchar pop = 0;
	uchar akt = 0;

	// kolumny
	for (int i = 0; i < wym_x; ++i)
	{
		auto& k = kolumny[i];
		for (int j = 0; j < wym_y; ++j)
		{
			uchar wart = pobierz_dane(i, j);
			if (wart == KROPKA) // kropka
			{
				if (akt > 0)
				{
					k.push_back(akt);
					akt = 0;
				}
			}
			else // if(wart == WYPELNIONE) czarny
			{
				akt += 1;
			}
		}
		if (akt > 0)
		{
			k.push_back(akt);
			akt = 0;
		}
		if (k.size() > max_k)
		{
			max_k = k.size();
		}
		std::reverse(k.begin(), k.end());
	}

	// wiersze
	for (int i = 0; i < wym_y; ++i)
	{
		auto& w = wiersze[i];
		for (int j = 0; j < wym_x; ++j)
		{
			uchar wart = pobierz_dane(j, i);
			if (wart == KROPKA) // kropka
			{
				if (akt > 0)
				{
					w.push_back(akt);
					akt = 0;
				}
			}
			else // if(wart == WYPELNIONE) czarny
			{
				akt += 1;
			}
		}
		if (akt > 0)
		{
			w.push_back(akt);
			akt = 0;
		}
		if (w.size() > max_w)
		{
			max_w = w.size();
		}
		std::reverse(w.begin(), w.end());
	}
	
}

const uchar& Obrazek::pobierz_dane(int x, int y) const
{
	if (x < 0 || y < 0 || x >= wym_x || y >= wym_y)
		return POZA_POLEM;
	return dane[y][x];
}

void Obrazek::ustaw_dane(int x, int y, uchar wart)
{
	dane[y][x] = wart;
}

std::string Obrazek::pobierz_dane_string(int x, int y) const
{
	switch (pobierz_dane(x,y))
	{
	case KROPKA:
		return ".";
	case WYPELNIONE:
		return "X";
	default:
		return " ";

	}
}
