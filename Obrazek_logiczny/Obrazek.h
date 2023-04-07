#pragma once
#include "Obrazek_baza.h"
#include<vector>
#include<string>
class Obrazek : public Obrazek_baza
{
public:
	void ustaw_wymiary(uint x, uint y);

	Obrazek() = default;
	Obrazek(uint x, uint y) { ustaw_wymiary(x, y); }

	void wczytaj_dane(int i, uchar* wiersz, int n = 0);
	void wyczysc_dane();
	void akt_liczby();
	const uchar& pobierz_dane(int x, int y) const; 
	void ustaw_dane(int x, int y, uchar wart); 
	std::string pobierz_dane_string(int x, int y) const;
};

