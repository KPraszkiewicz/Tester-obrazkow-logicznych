#include "Obrazek.h"

void Obrazek::ustaw_wymiary(uint x, uint y)
{
	wym_x = x;
	wym_y = y;
	max_k = 0;
	max_w = 0;
	wiersze.resize(y);
	kolumny.resize(x);
	dane.resize(x);
	etapy.resize(x);
	for (auto& d : dane)
	{
		d.resize(y);
	}
	for (auto& e : etapy)
	{
		e.resize(y);
	}
}

void Obrazek::wyczysc_dane()
{
	for (int i = 0; i < wym_x; ++i)
	{
		for (int j = 0; j < wym_y; ++j)
		{
			dane[i][j] = NIEZNANE;
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
			if (wart == PUSTE) // kropka
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
			if (wart == PUSTE) // kropka
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
	return dane[x][y];
}

const uchar& Obrazek::pobierz_dane(int i, bool dx, bool dy, int j) const
{
	return dane[i * dy + j * dx][i * dx + j * dy];
}

void Obrazek::ustaw_dane(int x, int y, uchar wart)
{
	dane[x][y] = wart;
	etapy[x][y] = etap;
}

std::string Obrazek::pobierz_dane_string(int x, int y) const
{
	switch (pobierz_dane(x,y))
	{
	case PUSTE:
		return ".";
	case WYPELNIONE:
		return "X";
	default:
		return " ";

	}
}

std::vector<Grupka> Obrazek::pobierz_grupki_kolumna(int x) const
{
	std::vector<Grupka> grupki;
	int dl{};
	for (int i = 0; i < wym_y; ++i)
	{
		auto pole{ dane[x][i] };
		if (pole == WYPELNIONE)
		{
			if (dl == 0)
			{
				grupki.emplace_back(Grupka{ i, 1 });
				dl = 1;
			}
			else
			{
				dl = ++(grupki.back().dl);
			}
		}
		else
		{
			dl = 0;
		}
	}

	return grupki;
}

std::vector<Grupka> Obrazek::pobierz_grupki_wiersz(int y) const
{
	std::vector<Grupka> grupki;
	int dl{};
	for (int i = 0; i < wym_x; ++i)
	{
		auto pole{ dane[i][y] };
		if (pole == WYPELNIONE)
		{
			if (dl == 0)
			{
				grupki.emplace_back(Grupka{ i, 1 });
				dl = 1;
			}
			else
			{
				dl = ++(grupki.back().dl);
			}
		}
		else
		{
			dl = 0;
		}
	}

	return grupki;
}

std::vector<uchar> Obrazek::pobierz_linie(int i, bool dx, bool dy) const
{
	if ((dx ^ dy) == false)
	{
		throw std::exception{ "xx" };
	}
	auto wym{ dx * wym_x + dy * wym_y };
	std::vector<uchar> linia(wym, 0);
	for (int j = 0; j < wym; ++j)
	{
		linia[j] = dane[i * dy + j * dx][i * dx + j * dy];
	}
	return linia;
}


bool Obrazek::akt_linie(int i, bool dx, bool dy, const std::vector<uchar>& linia)
{
	if ((dx ^ dy) == false)
	{
		throw std::exception{ "xx" };
	}
	auto wym{ dx * wym_x + dy * wym_y };
	bool wykonano_aktualizacje{ false };
	for (int j = 0; j < wym; ++j)
	{
		auto& d{ dane[i * dy + j * dx][i * dx + j * dy] };
		if (d != linia[j])
		{
			etapy[i * dy + j * dx][i * dx + j * dy] = etap;
			wykonano_aktualizacje = true;
		}
		d = linia[j];
	}
	return wykonano_aktualizacje;
}

const std::vector<uchar>& Obrazek::pobierz_liczby(int i, bool dx, bool dy) const
{
	if ((dx ^ dy) == false)
	{
		throw std::exception{ "xx" };
	}
	if (dx)
	{
		return wiersze[i];
	}
	else
	{
		return kolumny[i];
	}
}

int Obrazek::pobierz_etap(int x, int y) const
{
	return etapy[x][y];
}

void Obrazek::cofnij_do_etapu(int docelowy_etap)
{
	for (int i = 0; i < dane.size(); ++i)
	{
		for (int j = 0; j < dane[i].size(); ++j)
		{
			if (etapy[i][j] > docelowy_etap)
				dane[i][j] = Obrazek::NIEZNANE;
		}
	}

}

