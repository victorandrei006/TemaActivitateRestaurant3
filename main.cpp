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

using namespace std;


void incarcaAngajatiDinFisier(Restaurant& restaurant, const string& numeFisier) {
    ifstream fisier(numeFisier);
    if (!fisier.is_open()) {
        cerr << "[Avertisment] Nu s-a putut gasi/deschide fisierul: " << numeFisier << "\n";
        return;
    }

    string linie;
    int contor = 0;
    while (getline(fisier, linie)) {
        stringstream ss(linie);
        string tip, nume, prenume, varsta_str, salariu_str, extra;

        getline(ss, tip, ',');
        getline(ss, nume, ',');
        getline(ss, prenume, ',');
        getline(ss, varsta_str, ',');
        getline(ss, salariu_str, ',');
        getline(ss, extra, ',');

        try {
            int varsta = stoi(varsta_str);
            int salariu = stoi(salariu_str);

            if (tip == "Ospatar") {
                restaurant.adaugaAngajat(make_shared<Ospatar>(nume.c_str(), prenume.c_str(), varsta, salariu));
            } else if (tip == "Bucatar") {
                restaurant.adaugaAngajat(make_shared<Bucatar>(nume.c_str(), prenume.c_str(), varsta, salariu, extra));
            } else if (tip == "Manager") {
                int bonus = stoi(extra);
                restaurant.adaugaAngajat(make_shared<Manager>(nume.c_str(), prenume.c_str(), varsta, salariu, bonus));
            }
            contor++;
        } catch (const RestaurantException& e) {
            cerr << "Eroare domeniu la " << nume << ": " << e.what() << "\n";
        } catch (const exception& e) {
            cerr << "Eroare parsare la linia: " << linie << "\n";
        }
    }
    cout << "[Sistem] S-au incarcat " << contor << " angajati din fisier.\n";
}

void incarcaRezervariDinFisier(vector<Rezervari>& listaAsteptare, const string& numeFisier) {
    ifstream fisier(numeFisier);
    if (!fisier.is_open()) {
        cerr << "[Avertisment] Nu s-a putut deschide fisierul: " << numeFisier << "\n";
        return;
    }

    string linie;
    int contor = 0;
    while (getline(fisier, linie)) {
        stringstream ss(linie);
        string nume, prenume, pers_str, ora_str;

        getline(ss, nume, ',');
        getline(ss, prenume, ',');
        getline(ss, pers_str, ',');
        getline(ss, ora_str, ',');

        try {
            int pers = stoi(pers_str);
            int ora = stoi(ora_str);
            listaAsteptare.emplace_back(nume.c_str(), prenume.c_str(), pers, ora, false);
            contor++;
        } catch (const exception& e) {
            cerr << "Eroare parsare rezervare: " << linie << "\n";
        }
    }
    cout << "[Sistem] S-au incarcat " << contor << " rezervari in asteptare.\n";
}
void incarcaMeniuDinFisier(vector<shared_ptr<Meniu>>& listaMeniu, const string& numeFisier) {
    ifstream fisier(numeFisier);
    if (!fisier.is_open()) {
        cerr << "[Avertisment] Nu s-a putut deschide: " << numeFisier << "\n";
        return;
    }

    string linie;
    int contor = 0;
    while (getline(fisier, linie)) {
        stringstream ss(linie);
        string tip, nume, pret_str, disp_str, alergeni_str, special;

        getline(ss, tip, ',');
        getline(ss, nume, ',');
        getline(ss, pret_str, ',');
        getline(ss, disp_str, ',');
        getline(ss, alergeni_str, ',');
        getline(ss, special, ',');

        try {
            double pret = stod(pret_str);
            bool disp = stoi(disp_str);
            bool alergeni = stoi(alergeni_str);

            if (tip == "Normal") {
                listaMeniu.push_back(make_shared<MeniuNormal>(nume.c_str(), pret, disp, alergeni, special));
            } else if (tip == "Vegan") {
                listaMeniu.push_back(make_shared<MeniuVegan>(nume.c_str(), pret, disp, alergeni, special));
            }
            contor++;
        } catch (...) {
            cerr << "Eroare la parsarea liniei: " << linie << "\n";
        }
    }
    cout << "[Sistem] S-au incarcat " << contor << " preparate in meniu.\n";
}

