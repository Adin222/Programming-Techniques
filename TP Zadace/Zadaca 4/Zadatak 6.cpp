//TP 2023/2024: Zadaća 4, Zadatak 6
#include <cctype>
#include <iostream>
#include <string>
#include <map>
#include <stdexcept>
#include <iterator>
#include <algorithm>
#include <memory>

class Student{
    int indeks;
    std::string godina_studija;
    std::string ImePrezime;
    std::string Adresa;
    std::string BrojTelefona;
    std::string NormalizujPodatke(std::string Normalizuj);
    std::string GodinaStudija(std::string Normalizuj);
    bool ValidirajBroj(std::string BrojTelefona);
    bool ValidirajPodatke(int indeks, std::string godina_studija, std::string BrojTelefona);
public:
    Student(int indeks, std::string godina_studija, std::string ImePrezime, std::string Adresa, std::string BrojTelefona);
    int DajIndeks() const { return indeks; }
    std::string DajGodinuStudija() const { return godina_studija; }
    std::string DajImePrezime() const { return ImePrezime; }
    std::string DajAdresu() const { return Adresa; }
    std::string DajTelefon() const { return BrojTelefona; }
    void Ispisi() const;
};

bool Student::ValidirajBroj(std::string BrojTelefona){
    int len = BrojTelefona.length();
    int i = 0;

    for(int i = 0; i < len; i++){
        if(BrojTelefona.at(i) == ' ') return false;
    }

    if(i >= len || !std::isdigit(BrojTelefona[i])){
        return false;
    }
    while(i < len && std::isdigit(BrojTelefona[i])){
        i++;
    }
    if(i >= len || BrojTelefona[i] != '/'){
        return false;
    }
    i++;
    if(i >= len || !std::isdigit(BrojTelefona[i])){
        return false;
    }
    while(i < len && std::isdigit(BrojTelefona[i])){
        i++;
    }
    if(i >= len || (BrojTelefona[i] != '-' && BrojTelefona[i] != 45)){
        return false;
    }
    i++;
    if(i >= len || !std::isdigit(BrojTelefona[i])){
        return false;
    }
    while(i < len && std::isdigit(BrojTelefona[i])){
        i++;
    }
    return i == len;
}

std::string Student::NormalizujPodatke(std::string Normalizuj){
    int i = 0;
    while(i < Normalizuj.size() && Normalizuj.at(i) == ' '){
        i++;
    }
    if(i > 0){
        Normalizuj.erase(0, i);
    }
    int b = Normalizuj.size() - 1;
    while(b >= 0 && Normalizuj.at(b) == ' '){
        b--;
    }
    if(b < Normalizuj.size() - 1){
        Normalizuj.erase(b + 1);
    }
    return Normalizuj;
}

bool Student::ValidirajPodatke(int indeks, std::string godina_studija, std::string BrojTelefona){
    int temp = indeks; int brojac = 0;
    while(temp != 0){
        temp /= 10;
        brojac++;
    }
    if(brojac < 5) return false;
    std::string RijecnikUslova[11]{"1", "2", "3", "1/B", "2/B", "3/B", "1/M", "2/M", "1/D", "2/D", "3/D"};
    bool stanje = false;
    for(int i = 0; i < 11; i++){
        if(godina_studija == RijecnikUslova[i]){
            stanje = false;
            break;
        }else{
            stanje = true;
        }
    }
    if(stanje){
        return false;
    }
    
    if(!ValidirajBroj(NormalizujPodatke(BrojTelefona))) return false;
return true;
}

std::string Student::GodinaStudija(std::string Normalizuj){
    if(Normalizuj.substr(1, 2) == "/B" || Normalizuj.substr(1, 2) == "/M" || Normalizuj.substr(1, 2) == "/D"){
        return Normalizuj;
    }else{
        Normalizuj = std::to_string(std::stoi(Normalizuj)) + "/B";
    }
    return Normalizuj;
}

Student::Student(int indeks, std::string godina_studija, std::string ImePrezime, std::string Adresa, std::string BrojTelefona){
    if(!ValidirajPodatke(indeks, godina_studija, BrojTelefona)) throw std::domain_error("Neispravni parametri");

Student::indeks = indeks; Student::godina_studija = GodinaStudija(godina_studija);
Student::ImePrezime = NormalizujPodatke(ImePrezime); Student::Adresa = NormalizujPodatke(Adresa);
Student::BrojTelefona = NormalizujPodatke(BrojTelefona);
}

