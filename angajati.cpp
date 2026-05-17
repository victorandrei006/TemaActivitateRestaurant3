#include "angajati.h"
#include "exceptiiRestaurant.h"
#include <cstring>
#include <utility> 

using namespace std;

int Angajat::numarTotalAngajati = 0;

// Constructor fara parametri
Angajat::Angajat() : nume(nullptr), prenume(nullptr), varsta(0), salariu(0), vechime(0) {}

// Constructor cu parametri
Angajat::Angajat(const char *nume, const char *prenume, int varsta, int salariu) : vechime(0) {
    if (varsta < 16) throw VarstaInvalidaException(varsta);
    if (salariu < 2000) throw SalariuInvalidException(salariu);

    this->varsta = varsta;
    this->salariu = salariu;
    
    this->nume = new char[strlen(nume) + 1];
    strcpy(this->nume, nume);
    this->prenume = new char[strlen(prenume) + 1];
    strcpy(this->prenume, prenume);

    numarTotalAngajati++;
}

// Constructor de copiere
Angajat::Angajat(const Angajat &altul) : varsta(altul.varsta), salariu(altul.salariu), vechime(altul.vechime) {
    if (altul.nume) {
        this->nume = new char[strlen(altul.nume) + 1];
        strcpy(this->nume, altul.nume);
    } else {
        this->nume = nullptr;
    }

    if (altul.prenume) {
        this->prenume = new char[strlen(altul.prenume) + 1];
        strcpy(this->prenume, altul.prenume);
    } else {
        this->prenume = nullptr;
    }

    numarTotalAngajati++;
}

// Destructor
Angajat::~Angajat() {
    delete[] nume;
    delete[] prenume;
    numarTotalAngajati--;
}


void Angajat::afiseazaNumarTotalAngajati() {
    cout << "\n[INFO Sistem] Numar total angajati activi: " << numarTotalAngajati << "\n";
}

void swap(Angajat& primul, Angajat& alDoilea) noexcept {
    using std::swap; 
    swap(primul.nume, alDoilea.nume);
    swap(primul.prenume, alDoilea.prenume);
    swap(primul.varsta, alDoilea.varsta);
    swap(primul.salariu, alDoilea.salariu);
    swap(primul.vechime, alDoilea.vechime);
}

Angajat& Angajat::operator=(const Angajat& altul) {
    if (this != &altul) {
        Angajat* copie = altul.clone(); 
        swap(*this, *copie);
        delete copie; 
    }
    return *this;
}

ostream& operator<<(ostream& os, const Angajat& angajat) {
    angajat.afisareVirtuala(os);
    return os;
}

void Angajat::afisareVirtuala(ostream& os) const {
    os << "Nume: " << (nume ? nume : "") << " " << (prenume ? prenume : "") 
       << " | Varsta: " << varsta 
       << " | Salariu: " << salariu << " RON | Vechime: " << vechime << " ani";
}

// Getters
const char* Angajat::getNume() const { return nume; }
const char* Angajat::getPrenume() const { return prenume; }
int Angajat::getSalariu() const { return salariu; }

void Angajat::promovareAngajat() {
    if (vechime >= 5) {
        salariu += 1000;
        cout << (nume ? nume : "") << " a fost promovat!\n";
    } else {
        cout << (nume ? nume : "") << " nu are vechime suficienta.\n";
    }
}

//clasa derivata ospatar

Ospatar::Ospatar(const char *nume, const char *prenume, int varsta, int salariu)
    : Angajat(nume, prenume, varsta, salariu) {}

void Ospatar::executaSarcina() const {
    cout << "Ospatarul " << (getNume() ? getNume() : "") << " serveste clientii la mese.\n";
}

Angajat* Ospatar::clone() const {
    return new Ospatar(*this); 
}

void Ospatar::preiaRezervare(const Rezervari& r) {
    rezervariPreluate.push_back(r);
    cout << "Ospatarul " << (getNume() ? getNume() : "") << " a preluat rezervarea pentru " << r.getNumeClient() << ".\n";
}

int Ospatar::getNumarRezervari()const {return rezervariPreluate.size();}

void Ospatar::afisareVirtuala(ostream& os) const {
    os << "[Ospatar] ";
    Angajat::afisareVirtuala(os); 
    os << " | Rezervari preluate: " << rezervariPreluate.size() << "\n";
}

//clasa derivata bucatar 

Bucatar::Bucatar(const char *nume, const char *prenume, int varsta, int salariu, string specialitate)
    : Angajat(nume, prenume, varsta, salariu), specialitate(std::move(specialitate)) {}

void Bucatar::executaSarcina() const {
    cout << "Bucatarul " << (getNume() ? getNume() : "") << " gateste preparate la statia de " << specialitate << ".\n";
}

Angajat* Bucatar::clone() const {
    return new Bucatar(*this);
}

void Bucatar::afisareVirtuala(ostream& os) const {
    os << "[Bucatar] ";
    Angajat::afisareVirtuala(os);
    os << " | Specialitate: " << specialitate << "\n";
}

//clasa derivata manager
Manager::Manager(const char *nume, const char *prenume, int varsta, int salariu, int bonusConducere)
    : Angajat(nume, prenume, varsta, salariu), bonusConducere(bonusConducere) {}

void Manager::executaSarcina() const {
    cout << "Managerul " << (getNume() ? getNume() : "") << " face actele si verifica stocurile.\n";
}

Angajat* Manager::clone() const {
    return new Manager(*this);
}

void Manager::afisareVirtuala(ostream& os) const {
    os << "[Manager] ";
    Angajat::afisareVirtuala(os);
    os << " | Bonus Conducere: " << bonusConducere << " RON\n";
}