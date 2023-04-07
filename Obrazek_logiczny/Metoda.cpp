#include "Metoda.h"
#include <iostream>
#include <stack>
#include <queue>

const uchar& Metoda::pobierz_dane_linia(const Obrazek& obr, int i) const
{
	return obr.pobierz_dane(x + dx * i, y + dy * i);
}

void Metoda::poziom(Obrazek& obr, int y)
{
	auto liczby = obr.wiersze[y];
	std::reverse(liczby.begin(), liczby.end());
	this->x = 0;
	this->y = y;
	this->dx = 1;
	this->dy = 0;
	linia(obr, liczby, 0, y, 1, 0);
}

void Metoda::pion(Obrazek& obr, int x)
{
	auto liczby = obr.kolumny[x];
	std::reverse(liczby.begin(), liczby.end());
	this->x = x;
	this->y = 0;
	this->dx = 0;
	this->dy = 1;
	linia(obr, liczby, x, 0, 0, 1);
}

void Metoda_p1::linia(Obrazek& obr, const std::vector<uchar>& liczby, int x, int y, int dx, int dy)
{
	const auto wym = (dx == 0) ? obr.wym_y : obr.wym_x;
	const int max_liczba = liczby.size();
	std::vector<uchar> bufor(wym, 0xff);

	bool pewniak = true;
	int akt = 0;
	auto l = liczby[akt];
	int puste = 0;
	int w = 0; // iloœæ wype³nionych pól

	for (int i = 0; i < wym; ++i)
	{
		uchar p = obr.pobierz_dane(x + dx * i, y + dy * i);
		if (p == Obrazek::WYPELNIONE)
		{
			w += 1;
		}
		else if (p == Obrazek::KROPKA)
		{
			// czyszczenie bufora - nie jest potrzebne
			/*
			int j = i - 1;
			while (j >= 0 && bufor[j] == akt)
			{
				bufor[j++] = 255;
			}
			*/
			// rozpoczêcie od nowa
			w = 0;
			l = liczby[akt];
			continue;

		}
		else
		{
			puste += 1;
			uchar pop = obr.pobierz_dane(x + dx * (i - 1), y + dy * (i - 1));
			switch (pop)
			{
			case Obrazek::PUSTE:
				// do zmiany
				break;
			case Obrazek::WYPELNIONE:
				if (pewniak)
				{
					obr.ustaw_dane(x + dx * i, y + dy * i, obr.WYPELNIONE);
				}
				
				
				break;
			default:
				
				break;
			}
			bufor[i] = akt;
		}

		// nast pole
		if (--l == 0)
		{
			l = liczby[akt];
			if (w == l && pewniak) // liczba zrobiona
			{
				int start = i + 1;
				int koniec = i - l;
				if (start < wym)
					obr.ustaw_dane(x + dx * start, y + dy * start, obr.KROPKA);
				if (koniec >= 0)
					obr.ustaw_dane(x + dx * koniec, y + dy * koniec, obr.KROPKA);
			}
			if (l <= puste)
			{
				pewniak = false;
			}
			i += 1;
			if (++akt >= max_liczba)
			{
				break;
			}
			l = liczby[akt];
		}
	}


	akt = max_liczba - 1;
	l = liczby[akt];
	pewniak = true;
	w = 0;


	for (int i = wym - 1; i >= 0; --i)
	{
		if (bufor[i] == akt)
		{
			obr.ustaw_dane(x + dx*i, y + dy*i, obr.WYPELNIONE);
			pewniak = true;
		}
		uchar p = obr.pobierz_dane(x + dx * i, y + dy * i);
		if (p == Obrazek::WYPELNIONE)
		{
			w += 1;
			
		}
		else if (p == Obrazek::KROPKA)
		{
			w = 0;
		}
		else
		{
			if (pewniak)
			{
				uchar pop = obr.pobierz_dane(x + dx * (i+1), y + dy * (i+1));
				switch (pop)
				{
				case Obrazek::PUSTE:
					pewniak = false;
					break;
				case Obrazek::WYPELNIONE:
					obr.ustaw_dane(x + dx * i, y + dy * i, obr.WYPELNIONE);
					w += 1;
					break;
				default:
					pewniak = false;
					break;
				}
				
			}
			else
			{
				w = 0;
			}
			
		}


		if (--l == 0) // zmiana liczby
		{
			l = liczby[akt];
			if (w == l && pewniak) // liczba zrobiona
			{
				int start = i - 1;
				int koniec = i + l;
				if (start >= 0)
					obr.ustaw_dane(x + dx * start, y + dy * start, obr.KROPKA);
				if (koniec < wym)
					obr.ustaw_dane(x + dx * koniec, y + dy * koniec, obr.KROPKA);
			}
			
			if (--akt < 0)
			{
				break;
			}
			l = liczby[akt];
			w = 0;
			i -= 1;
			pewniak = false;
		}

	} 
}

