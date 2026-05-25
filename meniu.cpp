#include "meniu.h"
#include <cstring>
using namespace std;


MeniuBase::MeniuBase() : nume_produs(nullptr), pret(0), disponibilitate(false), alergeni(false) {}

MeniuBase::MeniuBase(const char *n, double p, bool d, bool a)
    : pret(p), disponibilitate(d), alergeni(a)
{
    nume_produs = new char[strlen(n) + 1];
    strcpy(nume_produs, n);
}

MeniuBase::MeniuBase(const MeniuBase &m)
    : pret(m.pret), disponibilitate(m.disponibilitate), alergeni(m.alergeni)
{
    if (m.nume_produs)
    {
        nume_produs = new char[strlen(m.nume_produs) + 1];
        strcpy(nume_produs, m.nume_produs);
    }
    else
    {
        nume_produs = nullptr;
    }
}

MeniuBase &MeniuBase::operator=(const MeniuBase &m)
{
    if (this != &m)
    {
        delete[] nume_produs;
        if (m.nume_produs)
        {
            nume_produs = new char[strlen(m.nume_produs) + 1];
            strcpy(nume_produs, m.nume_produs);
        }
        else
        {
            nume_produs = nullptr;
        }
        pret = m.pret;
        disponibilitate = m.disponibilitate;
        alergeni = m.alergeni;
    }
    return *this;
}

MeniuBase::~MeniuBase() { delete[] nume_produs; }

void MeniuBase::setNumeProdus(const char *n)
{
    delete[] nume_produs;
    nume_produs = new char[strlen(n) + 1];
    strcpy(nume_produs, n);
}
void MeniuBase::setPret(double p) { pret = p; }
void MeniuBase::setDisponibilitate(bool d) { disponibilitate = d; }
void MeniuBase::setAlergeni(bool a) { alergeni = a; }

const char *MeniuBase::getNumeProdus() const { return nume_produs; }
double MeniuBase::getPret() const { return pret; }
bool MeniuBase::getDisponibilitate() const { return disponibilitate; }
bool MeniuBase::getAlergeni() const { return alergeni; }

void MeniuBase::getAllMeniu() const
{
    cout << "Nume: " << (nume_produs ? nume_produs : "N/A")
         << " | Pret: " << pret << " RON\n";
    cout << "Disponibil: " << (disponibilitate ? "Da" : "Nu")
         << " | Alergeni: " << (alergeni ? "Da" : "Nu") << "\n";
}

MeniuNormal::MeniuNormal(const char *n, double p, bool d, bool a, string carne)
    : Meniu<string>(n, p, d, a, carne) {}

void MeniuNormal::getAllMeniu() const
{
    cout << "--- PREPARAT CLASIC ---\n";
    MeniuBase::getAllMeniu();
    cout << "Tip carne: " << informatiiExtra << "\n";
    cout << "-----------------------\n";
}

MeniuVegan::MeniuVegan(const char *n, double p, bool d, bool a, string proteina)
    : Meniu<string>(n, p, d, a, proteina) {}

void MeniuVegan::getAllMeniu() const
{
    cout << "--- PREPARAT VEGAN ---\n";
    MeniuBase::getAllMeniu();
    cout << "Sursa proteina: " << informatiiExtra << "\n";
    cout << "----------------------\n";
}

MeniuZilnic::MeniuZilnic(const char *n, double p, bool d, bool a,
                         int calorii, string desc)
    : Meniu<int>(n, p, d, a, calorii), descriere(desc) {}

void MeniuZilnic::getAllMeniu() const
{
    cout << "--- PREPARAT ZILNIC ---\n";
    MeniuBase::getAllMeniu();
    cout << "Calorii: " << informatiiExtra << " kcal\n";
    cout << "Descriere: " << descriere << "\n";
    cout << "-----------------------\n";
}

const string &MeniuZilnic::getDescriere() const { return descriere; }