#include "factura.h"
#include <iomanip>
#include <sstream>
using namespace std;

int Factura::contor = 0;

Factura::Factura(const string &client, const string &ospatar, int pers, int ora)
    : numarFactura(++contor), numeClient(client), numeOspatar(ospatar),
      nrPersoane(pers), oraRezervare(ora),
      totalBrut(0), discountValoare(0), totalNet(0),
      descriereStrategie("Necalculat"), platita(false) {}

void Factura::adaugaProdus(const string &nume, double pret, int cantitate)
{
    for (auto &linie : produse)
    {
        if (linie.numeProdus == nume)
        {
            linie.cantitate += cantitate;
            return;
        }
    }
    produse.push_back({nume, pret, cantitate});
}

void Factura::calculeazaTotal(const IStrategieDiscount &strategie)
{
    totalBrut = 0;
    for (const auto &l : produse)
        totalBrut += l.pret * l.cantitate;
    totalNet = strategie.aplicaDiscount(totalBrut);
    discountValoare = totalBrut - totalNet;
    descriereStrategie = strategie.descriere();
}

void Factura::linieBon(const string &stanga, const string &dreapta, int latime) const
{
    int spatiu = latime - 4 - (int)stanga.size() - (int)dreapta.size();
    if (spatiu < 1)
        spatiu = 1;
    cout << "  | " << stanga << string(spatiu, ' ') << dreapta << " |\n";
}

void Factura::afiseaza() const
{
    const int L = 44;
    const string linie(L, '-');
    const string linieD(L, '=');

    auto formaReala = [](double v) -> string
    {
        ostringstream ss;
        ss << fixed << setprecision(2) << v << " RON";
        return ss.str();
    };

    cout << "\n  " << linieD << "\n";
    {
        ostringstream titlu;
        titlu << "NOTA DE PLATA #" << setw(4) << setfill('0') << numarFactura;
        int pad = (L - 4 - (int)titlu.str().size()) / 2;
        cout << "  | " << string(pad, ' ') << titlu.str()
             << string(L - 4 - pad - (int)titlu.str().size(), ' ') << " |\n";
    }
    cout << "  " << linieD << "\n";
    linieBon("Client  : " + numeClient, "");
    linieBon("Ospatar : " + numeOspatar, "");
    linieBon("Persoane: " + to_string(nrPersoane),
             "Ora: " + to_string(oraRezervare) + "h");
    cout << "  " << linie << "\n";
    linieBon("PRODUSE", "SUBTOTAL");
    cout << "  " << linie << "\n";
    for (const auto &l : produse)
    {
        string desc = l.numeProdus + " x" + to_string(l.cantitate);
        linieBon(desc, formaReala(l.pret * l.cantitate));
    }
    cout << "  " << linie << "\n";
    linieBon("Total brut:", formaReala(totalBrut));
    if (discountValoare > 0.001)
        linieBon("Discount (" + descriereStrategie + "):", "-" + formaReala(discountValoare));
    linieBon("TOTAL NET:", formaReala(totalNet));
    cout << "  " << linie << "\n";
    linieBon("Status:", platita ? "*** PLATITA ***" : "*** NEPLATITA ***");
    cout << "  " << linieD << "\n\n";
}

void Factura::marcheazaPlatita() { platita = true; }
bool Factura::estePlatita() const { return platita; }
bool Factura::areProduseAdaugate() const { return !produse.empty(); }
double Factura::getTotalNet() const { return totalNet; }
double Factura::getTotalBrut() const { return totalBrut; }
int Factura::getNumar() const { return numarFactura; }
const string &Factura::getNumeClient() const { return numeClient; }
const string &Factura::getNumeOspatar() const { return numeOspatar; }