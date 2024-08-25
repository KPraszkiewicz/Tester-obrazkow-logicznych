#include "Metoda_perwniaki.h"
#include <stack>
#include "funkcje.h"

using Liczba = std::pair<int, int>;


// lewa
void rozsun_lewa(std::vector<Liczba>& liczby, const std::vector<uchar>& linia, const std::vector<int>& odl, const std::vector<int>& DL)
{
	const int max_liczba = liczby.size();
	int it{ 1 };
	uchar p;
	for (int i = 0; i < max_liczba;)
	{
		Liczba& liczba{ liczby[i] };
		it = liczba.first;
		// 1. sprawdzenie poprawnoœci u³o¿enia/ poprawianie
		// a. co jest przed?
		--it;
		while (linia[it] == Obrazek::WYPELNIONE)
		{
			++it;
		}
		++it;

		// b. w³aœciwe sprawdzanie
		int dl{ 0 };
		int debug = 0;
		while (dl < liczba.second)
		{
			debug += 1;
			int itd{ it + dl };
			if (itd >= linia.size())
			{
				// TODO: ERROR - nie mieœci siê
				throw std::runtime_error("ERROR - nie mieœci siê");
			}
			p = linia[itd];
			if (p == Obrazek::WYPELNIONE)
			{
				if (DL[itd] > liczba.second) // za du¿a grupka
				{
					it = itd + DL[itd] + 1;
					dl = 0;
					continue;
				}
				dl += DL[itd];
				if (dl > liczba.second) // przesuñ start (it)
				{
					// [it; itd]
					it = itd + DL[itd] - liczba.second - 1;
					while (linia[it] == Obrazek::WYPELNIONE)
					{
						++it;
					}
					++it;
				}
			}
			else if (p == Obrazek::PUSTE)
			{
				it = itd + 1;
				dl = 0;
			}
			else
			{
				dl += 1;
			}
		}

		liczba.first = it;

		// 2. nastêpna liczba
		if (++i >= max_liczba)
		{
			break;
		}

		int koniec_liczby{ liczba.first + liczba.second + 1 };
		if (liczby[i].first < koniec_liczby)
		{
			liczby[i].first = koniec_liczby;
		}
	}
}

void przesun_lewa(std::vector<Liczba>& liczby, const std::vector<uchar>& linia, const std::vector<int>& odl, const std::vector<int>& DL)
{
	const size_t wym{ linia.size() };
	int liczby_it = static_cast<int>(liczby.size() - 1);

	int dl{ 0 };
	int j{ 0 };

	for (int i = wym - 2; i > 0; --i)
	{
		if (liczby_it >= 0 && liczby[liczby_it].first + liczby[liczby_it].second > i)
		{
			i -= liczby[liczby_it].second;
			--liczby_it;
			
			if (i < 0)
			{
				break;
			}
		}

		if (linia[i] == Obrazek::WYPELNIONE)
		{
			if (liczby_it < 0)
			{
				throw std::runtime_error("ERR");
			}

			for (int j = 0; j < liczby[liczby_it].second; ++j)
			{
				if (linia[--i] == Obrazek::PUSTE)
				{
					break;
				}
			}
			liczby[liczby_it].first = i + 1;
		}
	}
}

bool sprawdz_poprwanosc_lewa(const std::vector<Liczba>& liczby_kopia, const std::vector<uchar>& linia)
{
	std::vector<Liczba> liczby;
	liczby.reserve(liczby_kopia.size() + 1);
	liczby = liczby_kopia;
	liczby.push_back({ linia.size(), 10000 });

	int k = 0;
	Liczba liczba{ liczby[k] };
	
	for (int i = 0; i < linia.size(); ++i)
	{
		if (i < liczba.first)
		{
			if (linia[i] == Obrazek::WYPELNIONE)
			{
				return false;
			}
		}
		else
		{
			i += liczba.second - 1;
			liczba = liczby[++k];
		}
	}
	return true;
}

void wypisz_lewa(const std::vector<Liczba>& liczby, const std::vector<uchar>& linia)
{
	const auto wym{ linia.size() - 1 };
	for (int i = 1; i < wym; ++i)
	{
		switch (linia[i])
		{
		case Obrazek::WYPELNIONE:
			std::cout << "X";
			break;
		case Obrazek::PUSTE:
			std::cout << "-";
			break;
		default:
			std::cout << ".";
			break;
		}
	}
	std::cout << std::endl;
	int j{ 0 };
	for (int i = 1; i < wym; ++i)
	{
		if (liczby[j].first == i)
		{
			std::cout << liczby[j].second;
			if (liczby[j].second > 9)
			{
				++i;
			}
			++j;
			if (j == liczby.size())
			{
				break;
			}
		}
		else
		{
			std::cout << " ";
		}
	}
	std::cout << std::endl;
	std::flush(std::cout);
}