void Student::Ispisi() const {
    std::cout << "Broj indeksa: " << DajIndeks() << std::endl;
    std::cout << "Godina studija: " << DajGodinuStudija() << std::endl;
    std::cout << "Ime i prezime: " << DajImePrezime() << std::endl;
    std::cout << "Adresa: " << DajAdresu() << std::endl;
    std::cout << "Telefon: " << DajTelefon() << std::endl;
}

class Laptop{
    int ev_broj;
    std::string naziv;
    std::string karakteristike;
    Student *kod_koga_je;
public:
    Laptop(int ev_broj, std::string naziv, std::string karakteristike);
    int DajEvidencijskiBroj() const { return ev_broj; }
    std::string DajNaziv() const {return naziv; }
    std::string DajKarakteristike() const { return karakteristike; }
    void Zaduzi(Student &stud);
    void Razduzi();
    bool DaLiJeZaduzen() const ;
    Student &DajKodKogaJe();
    Student *DajPokKodKogaJe();
    void Ispisi() const;
};

void Laptop::Ispisi() const{
    std::cout << "Evidencijski broj: " << ev_broj << std::endl;
    std::cout << "Naziv: " << naziv << std::endl;
    std::cout << "Karakteristike: " << karakteristike << std::endl;
}

Laptop::Laptop(int ev_broj, std::string naziv, std::string karakteristike) : kod_koga_je(nullptr){
    if(ev_broj < 0) throw std::domain_error("Neispravni parametri");
    Laptop::ev_broj = ev_broj; Laptop::naziv = naziv;
    Laptop::karakteristike = karakteristike;
}

void Laptop::Zaduzi(Student &stud){
    if(kod_koga_je != nullptr) throw std::domain_error("Laptop vec zaduzen");
    kod_koga_je = &stud;
}

void Laptop::Razduzi(){
    if(kod_koga_je == nullptr) return;
    kod_koga_je = nullptr;
}

bool Laptop::DaLiJeZaduzen() const {
    if(kod_koga_je != nullptr) return true;
return false;
}

Student &Laptop::DajKodKogaJe(){
    if(kod_koga_je == nullptr) throw std::domain_error("Laptop nije zaduzen");
    return *kod_koga_je;
}

Student *Laptop::DajPokKodKogaJe(){
    return kod_koga_je;
}

class Administracija{
    std::map<int, std::shared_ptr<Student>> studenti;
    std::map<int, std::shared_ptr<Laptop>> laptopi;
public:
    Administracija(){}
    Administracija(const Administracija &l);
    Administracija(Administracija &&l);
    Administracija &operator =(const Administracija &l);
    Administracija &operator =(Administracija &&l);
    ~Administracija();
    void RegistrirajNovogStudenta(int indeks, std::string godina_studija, std::string ImePrezime, std::string Adresa, std::string BrojTelefona);
    void RegistrirajNoviLaptop(int ev_broj, std::string naziv, std::string karakteristike);
    Student &NadjiStudenta(int indeks);
    Student NadjiStudenta(int indeks) const;
    Laptop &NadjiLaptop(int ev_broj);
    Laptop NadjiLaptop(int ev_broj) const;
    void IzlistajStudente() const;
    void IzlistajLaptope() const;
    void ZaduziLaptop(int indeks, int ev_lap);
    int NadjiSlobodniLaptop();
    void RazduziLaptop(int ev_lap);
    void PrikaziZaduzenja() const;
};

Administracija::~Administracija(){
    for(auto it = studenti.begin(); it != studenti.end(); it++){
        it->second = nullptr;
    }
    for(auto it = laptopi.begin(); it != laptopi.end(); it++){
        it->second = nullptr;
    }
}

Administracija::Administracija(const Administracija &l){
    for(auto it = l.studenti.begin(); it != l.studenti.end(); it++){
        std::shared_ptr<Student> stud = std::shared_ptr<Student>(new Student((*it->second).DajIndeks(), (*it->second).DajGodinuStudija(), (*it->second).DajImePrezime(), (*it->second).DajAdresu(), (*it->second).DajTelefon()));
        *stud = *it->second;
        studenti.insert({it->first, stud});
    }
    for(auto it = l.laptopi.begin(); it != l.laptopi.end(); it++){
        std::shared_ptr<Laptop> pok = std::shared_ptr<Laptop>(new Laptop(((*it->second).DajEvidencijskiBroj()), ((*it->second).DajNaziv()), ((*it->second).DajKarakteristike())));
        if(((*it->second).DajPokKodKogaJe()) != nullptr){
            std::shared_ptr<Student> neki = nullptr;
            for(auto sudo = studenti.begin(); sudo != studenti.end(); sudo++){
                if(((*sudo->second).DajIndeks() == (*((*it->second).DajPokKodKogaJe())).DajIndeks()) && ((*sudo->second).DajImePrezime() == (*((*it->second).DajPokKodKogaJe())).DajImePrezime()) && ((*sudo->second).DajAdresu() == (*((*it->second).DajPokKodKogaJe())).DajAdresu()) && ((*sudo->second).DajGodinuStudija() == (*((*it->second).DajPokKodKogaJe())).DajGodinuStudija()) && ((*sudo->second).DajTelefon() == (*((*it->second).DajPokKodKogaJe())).DajTelefon())){
                    neki = sudo->second;
                    break;
                }                                                    
            }
             (*pok).Zaduzi(*neki);
        }
       laptopi.insert({it->first, pok});
    }
}

