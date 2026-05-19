#include "jurnal.h"
using namespace std;

JurnalEvenimente* JurnalEvenimente::instanta = nullptr;

JurnalEvenimente* JurnalEvenimente::getInstanta() {
    if (!instanta)
        instanta = new JurnalEvenimente();
    return instanta;
}

void JurnalEvenimente::logheaza(const string& ev) {
    evenimente.push_back(ev);
}

void JurnalEvenimente::afiseazaJurnal() const {
    cout << "\n=== JURNAL EVENIMENTE (" << evenimente.size() << " intrari) ===\n";
    if (evenimente.empty()) { cout << "(gol)\n"; return; }
    for (size_t i = 0; i < evenimente.size(); i++)
        cout << "[" << i + 1 << "] " << evenimente[i] << "\n";
    cout << "=================================================\n";
}

void JurnalEvenimente::goleste() { evenimente.clear(); }
int JurnalEvenimente::getNrEvenimente() const { return (int)evenimente.size(); }