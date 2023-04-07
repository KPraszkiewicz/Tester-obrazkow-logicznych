// Obrazek_logiczny.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//
#include "funkcje.h"


#ifndef MAX_PATH
#define MAX_PATH	260
#endif

#include"Obrazek.h"
#include "Obrazek_rysunek.h"
#include "Metoda.h"


int main(int argc, char* argv[])
{
	Obrazek_rysunek::init_ft();
	init_FreeImage();
	std::string nazwa_obrazka;
	int ilosc_iteracji = 10;
	std::cout << "START!" << std::endl;
	if (argc > 1)
	{
		nazwa_obrazka = argv[1];
	}
	else
	{
		std::cout << "Podaj nazwe pliku: ";
		std::cin >> nazwa_obrazka;
		std::cout << "Podaj ilosc iteracji: ";
		std::cin >> ilosc_iteracji;
	}
	Obrazek obrazek = wczytaj_obrazek(nazwa_obrazka);
	obrazek.akt_liczby();
	obrazek.wyczysc_dane();
////////////////////////////////////////////////////////////////////////////////////////////////

	
	Metoda_p1 m1; 
	Metoda_s1 m2;
	Obrazek_rysunek obrr;
	std::filesystem::create_directories("./tmp/");
	for (int i = 0; i < ilosc_iteracji; i++)
	{
		m2(obrazek);
		rysuj_obrazek(obrazek);
		obrr.gen_obrazek(obrazek);
		obrr.odwroc_y();
		obrr.export_obr("tmp/test" + std::to_string(i + 1) + ".bmp");
	}
	//gen_obr_test();
	
	getchar();
////////////////////////////////////////////////////////////////////////////////////////////////
// 

	Obrazek_rysunek::free_ft();
	free_FreeImage();
	return 0;
}
