#pragma once

#include<vector>

using uchar = unsigned char;
using uint = unsigned int;

class Linia
{
public:
	enum Stan
	{
		NIEZMIENIONY,
		ZMIENIONY,
		SKONCZONY
	};
	enum Kierunek
	{
		KOLUMNA,
		WIERSZ
	};
	enum TYP_POLA
	{
		NIEZNANE,
		PUSTE,
		WYPELNIONE,


		POZA_POLEM
	};

	std::vector<uchar> liczby;
	std::vector<uchar> dane;
	Stan stan{ Stan::NIEZMIENIONY };
	Kierunek kierunek;
	int nr;

	Linia(const std::vector<uchar>& _liczby, size_t _rozmiar, Kierunek _kierunek, int _nr);
	Linia(std::vector<uchar>&& _liczby, size_t _rozmiar, Kierunek _kierunek, int _nr);

	// i - [0, n)
	bool ustaw0(int i, uchar w);
	// i - [1, n]
	bool ustaw1(int i, uchar w);
	void spr_koniec();

	void wyczysc_dane();
	size_t akt_liczby();
	// resetuje na zmienione stany, zostawia skoñczone
	void reset_stanu();

	// [0,n)
	uchar operator[] (int i) const;

	// [1,n]
	void akt_dane(const std::vector<uchar>& nowe);

	// [p,...]
	uchar* pobierz_dane_wsk(int przesuniecie = 1);

	// [1,n]
	std::vector<uchar> pobierz_dane_kopia() const;

};