Administracija::Administracija(Administracija &&l){
    for(auto it = l.studenti.begin(); it != l.studenti.end(); it++){
        studenti.insert({it->first, it->second});
        it->second = nullptr;
    }

    for(auto it = l.laptopi.begin(); it != l.laptopi.end(); it++){
        laptopi.insert({it->first, it->second});
        it->second = nullptr;
    }
}


Administracija &Administracija::operator =(const Administracija &l){
    for(auto it = this->studenti.begin(); it != this->studenti.end(); it++){
        it->second = nullptr;
    }

    this->studenti.clear();
    for(auto it = this->laptopi.begin(); it != this->laptopi.end(); it++){
        it->second = nullptr;
    }
    this->laptopi.clear();

    for(auto it = l.studenti.begin(); it != l.studenti.end(); it++){
        std::shared_ptr<Student> pok = std::shared_ptr<Student>(new Student((*it->second).DajIndeks(), (*it->second).DajGodinuStudija(), (*it->second).DajImePrezime(), (*it->second).DajAdresu(), (*it->second).DajTelefon()));
        *pok = *it->second;
        studenti.insert({it->first, pok});
    }

    for(auto it = l.laptopi.begin(); it != l.laptopi.end(); it++){
        std::shared_ptr<Laptop> pok = std::shared_ptr<Laptop> (new Laptop(((*it->second).DajEvidencijskiBroj()), ((*it->second).DajNaziv()), ((*it->second).DajKarakteristike())));
        if(((*it->second).DajPokKodKogaJe()) != nullptr){
            std::shared_ptr<Student> neki = nullptr;
            for(auto sudo = studenti.begin(); sudo != studenti.end(); sudo++){
                if(((*sudo->second).DajImePrezime() == (*((*it->second).DajPokKodKogaJe())).DajImePrezime()) && ((*sudo->second).DajIndeks() == (*((*it->second).DajPokKodKogaJe())).DajIndeks()) && ((*sudo->second).DajAdresu() == (*((*it->second).DajPokKodKogaJe())).DajAdresu()) && ((*sudo->second).DajGodinuStudija() == (*((*it->second).DajPokKodKogaJe())).DajGodinuStudija()) && ((*sudo->second).DajTelefon() == (*((*it->second).DajPokKodKogaJe())).DajTelefon())){
                    neki = sudo->second;
                    break;
                }
            }
            (*pok).Zaduzi(*neki);
        }
        laptopi.insert({it->first, pok});
    }
return *this;
}

Administracija &Administracija::operator =(Administracija &&l){
    for(auto it = l.studenti.begin(); it != l.studenti.end(); it++){
        studenti.insert({it->first, it->second});
        it->second = nullptr;
    }
    for(auto it = l.laptopi.begin(); it != l.laptopi.end(); it++){
        laptopi.insert({it->first, it->second});
        it->second = nullptr;
    }
    return *this;
}

void Administracija::RegistrirajNovogStudenta(int indeks, std::string godina_studija, std::string ImePrezime, std::string Adresa, std::string BrojTelefona){
    for(auto it = studenti.begin(); it != studenti.end(); it++){
        if(it->second->DajIndeks() == indeks){
            throw std::domain_error("Student s tim indeksom vec postoji");
        }
    }
    std::shared_ptr<Student> pok = std::shared_ptr<Student>(new Student(indeks, godina_studija, ImePrezime, Adresa, BrojTelefona));
    studenti.insert({indeks, pok});
}

void Administracija::RegistrirajNoviLaptop(int ev_broj, std::string naziv, std::string karakteristike){
    for(auto it = laptopi.begin(); it != laptopi.end(); it++){
        if(it->second->DajEvidencijskiBroj() == ev_broj){
            throw std::domain_error("Laptop s tim evidencijskim brojem vec postoji");
        }
    }
    std::shared_ptr<Laptop> pok = std::shared_ptr<Laptop>(new Laptop(ev_broj, naziv, karakteristike));
    laptopi.insert({ev_broj, pok});
}