int main() {
    
    Restaurant restaurant("Bistro FMI");
    cout << "=== PORNIRE SISTEM "<<restaurant.getNumeRestaurant()<< "===\n";
    vector<Rezervari> rezervariInAsteptare;
    vector<shared_ptr<Meniu>> meniuRestaurant;
    incarcaAngajatiDinFisier(restaurant, "angajati.csv");
    incarcaRezervariDinFisier(rezervariInAsteptare, "rezervari.csv");
    incarcaMeniuDinFisier(meniuRestaurant, "meniu.csv");
    int optiune = -1;

    do {
        cout << "\n====================================\n";
        cout << "    MENIU MANAGEMENT " << restaurant.getNumeRestaurant() << "\n";
        cout << "====================================\n";
        cout << "1. Afiseaza echipa curenta\n";
        cout << "2. Adauga o rezervare manuala\n";
        cout << "3. Proceseaza rezervarile in asteptare (Alocare Ospatar)\n";
        cout << "4. Incepe ziua de munca \n";
        cout << "5. Afiseaza detalii statistice \n";
        cout << "6. Afiseaza tot meniul restaurantului\n";
        cout << "7. Modifica pretul unui produs\n";
        cout << "8. Schimba disponibilitatea unui produs\n";
        cout << "0. Iesire program\n";
        cout << "------------------------------------\n";
        cout << "Alege o actiune: ";
        
        cin >> optiune;

        if (cin.fail()) {
            cin.clear(); cin.ignore(10000, '\n'); optiune = -1;
        }

        switch (optiune) {
            case 1:
                restaurant.afiseazaEchipa();
                break;
            
            case 2: {
                string nume, prenume;
                int pers, ora;
                cout << "Introdu Nume client: "; cin >> nume;
                cout << "Introdu Prenume client: "; cin >> prenume;
                cout << "Introdu Nr. Persoane: "; cin >> pers;
                cout << "Introdu Ora (ex: 19): "; cin >> ora;
                
                rezervariInAsteptare.emplace_back(nume.c_str(), prenume.c_str(), pers, ora, false);
                cout << "Rezervare adaugata cu succes in asteptare!\n";
                break;
            }

            case 3: {
                if (rezervariInAsteptare.empty()) {
                    cout << "Nu exista rezervari in asteptare!\n";
                    break;
                }
                
                cout << "\nProcesam " << rezervariInAsteptare.size() << " rezervari...\n";
                for (auto& rez : rezervariInAsteptare) {
                    try {
                        restaurant.primesteRezervare(rez);
                    } catch (const RestaurantException& e) {
                        cerr << "!!! " << e.what() << " !!!\n";
                    }
                }
                rezervariInAsteptare.clear();
                break;
            }

            case 4:
                restaurant.incepeZiuaDeMunca();
                break;

            case 5:
                Angajat::afiseazaNumarTotalAngajati();
                break;

            case 6: {
                if (meniuRestaurant.empty()) {
                    cout << "Meniul este gol!\n";
                    break;
                }
                cout << "\n=== MENIUL RESTAURANTULUI ===\n";
                for (const auto& produs : meniuRestaurant) {
                    produs->getAllMeniu();
                }
                break;
            }

            case 7: {
                cout << "Introdu numele produsului pe care vrei sa il modifici: ";
                string numeCautat;
                cin.ignore();
                getline(cin, numeCautat);
                
                bool gasit = false;
                for (auto& produs : meniuRestaurant) {
                    if (string(produs->getNumeProdus()) == numeCautat) {
                        cout << "Pretul actual este " << produs->getPret() << " RON. Introdu noul pret: ";
                        double pretNou;
                        cin >> pretNou;
                        produs->setPret(pretNou);
                        cout << "Pret actualizat cu succes!\n";
                        gasit = true;
                        break;
                    }
                }
                if (!gasit) cout << "Produsul nu a fost gasit.\n";
                break;
            }

            case 8: {
                cout << "Introdu numele produsului: ";
                string numeCautat;
                cin.ignore();
                getline(cin, numeCautat);
                
                bool gasit = false;
                for (auto& produs : meniuRestaurant) {
                    if (string(produs->getNumeProdus()) == numeCautat) {
                        bool stareCurenta = produs->getDisponibilitate();
                        produs->setDisponibilitate(!stareCurenta);
                        cout << "Disponibilitate schimbata in: " << (!stareCurenta ? "Da" : "Nu") << "\n";
                        gasit = true;
                        break;
                    }
                }
                if (!gasit) cout << "Produsul nu a fost gasit.\n";
                break;
            }
            case 0:
                cout << "\nSe inchide sistemul... La revedere!\n";
                break;

            default:
                cout << "\nOptiune invalida! Incearca din nou.\n";
                break;
        }
    } while (optiune != 0);

    return 0;
}