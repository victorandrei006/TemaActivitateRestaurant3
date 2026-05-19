#ifndef JURNAL_H
#define JURNAL_H

#include <string>
#include <vector>
#include <iostream>


class JurnalEvenimente {
private:
    static JurnalEvenimente* instanta;
    std::vector<std::string> evenimente;

    JurnalEvenimente() = default;
    JurnalEvenimente(const JurnalEvenimente&) = delete;
    JurnalEvenimente& operator=(const JurnalEvenimente&) = delete;

public:
    static JurnalEvenimente* getInstanta();

    void logheaza(const std::string& eveniment);
    void afiseazaJurnal() const;
    void goleste();
    int getNrEvenimente() const;
};

#endif