// Obrazek_logiczny.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//
#include "funkcje.h"

#include "Obrazek_rysunek.h"


#include <SFML/Graphics.hpp>
#include "Metoda_perwniaki.h"
#include "Metoda_brzegi.h"
#include <sstream>
#include <fstream>
#include <filesystem>

int main(int argc, char* argv[])
{
	int ilosc_iteracji = 0;
	std::string zapisy_kat = "test";
	int zapisy_cz = 3;
	bool okno = true;
	int zapisy_i = 0;

	std::filesystem::path program_startowy{ argv[0] };
	const std::string katalog_startowy{ program_startowy.remove_filename().string() };
////////////////////////////////////////////////////////////////////////////////////////////////

	std::ifstream config{ "config.ini" };

	if (!config)
	{
		std::cerr << "ERROR: nie znaleziono pliku konfiguracyjnego (config.ini)!!" << std::endl;
		return getchar();
	}

	std::string line;
	while (std::getline(config, line))
	{
		std::istringstream is_line(line);
		std::string key;
		if (std::getline(is_line, key, '='))
		{
			std::string value;
			if (std::getline(is_line, value))
			{
				std::cout << key << "=" << value << std::endl;
				if (key == "etapy")
				{
					ilosc_iteracji = std::stoi(value);
				}
				else if (key == "widok")
				{
					if (value == "okno")
						okno = true;
					else
						okno = false;
				}
				else if (key == "wyniki")
				{
					zapisy_kat = value;
					if (!std::filesystem::exists(zapisy_kat))
					{
						std::filesystem::create_directory(zapisy_kat);
					}
				}
				else if (key == "zapisy")
				{
					zapisy_cz = std::stoi(value);
					if (zapisy_cz < 1)
					{
						zapisy_cz = 999999;
					}
				}
				else if (key == "");
			}
		}
	}



////////////////////////////////////////////////////////////////////////////////////////////////
	Obrazek_rysunek::init();
	std::string nazwa_obrazka;

	std::cout << "START!" << std::endl;
	if (argc > 1)
	{
		nazwa_obrazka = argv[1];
	}
	else
	{
		std::cout << "Podaj polozenie obrazka (bez polskich znakow): ";
		std::getline(std::cin, nazwa_obrazka);
	}
	
	usun_cudzyslowy(nazwa_obrazka);

	Obrazek_baza2 obrazek2 = wczytaj_obrazek2(nazwa_obrazka);
	Obrazek_rysunek obrr;
	Metoda_brzegi b1;
	Metoda_perwniaki p1;

	if (obrazek2.wym_x % 5 != 0 || obrazek2.wym_y % 5 != 0)
	{
		std::cerr << "Niewlasciwe wymiary obrazka!" << std::endl;
		return getchar();
	}


	obrazek2.akt_liczby();
	obrazek2.wyczysc_dane();
	obrr.gen_obrazek(obrazek2);
	

	while (--ilosc_iteracji > 0)
	{
		std::cout << "Etap: " << obrazek2.etap << std::endl;
		p1(obrazek2);
		if (!obrazek2.czy_zmieniono_stan())
		{
			b1(obrazek2);
			if (!obrazek2.czy_zmieniono_stan())
			{
				b1(obrazek2, 1);
				if (!obrazek2.czy_zmieniono_stan())
				{
					ilosc_iteracji = -1;
					std::cerr << "Nie mozna wykonac ruchu!" << std::endl;
				}
			}
		}
		if (ilosc_iteracji <= 0 or ilosc_iteracji % zapisy_cz == 0)
		{
			obrr.gen_obrazek(obrazek2);
			std::string nazwa{ (zapisy_kat + "/zapis") + std::to_string(++zapisy_i) + ".png"};
			if (!obrr.export_obr(nazwa))
			{
				std::cerr << "Nie udało się zapisać pliku: " << nazwa << std::endl;
			}
		}
	}


	if (!okno)
	{
		Obrazek_rysunek::free_ft();
		return 0;
	}

////////////////////////////////////////////////////////////////////////////////////////////////
	sf::RenderWindow window(sf::VideoMode(obrr.b_wym_x, obrr.b_wym_y), "OBR");

	sf::Texture tex;
	tex.loadFromImage(obrr.bufor);

	sf::Sprite sprite{ tex };

	sprite.setPosition(0, 0);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;

			case sf::Event::Resized:
				//TODO
				break;

			case sf::Event::KeyPressed:
				if (event.key.code == sf::Keyboard::Num1)
				{
					p1(obrazek2);
					obrr.gen_obrazek(obrazek2);
					tex.update(obrr.bufor);
				}
				if (event.key.code == sf::Keyboard::Num2)
				{
					b1(obrazek2);
					obrr.gen_obrazek(obrazek2);
					tex.update(obrr.bufor);
				}
				if (event.key.code == sf::Keyboard::Num3)
				{
					b1(obrazek2,1);
					obrr.gen_obrazek(obrazek2);
					tex.update(obrr.bufor);
				}
				if (event.key.code == sf::Keyboard::T)
				{
					if (Obrazek_rysunek::tryb == 1)
					{
						Obrazek_rysunek::tryb = 0;
					}
					else
					{
						Obrazek_rysunek::tryb = 1;
					}
					obrr.gen_obrazek(obrazek2);
					tex.update(obrr.bufor);
				}
				break;
			default:
				break;
			}

		}
		
		window.clear();
		window.draw(sprite);
		window.display();
	}

////////////////////////////////////////////////////////////////////////////////////////////////


	Obrazek_rysunek::free_ft();
	return 0;
}
