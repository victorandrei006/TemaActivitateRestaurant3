#include "observator.h"
using namespace std;

void SistemNotificari::aboneaza(IObservator* obs) {
    observatori.push_back(obs);
}

void SistemNotificari::dezaboneaza(IObservator* obs) {
    observatori.erase(remove(observatori.begin(), observatori.end(), obs), observatori.end());
}

void SistemNotificari::notificaToti(const string& mesaj) const {
    for (auto* obs : observatori)
        obs->onNotificare(mesaj);
}

int SistemNotificari::getNrAbonati() const { return (int)observatori.size(); }

NotificatorManager::NotificatorManager(const string& nume) : numeManager(nume) {}

void NotificatorManager::onNotificare(const string& mesaj) {
    cout << "  [NOTIF -> Manager " << numeManager << "]: " << mesaj << "\n";
}

NotificatorOspatar::NotificatorOspatar(const string& nume) : numeOspatar(nume) {}

void NotificatorOspatar::onNotificare(const string& mesaj) {
    cout << "  [NOTIF -> Ospatar " << numeOspatar << "]: " << mesaj << "\n";
}