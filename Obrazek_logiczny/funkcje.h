#pragma once

#include <assert.h>
#include <stdio.h>
#include <io.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <cstring>
#include <filesystem>
#include <SFML/Graphics.hpp>
#include <span>

#include "Obrazek.h"
#include "Obrazek_baza2.h"

void rysuj_obrazek(const Obrazek& obrazek);

Obrazek wczytaj_obrazek(const std::string& nazwa);
Obrazek_baza2 wczytaj_obrazek2(const std::string& nazwa);

// domyœlnie odleg³oœci od lewej
std::vector<int> wyznacz_odleglosci(const std::vector<uchar>& linia);

std::vector<int> wyznacz_odleglosci_lewa(const std::vector<uchar>& linia);

std::vector<int> wyznacz_odleglosci_prawa(const std::vector<uchar>& linia);

std::vector<int> wyznacz_dlugosci(const std::vector<uchar>& linia);

void usun_cudzyslowy(std::string& nazwa);