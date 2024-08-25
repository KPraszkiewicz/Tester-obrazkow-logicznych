#include "Obrazek_baza2.h"
#include <set>



uchar Obrazek_baza2::pobierz_dane(int x, int y) const
{
	if (kolumny[x][y] != wiersze[y][x])
		throw std::runtime_error{ "wiersze i kolumny nie s¹ zsynchronizowane!" };
	return kolumny[x][y];
}

void Obrazek_baza2::ustaw_dane(int x, int y, uchar nowa)
{
	if (kolumny[x].ustaw1(y + 1, nowa) || wiersze[y].ustaw1(x + 1, nowa))
	{
		etapy[x][y] = etap;
	}
}

void Obrazek_baza2::ustaw_dane(bool kolumna, int i, int poz, uchar nowa)
{
	if (kolumna)
	{
		if (kolumny[i].ustaw1(poz, nowa) || wiersze[poz - 1].ustaw1(i + 1, nowa))
		{
			etapy[i][poz - 1] = etap;
		}
	}
	else
	{
		if (kolumny[poz - 1].ustaw1(i + 1, nowa) || wiersze[i].ustaw1(poz, nowa))
		{
			etapy[poz - 1][i] = etap;
		}
	}
	
}

void Obrazek_baza2::ustaw_dane(bool kolumna, int i, const std::vector<uchar>& nowa)
{
	ustaw_etapy(kolumna, i, nowa);
	if (kolumna)
	{
		kolumny[i].akt_dane(nowa);
	}
	else
	{
		wiersze[i].akt_dane(nowa);
	}
}

const Linia& Obrazek_baza2::pobierz_linie(bool kolumna, int index) const
{
	if (kolumna)
		return kolumny[index];
	return wiersze[index];
}

void Obrazek_baza2::ustaw_etapy(bool kolumna, int i, const std::vector<uchar>& nowa)
{
	if (kolumna)
	{
		Linia& stara{ kolumny[i] };
		for (int y = 1; y <= wym_y; ++y)
		{
			if (stara.dane[y] != nowa[y])
			{
				etapy[i][y - 1] = etap;
			}
		}
	}
	else
	{
		Linia& stara{ wiersze[i] };
		for (int x = 1; x <= wym_x; ++x)
		{
			if (stara.dane[x] != nowa[x])
			{
				etapy[x - 1][i] = etap;
			}
		}
	}
}

int Obrazek_baza2::pobierz_etap(int x, int y) const
{
	return etapy[x][y];
}

void Obrazek_baza2::cofnij_do_etapu(int docelowy_etap)
{
	for (int x = 0; x < wym_x; ++x)
	{
		for (int y = 0; y < wym_y; ++y)
		{
			if (etapy[x][y] > docelowy_etap)
			{
				etapy[x][y] = 0;
				kolumny[x].dane[y + 1] = Linia::NIEZNANE;
				wiersze[y].dane[x + 1] = Linia::NIEZNANE;
			}
		}
	}
	etap = docelowy_etap;
	akt_stany(etap);
}

void Obrazek_baza2::akt_stany(int od_etapu)
{
	std::set<int> wyb_wiersze;
	bool zmiana{ false };
	bool skonczona{ true };

	for (int x = 0; x < wym_x; ++x)
	{
		auto& kol{ kolumny[x] };
		for (int y = 0; y < wym_y; ++y)
		{
			if (etapy[x][y] >= od_etapu && etapy[x][y] <= etap)
			{
				zmiana = true;
				wyb_wiersze.insert(y);
			}
			if (kol[y] == Linia::NIEZNANE)
			{
				skonczona = false;
			}
		}
		if (skonczona)
		{
			kol.stan = Linia::Stan::SKONCZONY;
		}
		else if (zmiana)
		{
			kol.stan = Linia::Stan::ZMIENIONY;
			zmiana = false;
		}
		else
		{
			kol.stan = Linia::Stan::NIEZMIENIONY;
		}
		skonczona = true;
	}
	for (int y = 0; y < wym_y; ++y)
	{
		auto& wiersz{ wiersze[y] };
		for (int x = 0; x < wym_x; ++x)
		{
			if (etapy[x][y] >= od_etapu && etapy[x][y] <= etap)
			{
				zmiana = true;
			}
			if (wiersz[x] == Linia::NIEZNANE)
			{
				skonczona = false;
			}
		}
		if (skonczona)
		{
			wiersz.stan = Linia::Stan::SKONCZONY;
		}
		else if (zmiana)
		{
			wiersz.stan = Linia::Stan::ZMIENIONY;
			zmiana = false;
		}
		else
		{
			wiersz.stan = Linia::Stan::NIEZMIENIONY;
		}
		skonczona = true;
	}
}

