#ifndef MENIU_H
#define MENIU_H

#include <string>

class Meniu {
protected:
    char *nume_produs;
    double pret;
    bool disponibilitate;
    bool alergeni;

public:
    Meniu();
    Meniu(const char *nume_produs, double pret, bool disponibilitate, bool alergeni);
    Meniu(const Meniu &meniu);
    Meniu& operator=(const Meniu& m);
    virtual ~Meniu();

    void setNumeProdus(const char *nume_produs);
    void setPret(double pret);
    void setDisponibilitate(bool disponibilitate);
    void setAlergeni(bool alergeni);

    const char* getNumeProdus() const;
    double getPret() const;
    bool getDisponibilitate() const;
    bool getAlergeni() const;

    virtual void getAllMeniu() const;
};

class MeniuNormal : public Meniu {
private:
    std::string tipCarne;
public:
    MeniuNormal(const char *nume, double pret, bool disp, bool alergeni, std::string carne);
    void getAllMeniu() const override;
};

class MeniuVegan : public Meniu {
private:
    std::string sursaProteina;
public:
    MeniuVegan(const char *nume, double pret, bool disp, bool alergeni, std::string proteina);
    void getAllMeniu() const override;
};

#endif