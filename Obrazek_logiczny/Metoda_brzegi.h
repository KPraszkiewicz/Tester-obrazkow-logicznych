#pragma once
#include "Metoda.h"
#include "Metoda_perwniaki.h"

class Metoda_brzegi
{
public:
    struct Zalozenie
    {
        bool kolumna;
        int index;

        size_t start;
        int delta;
    };
    bool operator()(Obrazek_baza2& obr, int odl = 0, int kroki=2);

  
    std::vector<Zalozenie> pobierz_kierunki(const Obrazek_baza2& obr, int odl = 0) const;
};

