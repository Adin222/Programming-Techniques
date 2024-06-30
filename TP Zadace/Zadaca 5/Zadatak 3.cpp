//TP 2023/2024: Zadaća 5, Zadatak 3
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <fstream>
#include <stdexcept>

class Spremnik{
protected:
    std::string Naziv;
    double tezina;
public:
    Spremnik(double tezina, std::string Naziv) : Naziv(Naziv), tezina(tezina){}
    virtual ~Spremnik() = default; 
    virtual double DajTezinu() const { return tezina; }
    virtual double DajUkupnuTezinu() const = 0;
    virtual void Ispisi() const = 0;
    virtual Spremnik *DajKopiju() const = 0;
};

class Sanduk : public Spremnik {
    std::vector<double> TezineElemenata;
public:
    Sanduk(double tezina, std::string Naziv, std::vector<double> TezineElemenata) : Spremnik(tezina, Naziv), TezineElemenata(TezineElemenata){}
    double DajUkupnuTezinu() const override;
    void Ispisi() const override;
    Sanduk *DajKopiju() const override { return new Sanduk(*this);}
};

double Sanduk::DajUkupnuTezinu() const {
    double suma = 0.;
    for(double el : TezineElemenata){
        suma += el;
    }
    double temp = DajTezinu();
    return suma + temp;
}

void Sanduk::Ispisi() const {
    std::cout << "Vrsta spremnika: Sanduk" << std::endl;
    std::cout << "Sadrzaj: " << Naziv << std::endl;
    std::cout << "Tezine predmeta: ";
    for(auto tez : TezineElemenata){
        std::cout << tez << " ";
    }
    std::cout << "(kg)";
    std::cout << std::endl;
    std::cout << "Vlastita tezina: " << DajTezinu() << " (kg)" << std::endl;
    std::cout << "Ukupna tezina: " << DajUkupnuTezinu() << " (kg)" << std::endl;
}

class Vreca : public Spremnik{
    double TezinaMaterije;
public: 
    Vreca(double tezina, std::string Naziv, double TezinaMaterije) : Spremnik(tezina, Naziv), TezinaMaterije(TezinaMaterije) {}
    double DajUkupnuTezinu() const override { return DajTezinu() + TezinaMaterije; }
    void Ispisi() const override;
    Vreca *DajKopiju() const override { return new Vreca(*this); }
};

void Vreca::Ispisi() const {
    std::cout << "Vrsta spremnika: Vreca" << std::endl;
    std::cout << "Sadrzaj: " << Naziv << std::endl;
    std::cout << "Vlastita tezina: " << DajTezinu() << " (kg)" << std::endl;
    std::cout << "Tezina pohranjene materije: " << TezinaMaterije << " (kg)" << std::endl;
    std::cout << "Ukupna tezina: " << DajUkupnuTezinu() << " (kg)" << std::endl;
}

class Bure : public Spremnik{
    double SpecTezina, Zapremina;
public:
    Bure(double tezina, std::string Naziv, double SpecTezina, double Zapremina) : Spremnik(tezina, Naziv), SpecTezina(SpecTezina), Zapremina(Zapremina){}
    double DajUkupnuTezinu() const override { return DajTezinu() + ((SpecTezina * Zapremina) / 1000); }
    void Ispisi() const override;
    Bure *DajKopiju() const override { return new Bure(*this); }
};

void Bure::Ispisi() const {
    std::cout << "Vrsta spremnika: Bure" << std::endl;
    std::cout << "Sadrzaj: " << Naziv << std::endl;
    std::cout << "Vlastita tezina: " << DajTezinu() << " (kg)" << std::endl;
    std::cout << "Specificna tezina tecnosti: " << SpecTezina << " (kg/m^3)" << std::endl;
    std::cout << "Zapremina tecnosti: " << Zapremina << " (l)" << std::endl;
    std::cout << "Ukupna tezina: " << DajUkupnuTezinu() << " (kg)" << std::endl;
}

class Skladiste{
    std::vector<std::shared_ptr<Spremnik>> PametniVektor;
public:
    Skladiste(){}
    Skladiste(const Skladiste &s);
    Skladiste(Skladiste &&s);
    Skladiste &operator =(const Skladiste &s);
    Skladiste &operator =(Skladiste &&s);
    Spremnik *DodajBure(double Tezina, std::string Naziv, double SpecTezina, double Zapremina);
    Spremnik *DodajVrecu(double tezina, std::string Naziv, double TezinaMaterije);
    Spremnik *DodajSanduk(double tezina, std::string Naziv, std::vector<double> vek);
    Spremnik *DodajSpremnik(Spremnik *spremnik, bool uslov);
    void BrisiSpremnik(Spremnik *spremnik);
    Spremnik &DajNajtezi();
    Spremnik &DajNajlaksi();
    int BrojPreteskih(int iznos) const;
    void IzlistajSkladiste() const;
    void UcitajIzDatoteke(std::string naziv);
};

void Skladiste::BrisiSpremnik(Spremnik *spremnik){
    for(int i = 0; i < PametniVektor.size(); i++){
        if(spremnik == PametniVektor[i].get()){
            PametniVektor.erase(PametniVektor.begin() + i);
            break;
        }
    }
}

