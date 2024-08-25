#pragma once
#include "Obrazek_baza2.h"


class Metoda_perwniaki
{
	void linia(std::vector<uchar>& linia, const std::vector<uchar>& liczby);
	
public:
	void operator()(Obrazek_baza2& obr);

	void wiersze(Obrazek_baza2& obr);
	void kolumny(Obrazek_baza2& obr);

};

