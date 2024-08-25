#pragma once
#include "Obrazek_baza.h"
#include<vector>
#include<string>
#include <exception>
struct Grupka
{
	int start;
	int dl;
	inline int ostatni() { return start + dl - 1; }
};



class Obrazek : public Obrazek_baza
{
	std::vector<std::vector<int>> etapy;
public:
	int etap{};

	void ustaw_wymiary(uint x, uint y);

	Obrazek() = default;
	Obrazek(uint x, uint y) { ustaw_wymiary(x, y); }

	void wyczysc_dane();
	void akt_liczby();
	const uchar& pobierz_dane(int x, int y) const;
	const uchar& pobierz_dane(int i, bool dx, bool dy, int j) const;
	void ustaw_dane(int x, int y, uchar wart); 

	std::string pobierz_dane_string(int x, int y) const;

	std::vector<Grupka> pobierz_grupki_kolumna(int x) const;
	std::vector<Grupka> pobierz_grupki_wiersz(int y) const;

	std::vector<uchar> pobierz_linie(int i, bool dx, bool dy) const;
	bool akt_linie(int i, bool dx, bool dy, const std::vector<uchar>& linia);

	const std::vector<uchar>& pobierz_liczby(int i, bool dx, bool dy) const;

	// etapy
	int pobierz_etap(int x, int y) const;
	void cofnij_do_etapu(int docelowy_etap);
};

