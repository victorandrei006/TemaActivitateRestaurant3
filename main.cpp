#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <memory>
#include "angajati.h"
#include "restaurant.h"
#include "rezervari.h"
#include "meniu.h"
#include "exceptiiRestaurant.h"
#include "jurnal.h"
#include "observator.h"
#include "depozit.h"

using namespace std;

void incarcaAngajatiDinFisier(Restaurant &restaurant, const string &numeFisier)
{
    ifstream fisier(numeFisier);
    if (!fisier.is_open())
    {
        cerr << "[Avertisment] Nu s-a putut gasi/deschide fisierul: " << numeFisier << "\n";
        return;
    }
    string linie;
    int contor = 0;
    while (getline(fisier, linie))
    {
        stringstream ss(linie);
        string tip, nume, prenume, varsta_str, salariu_str, extra;
        getline(ss, tip, ',');
        getline(ss, nume, ',');
        getline(ss, prenume, ',');
        getline(ss, varsta_str, ',');
        getline(ss, salariu_str, ',');
        getline(ss, extra, ',');
        try
        {
            int varsta = stoi(varsta_str);
            int salariu = stoi(salariu_str);
            if (tip == "Ospatar")
                restaurant.adaugaAngajat(make_shared<Ospatar>(nume.c_str(), prenume.c_str(), varsta, salariu));
            else if (tip == "Bucatar")
                restaurant.adaugaAngajat(make_shared<Bucatar>(nume.c_str(), prenume.c_str(), varsta, salariu, extra));
            else if (tip == "Manager")
                restaurant.adaugaAngajat(make_shared<Manager>(nume.c_str(), prenume.c_str(), varsta, salariu, stoi(extra)));
            contor++;
        }
        catch (const RestaurantException &e)
        {
            cerr << "Eroare domeniu la " << nume << ": " << e.what() << "\n";
        }
        catch (const exception &)
        {
            cerr << "Eroare parsare la linia: " << linie << "\n";
        }
    }
    cout << "[Sistem] S-au incarcat " << contor << " angajati din fisier.\n";
}

void incarcaRezervariDinFisier(Depozit<Rezervari> &depozit, const string &numeFisier)
{
    ifstream fisier(numeFisier);
    if (!fisier.is_open())
    {
        cerr << "[Avertisment] Nu s-a putut deschide fisierul: " << numeFisier << "\n";
        return;
    }
    string linie;
    int contor = 0;
    while (getline(fisier, linie))
    {
        stringstream ss(linie);
        string nume, prenume, pers_str, ora_str;
        getline(ss, nume, ',');
        getline(ss, prenume, ',');
        getline(ss, pers_str, ',');
        getline(ss, ora_str, ',');
        try
        {
            depozit.adauga(Rezervari(nume.c_str(), prenume.c_str(), stoi(pers_str), stoi(ora_str), false));
            contor++;
        }
        catch (const exception &e)
        {
            cerr << "Eroare parsare rezervare: " << linie << "\n";
        }
    }
    cout << "[Sistem] S-au incarcat " << contor << " rezervari in asteptare.\n";
}

void incarcaMeniuDinFisier(Depozit<shared_ptr<Meniu>> &depozit, const string &numeFisier)
{
    ifstream fisier(numeFisier);
    if (!fisier.is_open())
    {
        cerr << "[Avertisment] Nu s-a putut deschide: " << numeFisier << "\n";
        return;
    }
    string linie;
    int contor = 0;
    while (getline(fisier, linie))
    {
        stringstream ss(linie);
        string tip, nume, pret_str, disp_str, alergeni_str, special;
        getline(ss, tip, ',');
        getline(ss, nume, ',');
        getline(ss, pret_str, ',');
        getline(ss, disp_str, ',');
        getline(ss, alergeni_str, ',');
        getline(ss, special, ',');
        try
        {
            double pret = stod(pret_str);
            bool disp = stoi(disp_str);
            bool alerg = stoi(alergeni_str);
            if (tip == "Normal")
                depozit.adauga(make_shared<MeniuNormal>(nume.c_str(), pret, disp, alerg, special));
            else if (tip == "Vegan")
                depozit.adauga(make_shared<MeniuVegan>(nume.c_str(), pret, disp, alerg, special));
            contor++;
        }
        catch (...)
        {
            cerr << "Eroare la parsarea liniei: " << linie << "\n";
        }
    }
    cout << "[Sistem] S-au incarcat " << contor << " preparate in meniu.\n";
}

