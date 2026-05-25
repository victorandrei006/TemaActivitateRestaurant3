#ifndef ANGAJATI_H
#define ANGAJATI_H

#include <iostream>
#include <vector>
#include <string>
#include "rezervari.h"

class Angajat
{
private:
    char *nume;
    char *prenume;
    static int numarTotalAngajati;

protected:
    int varsta;
    int salariu;
    int vechime;
    virtual void afisareVirtuala(std::ostream &os) const;

public:
    static void afiseazaNumarTotalAngajati(); // Functie statica

    // Constructori si Destructor
    Angajat();
    Angajat(const char *nume, const char *prenume, int varsta, int salariu);
    Angajat(const Angajat &altul);
    Angajat &operator=(const Angajat &altul); // Copy-and-Swap
    virtual ~Angajat();

    friend void swap(Angajat &primul, Angajat &alDoilea) noexcept;

    // Functii virtuale pure
    virtual void executaSarcina() const = 0;
    virtual Angajat *clone() const = 0;

    // NVI
    friend std::ostream &operator<<(std::ostream &os, const Angajat &angajat);

    const char *getNume() const;
    const char *getPrenume() const;
    int getSalariu() const;
    void promovareAngajat();
};

// clasa derivata ospatar
class Ospatar : public Angajat
{
private:
    std::vector<Rezervari> rezervariPreluate;

public:
    void afiseazaRezervariProprii() const;
    Ospatar(const char *nume, const char *prenume, int varsta, int salariu);
    void executaSarcina() const override;
    const Rezervari& getRezervare(int idx) const;
    Angajat *clone() const override;
    void preiaRezervare(const Rezervari &r);
    int getNumarRezervari() const;

protected:
    void afisareVirtuala(std::ostream &os) const override;
};
// clasa derivata bucatar
class Bucatar : public Angajat
{
private:
    std::string specialitate;

public:
    Bucatar(const char *nume, const char *prenume, int varsta, int salariu, std::string specialitate);
    void executaSarcina() const override;
    Angajat *clone() const override;

protected:
    void afisareVirtuala(std::ostream &os) const override;
};
// clasa derivata manager
class Manager : public Angajat
{
private:
    int bonusConducere;

public:
    Manager(const char *nume, const char *prenume, int varsta, int salariu, int bonusConducere);
    void executaSarcina() const override;
    Angajat *clone() const override;

protected:
    void afisareVirtuala(std::ostream &os) const override;
};

#endif