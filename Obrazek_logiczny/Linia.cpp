#include "Linia.h"

Linia::Linia(const std::vector<uchar>& _liczby, size_t _rozmiar, Kierunek _kierunek, int _nr)
	:liczby(_liczby)
	, dane(_rozmiar + 2ull, 0)
	, kierunek(_kierunek)
	, nr{ _nr }
{
	dane[0] = TYP_POLA::PUSTE;
	dane[_rozmiar + 1] = TYP_POLA::PUSTE;
}

Linia::Linia(std::vector<uchar>&& _liczby, size_t _rozmiar, Kierunek _kierunek, int _nr)
	:liczby(std::move(_liczby))
	, dane(_rozmiar + 2ull, 0)
	, kierunek(_kierunek)
	, nr{ _nr }
{
	dane[0] = TYP_POLA::PUSTE;
	dane[_rozmiar + 1] = TYP_POLA::PUSTE;
}

uchar Linia::operator[](int i) const
{
	return dane[i + 1];
}

bool Linia::ustaw0(int i, uchar w)
{
	if (dane[i + 1] == w)
		return false;
	dane[i + 1] = w;
	stan = Stan::ZMIENIONY;
	return true;
}

bool Linia::ustaw1(int i, uchar w)
{
	if (dane[i] == w)
		return false;
	dane[i] = w;
	stan = Stan::ZMIENIONY;
	return true;
}

void Linia::spr_koniec()
{
	for (int i = 1; i < dane.size() - 2; ++i)
	{
		if (dane[i] == NIEZNANE)
		{
			return;
		}
	}
	stan = Stan::SKONCZONY;
}



void Linia::wyczysc_dane()
{
	int wym = dane.size() - 1;
	for (int i = 1; i < wym; ++i)
	{
		dane[i] = NIEZNANE;
	}
	stan = Stan::ZMIENIONY;
}

size_t Linia::akt_liczby()
{
	uchar akt = 0;
	int koniec = dane.size() - 1;

	for (int i = 1; i < koniec; ++i)
	{
		if (dane[i] != WYPELNIONE)
		{
			if (akt > 0)
			{
				liczby.push_back(akt);
				akt = 0;
			}
		}
		else
		{
			akt += 1;
		}
	}
	if (akt > 0)
	{
		liczby.push_back(akt);
	}
	std::reverse(liczby.begin(), liczby.end());
	return liczby.size();
}

void Linia::reset_stanu()
{
	if (stan != Stan::SKONCZONY)
		stan = Stan::ZMIENIONY;
}

void Linia::akt_dane(const std::vector<uchar>& nowe)
{
	bool wszystkie_pola_wypelnione{ true };
	for (int i = 1; i < dane.size() - 1; ++i)
	{
		if (dane[i] == NIEZNANE)
		{
			wszystkie_pola_wypelnione = false;
			if (nowe[i] != NIEZNANE)
			{
				dane[i] = nowe[i];
				stan = Stan::ZMIENIONY;
			}
		}
	}
	if (wszystkie_pola_wypelnione)
	{
		stan = Stan::SKONCZONY;
	}
}

uchar* Linia::pobierz_dane_wsk(int przesuniecie)
{
	return dane.data() + przesuniecie;
}

std::vector<uchar> Linia::pobierz_dane_kopia() const
{
	return dane;
}