#ifndef DEPOZIT_H
#define DEPOZIT_H

#include <vector>
#include <string>
#include <stdexcept>


template<typename T>
class Depozit {
private:
    std::vector<T> elemente;
    std::string denumire;
    int capacitateMaxima;

public:
    explicit Depozit(const std::string& den, int cap = 100)
        : denumire(den), capacitateMaxima(cap) {}

    void adauga(const T& elem) {
        if ((int)elemente.size() >= capacitateMaxima)
            throw std::overflow_error("Depozit plin: " + denumire);
        elemente.push_back(elem);
    }

    void sterge(int idx) {
        if (idx < 0 || idx >= (int)elemente.size())
            throw std::out_of_range("Index invalid in depozit: " + denumire);
        elemente.erase(elemente.begin() + idx);
    }

    T& get(int idx) {
        if (idx < 0 || idx >= (int)elemente.size())
            throw std::out_of_range("Index invalid!");
        return elemente[idx];
    }

    const T& get(int idx) const {
        if (idx < 0 || idx >= (int)elemente.size())
            throw std::out_of_range("Index invalid!");
        return elemente[idx];
    }

    void goleste()  { elemente.clear(); }
    int  size()     const { return (int)elemente.size(); }
    bool esteGol()  const { return elemente.empty(); }
    bool estePlin() const { return (int)elemente.size() >= capacitateMaxima; }
    const std::string& getDenumire() const { return denumire; }

    typename std::vector<T>::iterator begin() { return elemente.begin(); }
    typename std::vector<T>::iterator end()   { return elemente.end(); }
    typename std::vector<T>::const_iterator begin() const { return elemente.begin(); }
    typename std::vector<T>::const_iterator end()   const { return elemente.end(); }
};


template<typename T, typename Predicat>
int cautaIndex(const Depozit<T>& dep, Predicat pred) {
    for (int i = 0; i < dep.size(); i++)
        if (pred(dep.get(i))) return i;
    return -1;
}


template<typename T, typename Extractor>
double calculeazaSuma(const Depozit<T>& dep, Extractor extr) {
    double s = 0.0;
    for (int i = 0; i < dep.size(); i++)
        s += extr(dep.get(i));
    return s;
}

#endif