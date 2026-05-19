#include "restaurant.h"
#include "exceptiiRestaurant.h"
#include "jurnal.h"
#include <iostream>
using namespace std;

Restaurant::Restaurant(string nume) : numeRestaurant(nume)
{
    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 5; j++)
            mese_disponibile[i][j] = true;
}

void Restaurant::adaugaAngajat(shared_ptr<Angajat> angajatNou)
{
    if (angajatNou)
    {
        echipa.push_back(angajatNou);
        JurnalEvenimente::getInstanta()->logheaza(
            "Angajat adaugat: " + string(angajatNou->getNume()) +
            " " + string(angajatNou->getPrenume()));
        cout << "Angajatul " << angajatNou->getNume()
             << " a fost adaugat in echipa restaurantului " << numeRestaurant << ".\n";
    }
}

bool Restaurant::gasesteMasaLibera()
{
    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 5; j++)
            if (mese_disponibile[i][j])
            {
                mese_disponibile[i][j] = false;
                return true;
            }
    return false;
}

void Restaurant::afiseazaEchipa() const
{
    cout << "\n=== Echipa Restaurantului " << numeRestaurant << " ===\n";
    for (const auto &angajat : echipa)
        cout << *angajat << "\n";
    cout << "=======================================\n";
}

void Restaurant::incepeZiuaDeMunca() const
{
    cout << "\n--- Incepe ziua de munca! ---\n";
    JurnalEvenimente::getInstanta()->logheaza("S-a inceput ziua de munca la " + numeRestaurant);
    for (const auto &angajat : echipa)
        angajat->executaSarcina();
    cout << "-----------------------------\n";
}

void Restaurant::primesteRezervare(Rezervari &rezervare)
{
    cout << "\n[Sistem] Cerere rezervare: " << rezervare.getNumeClient() << "\n";

    if (!gasesteMasaLibera())
    {
        JurnalEvenimente::getInstanta()->logheaza(
            "Rezervare REFUZATA (restaurant plin): " + string(rezervare.getNumeClient()));
        throw MasaIndisponibilaException();
    }

    rezervare.setConfirmare(true);

    string mesajLog = "Rezervare confirmata: " + string(rezervare.getNumeClient()) +
                      " | " + to_string(rezervare.getNumarPersoane()) +
                      " pers. | ora " + to_string(rezervare.getOraRezervare());

    JurnalEvenimente::getInstanta()->logheaza(mesajLog);

    sistemNotificari.notificaToti(mesajLog);

    bool alocata = false;
    for (auto &angajat : echipa)
    {
        shared_ptr<Ospatar> ospatar = dynamic_pointer_cast<Ospatar>(angajat);
        if (ospatar && ospatar->getNumarRezervari() < 3)
        {
            ospatar->preiaRezervare(rezervare);
            alocata = true;
            break;
        }
    }

    if (!alocata)
        cout << "Rezervarea e confirmata, dar nu avem ospatar liber sa serveasca!\n";
}

void Restaurant::aboneazaObservator(IObservator *obs)
{
    sistemNotificari.aboneaza(obs);
}

const string Restaurant::getNumeRestaurant() const { return numeRestaurant; }