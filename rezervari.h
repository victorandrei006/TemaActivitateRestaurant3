#ifndef REZERVARI_H
#define REZERVARI_H

class Rezervari{
    private:
        char *nume_client, *prenume_client;
        int numar_persoane, ora_rezervare;
        bool confirmare;
        
    public:
        Rezervari();
        Rezervari(const char *nume_client, const char *prenume_client, int numar_persoane, int ora_rezervare, bool confirmare);
        Rezervari(const Rezervari &rezervare);
        void setNumeClient(const char *nume_client);
        void setPrenumeClient(const char *prenume_client);
        void setNumarPersoane(int numar_persoane);
        void setOraRezervare(int ora_rezervare);
        void setConfirmare(bool confirmare);
        const char* getNumeClient() const;
        const char* getPrenumeClient() const;
        int getNumarPersoane() const;
        int getOraRezervare() const;
        bool getConfirmare() const;
        void getAllRezervari() const;
        Rezervari& operator=(const Rezervari& r);
        ~Rezervari();
};




#endif