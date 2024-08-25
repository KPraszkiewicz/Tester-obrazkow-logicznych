#include "Metoda_brzegi.h"
#include<iostream>
bool Metoda_brzegi::operator()(Obrazek_baza2& obr, int odl, int kroki)
{
    Metoda_perwniaki pewniaki;
    auto zalozenia{ pobierz_kierunki(obr, odl) };
    int start_etap{ obr.etap + 1 };

    for (int i = 0; i < zalozenia.size(); ++i)
    {
        auto& zalozenie{ zalozenia[i] };
        int etap{ obr.etap };
        size_t start{ zalozenie.start };
        auto& linia{ obr.pobierz_linie(zalozenie.kolumna, zalozenie.index) };
        /*while (zalozenie.linia->stan != Linia::Stan::SKONCZONY)
        {*/
        try
        {
            ++obr.etap;
            obr.ustaw_dane(zalozenie.kolumna, zalozenie.index, start, Linia::WYPELNIONE);

            obr.synchronizuj();
            if (zalozenie.kolumna)
            {
                pewniaki.kolumny(obr);
                pewniaki.wiersze(obr);
                pewniaki.kolumny(obr);
            }
            else
            {
                pewniaki.wiersze(obr);
                pewniaki.kolumny(obr);
                pewniaki.wiersze(obr);
            }

            obr.cofnij_do_etapu(etap);
            obr.etap = etap;
            //break;
        }
        catch (...)
        {
            obr.cofnij_do_etapu(etap);
            obr.etap += 1;
            obr.ustaw_dane(zalozenie.kolumna, zalozenie.index, start, Linia::PUSTE);
            obr.synchronizuj();
            start += zalozenie.delta;
        }
        
        //}
    }

    obr.akt_stany(start_etap);
    return true;
}

/*
  +----------+
  |  gl  gp  |
  |lg      pg|
  |          |
  |ld      pd|
  |  dl  dp  |
  +----------+
  */

std::vector<Metoda_brzegi::Zalozenie> Metoda_brzegi::pobierz_kierunki(const Obrazek_baza2& obr, int odl) const 
{
    std::vector<Zalozenie> kierunki;
    kierunki.reserve(8);
    int odl2;

    odl2 = obr.wym_y - odl - 1;
    if (auto& wiersz{ obr.wiersze[odl] }; wiersz.stan != Linia::Stan::SKONCZONY)
    {
        kierunki.push_back(Zalozenie{ false, odl, 1, 1 });
        kierunki.push_back(Zalozenie{ false, odl, obr.wym_x, -1 });
    }
    if (auto& wiersz{ obr.wiersze[odl2] }; wiersz.stan != Linia::Stan::SKONCZONY)
    {
        kierunki.push_back(Zalozenie{ false, odl2, 1, 1 });
        kierunki.push_back(Zalozenie{ false, odl2, obr.wym_x, -1 });
    }
    odl2 = obr.wym_x - odl - 1;
    if (auto& kolumna{ obr.kolumny[odl] }; kolumna.stan != Linia::Stan::SKONCZONY)
    {
        kierunki.push_back(Zalozenie{ true, odl, 1, 1 });
        kierunki.push_back(Zalozenie{ true, odl, obr.wym_y, -1 });
    }
    if (auto& kolumna{ obr.kolumny[odl2] }; kolumna.stan != Linia::Stan::SKONCZONY)
    {
        kierunki.push_back(Zalozenie{ true, odl2, 1, 1 });
        kierunki.push_back(Zalozenie{ true, odl2, obr.wym_y, -1 });
    }

    for (auto& kier: kierunki)
    {
        auto& linia{ obr.pobierz_linie(kier.kolumna, kier.index) };
        while (linia.dane[kier.start] != Linia::NIEZNANE)
        {
            kier.start += kier.delta;
        }
    }

    return kierunki;
}
