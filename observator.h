#ifndef OBSERVATOR_H
#define OBSERVATOR_H

#include <string>
#include <vector>
#include <algorithm>
#include <iostream>


class IObservator {
public:
    virtual void onNotificare(const std::string& mesaj) = 0;
    virtual ~IObservator() = default;
};

class SistemNotificari {
private:
    std::vector<IObservator*> observatori;
public:
    void aboneaza(IObservator* obs);
    void dezaboneaza(IObservator* obs);
    void notificaToti(const std::string& mesaj) const;
    int getNrAbonati() const;
};

class NotificatorManager : public IObservator {
private:
    std::string numeManager;
public:
    explicit NotificatorManager(const std::string& nume);
    void onNotificare(const std::string& mesaj) override;
};

class NotificatorOspatar : public IObservator {
private:
    std::string numeOspatar;
public:
    explicit NotificatorOspatar(const std::string& nume);
    void onNotificare(const std::string& mesaj) override;
};

#endif