#include "rezervari.h"
#include <cstring>
#include <iostream>

using namespace std;

// Constructor default
Rezervari::Rezervari() {
    nume_client = nullptr;
    prenume_client = nullptr;
    numar_persoane = 0;
    ora_rezervare = 0;
    confirmare = false;
}

// Constructor cu parametri
Rezervari::Rezervari(const char *nume_client, const char *prenume_client, int numar_persoane, int ora_rezervare, bool confirmare) {
    this->nume_client = new char[strlen(nume_client) + 1];
    strcpy(this->nume_client, nume_client);

    this->prenume_client = new char[strlen(prenume_client) + 1];
    strcpy(this->prenume_client, prenume_client);

    this->numar_persoane = numar_persoane;
    this->ora_rezervare = ora_rezervare;
    this->confirmare = confirmare;
}

// Constructor de copiere
Rezervari::Rezervari(const Rezervari &rezervare) {
    this->nume_client = new char[strlen(rezervare.nume_client) + 1];
    strcpy(this->nume_client, rezervare.nume_client);

    this->prenume_client = new char[strlen(rezervare.prenume_client) + 1];
    strcpy(this->prenume_client, rezervare.prenume_client);

    this->numar_persoane = rezervare.numar_persoane;
    this->ora_rezervare = rezervare.ora_rezervare;
    this->confirmare = rezervare.confirmare;
}

// Operatorul =
Rezervari& Rezervari::operator=(const Rezervari& r) {
    if (this != &r) {
        delete[] nume_client;
        delete[] prenume_client;

        this->nume_client = new char[strlen(r.nume_client) + 1];
        strcpy(this->nume_client, r.nume_client);

        this->prenume_client = new char[strlen(r.prenume_client) + 1];
        strcpy(this->prenume_client, r.prenume_client);

        this->numar_persoane = r.numar_persoane;
        this->ora_rezervare = r.ora_rezervare;
        this->confirmare = r.confirmare;
    }
    return *this;
}

// Setteri
void Rezervari::setNumeClient(const char *nume_client) {
    if (this->nume_client != nullptr) delete[] this->nume_client;
    this->nume_client = new char[strlen(nume_client) + 1];
    strcpy(this->nume_client, nume_client);
}

void Rezervari::setPrenumeClient(const char *prenume_client) {
    if (this->prenume_client != nullptr) delete[] this->prenume_client;
    this->prenume_client = new char[strlen(prenume_client) + 1];
    strcpy(this->prenume_client, prenume_client);
}

void Rezervari::setNumarPersoane(int numar_persoane) { this->numar_persoane = numar_persoane; }
void Rezervari::setOraRezervare(int ora_rezervare) { this->ora_rezervare = ora_rezervare; }
void Rezervari::setConfirmare(bool confirmare) { this->confirmare = confirmare; }

// Getteri
const char* Rezervari::getNumeClient() const { return this->nume_client; }
const char* Rezervari::getPrenumeClient() const { return this->prenume_client; }
int Rezervari::getNumarPersoane() const { return this->numar_persoane; }
int Rezervari::getOraRezervare() const { return this->ora_rezervare; }
bool Rezervari::getConfirmare() const { return this->confirmare; }

void Rezervari::getAllRezervari() const {
    cout << "------------------------------" << endl;
    cout << "Nume client: " << (nume_client ? nume_client : "N/A") << endl;
    cout << "Prenume client: " << (prenume_client ? prenume_client : "N/A") << endl;
    cout << "Numar persoane: " << numar_persoane << endl;
    cout << "Ora rezervare: " << ora_rezervare << endl;
    cout << "Confirmare: " << (confirmare ? "Da" : "Nu") << endl;
    cout << "------------------------------" << endl << endl;
}

// Destructor
Rezervari::~Rezervari() {
    delete[] nume_client;
    delete[] prenume_client;
}