int main()
{
    Restaurant restaurant("Bistro FMI");
    cout << "=== PORNIRE SISTEM " << restaurant.getNumeRestaurant() << " ===\n";

    NotificatorManager notifManager("Ion Popescu");
    NotificatorOspatar notifOspatar("Maria Ionescu");
    restaurant.aboneazaObservator(&notifManager);
    restaurant.aboneazaObservator(&notifOspatar);
    JurnalEvenimente::getInstanta()->logheaza("Sistem pornit: " + restaurant.getNumeRestaurant());

    Depozit<Rezervari> rezervariInAsteptare("Lista Asteptare", 50);
    Depozit<shared_ptr<Meniu>> meniuRestaurant("Meniu Restaurant", 200);

    incarcaAngajatiDinFisier(restaurant, "angajati.csv");
    incarcaRezervariDinFisier(rezervariInAsteptare, "rezervari.csv");
    incarcaMeniuDinFisier(meniuRestaurant, "meniu.csv");

    int optiune = -1;
    do
    {
        cout << "\n====================================\n";
        cout << "    MENIU MANAGEMENT " << restaurant.getNumeRestaurant() << "\n";
        cout << "====================================\n";
        cout << "1.  Afiseaza echipa curenta\n";
        cout << "2.  Adauga o rezervare manuala\n";
        cout << "3.  Proceseaza rezervarile in asteptare\n";
        cout << "4.  Incepe ziua de munca\n";
        cout << "5.  Afiseaza detalii statistice\n";
        cout << "6.  Afiseaza tot meniul restaurantului\n";
        cout << "7.  Modifica pretul unui produs\n";
        cout << "8.  Schimba disponibilitatea unui produs\n";
        cout << "9.  Afiseaza jurnalul de evenimente\n";
        cout << "10. Statistici avansate\n";
        cout << "0.  Iesire program\n";
        cout << "------------------------------------\n";
        cout << "Alege o actiune: ";

        cin >> optiune;
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(10000, '\n');
            optiune = -1;
        }

        switch (optiune)
        {
        case 1:
            restaurant.afiseazaEchipa();
            break;

        case 2:
        {
            string nume, prenume;
            int pers, ora;
            cout << "Introdu Nume client: ";
            cin >> nume;
            cout << "Introdu Prenume client: ";
            cin >> prenume;
            cout << "Introdu Nr. Persoane: ";
            cin >> pers;
            cout << "Introdu Ora (ex: 19): ";
            cin >> ora;
            try
            {
                rezervariInAsteptare.adauga(Rezervari(nume.c_str(), prenume.c_str(), pers, ora, false));
                cout << "Rezervare adaugata cu succes in asteptare!\n";
            }
            catch (const overflow_error &e)
            {
                cerr << "Eroare: " << e.what() << "\n";
            }
            break;
        }

        case 3:
        {
            if (rezervariInAsteptare.esteGol())
            {
                cout << "Nu exista rezervari in asteptare!\n";
                break;
            }
            cout << "\nProcesam " << rezervariInAsteptare.size() << " rezervari...\n";
            for (auto &rez : rezervariInAsteptare)
            {
                try
                {
                    restaurant.primesteRezervare(rez);
                }
                catch (const RestaurantException &e)
                {
                    cerr << "!!! " << e.what() << " !!!\n";
                }
            }
            rezervariInAsteptare.goleste();
            break;
        }

        case 4:
            restaurant.incepeZiuaDeMunca();
            break;

        case 5:
            Angajat::afiseazaNumarTotalAngajati();
            break;

        case 6:
        {
            if (meniuRestaurant.esteGol())
            {
                cout << "Meniul este gol!\n";
                break;
            }
            cout << "\n=== MENIUL RESTAURANTULUI ===\n";
            for (int i = 0; i < meniuRestaurant.size(); i++)
                meniuRestaurant.get(i)->getAllMeniu();
            break;
        }

        case 7:
        {
            cout << "Introdu numele produsului: ";
            string numeCautat;
            cin.ignore();
            getline(cin, numeCautat);

            int idx = cautaIndex(meniuRestaurant,
                                 [&](const shared_ptr<Meniu> &m)
                                 {
                                     return string(m->getNumeProdus()) == numeCautat;
                                 });

            if (idx == -1)
            {
                cout << "Produsul nu a fost gasit.\n";
                break;
            }

            cout << "Pretul actual: " << meniuRestaurant.get(idx)->getPret() << " RON. Noul pret: ";
            double pretNou;
            cin >> pretNou;
            meniuRestaurant.get(idx)->setPret(pretNou);
            JurnalEvenimente::getInstanta()->logheaza(
                "Pret modificat: " + numeCautat + " -> " + to_string(pretNou) + " RON");
            cout << "Pret actualizat cu succes!\n";
            break;
        }

        case 8:
        {
            cout << "Introdu numele produsului: ";
            string numeCautat;
            cin.ignore();
            getline(cin, numeCautat);

            int idx = cautaIndex(meniuRestaurant,
                                 [&](const shared_ptr<Meniu> &m)
                                 {
                                     return string(m->getNumeProdus()) == numeCautat;
                                 });

            if (idx == -1)
            {
                cout << "Produsul nu a fost gasit.\n";
                break;
            }

            bool stareNoua = !meniuRestaurant.get(idx)->getDisponibilitate();
            meniuRestaurant.get(idx)->setDisponibilitate(stareNoua);
            cout << "Disponibilitate schimbata in: " << (stareNoua ? "Da" : "Nu") << "\n";
            JurnalEvenimente::getInstanta()->logheaza(
                "Disponibilitate modificata: " + numeCautat);
            break;
        }

        case 9:
            JurnalEvenimente::getInstanta()->afiseazaJurnal();
            break;

        case 10:
        {
            cout << "\n=== STATISTICI AVANSATE ===\n";

            double totalPersoane = calculeazaSuma(rezervariInAsteptare,
                                                  [](const Rezervari &r)
                                                  { return (double)r.getNumarPersoane(); });
            cout << "Total persoane in lista de asteptare: " << totalPersoane << "\n";

            double totalMeniu = calculeazaSuma(meniuRestaurant,
                                               [](const shared_ptr<Meniu> &m)
                                               { return m->getPret(); });
            cout << "Valoarea totala a meniului: " << totalMeniu << " RON\n";

            cout << "\nIntrodu un nume client de cautat in lista de asteptare: ";
            string numeCautat;
            cin.ignore();
            getline(cin, numeCautat);
            int idxRez = cautaIndex(rezervariInAsteptare,
                                    [&](const Rezervari &r)
                                    { return string(r.getNumeClient()) == numeCautat; });
            if (idxRez != -1)
            {
                cout << "Rezervare gasita la pozitia " << idxRez << ":\n";
                rezervariInAsteptare.get(idxRez).getAllRezervari();
            }
            else
            {
                cout << "Nicio rezervare gasita pentru clientul: " << numeCautat << "\n";
            }

            cout << "Jurnal contine " << JurnalEvenimente::getInstanta()->getNrEvenimente()
                 << " evenimente inregistrate.\n";
            break;
        }

        case 0:
            JurnalEvenimente::getInstanta()->logheaza("Sistem oprit.");
            cout << "\nSe inchide sistemul... La revedere!\n";
            break;

        default:
            cout << "\nOptiune invalida! Incearca din nou.\n";
            break;
        }
    } while (optiune != 0);

    return 0;
}