Student &Administracija::NadjiStudenta(int indeks){
    for(auto &par : studenti){
        if(par.first == indeks){
            return *(par.second);
        }
    }   
    throw std::domain_error("Student nije nadjen");
}

Student Administracija::NadjiStudenta(int indeks) const {
    for(auto &par : studenti){
        if(par.first == indeks){
            return *(par.second);
        }
    }
    throw std::domain_error("Student nije nadjen");
}

Laptop &Administracija::NadjiLaptop(int ev_broj){
    for(auto &par : laptopi){
        if(par.first == ev_broj){
            return *(par.second);
        }
    }
    throw std::domain_error("Laptop nije nadjen");
}

Laptop Administracija::NadjiLaptop(int ev_broj) const {
    for(auto &par : laptopi){
        if(par.first == ev_broj){
            return *(par.second);
        }
    }
    throw std::domain_error("Laptop nije nadjen");
}

void Administracija::IzlistajStudente() const {
    for(auto it = studenti.begin(); it != studenti.end(); it++){
        std::cout << std::endl << "Broj indeksa: " << it->first;
        std::cout << std::endl << "Godina studija: " << (*it->second).DajGodinuStudija();
        std::cout << std::endl << "Ime i prezime: " << (*it->second).DajImePrezime();
        std::cout << std::endl << "Adresa: " << (*it->second).DajAdresu();
        std::cout << std::endl << "Broj telefona: " << (*it->second).DajTelefon();
    }
}

void Administracija::IzlistajLaptope() const {
    for(auto it = laptopi.begin(); it != laptopi.end(); it++){
        std::cout << "Evidencijski broj: " << it->first;
        std::cout << std::endl << "Naziv: " << (*it->second).DajNaziv();
        std::cout << std::endl << "Karakteristike: " << (*it->second).DajKarakteristike();
        std::cout << std::endl; std::cout << std::endl;
        if(it->second->DajPokKodKogaJe() != nullptr) std::cout << std::endl << "Zaduzio(la): " << it->second->DajKodKogaJe().DajImePrezime() << " (" << it->second->DajKodKogaJe().DajIndeks() << ") ";
    }
    std::cout << std::endl;
}

void Administracija::ZaduziLaptop(int indeks, int ev_lap){
    std::shared_ptr<Student> nadjenStud = nullptr;
    std::shared_ptr<Laptop> nadjenLap = nullptr;
    for(auto &par : studenti){
        if(par.first == indeks){
            nadjenStud = par.second;
            break;
        }
    }
    if(nadjenStud == nullptr){
        throw std::domain_error("Student nije nadjen");
    }
    for(auto &par : laptopi){
        if(par.first == ev_lap){
            nadjenLap = par.second;
            break;
        }
    }
    if(nadjenLap == nullptr){
        throw std::domain_error("Laptop nije nadjen");
    }
    if(nadjenLap->DaLiJeZaduzen()){
        throw std::domain_error("Laptop vec zaduzen");
    }
    for(const auto &par : laptopi){
        if(par.second->DaLiJeZaduzen() && par.second->DajKodKogaJe().DajIndeks() == indeks){
            throw std::domain_error("Student je vec zaduzio laptop");
        }
    }
    laptopi[ev_lap]->Zaduzi(*studenti[indeks]);
}

int Administracija::NadjiSlobodniLaptop(){
    for(auto it = laptopi.begin(); it != laptopi.end(); it++){
        if(!it->second->DaLiJeZaduzen()){
           return it->second->DajEvidencijskiBroj();
        }
    }
    throw std::domain_error("Nema slobodnih laptopa");
}

void Administracija::RazduziLaptop(int ev_lap){
  std::shared_ptr<Laptop> nadjenLap = nullptr;
  for(auto &par : laptopi){
      if(par.first == ev_lap){
          nadjenLap = par.second;
          break;
      }
  }
  if(nadjenLap == nullptr){
      throw std::domain_error("Laptop nije nadjen");
  }
  if(nadjenLap->DajPokKodKogaJe() == nullptr){
      throw std::domain_error("Laptop nije zaduzen");
  }
  laptopi[ev_lap]->Razduzi();
}

