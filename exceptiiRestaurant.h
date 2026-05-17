#ifndef EXCEPTII_RESTAURANT_H
#define EXCEPTII_RESTAURANT_H

#include <exception>
#include <string>

class RestaurantException : public std::exception {
protected:
    std::string mesaj;
public:
    RestaurantException(const std::string& msg) : mesaj(msg) {}
    virtual const char* what() const noexcept override {
        return mesaj.c_str();
    }
};

class VarstaInvalidaException : public RestaurantException {
public:
    VarstaInvalidaException(int varsta) 
        : RestaurantException("EROARE HR: Varsta invalida (" + std::to_string(varsta) + " ani). Angajatul trebuie sa aiba minim 16 ani!") {}
};

class SalariuInvalidException : public RestaurantException {
public:
    SalariuInvalidException(int salariu) 
        : RestaurantException("EROARE FINANCIARA: Salariul introdus (" + std::to_string(salariu) + " RON) este sub minimul acceptat!") {}
};

class MasaIndisponibilaException : public RestaurantException {
public:
    MasaIndisponibilaException() 
        : RestaurantException("EROARE OPERATIUNI: Nu mai sunt mese disponibile pentru aceasta rezervare!") {}
};

#endif