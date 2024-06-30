//TP 2023/2024: Zadaća 4, Zadatak 4
#include <iostream>
#include <stdexcept>
#include <tuple>
#include <utility>
#include <string>
#include <initializer_list>
#include <iomanip>
#include <algorithm>
#include <vector>
#include <memory>

class Datum{
    int dan, mjesec, godina;
public:
    Datum(int dan, int mjesec, int godina){Postavi(dan, mjesec, godina);}
    void Postavi(int dan, int mjesec, int godina);
    std::tuple<int, int, int> Ocitaj() const {return std::make_tuple(dan, mjesec, godina);}
    void Ispisi() const {std::cout << dan << "/" << mjesec << "/" << godina;}
};

void Datum::Postavi(int d, int m, int g){
    int broj_dana[12]{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if(g % 4 == 0 && g % 100 != 0 || g % 400 == 0) broj_dana[1]++;
    if(g < 1 || d < 1 || m < 1 || m > 12 || d > broj_dana[m - 1]) throw std::domain_error("Neispravan datum");
    dan = d; mjesec = m; godina = g;
}

class Vrijeme{
    int sati, minute;
public:
    Vrijeme(int sati, int minute) { Postavi(sati, minute); }
    void Postavi(int sati, int minute);
    std::pair<int, int> Ocitaj() const { return std::make_pair(sati, minute);}
    void Ispisi() const;
};

void Vrijeme::Postavi(int sati, int minute){
    if(sati < 0 || sati > 23) throw std::domain_error("Neispravno vrijeme");
    if(minute < 0 || minute > 59) throw std::domain_error("Neispravno vrijeme");
    Vrijeme::sati = sati; Vrijeme::minute = minute;
}

void Vrijeme::Ispisi() const {
    std::string ss = (sati < 10) ? "0" + std::to_string(sati) : std::to_string(sati);
    std::string mm = (minute < 10) ? "0" + std::to_string(minute) : std::to_string(minute);
    std::cout << ss << ":" << mm;
}

class Pregled{
    std::string ImePacijenta;
    Datum datum;
    Vrijeme vrijeme;
public:
    Pregled(std::string ImePacijenta, Datum datum, Vrijeme vrijeme) : ImePacijenta(ImePacijenta), datum(datum), vrijeme(vrijeme){}
    Pregled(std::string ImePacijenta, int dan, int mjesec, int godina, int sati, int minute) : ImePacijenta(ImePacijenta), datum(dan, mjesec, godina), vrijeme(sati, minute){}
    Pregled &PromijeniPacijenta(std::string NovoIme){ImePacijenta = NovoIme; return *this;}
    Pregled &PromijeniDatum(Datum NoviDatum) {datum = NoviDatum; return *this;}
    Pregled &PromijeniVrijeme(Vrijeme NovoVrijeme){vrijeme = NovoVrijeme; return *this;}
    void PomjeriDanUnaprijed();
    void PomjeriDanUnazad();
    std::string DajImePacijenta() const { return ImePacijenta;}
    Datum DajDatumPregleda() const { return datum; }
    Vrijeme DajVrijemePregleda() const { return vrijeme; }
    void Ispisi() const;
    static bool DolaziPrije(Pregled &A, Pregled &B);
};

bool Pregled::DolaziPrije(Pregled &A, Pregled &B){
    int a1, b1, c1, a2, b2, c2, ss1, mm1, ss2, mm2;
    std::tie(a1, b1, c1) = A.datum.Ocitaj();
    std::tie(a2, b2, c2) = B.datum.Ocitaj();
    std::tie(ss1, mm1) = A.vrijeme.Ocitaj();
    std::tie(ss2, mm2) = B.vrijeme.Ocitaj();

    if(c1 < c2) return true;
    if(c1 > c2) return false;

    if(b1 < b2) return true;
    if(b1 > b2) return false;

    if(a1 < a2) return true;
    if(a1 > a2) return false;

    if(ss1 < ss2) return true;
    if(ss1 > ss2) return false;
return (mm1 < mm2);
}

void Pregled::PomjeriDanUnaprijed(){
    int dan, mjesec, godina;
    std::tie(dan, mjesec, godina) = datum.Ocitaj();
    dan++;

    int broj_dana[12]{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if(godina % 4 == 0 && godina % 100 != 0 || godina % 400 == 0) broj_dana[1]++;

    if(dan > broj_dana[mjesec - 1]){
        dan = 1;
        mjesec++;
        if(mjesec > 12){
            mjesec = 1;
            godina++;
        }
    }
    datum.Postavi(dan, mjesec, godina);
}

void Pregled::Ispisi() const {
    std::cout << std::left << std::setw(30) << ImePacijenta;
    datum.Ispisi();
    std::cout << " ";
    vrijeme.Ispisi();
    std::cout << std::endl;
}

void Pregled::PomjeriDanUnazad() {
    int dan, mjesec, godina;
    std::tie(dan, mjesec, godina) = datum.Ocitaj();
    dan--;

    int broj_dana[12]{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if(godina % 4 == 0 && godina % 100 != 0 || godina % 400 == 0) broj_dana[1]++;

    if(dan < 1){
        mjesec--;
        if(mjesec < 1){
            mjesec = 12;
            godina--;
        }
        dan = broj_dana[mjesec - 1];
    }
    datum.Postavi(dan, mjesec, godina);
}

class Pregledi{
    std::vector<std::shared_ptr<Pregled>> niz_pregleda;
public:
    Pregledi() = default;
    Pregledi(std::initializer_list<Pregled> lista_pregleda);
    Pregledi(const Pregledi &l);
    Pregledi(Pregledi &&l) = default;
    Pregledi &operator =(const Pregledi &l);
    Pregledi &operator =(Pregledi &&l) = default;
    void RegistrirajPregled(std::string ImePacijenta, Datum datum, Vrijeme vrijeme);
    void RegistrirajPregled(std::string ImePacijenta, int dan, int mjesec, int godina, int sati, int minute);
    void RegistrirajPregled(std::shared_ptr<Pregled> pregled);
    int DajBrojPregleda() const { return niz_pregleda.size();}
    int DajBrojPregledaNaDatum(Datum dat) const;
    Pregled DajNajranijiPregled() const;
    Pregled &DajNajranijiPregled();
    void IsprazniKolekciju();
    void ObrisiNajranijiPregled();
    void ObrisiPregledePacijenta(std::string pacijent);
    void IspisiPregledeNaDatum(Datum dat) const;
    void IspisiSvePreglede() const;
};

Pregledi::Pregledi(std::initializer_list<Pregled> lista_pregleda) :  niz_pregleda(lista_pregleda.size()){
        int i = 0;
        for(const Pregled &pregled : lista_pregleda) niz_pregleda[i++] = std::make_shared<Pregled>(pregled);
}


Pregledi::Pregledi(const Pregledi &l) : niz_pregleda(l.niz_pregleda.size()){
        for(int i = 0; i < l.niz_pregleda.size(); i++){
            niz_pregleda[i] = std::make_shared<Pregled>(*l.niz_pregleda[i]);
        }
} 


Pregledi &Pregledi::operator =(const Pregledi &l){
    int StaraVelicina = niz_pregleda.size();
    niz_pregleda.resize(l.niz_pregleda.size());
    if(niz_pregleda.size() > StaraVelicina){
        for(int i = StaraVelicina; i < niz_pregleda.size(); i++)
        niz_pregleda[i] = std::make_shared<Pregled>(*l.niz_pregleda[i]);
        for(int i = 0; i < StaraVelicina; i++) *niz_pregleda[i] = *l.niz_pregleda[i];
    }else{
        for(int i = 0; i < niz_pregleda.size(); i++) *niz_pregleda[i] = *l.niz_pregleda[i];
    }
    return *this;
}

void Pregledi::RegistrirajPregled(std::string ImePacijenta, Datum datum, Vrijeme vrijeme){
    niz_pregleda.push_back(std::make_shared<Pregled>(ImePacijenta, datum, vrijeme));
}

void Pregledi::RegistrirajPregled(std::string ImePacijenta, int dan, int mjesec, int godina, int sati, int minute){
    niz_pregleda.push_back(std::make_shared<Pregled>(ImePacijenta, dan, mjesec, godina, sati, minute));
}

void Pregledi::RegistrirajPregled(std::shared_ptr<Pregled> pregled){
    niz_pregleda.push_back(pregled);
}

int Pregledi::DajBrojPregledaNaDatum(Datum dat) const {
    int dan2, mjesec2, godina2;
    std::tie(dan2, mjesec2, godina2) = dat.Ocitaj();

    int br_pregleda = std::count_if(niz_pregleda.begin(), niz_pregleda.end(), [dan2, mjesec2, godina2](std::shared_ptr<Pregled> A){
        int dan1, mjesec1, godina1;
        std::tie(dan1, mjesec1, godina1) = A->DajDatumPregleda().Ocitaj();
        return (dan1 == dan2 && mjesec1 == mjesec2 && godina1 == godina2);
    });
    return br_pregleda;
}

Pregled Pregledi::DajNajranijiPregled() const {
    if(niz_pregleda.size() == 0) throw std::domain_error("Nema registriranih pregleda");
    auto najraniji = std::min_element(niz_pregleda.begin(), niz_pregleda.end(), [](std::shared_ptr<Pregled> p1, std::shared_ptr<Pregled> p2){
        auto datum1 = p1->DajDatumPregleda().Ocitaj();
        auto datum2 = p2->DajDatumPregleda().Ocitaj();
        if(datum1 != datum2){
            return datum1 < datum2;
        }
        auto vrijeme1 = p1->DajVrijemePregleda().Ocitaj();
        auto vrijeme2 = p2->DajVrijemePregleda().Ocitaj();
        return vrijeme1 < vrijeme2;
    });
    return **najraniji;
}

Pregled &Pregledi::DajNajranijiPregled(){
    if(niz_pregleda.size() == 0) throw std::domain_error("Nema registriranih pregleda");
    auto najraniji = std::min_element(niz_pregleda.begin(), niz_pregleda.end(), [](std::shared_ptr<Pregled> p1, std::shared_ptr<Pregled> p2){
        auto datum1 = p1->DajDatumPregleda().Ocitaj();
        auto datum2 = p2->DajDatumPregleda().Ocitaj();
        if(datum1 != datum2){
            return datum1 < datum2;
        }
        auto vrijeme1 = p1->DajVrijemePregleda().Ocitaj();
        auto vrijeme2 = p2->DajVrijemePregleda().Ocitaj();
        return vrijeme1 < vrijeme2;
    });
    return **najraniji;
}

void Pregledi::IsprazniKolekciju(){
    niz_pregleda.clear();
}

void Pregledi::ObrisiNajranijiPregled(){
    if(niz_pregleda.size() == 0) throw std::range_error("Prazna kolekcija");

    std::sort(niz_pregleda.begin(), niz_pregleda.end(), [](const std::shared_ptr<Pregled>& A, const std::shared_ptr<Pregled>& B){
        return Pregled::DolaziPrije(*A, *B);
    });
    niz_pregleda.erase(niz_pregleda.begin());
}

void Pregledi::ObrisiPregledePacijenta(std::string pacijent){
    niz_pregleda.erase(std::remove_if(niz_pregleda.begin(), niz_pregleda.end(), [pacijent](const std::shared_ptr<Pregled> &pregled){
        return pregled->DajImePacijenta() == pacijent;
    }), niz_pregleda.end());
}

void Pregledi::IspisiPregledeNaDatum(Datum dat) const {
    std::vector<std::shared_ptr<Pregled>> kopija_pr = niz_pregleda;
    std::sort(kopija_pr.begin(), kopija_pr.end(), [](const std::shared_ptr<Pregled> a, const std::shared_ptr<Pregled> b){
        auto vrijeme1 = a->DajVrijemePregleda().Ocitaj();
        auto vrijeme2 = b->DajVrijemePregleda().Ocitaj();
        return vrijeme1 < vrijeme2;
    });
int d1, m1, g1, d2, m2, g2;
std::tie(d1, m1, g1) = dat.Ocitaj();
for(int i = 0; i < kopija_pr.size(); i++){
    std::tie(d2, m2, g2) = niz_pregleda[i]->DajDatumPregleda().Ocitaj();
    if(d1 == d2 && m1 == m2 && g1 == g2){
        niz_pregleda[i]->Ispisi();
    }
}
}

void Pregledi::IspisiSvePreglede() const {
    std::vector<std::shared_ptr<Pregled>> kopija_pr = niz_pregleda;
   
    std::sort(kopija_pr.begin(), kopija_pr.end(), [](const std::shared_ptr<Pregled> a, const std::shared_ptr<Pregled> b){
        auto datum1 = a->DajDatumPregleda().Ocitaj();
        auto datum2 = b->DajDatumPregleda().Ocitaj();
        if(datum1 != datum2){
            return datum1 < datum2;
        }else{
        auto vrijeme1 = a->DajVrijemePregleda().Ocitaj();
        auto vrijeme2 = b->DajVrijemePregleda().Ocitaj();
        return vrijeme1 < vrijeme2;
        }
    });
   
    for(const auto &pregled : kopija_pr) pregled->Ispisi();
}

int main ()
{
    Pregledi pregledi;

    for(;;){
        std::cout << "1. Registriraj Pregled" << std::endl;
        std::cout << "2. Daj Broj Pregleda" << std::endl;
        std::cout << "3. Daj Broj Pregleda Na Datum" << std::endl;
        std::cout << "4. Daj Najraniji Pregled" << std::endl;
        std::cout << "5. Obrisi Najraniji Pregled" << std::endl;
        std::cout << "6. Obrisi Preglede Pacijenta" << std::endl;
        std::cout << "7. Ispisi Preglede Na Datum" << std::endl;
        std::cout << "8. Ispisi Sve Preglede" << std::endl;
        std::cout << "9. Isprazni Kolekciju" << std::endl;
        std::cout << "10. Izlaz" << std::endl;
        std::cout << "Unesite izbor: ";

        int rezim;
        std::cin >> rezim;

        if(rezim == 10) break;

        switch(rezim){
            case 1:{
                std::string ImePac;
            int dan, mjesec, godina, sati, minute;
            std::cout << "Unesite ime pacijenta: ";
            std::cin.ignore(10000, '\n');
            std::getline(std::cin, ImePac);
            std::cout << "Unesite datum i vrijeme pregleda (dan mjesec godina sati minute): ";
            std::cin >> dan >> mjesec >> godina >> sati >> minute;
            try{
                pregledi.RegistrirajPregled(ImePac, dan, mjesec, godina, sati, minute);
            }catch(const std::range_error &greska){
                std::cout << greska.what() << std::endl;
            }
            break;
        }
          case 2:{
            std::cout << "Ukupan broj pregleda: " << pregledi.DajBrojPregleda() << std::endl;
            break;
          }
          case 3: {
            int dan, mjesec, godina;
            std::cout << "Unesite datum (dan mjesec godina): ";
            std::cin >> dan >> mjesec >> godina;
            Datum datum(dan, mjesec, godina);
            std::cout << "Broj pregleda na datum: " << pregledi.DajBrojPregledaNaDatum(datum) << std::endl;
            break;
        }case 4:{
            try{
                Pregled najraniji = pregledi.DajNajranijiPregled();
                najraniji.Ispisi();
            }catch(const std::range_error &greska){
                std::cout << greska.what() << std::endl;
            }
            break;
        }case 5:{
            try{
                pregledi.ObrisiNajranijiPregled();
                std::cout << "Najraniji pregled je obrisan." << std::endl;
            }catch(const std::range_error &greska){
                std::cout << greska.what() << std::endl;
            }
            break;
        }case 6:{
            std::string ImePacijenta;
            std::cout << "Unesite ime pacijenta: ";
            std::cin.ignore(10000, '\n');
            std::getline(std::cin, ImePacijenta);
            pregledi.ObrisiPregledePacijenta(ImePacijenta);
            std::cout << "Pregledi za pacijenta " << ImePacijenta << " su obrisani." << std::endl;
            break;
        }case 7: {
            int dan, mjesec, godina;
            std::cout << "Unesite datum: ";
            std::cin >> dan >> mjesec >> godina;
            Datum datum(dan, mjesec, godina);
            pregledi.IspisiPregledeNaDatum(datum);
            break;
        }case 8:{
            std::cout << "Svi pregledi: " << std::endl;
            pregledi.IspisiSvePreglede();
            break;
        }case 9: {
            pregledi.IsprazniKolekciju();
            std::cout << "Kolekcija ispraznjena." << std::endl;
            break;
        }default: {
            std::cout << "Nepoznat izbor" << std::endl;
            break;
        }
       }
    }
    std::cout << "Kraj programa." << std::endl;
	return 0;
}
