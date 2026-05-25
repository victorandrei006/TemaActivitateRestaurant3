#ifndef FACTURA_H
#define FACTURA_H

#include <string>
#include <vector>
#include <iostream>
#include "strategie.h"

struct LinieProdus
{
    std::string numeProdus;
    double pret;
    int cantitate;
};

class Factura
{
private:
    static int contor;
    int numarFactura;
    std::string numeClient;
    std::string numeOspatar;
    int nrPersoane;
    int oraRezervare;
    std::vector<LinieProdus> produse;
    double totalBrut;
    double discountValoare;
    double totalNet;
    std::string descriereStrategie;
    bool platita;

    void linieBon(const std::string &stanga, const std::string &dreapta,
                  int latimeTotala = 44) const;

public:
    Factura(const std::string &client, const std::string &ospatar,
            int pers, int ora);

    void adaugaProdus(const std::string &nume, double pret, int cantitate = 1);
    void calculeazaTotal(const IStrategieDiscount &strategie);
    void afiseaza() const;
    void marcheazaPlatita();

    bool estePlatita() const;
    bool areProduseAdaugate() const;
    double getTotalNet() const;
    double getTotalBrut() const;
    int getNumar() const;
    const std::string &getNumeClient() const;
    const std::string &getNumeOspatar() const;
};

#endif