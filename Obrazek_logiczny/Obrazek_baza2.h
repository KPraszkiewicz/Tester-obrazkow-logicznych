#pragma once

#include<vector>
#include"Linia.h"
#include<stdexcept>

class Obrazek_baza2
{
protected:

public:
	std::vector<Linia> wiersze; // Y
	std::vector<Linia> kolumny; // X
	std::vector<std::vector<int>> etapy; // X,Y
	int etap{};

	size_t max_k, max_w;
	size_t wym_x, wym_y;

	Obrazek_baza2(size_t wx, size_t wy) { init(wx, wy); }
	void init(size_t wx, size_t wy);
	void wyczysc_dane();
	void akt_liczby();


	uchar pobierz_dane(int x, int y) const;
	void ustaw_dane(int x, int y, uchar nowa); // od 0
	void ustaw_dane(bool kolumna, int i, int poz, uchar nowa); // i - [0,n), poz - [1,n]
	void ustaw_dane(bool kolumna, int i, const std::vector<uchar>& nowa);
	const Linia& pobierz_linie(bool kolumna, int index) const;

	// etapy
	void ustaw_etapy(bool kolumna, int i, const std::vector<uchar>& nowa);
	int pobierz_etap(int x, int y) const;
	void cofnij_do_etapu(int docelowy_etap);
	void akt_stany(int od_etapu);
	void reset_stanow();
	bool czy_zmieniono_stan() const;

	void synchronizuj();
	void synchronizuj_wiersze();
	void synchronizuj_wszystkie_wiersze();
	void synchronizuj_kolumny();
	void synchronizuj_wszystkie_kolumny();
};