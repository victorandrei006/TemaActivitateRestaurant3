#ifndef RESTAURANT_H
#define RESTAURANT_H

#include <vector>
#include <memory>
#include <string>
#include "angajati.h"
#include "rezervari.h"
#include "observator.h"

class Restaurant {
private:
    std::string numeRestaurant;
    std::vector<std::shared_ptr<Angajat>> echipa;
    bool mese_disponibile[5][5];
    SistemNotificari sistemNotificari;

public:
    Restaurant(std::string nume);

    void adaugaAngajat(std::shared_ptr<Angajat> angajatNou);
    void afiseazaEchipa() const;
    void incepeZiuaDeMunca() const;
    bool gasesteMasaLibera();
    void primesteRezervare(Rezervari& rezervare);
    void aboneazaObservator(IObservator* obs);
    int getMeseOcupate()  const;
    int getMeseTotale()   const;
    std::shared_ptr<Ospatar> gasestOspatar(const std::string& nume) const;
    void promoveazaAngajat(const std::string& nume);
    void afiseazaHartaMese() const;
    const std::string getNumeRestaurant() const;
};

#endif