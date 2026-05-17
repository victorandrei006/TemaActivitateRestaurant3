#include "meniu.h"
#include <cstring>
#include <iostream>

using namespace std;

// clasa de baza

Meniu::Meniu() : nume_produs(nullptr), pret(0), disponibilitate(false), alergeni(false) {}

Meniu::Meniu(const char *nume_produs, double pret, bool disponibilitate, bool alergeni) {
    this->nume_produs = new char[strlen(nume_produs) + 1];
    strcpy(this->nume_produs, nume_produs);
    this->pret = pret;
    this->disponibilitate = disponibilitate;
    this->alergeni = alergeni;
}

Meniu::Meniu(const Meniu &meniu) {
    this->nume_produs = new char[strlen(meniu.nume_produs) + 1];
    strcpy(this->nume_produs, meniu.nume_produs);
    this->pret = meniu.pret;
    this->disponibilitate = meniu.disponibilitate;
    this->alergeni = meniu.alergeni;
}

Meniu& Meniu::operator=(const Meniu& m) {
    if (this != &m) {
        delete[] nume_produs;
        this->nume_produs = new char[strlen(m.nume_produs) + 1];
        strcpy(this->nume_produs, m.nume_produs);
        this->pret = m.pret;
        this->disponibilitate = m.disponibilitate;
        this->alergeni = m.alergeni;
    }
    return *this;
}

Meniu::~Meniu() {
    delete[] nume_produs;
}

void Meniu::setNumeProdus(const char *nume_produs_set) {
    delete[] this->nume_produs;
    this->nume_produs = new char[strlen(nume_produs_set) + 1];
    strcpy(this->nume_produs, nume_produs_set);
}

void Meniu::setPret(double pret) { this->pret = pret; }
void Meniu::setDisponibilitate(bool disponibilitate) { this->disponibilitate = disponibilitate; }
void Meniu::setAlergeni(bool alergeni) { this->alergeni = alergeni; }

const char* Meniu::getNumeProdus() const { return this->nume_produs; }
double Meniu::getPret() const { return pret; }
bool Meniu::getDisponibilitate() const { return disponibilitate; }
bool Meniu::getAlergeni() const { return alergeni; }

void Meniu::getAllMeniu() const {
    cout << "Nume: " << (nume_produs ? nume_produs : "N/A") << " | Pret: " << pret << " RON\n";
    cout << "Disponibil: " << (disponibilitate ? "Da" : "Nu") << " | Alergeni: " << (alergeni ? "Da" : "Nu") << "\n";
}

//Meniu Normal

MeniuNormal::MeniuNormal(const char *nume, double pret, bool disp, bool alergeni, string carne)
    : Meniu(nume, pret, disp, alergeni), tipCarne(carne) {}

void MeniuNormal::getAllMeniu() const {
    cout << "--- PREPARAT CLASIC ---\n";
    Meniu::getAllMeniu();
    cout << "Tip carne: " << tipCarne << "\n";
    cout << "-----------------------\n";
}

// Meniu Vegan

MeniuVegan::MeniuVegan(const char *nume, double pret, bool disp, bool alergeni, string proteina)
    : Meniu(nume, pret, disp, alergeni), sursaProteina(proteina) {}

void MeniuVegan::getAllMeniu() const {
    cout << "--- PREPARAT VEGAN ---\n";
    Meniu::getAllMeniu();
    cout << "Sursa proteina vegetala: " << sursaProteina << "\n";
    cout << "-----------------------\n";
}