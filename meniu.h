#ifndef MENIU_H
#define MENIU_H

#include <string>
#include <iostream>


class MeniuBase
{
protected:
    char *nume_produs;
    double pret;
    bool disponibilitate;
    bool alergeni;

public:
    MeniuBase();
    MeniuBase(const char *n, double p, bool d, bool a);
    MeniuBase(const MeniuBase &m);
    MeniuBase &operator=(const MeniuBase &m);
    virtual ~MeniuBase();

    void setPret(double p);
    void setDisponibilitate(bool d);
    void setNumeProdus(const char *n);
    void setAlergeni(bool a);

    const char *getNumeProdus() const;
    double getPret() const;
    bool getDisponibilitate() const;
    bool getAlergeni() const;

    virtual void getAllMeniu() const;
};


template <typename T>
void afiseazaInfoExtra(const T &produs)
{
    std::cout << "  [Info extra] " << produs.getNumeProdus()
              << " -> " << produs.getInformatiiExtra() << "\n";
}

template <typename T>
class Meniu : public MeniuBase
{
protected:
    T informatiiExtra;

public:
    Meniu() : MeniuBase(), informatiiExtra{} {}

    Meniu(const char *n, double p, bool d, bool a, const T &extra)
        : MeniuBase(n, p, d, a), informatiiExtra(extra) {}

    Meniu(const Meniu<T> &m)
        : MeniuBase(m), informatiiExtra(m.informatiiExtra) {}

    Meniu<T> &operator=(const Meniu<T> &m)
    {
        if (this != &m)
        {
            MeniuBase::operator=(m);
            informatiiExtra = m.informatiiExtra;
        }
        return *this;
    }

    virtual ~Meniu() = default;

    T getInformatiiExtra() const { return informatiiExtra; }
    void setInformatiiExtra(const T &info) { informatiiExtra = info; }

    void getAllMeniu() const override
    {
        MeniuBase::getAllMeniu();
        std::cout << "Info extra: " << informatiiExtra << "\n";
    }
};


class MeniuNormal : public Meniu<std::string>
{
public:
    MeniuNormal(const char *n, double p, bool d, bool a, std::string carne);
    void getAllMeniu() const override;
};

class MeniuVegan : public Meniu<std::string>
{
public:
    MeniuVegan(const char *n, double p, bool d, bool a, std::string proteina);
    void getAllMeniu() const override;
};


class MeniuZilnic : public Meniu<int>
{
private:
    std::string descriere;

public:
    MeniuZilnic(const char *n, double p, bool d, bool a,
                int calorii, std::string descriere);
    void getAllMeniu() const override;
    const std::string &getDescriere() const;
};

#endif