void Metoda_p1::operator()(Obrazek& obr)
{
	for (int i = 0; i < obr.wym_x; ++i)
	{
		pion(obr, i);
	}
	for (int i = 0; i < obr.wym_y; ++i)
	{
		poziom(obr, i);
	}
}


void Metoda_s1::linia(Obrazek& obr, const std::vector<uchar>& liczby, int x, int y, int dx, int dy)
{
	const auto wym = (dx == 0) ? obr.wym_y : obr.wym_x;
	const int max_liczba = liczby.size();
	constexpr int ODL_MAX = 255;
	uchar p;

	std::vector<int> odleglosci(wym, ODL_MAX);
	std::vector<Grupka> grupki;

	Grupka nowa{ 0,0 };
	int odleglosc = ODL_MAX;
	int dl_wyspy = 0;
	
	for (int i = 0; i < wym; ++i)
	{
		p = obr.pobierz_dane(x + dx * i, y + dy * i);
		switch (p)
		{
		case Obrazek::WYPELNIONE:
			if (nowa.dl == 0)
				nowa.start = i;
			nowa.dl += 1;
			odleglosc = 0;
			
			break;
		case Obrazek::KROPKA:
			odleglosc = ODL_MAX;
			//break;
		case Obrazek::PUSTE:
			odleglosc += 1;
			if (nowa.dl > 0)
			{
				grupki.push_back(nowa);
				nowa.dl = 0;
			}
			break;
		}
		odleglosci[i] = odleglosc;
	}
	if (nowa.dl > 0)
	{
		grupki.push_back(nowa);
		nowa.dl = 0;
	}
	/*if (grupki.size() == 0)
		return;*/

	odleglosc = ODL_MAX;

	for (int i = wym - 1; i >= 0; --i)
	{
		p = obr.pobierz_dane(x + dx * i, y + dy * i);
		switch (p)
		{
		case Obrazek::WYPELNIONE:
			odleglosc = 0;
			break;
		case Obrazek::KROPKA:
		case Obrazek::POZA_POLEM:
			odleglosc = ODL_MAX;
			if (dl_wyspy > 0)
			{
				for (int j = i + 1; j <= i + dl_wyspy; ++j)
				{
					odleglosci[j] = dl_wyspy;
				}
				dl_wyspy = 0;
			}
			break;
		case Obrazek::PUSTE:
			odleglosc += 1;
			if (odleglosci[i] >= ODL_MAX)
			{
				dl_wyspy += 1;
			}
			break;
		}
		if (odleglosc < odleglosci[i])
		{
			odleglosci[i] = odleglosc;
		}
	}
	if (dl_wyspy > 0)
	{
		for (int j = 0; j < dl_wyspy; ++j)
		{
			odleglosci[j] = dl_wyspy;
		}
	}
	/////////////////////////////////////////////
	
	std::vector<int> starty(max_liczba, 0);
	Grupka g;
	int max_g = grupki.size();
	std::vector<uchar> lewa(wym, 0xff);
	std::vector<uchar> prawa(wym, 0xff);
	
	int kierunek = 1;

	auto dodaj = [&liczby, &starty,&kierunek](std::vector<uchar>& bufor, int nr, int start)
	{
		starty[nr] = start;
		for (int i = 0; i < liczby[nr]; ++i)
		{
			bufor[start + i * kierunek] = nr;
		}
	};
	auto usun = [&liczby, &starty, &kierunek](std::vector<uchar>& bufor, int nr)
	{
		int start = starty[nr];
		for (int i = 0; i < liczby[nr]; ++i)
		{
			auto& tmp = bufor[start + i * kierunek];
			if(tmp == nr)
				tmp = 0xff;
		}
	};

	// 1 - niezaznaczone wype³nione pole
	// 2 - zaznaczona kropka
	int spr_typ;
	int spr_index;
	int spr_nr;
	auto sprawdz = [&obr, x,y,dx,dy, max_liczba, &spr_index, &spr_typ, &kierunek,&spr_nr, wym](std::vector<uchar>& bufor)
	{

		// sprawdzenie
		int i = (kierunek == 1) ? 0 : wym - 1;
		int nr = (kierunek == 1) ? 0 : max_liczba - 1;
		while (i >= 0 && i < wym)
		{
			auto p = obr.pobierz_dane(x + dx * i, y + dy * i);
			auto bi = bufor[i];
			if (bi != 0xff)
			{
				// poprawne przypadki -> bi - nr == 0 lub kierunek
				if (nr != bi)
				{
					if (bi - nr == kierunek)
					{
						nr += kierunek;
					}
					else
					{
						spr_index = i;
						spr_nr = nr; // ostatni dobry
						spr_typ = 3;
						return false;
					}
				}
			}
		
			if (p == Obrazek::WYPELNIONE)
			{
				if (bi == 0xff)
				{
					spr_index = i;
					spr_typ = 1;
					return false;
				}
			}
			else if (p == Obrazek::KROPKA)
			{
				if (bi != 0xff)
				{
					spr_index = i;
					spr_typ = 2;
					spr_nr = bi;
					return false;
				}
			}
			i += kierunek;
		}
		return true;
	};
	
	// LEWA
	auto it = 0;
	for (int i = 0; i < max_liczba; ++i)
	{
		int l = liczby[i];
		starty[i] = it;
		while (l-- > 0)
		{
			lewa[it++] = i;
		}
		++it;
	}


	int nr{ 0 }, nr_g, nowy_start{ 0 },l;
	while (!sprawdz(lewa))
	{
		if (spr_typ == 1) // niezaznaczone wype³nione pole
		{

			nr = max_liczba - 1;
			nr_g = max_g - 1;
			while (grupki[nr_g].start > spr_index) nr_g -= 1;
			auto g = grupki[nr_g];
			while (starty[nr] > spr_index || liczby[nr] < g.dl) nr -= 1; // mo¿e powodowaæ przestawienie liczb

			nowy_start = g.start + g.dl - liczby[nr];
		}
		else if (spr_typ == 2) // zaznaczona kropka
		{
			nr = spr_nr;
			nowy_start = spr_index + 1;
		}
		else if (spr_typ == 3)
		{

		}
		/*l = liczby[nr];
		liczby_do_wpisania.push(nr);
		usun(lewa, nr);
		starty[nr] = nowy_start;
		while (nr + 1 < max_liczba && nowy_start + l >= starty[nr + 1])
		{
			nr += 1;
			liczby_do_wpisania.push(nr);
			usun(lewa, nr);
			starty[nr] = starty[nr - 1] + l + 1;
			l = liczby[nr];
		}*/

		do
		{
			l = liczby[nr];
			usun(lewa, nr);
			dodaj(lewa, nr, nowy_start);
			nowy_start += l + 1;
			nr += 1;
		} while (nr < max_liczba && nowy_start > starty[nr]);

		/*dodaj(lewa, nr, nowy_start);
		

		int i = starty[nr] + liczby[nr];
		while (i < wym && lewa[i] != 0xff)
		{
			nr = lewa[i];
			int dl = liczby[nr];
			lewa[i] = 0xff;
			lewa[i + dl] = nr;
			starty[nr] += 1;
			i = i + dl + 1;
		}*/

		
	}

	// PRAWA
	kierunek = -1;
	it = wym - 1;
	for (int i = max_liczba - 1; i >= 0; --i)
	{
		int l = liczby[i];
		starty[i] = it;
		while (l-- > 0)
		{
			prawa[it--] = i;
		}
		--it;
	}
	if (liczby.size() == 3 && liczby[0] == 2)
	{
		liczby[0]+ 1;
	}
	while (!sprawdz(prawa))
	{
		if (spr_typ == 1) // niezaznaczone wype³nione pole
		{
			nr = 0;
			nr_g = 0;
			while (grupki[nr_g].ostatni() < spr_index) nr_g += 1;
			auto g = grupki[nr_g];
			while (starty[nr] < spr_index || liczby[nr] < g.dl) nr += 1;
			
			nowy_start = g.start + liczby[nr] - 1;
		}
		else if (spr_typ == 2) // zaznaczona kropka
		{
			nr = spr_nr;
			nowy_start = spr_index - 1;
		}

		do
		{
			l = liczby[nr];
			usun(prawa, nr);
			dodaj(prawa, nr, nowy_start);
			nowy_start -= l + 1;
			nr -= 1;
		} while (nr >= 0 && nowy_start < starty[nr]);

		/*usun(prawa, nr);
		dodaj(prawa, nr, nowy_start);
		starty[nr] = nowy_start;

		int i = starty[nr] - liczby[nr];
		while (i >= 0 && prawa[i] != 0xff)
		{
			nr = prawa[i];
			int dl = liczby[nr];
			prawa[i] = 0xff;
			prawa[i - dl] = nr;
			starty[nr] -= 1;
			i = i - dl - 1;
		}*/
	}

	std::queue<uchar> otwarte;
	for (int i = 0; i < wym; ++i)
	{
		auto l = lewa[i];
		auto p = prawa[i];
		if (l == 0xff && p == 0xff && otwarte.empty())
		{
			obr.ustaw_dane(x + dx * i, y + dy * i, Obrazek::KROPKA);
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
			obr.ustaw_dane(x + dx * i, y + dy * i, Obrazek::WYPELNIONE);
		}
	}
}

void Metoda_s1::operator()(Obrazek& obr)
{
	for (int i = 0; i < obr.wym_x; ++i)
	{
		pion(obr, i);
	}
	for (int i = 0; i < obr.wym_y; ++i)
	{
		poziom(obr, i);
	}
}