void Obrazek_baza2::reset_stanow()
{
	for (auto& k : kolumny)
	{
		k.reset_stanu();
	}
	for (auto& w : wiersze)
	{
		w.reset_stanu();
	}
}

bool Obrazek_baza2::czy_zmieniono_stan() const
{
	for (auto& k : kolumny)
	{
		if (k.stan == Linia::Stan::ZMIENIONY)
			return true;
	}
	for (auto& w : wiersze)
	{
		if (w.stan == Linia::Stan::ZMIENIONY)
			return true;
	}
	return false;
}

void Obrazek_baza2::init(size_t wx, size_t wy)
{
	wiersze.reserve(wy);
	kolumny.reserve(wx);
	std::vector<uchar> liczby;
	wym_x = wx;
	wym_y = wy;
	max_k = 0;
	max_w = 0;

	etapy.resize(wx);
	for (size_t x = 0; x < wx; ++x)
	{
		kolumny.emplace_back(liczby, wy, Linia::Kierunek::KOLUMNA, x);
		etapy[x].resize(wy);
	}
	for (size_t y = 0; y < wy; ++y)
	{
		wiersze.emplace_back(liczby, wx, Linia::Kierunek::WIERSZ, y);
	}

	
}

void Obrazek_baza2::wyczysc_dane()
{
	for (auto& k : kolumny)
	{
		k.wyczysc_dane();
	}
	for (auto& w : wiersze)
	{
		w.wyczysc_dane();
	}
}

void Obrazek_baza2::akt_liczby()
{
	for (auto& k : kolumny)
	{
		max_k = std::max(k.akt_liczby(), max_k);
	}
	for (auto& w : wiersze)
	{
		max_w = std::max(w.akt_liczby(), max_w);
	}
}

void Obrazek_baza2::synchronizuj()
{
	synchronizuj_wiersze();
	synchronizuj_kolumny();
}

void Obrazek_baza2::synchronizuj_wiersze()
{
	for (int x = 0; x < wym_x; ++x)
	{
		if (kolumny[x].stan == Linia::Stan::ZMIENIONY)
		{
			for (int y = 0; y < wym_y; ++y)
			{
				wiersze[y].ustaw1(x + 1, kolumny[x][y]);
			}
			kolumny[x].stan = Linia::Stan::NIEZMIENIONY;
			kolumny[x].spr_koniec();
		}
	}
}

void Obrazek_baza2::synchronizuj_wszystkie_wiersze()
{
	for (int x = 0; x < wym_x; ++x)
	{
		for (int y = 0; y < wym_y; ++y)
		{
			wiersze[y].ustaw1(x + 1, kolumny[x][y]);
		}
	}
}

void Obrazek_baza2::synchronizuj_kolumny()
{
	for (int y = 0; y < wym_y; ++y)
	{
		if (wiersze[y].stan == Linia::Stan::ZMIENIONY)
		{
			for (int x = 0; x < wym_x; ++x)
			{
				kolumny[x].ustaw1(y + 1, wiersze[y][x]);
			}
			wiersze[y].stan = Linia::Stan::NIEZMIENIONY;
			wiersze[y].spr_koniec();
		}
	}
}

void Obrazek_baza2::synchronizuj_wszystkie_kolumny()
{
	for (int y = 0; y < wym_y; ++y)
	{
		for (int x = 0; x < wym_x; ++x)
		{
			kolumny[x].ustaw1(y + 1, wiersze[y][x]);
		}
	}
}
