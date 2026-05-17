#ifndef RESTAURANT_H
#define RESTAURANT_H

#include <vector>
#include <memory>
#include <string>
#include "angajati.h"
#include "rezervari.h"

class Restaurant {
private:
    std::string numeRestaurant;
    std::vector<std::shared_ptr<Angajat>> echipa;
    bool mese_disponibile[5][5];

public:
    // Constructor
    Restaurant(std::string nume);

    void adaugaAngajat(std::shared_ptr<Angajat> angajatNou);
    void afiseazaEchipa() const;
    void incepeZiuaDeMunca() const;
    bool gasesteMasaLibera();
    void primesteRezervare( Rezervari& rezervare);

    // Getteri
    const std::string getNumeRestaurant() const;
};

#endif