#include "funkcje.h"

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
	sf::Image obr;
	obr.loadFromFile(nazwa);

	unsigned int wym_y{ obr.getSize().y };
	unsigned int wym_x{ obr.getSize().x };
	Obrazek obrazek{ wym_x,wym_y };

	auto& dane{ obrazek.dane };

	for (int i = 0; i < wym_x; ++i)
	{
		for (int j = 0; j < wym_y; ++j)
		{
			auto kolor{ obr.getPixel(i, j) };
			int sr{ (kolor.r + kolor.g + kolor.b) / 3 };
			if (sr < 127)
			{
				dane[i][j] = Obrazek::WYPELNIONE;
			}
			else
			{
				dane[i][j] = Obrazek::PUSTE;
			}

		}
	}
	return obrazek;
}

Obrazek_baza2 wczytaj_obrazek2(const std::string& nazwa)
{
	sf::Image obr;
	obr.loadFromFile(nazwa);

	unsigned int wym_y{ obr.getSize().y };
	unsigned int wym_x{ obr.getSize().x };
	Obrazek_baza2 obrazek{ wym_x,wym_y };


	for (int i = 0; i < wym_y; ++i)
	{
		auto& wiersz{ obrazek.wiersze[i].dane };
		for (int j = 1; j <= wym_x; ++j)
		{
			auto kolor{ obr.getPixel(j - 1, i) };
			int sr{ (kolor.r + kolor.g + kolor.b) / 3 };
			if (sr < 127)
			{
				wiersz[j] = Obrazek::WYPELNIONE;
			}
			else
			{
				wiersz[j] = Obrazek::PUSTE;
			}
		}
		obrazek.wiersze[i].stan = Linia::Stan::ZMIENIONY;
	}
	obrazek.synchronizuj_kolumny();

	return obrazek;
}

// domyœlnie odleg³oœci od lewej
std::vector<int> wyznacz_odleglosci(const std::vector<uchar>& linia)
{
	constexpr int ODL_MAX = 255;
	int odl{};
	const auto wym{ linia.size() };
	std::vector<int> ODL(wym, ODL_MAX);

	for (int i = wym - 1; i >= 0; --i)
	{
		uchar p{ linia[i] };
		if (p == Obrazek::NIEZNANE)
		{
			odl += 1;
		}
		else
		{
			odl = 0;
		}
		ODL[i] = odl;
	}
	return ODL;
}

std::vector<int> wyznacz_odleglosci_lewa(const std::vector<uchar>& linia)
{
	return wyznacz_odleglosci(linia);
}

std::vector<int> wyznacz_odleglosci_prawa(const std::vector<uchar>& linia)
{
	const auto wym{ linia.size() };
	std::vector<uchar> linia2;
	linia2.resize(wym);
	for (int i = 0; i < wym; ++i)
	{
		linia2[i] = linia[wym - i - 1];
	}
	return wyznacz_odleglosci(linia2);
}

std::vector<int> wyznacz_dlugosci(const std::vector<uchar>& linia)
{
	constexpr int ODL_MAX{ 255 };
	int dl_puste{};
	int dl_wypelnione{};
	int dl{};

	const auto wym{ linia.size() };
	std::vector<int> DL(wym, ODL_MAX);

	for (int i = 0; i < wym; ++i)
	{
		uchar p{ linia[i] };

		if (dl_wypelnione > 0)
		{
			if (p != Obrazek::WYPELNIONE)
			{
				dl = dl_wypelnione;
				while (dl_wypelnione > 0)
				{
					DL[i - dl_wypelnione--] = dl;
				}
			}
		}
		if (dl_puste > 0)
		{
			if (p != Obrazek::PUSTE)
			{
				dl = dl_puste;
				while (dl_puste > 0)
				{
					DL[i - dl_puste--] = dl;
				}
			}
		}

		if (p == Obrazek::WYPELNIONE)
		{
			dl_wypelnione += 1;

		}

		if (p == Obrazek::PUSTE)
		{
			dl_puste += 1;
		}


	}
	dl = dl_wypelnione + dl_puste; // conajmniej jedno zawsze = 0
	for (int i = wym - dl; i < wym; ++i)
	{
		DL[i] = dl;
	}
	return DL;
}

void usun_cudzyslowy(std::string& nazwa)
{
	/*const size_t wym{ nazwa.size() };
	std::string nowa;
	nowa.reserve(wym);
	size_t it{ 0 };

	for (int i = 0; i < wym; ++i)
	{
		if (nazwa[i] != '"')
		{
			nowa.
			nowa.append(nazwa[i]);
		}
	}
	return nowa;*/
	nazwa.erase(
		std::remove(nazwa.begin(), nazwa.end(), '\"'), 
		nazwa.end()
	);
}
