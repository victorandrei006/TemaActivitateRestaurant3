#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <memory>
#include <limits>
#include <iomanip>
#include <algorithm>
#include "angajati.h"
#include "restaurant.h"
#include "rezervari.h"
#include "meniu.h"
#include "exceptiiRestaurant.h"
#include "jurnal.h"
#include "observator.h"
#include "strategie.h"
#include "factura.h"

using namespace std;


struct StareSistem
{
    Restaurant restaurant;
    vector<Rezervari> rezervariInAsteptare;
    vector<shared_ptr<MeniuBase>> meniuRestaurant;
    vector<Factura> facturi;
    shared_ptr<Ospatar> ospatarCurent;

    StareSistem() : restaurant("Bistro FMI") {}
};


static void golesteCin()
{
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

static void separator(char ch = '=', int n = 44)
{
    cout << "  " << string(n, ch) << "\n";
}

static unique_ptr<IStrategieDiscount> alegerStrategie()
{
    cout << "  Alege discount:\n";
    cout << "    1. Fara discount\n";
    cout << "    2. Discount grup  (10%)\n";
    cout << "    3. Happy Hour     (20%)\n";
    cout << "  Optiune: ";
    int opt;
    cin >> opt;
    if (opt == 2)
        return make_unique<DiscountGrup>(10.0);
    else if (opt == 3)
        return make_unique<HappyHour>();
    else
        return make_unique<FaraDiscount>();
}

static int afiseazaMeniuIndexat(const vector<shared_ptr<MeniuBase>> &meniu,
                                bool doarDisponibile = true)
{
    int idx = 1;
    for (const auto &p : meniu)
    {
        if (doarDisponibile && !p->getDisponibilitate())
            continue;
        cout << "  [" << idx++ << "] "
             << left << setw(26) << p->getNumeProdus()
             << right << setw(7) << fixed << setprecision(2) << p->getPret()
             << " RON"
             << (p->getDisponibilitate() ? "" : "  [INDISPONIBIL]") << "\n";
    }
    return idx - 1;
}

static shared_ptr<MeniuBase> produsLaIndex(
    const vector<shared_ptr<MeniuBase>> &meniu, int idx, bool doarDisponibile = true)
{
    int cnt = 0;
    for (const auto &p : meniu)
    {
        if (doarDisponibile && !p->getDisponibilitate())
            continue;
        if (++cnt == idx)
            return p;
    }
    return nullptr;
}



static void incarcaAngajatiDinFisier(Restaurant &restaurant, const string &f)
{
    ifstream fisier(f);
    if (!fisier.is_open())
    {
        cerr << "[Avertisment] Nu s-a putut deschide: " << f << "\n";
        return;
    }
    string linie;
    int contor = 0;
    while (getline(fisier, linie))
    {
        stringstream ss(linie);
        string tip, nume, prenume, v_str, s_str, extra;
        getline(ss, tip, ',');
        getline(ss, nume, ',');
        getline(ss, prenume, ',');
        getline(ss, v_str, ',');
        getline(ss, s_str, ',');
        getline(ss, extra, ',');
        try
        {
            int v = stoi(v_str), s = stoi(s_str);
            if (tip == "Ospatar")
                restaurant.adaugaAngajat(make_shared<Ospatar>(nume.c_str(), prenume.c_str(), v, s));
            else if (tip == "Bucatar")
                restaurant.adaugaAngajat(make_shared<Bucatar>(nume.c_str(), prenume.c_str(), v, s, extra));
            else if (tip == "Manager")
                restaurant.adaugaAngajat(make_shared<Manager>(nume.c_str(), prenume.c_str(), v, s, stoi(extra)));
            contor++;
        }
        catch (const RestaurantException &e)
        {
            cerr << "[EROARE] " << e.what() << "\n";
        }
        catch (const exception &)
        {
            cerr << "[EROARE] Parsare: " << linie << "\n";
        }
    }
    cout << "[Sistem] " << contor << " angajati incarcati.\n";
}

static void incarcaRezervariDinFisier(vector<Rezervari> &lista, const string &f)
{
    ifstream fisier(f);
    if (!fisier.is_open())
    {
        cerr << "[Avertisment] Nu s-a putut deschide: " << f << "\n";
        return;
    }
    string linie;
    int contor = 0;
    while (getline(fisier, linie))
    {
        stringstream ss(linie);
        string nume, prenume, p_str, o_str;
        getline(ss, nume, ',');
        getline(ss, prenume, ',');
        getline(ss, p_str, ',');
        getline(ss, o_str, ',');
        try
        {
            lista.emplace_back(nume.c_str(), prenume.c_str(), stoi(p_str), stoi(o_str), false);
            contor++;
        }
        catch (...)
        {
            cerr << "[EROARE] Parsare rezervare: " << linie << "\n";
        }
    }
    cout << "[Sistem] " << contor << " rezervari incarcate.\n";
}

static void incarcaMeniuDinFisier(vector<shared_ptr<MeniuBase>> &lista, const string &f)
{
    ifstream fisier(f);
    if (!fisier.is_open())
    {
        cerr << "[Avertisment] Nu s-a putut deschide: " << f << "\n";
        return;
    }
    string linie;
    int contor = 0;
    while (getline(fisier, linie))
    {
        stringstream ss(linie);
        string tip, nume, p_str, d_str, a_str, special;
        getline(ss, tip, ',');
        getline(ss, nume, ',');
        getline(ss, p_str, ',');
        getline(ss, d_str, ',');
        getline(ss, a_str, ',');
        getline(ss, special, ',');
        try
        {
            double pret = stod(p_str);
            bool disp = stoi(d_str), alerg = stoi(a_str);
            if (tip == "Normal")
                lista.push_back(make_shared<MeniuNormal>(nume.c_str(), pret, disp, alerg, special));
            else if (tip == "Vegan")
                lista.push_back(make_shared<MeniuVegan>(nume.c_str(), pret, disp, alerg, special));
            else if (tip == "Zilnic")
            {
                string descriere;
                getline(ss, descriere, ',');
                lista.push_back(make_shared<MeniuZilnic>(nume.c_str(), pret, disp, alerg, stoi(special), descriere));
            }
            contor++;
        }
        catch (...)
        {
            cerr << "[EROARE] Parsare meniu: " << linie << "\n";
        }
    }
    cout << "[Sistem] " << contor << " preparate incarcate.\n";
}



static void creeazaNotaDePlata(StareSistem &stare, shared_ptr<Ospatar> &ospatar)
{
    if (ospatar->getNumarRezervari() == 0)
    {
        cout << "  Nu ai rezervari preluate. Preia mai intai o rezervare.\n";
        return;
    }

    separator();
    cout << "  REZERVARILE TALE\n";
    separator('-');
    ospatar->afiseazaRezervariProprii();
    cout << "  Alege rezervarea (1-" << ospatar->getNumarRezervari() << ", 0=anuleaza): ";
    int idxRez;
    cin >> idxRez;
    if (idxRez == 0)
        return;
    if (idxRez < 1 || idxRez > ospatar->getNumarRezervari())
    {
        cout << "  Index invalid.\n";
        return;
    }
    const Rezervari &rez = ospatar->getRezervare(idxRez - 1);

    string numeClient = string(rez.getNumeClient()) + " " + string(rez.getPrenumeClient());
    for (const auto &f : stare.facturi)
    {
        if (f.getNumeClient() == numeClient && !f.estePlatita())
        {
            cout << "  [Atentie] Exista deja o nota de plata neplatita pentru "
                 << numeClient << " (Factura #" << f.getNumar() << ").\n";
            cout << "  Continui oricum? (1=Da, 0=Nu): ";
            int confirm;
            cin >> confirm;
            if (confirm != 1)
                return;
            break;
        }
    }

    Factura factura(numeClient, string(ospatar->getNume()),
                    rez.getNumarPersoane(), rez.getOraRezervare());

    bool continua = true;
    while (continua)
    {
        separator();
        cout << "  ADAUGA PRODUSE (0 = finalizeaza)\n";
        separator('-');
        int nrAfisate = afiseazaMeniuIndexat(stare.meniuRestaurant, true);
        if (nrAfisate == 0)
        {
            cout << "  Niciun produs disponibil.\n";
            break;
        }

        cout << "  Produs: ";
        int idxProd;
        cin >> idxProd;
        if (idxProd == 0)
        {
            continua = false;
            break;
        }

        shared_ptr<MeniuBase> p = produsLaIndex(stare.meniuRestaurant, idxProd);
        if (!p)
        {
            cout << "  Index invalid.\n";
            continue;
        }

        cout << "  Cantitate: ";
        int cant;
        cin >> cant;
        if (cant < 1)
        {
            cout << "  Cantitate invalida.\n";
            continue;
        }

        factura.adaugaProdus(p->getNumeProdus(), p->getPret(), cant);
        cout << "  [OK] " << p->getNumeProdus() << " x" << cant
             << " (" << fixed << setprecision(2) << p->getPret() * cant << " RON)\n";
    }

    if (!factura.areProduseAdaugate())
    {
        cout << "  Niciun produs adaugat. Nota de plata anulata.\n";
        return;
    }

    separator();
    auto strategie = alegerStrategie();

    factura.calculeazaTotal(*strategie);
    factura.afiseaza();

    stare.facturi.push_back(factura);
    JurnalEvenimente::getInstanta()->logheaza(
        "Factura #" + to_string(factura.getNumar()) +
        " creata de ospatar " + ospatar->getNume() +
        " pentru " + numeClient +
        " | Total: " + to_string(factura.getTotalNet()) + " RON");
    cout << "  [OK] Nota de plata salvata.\n";
}

static void marcheazaPlatita(StareSistem &stare, const string &numeOspatar = "")
{
    vector<pair<int, int>> neplatite;
    for (int i = 0; i < (int)stare.facturi.size(); i++)
    {
        const Factura &f = stare.facturi[i];
        if (f.estePlatita())
            continue;
        if (!numeOspatar.empty() && f.getNumeOspatar() != numeOspatar)
            continue;
        neplatite.push_back({i, f.getNumar()});
    }

    if (neplatite.empty())
    {
        cout << "  Nu exista facturi neplatite.\n";
        return;
    }

    separator();
    cout << "  FACTURI NEPLATITE\n";
    separator('-');
    for (int i = 0; i < (int)neplatite.size(); i++)
    {
        const Factura &f = stare.facturi[neplatite[i].first];
        cout << "  [" << i + 1 << "] Factura #"
             << setw(4) << setfill('0') << f.getNumar() << setfill(' ')
             << " | " << left << setw(20) << f.getNumeClient()
             << " | " << fixed << setprecision(2) << f.getTotalNet() << " RON\n";
    }
    cout << "  Alege (0=anuleaza): ";
    int alegere;
    cin >> alegere;
    if (alegere < 1 || alegere > (int)neplatite.size())
        return;

    stare.facturi[neplatite[alegere - 1].first].marcheazaPlatita();
    int nrF = neplatite[alegere - 1].second;
    cout << "  [OK] Factura #" << nrF << " marcata ca platita.\n";
    JurnalEvenimente::getInstanta()->logheaza("Factura #" + to_string(nrF) + " platita.");
}


static void meniuOspatar(StareSistem &stare)
{
    shared_ptr<Ospatar> &osp = stare.ospatarCurent;
    int optiune = -1;

    do
    {
        separator();
        cout << "  PORTAL OSPATAR  —  "
             << osp->getNume() << " " << osp->getPrenume() << "\n";
        separator();
        cout << "  Rezervari preluate: " << osp->getNumarRezervari() << "/3\n";
        separator('-');
        cout << "  [REZERVARI]\n";
        cout << "   1. Afiseaza rezervarile mele\n";
        cout << "   2. Adauga rezervare in lista de asteptare\n";
        cout << "   3. Preia rezervari din lista de asteptare\n";
        separator('-');
        cout << "  [MENIU & COMENZI]\n";
        cout << "   4. Afiseaza meniul disponibil\n";
        cout << "   5. Creeaza nota de plata\n";
        cout << "   6. Afiseaza facturile mele\n";
        cout << "   7. Marcheaza factura ca platita\n";
        separator('-');
        cout << "  [ACTIVITATE]\n";
        cout << "   8. Executa sarcina de serviciu\n";
        cout << "   9. Afiseaza rezumat tura mea\n";
        separator('-');
        cout << "   0. Logout\n";
        separator('-');
        cout << "  Alege: ";
        cin >> optiune;
        if (cin.fail())
        {
            golesteCin();
            optiune = -1;
        }

        switch (optiune)
        {

        case 1:
            separator();
            cout << "  REZERVARILE LUI " << osp->getNume() << "\n";
            separator('-');
            osp->afiseazaRezervariProprii();
            break;

        case 2:
        {
            string nume, prenume;
            int pers, ora;
            cout << "  Nume client: ";
            cin >> nume;
            cout << "  Prenume: ";
            cin >> prenume;
            cout << "  Nr. persoane: ";
            cin >> pers;
            cout << "  Ora (ex. 19): ";
            cin >> ora;
            stare.rezervariInAsteptare.emplace_back(
                nume.c_str(), prenume.c_str(), pers, ora, false);
            JurnalEvenimente::getInstanta()->logheaza(
                "Ospatar " + string(osp->getNume()) + ": rezervare adaugata pt. " + nume);
            cout << "  [OK] Rezervare adaugata.\n";
            break;
        }

        case 3:
        {
            if (stare.rezervariInAsteptare.empty())
            {
                cout << "  Lista de asteptare e goala.\n";
                break;
            }
            if (osp->getNumarRezervari() >= 3)
            {
                cout << "  Ai deja 3 rezervari. Nu mai poti prelua altele.\n";
                break;
            }
            cout << "  Procesam " << stare.rezervariInAsteptare.size() << " rezervari...\n";
            for (auto &rez : stare.rezervariInAsteptare)
            {
                try
                {
                    stare.restaurant.primesteRezervare(rez);
                }
                catch (const RestaurantException &e)
                {
                    cerr << "  !!! " << e.what() << " !!!\n";
                }
            }
            stare.rezervariInAsteptare.clear();
            break;
        }

        case 4:
        {
            separator();
            cout << "  MENIU DISPONIBIL\n";
            separator('-');
            int n = afiseazaMeniuIndexat(stare.meniuRestaurant, true);
            if (n == 0)
                cout << "  Niciun preparat disponibil momentan.\n";
            cout << "\n  -- Info extra --\n";
            for (const auto &p : stare.meniuRestaurant)
            {
                if (!p->getDisponibilitate())
                    continue;
                if (auto *mn = dynamic_cast<MeniuNormal *>(p.get()))
                    afiseazaInfoExtra(*mn);
                else if (auto *mz = dynamic_cast<MeniuZilnic *>(p.get()))
                    afiseazaInfoExtra(*mz);
            }
            break;
        }

        case 5:
            creeazaNotaDePlata(stare, osp);
            break;

        case 6:
        {
            separator();
            cout << "  FACTURILE LUI " << osp->getNume() << "\n";
            separator('-');
            bool am = false;
            for (auto &f : stare.facturi)
            {
                if (f.getNumeOspatar() != string(osp->getNume()))
                    continue;
                f.afiseaza();
                am = true;
            }
            if (!am)
                cout << "  Nu ai nicio factura emisa.\n";
            break;
        }

        case 7:
            marcheazaPlatita(stare, string(osp->getNume()));
            break;

        case 8:
            osp->executaSarcina();
            JurnalEvenimente::getInstanta()->logheaza(
                string(osp->getNume()) + " si-a executat sarcina.");
            break;

        case 9:
        {
            separator();
            cout << "  REZUMAT TURA — " << osp->getNume() << "\n";
            separator('-');
            cout << "  Rezervari preluate: " << osp->getNumarRezervari() << "\n";
            double totalCollected = 0, totalPending = 0;
            int nrFacturi = 0;
            for (const auto &f : stare.facturi)
            {
                if (f.getNumeOspatar() != string(osp->getNume()))
                    continue;
                nrFacturi++;
                if (f.estePlatita())
                    totalCollected += f.getTotalNet();
                else
                    totalPending += f.getTotalNet();
            }
            cout << "  Facturi emise  : " << nrFacturi << "\n";
            cout << "  Incasat        : " << fixed << setprecision(2) << totalCollected << " RON\n";
            cout << "  In asteptare   : " << fixed << setprecision(2) << totalPending << " RON\n";
            cout << "  Salariu curent : " << osp->getSalariu() << " RON\n";
            break;
        }

        case 0:
            cout << "  La revedere, " << osp->getNume() << "!\n";
            JurnalEvenimente::getInstanta()->logheaza(
                "Logout ospatar: " + string(osp->getNume()));
            break;

        default:
            cout << "  Optiune invalida.\n";
            break;
        }
    } while (optiune != 0);
}

static void meniuManager(StareSistem &stare)
{
    int optiune = -1;

    do
    {
        separator();
        cout << "  PORTAL MANAGER  —  " << stare.restaurant.getNumeRestaurant() << "\n";
        separator();
        cout << "  [ECHIPA]\n";
        cout << "   1. Afiseaza echipa curenta\n";
        cout << "   2. Statistici angajati\n";
        cout << "   3. Adauga angajat nou\n";
        cout << "   4. Promoveaza un angajat\n";
        cout << "   5. Incepe ziua de munca (toti)\n";
        separator('-');
        cout << "  [REZERVARI & MESE]\n";
        cout << "   6. Afiseaza rezervarile in asteptare\n";
        cout << "   7. Adauga rezervare manuala\n";
        cout << "   8. Proceseaza toate rezervarile\n";
        cout << "   9. Status ocupare restaurant\n";
        separator('-');
        cout << "  [MENIU]\n";
        cout << "  10. Afiseaza meniu complet\n";
        cout << "  11. Modifica pretul unui produs\n";
        cout << "  12. Schimba disponibilitatea unui produs\n";
        cout << "  13. Calculeaza total cu discount  [Strategy]\n";
        separator('-');
        cout << "  [FACTURARE & RAPOARTE]\n";
        cout << "  14. Afiseaza toate facturile\n";
        cout << "  15. Marcheaza factura ca platita\n";
        cout << "  16. Raport venituri\n";
        separator('-');
        cout << "  [SISTEM]\n";
        cout << "  17. Afiseaza jurnalul             [Singleton]\n";
        cout << "  18. Goleste jurnalul\n";
        separator('-');
        cout << "   0. Logout\n";
        separator('-');
        cout << "  Alege: ";
        cin >> optiune;
        if (cin.fail())
        {
            golesteCin();
            optiune = -1;
        }

        switch (optiune)
        {

        case 1:
            stare.restaurant.afiseazaEchipa();
            break;

        case 2:
            Angajat::afiseazaNumarTotalAngajati();
            break;

        case 3:
        {
            separator();
            cout << "  ADAUGA ANGAJAT NOU\n";
            separator('-');
            cout << "  Tip (Ospatar / Bucatar / Manager): ";
            string tip;
            cin >> tip;
            cout << "  Nume: ";
            string nume;
            cin >> nume;
            cout << "  Prenume: ";
            string prenume;
            cin >> prenume;
            cout << "  Varsta: ";
            int varsta;
            cin >> varsta;
            cout << "  Salariu: ";
            int salariu;
            cin >> salariu;

            try
            {
                if (tip == "Ospatar")
                {
                    stare.restaurant.adaugaAngajat(
                        make_shared<Ospatar>(nume.c_str(), prenume.c_str(), varsta, salariu));
                }
                else if (tip == "Bucatar")
                {
                    cout << "  Specialitate: ";
                    string spec;
                    cin >> spec;
                    stare.restaurant.adaugaAngajat(
                        make_shared<Bucatar>(nume.c_str(), prenume.c_str(), varsta, salariu, spec));
                }
                else if (tip == "Manager")
                {
                    cout << "  Bonus conducere: ";
                    int bonus;
                    cin >> bonus;
                    stare.restaurant.adaugaAngajat(
                        make_shared<Manager>(nume.c_str(), prenume.c_str(), varsta, salariu, bonus));
                }
                else
                {
                    cout << "  Tip necunoscut.\n";
                    break;
                }
                cout << "  [OK] Angajat adaugat.\n";
            }
            catch (const RestaurantException &e)
            {
                cerr << "  [EROARE] " << e.what() << "\n";
            }
            break;
        }

        case 4:
        {
            stare.restaurant.afiseazaEchipa();
            cout << "  Introdu numele angajatului de promovat: ";
            string numeP;
            cin >> numeP;
            stare.restaurant.promoveazaAngajat(numeP);
            JurnalEvenimente::getInstanta()->logheaza(
                "Manager: tentativa promovare " + numeP);
            break;
        }

        case 5:
            stare.restaurant.incepeZiuaDeMunca();
            JurnalEvenimente::getInstanta()->logheaza("Manager a pornit ziua de munca.");
            break;

        case 6:
        {
            separator();
            cout << "  REZERVARI IN ASTEPTARE (" << stare.rezervariInAsteptare.size() << ")\n";
            separator('-');
            if (stare.rezervariInAsteptare.empty())
            {
                cout << "  Lista e goala.\n";
                break;
            }
            for (size_t i = 0; i < stare.rezervariInAsteptare.size(); i++)
            {
                cout << "  [" << i + 1 << "] ";
                stare.rezervariInAsteptare[i].getAllRezervari();
            }
            break;
        }

        case 7:
        {
            string nume, prenume;
            int pers, ora;
            cout << "  Nume: ";
            cin >> nume;
            cout << "  Prenume: ";
            cin >> prenume;
            cout << "  Persoane: ";
            cin >> pers;
            cout << "  Ora: ";
            cin >> ora;
            stare.rezervariInAsteptare.emplace_back(
                nume.c_str(), prenume.c_str(), pers, ora, false);
            JurnalEvenimente::getInstanta()->logheaza(
                "Manager: rezervare manuala pt. " + nume + " " + prenume);
            cout << "  [OK] Rezervare adaugata.\n";
            break;
        }

        case 8:
        {
            if (stare.rezervariInAsteptare.empty())
            {
                cout << "  Lista e goala.\n";
                break;
            }
            cout << "  Procesam " << stare.rezervariInAsteptare.size() << " rezervari...\n";
            for (auto &rez : stare.rezervariInAsteptare)
            {
                try
                {
                    stare.restaurant.primesteRezervare(rez);
                }
                catch (const RestaurantException &e)
                {
                    cerr << "  !!! " << e.what() << " !!!\n";
                }
            }
            stare.rezervariInAsteptare.clear();
            break;
        }

        case 9:
        {
            int ocupate = stare.restaurant.getMeseOcupate();
            int totale = stare.restaurant.getMeseTotale();
            int libere = totale - ocupate;
            double proc = totale > 0 ? (100.0 * ocupate / totale) : 0;
            separator();
            cout << "  STATUS OCUPARE RESTAURANT\n";
            separator('-');
            cout << "  Total mese    : " << totale << "\n";
            cout << "  Mese ocupate  : " << ocupate << "\n";
            cout << "  Mese libere   : " << libere << "\n";
            cout << "  Grad ocupare  : " << fixed << setprecision(1) << proc << "%\n";
            cout << "\n  Harta mese (O=ocupat, L=liber):\n  ";
            stare.restaurant.afiseazaHartaMese();
            break;
        }

        case 10:
        {
            separator();
            cout << "  MENIU COMPLET (inclusiv indisponibile)\n";
            separator('-');
            afiseazaMeniuIndexat(stare.meniuRestaurant, false);
            cout << "\n  -- Info extra --\n";
            for (const auto &p : stare.meniuRestaurant)
            {
                if (auto *mn = dynamic_cast<MeniuNormal *>(p.get()))
                    afiseazaInfoExtra(*mn);
                else if (auto *mz = dynamic_cast<MeniuZilnic *>(p.get()))
                    afiseazaInfoExtra(*mz);
            }
            break;
        }

        case 11:
        {
            cout << "  Numele produsului: ";
            string numeCautat;
            golesteCin();
            getline(cin, numeCautat);
            bool gasit = false;
            for (auto &p : stare.meniuRestaurant)
            {
                if (string(p->getNumeProdus()) == numeCautat)
                {
                    cout << "  Pret actual: " << p->getPret() << " RON. Pret nou: ";
                    double pNou;
                    cin >> pNou;
                    p->setPret(pNou);
                    JurnalEvenimente::getInstanta()->logheaza(
                        "Manager: pret [" + numeCautat + "] -> " + to_string(pNou) + " RON");
                    cout << "  [OK] Pret actualizat.\n";
                    gasit = true;
                    break;
                }
            }
            if (!gasit)
                cout << "  Produsul nu a fost gasit.\n";
            break;
        }

        case 12:
        {
            cout << "  Numele produsului: ";
            string numeCautat;
            golesteCin();
            getline(cin, numeCautat);
            bool gasit = false;
            for (auto &p : stare.meniuRestaurant)
            {
                if (string(p->getNumeProdus()) == numeCautat)
                {
                    bool nou = !p->getDisponibilitate();
                    p->setDisponibilitate(nou);
                    JurnalEvenimente::getInstanta()->logheaza(
                        "Manager: disponibilitate [" + numeCautat + "] -> " + (nou ? "Da" : "Nu"));
                    cout << "  [OK] Disponibilitate: " << (nou ? "Da" : "Nu") << "\n";
                    gasit = true;
                    break;
                }
            }
            if (!gasit)
                cout << "  Produsul nu a fost gasit.\n";
            break;
        }

        case 13:
        {
            separator();
            auto strategie = alegerStrategie();
            separator();
            cout << "  TOTAL MENIU [" << strategie->descriere() << "]\n";
            separator('-');
            double total = 0;
            for (const auto &p : stare.meniuRestaurant)
            {
                if (!p->getDisponibilitate())
                    continue;
                double pFinal = strategie->aplicaDiscount(p->getPret());
                cout << "  " << left << setw(26) << p->getNumeProdus()
                     << right << setw(7) << fixed << setprecision(2) << p->getPret()
                     << " -> " << setw(7) << pFinal << " RON\n";
                total += pFinal;
            }
            separator('-');
            cout << "  TOTAL: " << fixed << setprecision(2) << total << " RON\n";
            JurnalEvenimente::getInstanta()->logheaza(
                "Manager: total meniu [" + strategie->descriere() +
                "] = " + to_string(total) + " RON");
            break;
        }

        case 14:
        {
            separator();
            cout << "  TOATE FACTURILE (" << stare.facturi.size() << ")\n";
            separator('-');
            if (stare.facturi.empty())
            {
                cout << "  Nicio factura emisa.\n";
                break;
            }
            for (auto &f : stare.facturi)
                f.afiseaza();
            break;
        }

        case 15:
            marcheazaPlatita(stare);
            break;

        case 16:
        {
            separator();
            cout << "  RAPORT VENITURI\n";
            separator('-');
            double incasat = 0, pending = 0;
            int nrP = 0, nrNP = 0;
            for (const auto &f : stare.facturi)
            {
                if (f.estePlatita())
                {
                    incasat += f.getTotalNet();
                    nrP++;
                }
                else
                {
                    pending += f.getTotalNet();
                    nrNP++;
                }
            }
            cout << "  Facturi platite   : " << nrP << " | " << fixed << setprecision(2) << incasat << " RON\n";
            cout << "  Facturi neplatite : " << nrNP << " | " << fixed << setprecision(2) << pending << " RON\n";
            separator('-');
            cout << "  TOTAL INCASAT     : " << incasat << " RON\n";
            cout << "  TOTAL ASTEPTAT    : " << (incasat + pending) << " RON\n";

            if (!stare.facturi.empty())
            {
                separator('-');
                cout << "  Breakdown pe ospatar:\n";
                vector<string> vazuti;
                for (const auto &f : stare.facturi)
                {
                    const string &oNume = f.getNumeOspatar();
                    if (find(vazuti.begin(), vazuti.end(), oNume) != vazuti.end())
                        continue;
                    vazuti.push_back(oNume);
                    double totalO = 0;
                    for (const auto &f2 : stare.facturi)
                        if (f2.getNumeOspatar() == oNume)
                            totalO += f2.getTotalNet();
                    cout << "    " << left << setw(16) << oNume
                         << right << setw(8) << totalO << " RON\n";
                }
            }
            JurnalEvenimente::getInstanta()->logheaza(
                "Manager: raport venituri — incasat " + to_string(incasat) + " RON");
            break;
        }

        case 17:
            JurnalEvenimente::getInstanta()->afiseazaJurnal();
            break;

        case 18:
            JurnalEvenimente::getInstanta()->goleste();
            cout << "  [OK] Jurnalul a fost golit.\n";
            break;

        case 0:
            JurnalEvenimente::getInstanta()->logheaza("Logout manager.");
            cout << "  Logout manager.\n";
            break;

        default:
            cout << "  Optiune invalida.\n";
            break;
        }
    } while (optiune != 0);
}


static shared_ptr<Ospatar> autentificaOspatar(Restaurant &restaurant)
{
    cout << "  Numele tau (exact cum apare in sistem): ";
    string numeInput;
    cin >> numeInput;
    shared_ptr<Ospatar> gasit = restaurant.gasestOspatar(numeInput);
    if (!gasit)
        cout << "  Ospatar \"" << numeInput << "\" negasit in sistem.\n";
    return gasit;
}

int main()
{
    StareSistem stare;

    cout << "\n";
    separator('*');
    cout << "  SISTEM MANAGEMENT " << stare.restaurant.getNumeRestaurant() << "\n";
    separator('*');

    NotificatorManager notifManager("Ion Popescu");
    NotificatorOspatar notifOspatar("Receptie");
    stare.restaurant.aboneazaObservator(&notifManager);
    stare.restaurant.aboneazaObservator(&notifOspatar);

    JurnalEvenimente::getInstanta()->logheaza(
        "Sistem pornit: " + stare.restaurant.getNumeRestaurant());

    incarcaAngajatiDinFisier(stare.restaurant, "angajati.csv");
    incarcaRezervariDinFisier(stare.rezervariInAsteptare, "rezervari.csv");
    incarcaMeniuDinFisier(stare.meniuRestaurant, "meniu.csv");

    int rol = -1;
    do
    {
        cout << "\n";
        separator();
        cout << "  SELECTEAZA ROL\n";
        separator('-');
        cout << "  1. Ospatar\n";
        cout << "  2. Manager / Admin\n";
        cout << "  0. Inchide sistemul\n";
        separator('-');
        cout << "  Alege: ";
        cin >> rol;
        if (cin.fail())
        {
            golesteCin();
            rol = -1;
        }

        switch (rol)
        {

        case 1:
        {
            shared_ptr<Ospatar> osp = autentificaOspatar(stare.restaurant);
            if (!osp)
            {
                cout << "  Autentificare esuata.\n";
                break;
            }
            stare.ospatarCurent = osp;
            JurnalEvenimente::getInstanta()->logheaza(
                "Login ospatar: " + string(osp->getNume()));
            meniuOspatar(stare);
            stare.ospatarCurent = nullptr;
            break;
        }

        case 2:
        {
            cout << "  Parola admin: ";
            string parola;
            cin >> parola;
            if (parola != "admin1234")
            {
                cout << "  Parola gresita.\n";
                JurnalEvenimente::getInstanta()->logheaza(
                    "Tentativa login manager esuata.");
                break;
            }
            JurnalEvenimente::getInstanta()->logheaza("Login manager.");
            meniuManager(stare);
            break;
        }

        case 0:
            JurnalEvenimente::getInstanta()->logheaza("Sistem oprit.");
            cout << "\n  La revedere!\n";
            break;

        default:
            cout << "  Optiune invalida.\n";
            break;
        }
    } while (rol != 0);

    return 0;
}