void Metoda_perwniaki::linia(std::vector<uchar>& linia, const std::vector<uchar>& liczby)
{
	const auto wym{ linia.size() };
	const int max_liczba = liczby.size();
	constexpr int ODL_MAX = 255;
	std::vector<uchar> linia_odw{ linia };
	std::reverse(linia_odw.begin(), linia_odw.end());


	std::vector<int> odlL = wyznacz_odleglosci_lewa(linia);
	std::vector<int> odlP = wyznacz_odleglosci_prawa(linia_odw);
	std::vector<int> DLL = wyznacz_dlugosci(linia);
	std::vector<int> DLP = wyznacz_dlugosci(linia_odw);
	std::vector<Liczba> liczbyLewo(max_liczba, {1, 0}); // start, d³ugoœæ
	std::vector<Liczba> liczbyPrawo(max_liczba, { 1, 0}); // start, d³ugoœæ

	for (int i = 0; i < max_liczba; ++i)
	{
		liczbyLewo[i].second = liczby[i];
		liczbyPrawo[i].second = liczby[max_liczba - i - 1];
	}

	/**
		  |...XX.-..|
	odlL: |321001021| 
	odlP: |123001012| 
	DL:   |000220100|

	grupki	 |...XXXX.X.XX.|
	  start:     3    8 10
	  dl:        4    1 2

			3 1 2 |..........| 3 1 2
	zlewa:  3 1 2 |111.2.33..|
	zprawa:       |..111.2.33| 3 1 2
				  |..X.......|

			3 1 2 |.....X....| 3 1 2
	zlewa:  3 1 2 |111.2.33..|
			3 1 2 |111..233..|
			3 1 2 |111..2.33.|
	zprawa:       |..111.2.33| 3 1 2
				  |..1112..33| 3 1 2
				  |.111.2..33| 3 1 2
				  |.XX.-X-.X.|


	*/
	//std::cout << "======NOWA LINIA=======" << std::endl;
	rozsun_lewa(liczbyLewo, linia, odlL, DLL);
	//wypisz_lewa(liczbyLewo, linia);

	while (!sprawdz_poprwanosc_lewa(liczbyLewo, linia))
	{
		przesun_lewa(liczbyLewo, linia, odlL, DLL);
		//wypisz_lewa(liczbyLewo, linia);
		rozsun_lewa(liczbyLewo, linia, odlL, DLL);
		//wypisz_lewa(liczbyLewo, linia);
	}
	//std::cout << "- PRAWA -" << std::endl;
	//wypisz_lewa(liczbyPrawo, linia_odw);
	rozsun_lewa(liczbyPrawo, linia_odw, odlP, DLP);
	//wypisz_lewa(liczbyPrawo, linia_odw);

	while (!sprawdz_poprwanosc_lewa(liczbyPrawo, linia_odw))
	{
		przesun_lewa(liczbyPrawo, linia_odw, odlP, DLP);
		//wypisz_lewa(liczbyPrawo, linia_odw);
		rozsun_lewa(liczbyPrawo, linia_odw, odlP, DLP);
		//wypisz_lewa(liczbyPrawo, linia_odw);
	}

	// odwracanie liczb - prawo
	const size_t N{ wym - 1 };
	for (auto& l : liczbyPrawo)
	{
		l.first = N - l.first;
	}


	// ³¹czenie liczb
	std::vector<int> lewa(wym, 0xff);
	std::vector<int> prawa(wym, 0xff);
	std::vector<int> max_dl(wym, 0);
	std::stack<int> otwarte;

	for (int i = 0; i < max_liczba; ++i)
	{
		int j{ max_liczba - i - 1 };
		Liczba ll{ liczbyLewo[i] };
		Liczba lp{ liczbyPrawo[j] };
		int start_l{ ll.first };
		int start_p{ lp.first };
		for (int j = 0; j < ll.second; ++j)
		{
			lewa[start_l + j] = i;
			prawa[start_p - j] = i;
		}
		for (int j = start_l; j <= start_p; ++j)
		{
			max_dl[j] = std::max(max_dl[j], ll.second);
		}
	}

	int dl{ 0 };
	for (int i = 1; i < wym - 1; ++i)
	{
		/*if (dl == max_dl[i])
		{
			linia[i] = Obrazek::PUSTE;
			if (i - dl >= 0)
			{
				linia[i - dl] = Obrazek::PUSTE;
			}
			
		}

		if (linia[i] == Obrazek::WYPELNIONE)
		{
			dl += 1;
		}
		else
		{
			dl = 0;
		}*/


		auto l = lewa[i];
		auto p = prawa[i];
		if (l == 0xff && p == 0xff && otwarte.empty())
		{
			linia[i] = Obrazek::PUSTE;
		}
		if (l != 0xff)
		{
			otwarte.push(l);
		}
		if (p != 0xff)
		{
			otwarte.pop();
		}

		if (l != 0xff && l == p)
		{
			linia[i] = Obrazek::WYPELNIONE;
		}
	}
}

void Metoda_perwniaki::operator()(Obrazek_baza2& obr)
{
	obr.etap++;
	
	kolumny(obr);
	wiersze(obr);
	
}

void Metoda_perwniaki::wiersze(Obrazek_baza2& obr)
{
	for (int i = 0; i < obr.wym_y; ++i)
	{
		auto linia{ obr.wiersze[i].dane };
		auto liczby{ obr.wiersze[i].liczby };
		std::reverse(liczby.begin(), liczby.end());
		this->linia(linia, liczby);
		obr.ustaw_dane(false, i, linia);
	}
	obr.synchronizuj_kolumny();
}

void Metoda_perwniaki::kolumny(Obrazek_baza2& obr)
{	
	for (int i = 0; i < obr.wym_x; ++i) // kolumny
	{
		auto linia{ obr.kolumny[i].dane };
		auto liczby{ obr.kolumny[i].liczby };
		std::reverse(liczby.begin(), liczby.end());
		this->linia(linia, liczby);
		obr.ustaw_dane(true, i, linia);
	}
	obr.synchronizuj_wiersze();
}
