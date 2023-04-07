#pragma once
#include"Obrazek.h"
class Metoda
{
protected:
	int x, y, dx, dy;
	const uchar& pobierz_dane_linia(const Obrazek& obr,int i) const;
	void poziom(Obrazek& obr, int y);
	void pion(Obrazek& obr, int x);
	virtual void linia(Obrazek& obr, const std::vector<uchar>& liczby, int x, int y, int dx, int dy) {};
public:
	virtual void operator()(Obrazek& obr) {};

};

// pewniaki
class Metoda_p1 : public Metoda
{
	void linia(Obrazek& obr, const std::vector<uchar>& liczby, int x, int y, int dx, int dy) override;
public:
	void operator()(Obrazek& obr) override;
};

// sprawdzanie - wype³nienie kropkami
class Metoda_s1 : public Metoda
{
	struct Grupka
	{
		int start;
		int dl;
		inline int ostatni() { return start + dl - 1; }
	};
	void linia(Obrazek& obr, const std::vector<uchar>& liczby, int x, int y, int dx, int dy) override;
public:
	void operator()(Obrazek& obr) override;
};