void Skladiste::UcitajIzDatoteke(std::string naziv){
    PametniVektor.clear();

    std::ifstream citanje(naziv);

    if(!citanje) throw std::logic_error("Trazena datoteka ne postoji");

    std::string str;
    char simbol;
    double tezina;

    while(true){
        citanje >> simbol;
        if(!citanje) break;
        citanje >> std::ws;
        std::getline(citanje, str);

        if(simbol == 'S'){
            int sirina;
            citanje >> tezina >> sirina;
            std::vector<double> vek(sirina);
            for(int i = 0; i < sirina; i++){
                citanje >> vek[i];
                if(!citanje) throw std::logic_error("Datoteka sadrzi besmislene podatke");
            }
            if(!citanje) throw std::logic_error("Datoteka sadrzi besmislene podatke");
            DodajSanduk(tezina, str, vek);
        }else if(simbol == 'V'){
            double materija;
            citanje >> tezina >> materija;
            if(!citanje) throw std::logic_error("Datoteka sadrzi besmislene podatke");
            DodajVrecu(tezina, str, materija);
        }else if(simbol == 'B'){
            double specT, Zaprem;
            citanje >> tezina >> specT >> Zaprem;
            if(!citanje) throw std::logic_error("Datoteka sadrzi besmislene podatke");
            DodajBure(tezina, str, specT, Zaprem);
        }else{
            std::string NijeValidan;
            std::getline(citanje, NijeValidan);
            if(!citanje) break;
        }
    }
    if(citanje.bad()){
       throw std::logic_error("Problemi pri citanju datoteke");  
    }
}

void Skladiste::IzlistajSkladiste() const {
    std::vector<std::shared_ptr<Spremnik>> temp = PametniVektor;

    std::sort(temp.begin(), temp.end(), [](std::shared_ptr<Spremnik> a, std::shared_ptr<Spremnik> b){
        return a->DajUkupnuTezinu() > b->DajUkupnuTezinu();
    });
    for(auto elem : temp){
        elem->Ispisi();
    }
}

int Skladiste::BrojPreteskih(int iznos) const {
    int veci = std::count_if(PametniVektor.begin(), PametniVektor.end(), [iznos](std::shared_ptr<Spremnik> a){
        return a->DajUkupnuTezinu() > iznos;
    });
    return veci;
}

Spremnik &Skladiste::DajNajtezi(){
    if(PametniVektor.empty()) throw std::range_error("Skladiste je prazno");

    auto najtezi = std::max_element(PametniVektor.begin(), PametniVektor.end(), [](std::shared_ptr<Spremnik> a, std::shared_ptr<Spremnik> b){
        return a->DajTezinu() < b->DajTezinu();
    });
    return **najtezi;
}

Spremnik &Skladiste::DajNajlaksi(){
    if(PametniVektor.empty()) throw std::range_error("Skladiste je prazno");

    auto najtezi = std::min_element(PametniVektor.begin(), PametniVektor.end(), [](std::shared_ptr<Spremnik> a, std::shared_ptr<Spremnik> b){
        return a->DajTezinu() < b->DajTezinu();
    });
    return **najtezi;
}

Skladiste::Skladiste(const Skladiste &s){
    for(int i = 0; i < s.PametniVektor.size(); i++) PametniVektor.push_back(std::shared_ptr<Spremnik>(s.PametniVektor[i]->DajKopiju()));
}

Skladiste::Skladiste(Skladiste &&s){
    std::swap(PametniVektor, s.PametniVektor);
    s.PametniVektor.resize(0);
}

Skladiste &Skladiste::operator =(const Skladiste &s){
    for(auto element : PametniVektor){
        element = nullptr;
    }
    PametniVektor.resize(0);
    for(auto element : s.PametniVektor){
        PametniVektor.push_back(std::shared_ptr<Spremnik>(element));
    }
    return *this;
}

Skladiste &Skladiste::operator =(Skladiste &&s){
    std::swap(PametniVektor, s.PametniVektor);
    s.PametniVektor.resize(0);
    return *this;
}

Spremnik *Skladiste::DodajBure(double Tezina, std::string Naziv, double SpecTezina, double Zapremina) {
    Bure *temp = new Bure(Tezina, Naziv, SpecTezina, Zapremina);
    PametniVektor.push_back(std::shared_ptr<Spremnik>(temp));
    return temp;
}

Spremnik *Skladiste::DodajVrecu(double tezina, std::string Naziv, double TezinaMaterije) {
    Vreca *temp = new Vreca(tezina, Naziv, TezinaMaterije);
    PametniVektor.push_back(std::shared_ptr<Spremnik>(temp));
    return temp;
}

Spremnik *Skladiste::DodajSanduk(double tezina, std::string Naziv, std::vector<double> vek) {
    Sanduk *temp = new Sanduk(tezina, Naziv, vek);
    PametniVektor.push_back(std::shared_ptr<Spremnik>(temp));
    return temp;
}

Spremnik *Skladiste::DodajSpremnik(Spremnik *spremnik, bool uslov) {
    if(uslov){
        Spremnik *temp = spremnik->DajKopiju();
        delete spremnik;
        PametniVektor.push_back(std::shared_ptr<Spremnik>(temp));
        return temp;
    }else{
        Spremnik *temp = spremnik->DajKopiju();
        PametniVektor.push_back(std::shared_ptr<Spremnik>(temp));
        return temp;
    }
}

int main ()
{
    Skladiste skladiste;
    try{
        skladiste.UcitajIzDatoteke("ROBA.TXT");
        skladiste.IzlistajSkladiste();
    }catch(std::logic_error err){
        std::cout << err.what() << std::endl;
    }
	return 0;
}