void Administracija::PrikaziZaduzenja() const {
    int brojac = 0;
    for(auto it = laptopi.begin(); it != laptopi.end(); it++){
        if(it->second->DaLiJeZaduzen()){
            brojac++; 
        }
    }
    if(brojac == 0){
        std::cout << "Nema zaduzenja" << std::endl;
    }
    for(auto it = laptopi.begin(); it != laptopi.end(); it++){
        if(it->second->DaLiJeZaduzen()){
            std::cout << "Student " << it->second->DajKodKogaJe().DajImePrezime() << " (" << it->second->DajKodKogaJe().DajIndeks() << ") " << "zaduzio/la laptop broj " << it->second->DajEvidencijskiBroj() << std::endl; 
        }
    }
}



int main ()
{
    Administracija admin;
    int opcija;
    std::string str_ulaz;

    for(;;){
        std::cin.clear();
        std::cout << "Odaberite jednu od sljedecih opcija: " << std::endl;
        std::cout << "1 - RegistrirajNovogStudenta" << std::endl;
        std::cout << "2 - RegistrirajNoviLaptop" << std::endl;
        std::cout << "3 - IzlistajStudente" << std::endl;
        std::cout << "4 - IzlistajLaptope" << std::endl;
        std::cout << "5 - NadjiSlobodniLaptop" << std::endl;
        std::cout << "6 - ZaduziLaptop" << std::endl;
        std::cout << "7 - RazduziLaptop" << std::endl;
        std::cout << "8 - PrikaziZaduzenja" << std::endl;
        std::cout << "K - Kraj programa" << std::endl;

        std::getline(std::cin, str_ulaz);

        try{
            opcija = std::stoi(str_ulaz);
        }catch(...){
            break;
        }

        try{
            switch(opcija){
                case 1: {
                     int indeks;
                     std::string gs, ime_prez, adresa, broj;
                     std::cout << "Unesite broj indeksa: " << std::endl;
                     std::cin >> indeks;
                     std::cin.ignore();
                     std::cout << "Unesite godinu studija (formata A/X, gdje je A godina studija, a X prima vrijednosti B,M,D, zavisno od studija): " << std::endl;
                     std::getline(std::cin, gs);
                     std::cout << "Unesite ime i prezime studenta: " << std::endl;
                     std::getline(std::cin, ime_prez);
                     std::cout << "Unesite adresu studenta: " << std::endl;
                     std::getline(std::cin, adresa);
                     std::cout << "Unesite broj telefona (formata x/x-x): " << std::endl;
                     std::getline(std::cin, broj);
                     admin.RegistrirajNovogStudenta(indeks, gs, ime_prez, adresa, broj);
                     break;
                }
                case 2: {
                    int ev_broj;
                    std::string naziv, karak;
                    std::cout << "Unesite evidencijski broj laptopa " << std::endl;
                    std::cin >> ev_broj;
                    std::cin.ignore();
                    std::cout << "Unesite naziv laptopa " << std::endl;
                    std::getline(std::cin, naziv);
                    std::cout << "Unesite karakteristike laptopa: " << std::endl;
                    std::getline(std::cin, karak);
                    admin.RegistrirajNoviLaptop(ev_broj, naziv, karak);
                    std::cout << "Laptop uspjesno registrovan!" << std::endl;
                    break;
                }
                case 3: {
                    admin.IzlistajStudente();
                    break;
                }
                case 4: {
                    admin.IzlistajLaptope();
                    break;

                }case 5: {
                    try{
                        int slobodni = admin.NadjiSlobodniLaptop();
                        std::cout << "Prvi slobodni laptop ima evidencijski broj: " << slobodni << std::endl;
                    }catch(std::domain_error &izz){
                        std::cout << izz.what() << std::endl;
                    }
                    break;
                }
                case 6: {
                    int indeks, ev_broj;
                    std::cout << "Unesite indeks studenta: ";
                    std::cin >> indeks;
                    std::cout << "Unesite evidencijski broj laptopa: ";
                    std::cin >> ev_broj;
                    admin.ZaduziLaptop(indeks, ev_broj);
                    break;
                }
                case 7 : {
                    int ev_broj;
                    std::cout << "Unesite evidencijski broj laptopa: ";
                    std::cin >> ev_broj;
                    admin.RazduziLaptop(ev_broj);
                    break;
                }
                
                case 8 : {
                    admin.PrikaziZaduzenja();
                    break;
                }
                default: std::cout << "Nepostojeca opcija. Pokusajte ponovo." << std::endl;
            }
        }catch(std::domain_error& e){
            std::cout << "Izuzetak: " << e.what() << "!"<< std::endl;
        }catch(std::logic_error &e){
            std::cout << "Izuzetak: " << e.what() << "!" << std::endl;
        }
    }
	return 0;
}
 
