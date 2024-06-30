#include <iostream>
#include <string>
#include <stdexcept>
#include <iomanip>
#include <limits>
#include <algorithm>

class GradjaninBiH{
    std::string ImePrezime;
    long long int JMBG;
    long long int OkreniMaticni(long long int jmbg);
    bool JeValidanJMBG(long long int JMBG);
    bool JedinstvenJMBG(long long int JMBG);
    bool ValidanJe(int dan, int mjesec, int godina, int sifra_regije);
    int DajKod(long long int jmbg);
    int C13(long long int jmbg);
    std::string DajDatumIRegiju(long long int broj);
    long long int Stoll(std::string broj);
    GradjaninBiH *prethodni;
    static GradjaninBiH *posljednji;
public:
    enum Pol {Musko, Zensko};
    GradjaninBiH(std::string ImePrezime, long long int JMBG);
    GradjaninBiH(std::string ImePrezime, int dan, int mjesec, int godina, int sifra_regije, Pol spol);
    ~GradjaninBiH();
    GradjaninBiH(const GradjaninBiH &l) = delete;
    GradjaninBiH &operator = (const GradjaninBiH &l) = delete;
    std::string DajImeIPrezime() const { return ImePrezime; }
    long long int DajJMBG() const { return JMBG; }
    int DajDanRodjenja() const;
    int DajMjesecRodjenja() const;
    int DajGodinuRodjenja() const;
    int DajSifruRegije() const;
    Pol DajPol() const;
    GradjaninBiH &PromijeniImeIPrezime(std::string DrugoImePrezime);
    friend int NajmanjiNedostajuci(int niz[], int velicina, Pol spol);
};

long long int GradjaninBiH::Stoll(std::string str){
    long long int rezultat = 0;
    int znak = 1;
    int i = 0;

    if(str[0] == '-'){
        znak = -1;
        i = 1;
    }
    for(; i < str.size(); i++){
        char cifra = str[i];
        if(cifra >= '0' && cifra <= '9'){
            rezultat = rezultat * 10 + (cifra - '0');
        }else{
            continue;
        }
    }
    return rezultat * znak;
}

int GradjaninBiH::DajKod(long long int jmbg){
    jmbg /= 10;
    int vrati = jmbg % 1000;
return vrati;
}

GradjaninBiH::Pol GradjaninBiH::DajPol() const {
    long long int jmbg = JMBG;
    jmbg /= 10;
    int broj = jmbg % 1000;
    if(broj >= 0 && broj <= 499) return Pol::Musko;
    return Pol::Zensko;
}

long long int GradjaninBiH::OkreniMaticni(long long int JMBG){
    long long int temp = JMBG, povratni = 0;
    int n;
    while(temp != 0){
        n = temp % 10;
        povratni = povratni * 10 + n;
        temp /= 10;
    }
    return povratni;
}

bool GradjaninBiH::JedinstvenJMBG(long long int JMBG){
    for(GradjaninBiH *p = posljednji; p != nullptr; p = p->prethodni){
        if(p->JMBG == JMBG) return false;
    }
    return true;
}

bool GradjaninBiH::JeValidanJMBG(long long int JMBG){
    long long int okrenuti = OkreniMaticni(JMBG);
    long long int testni = OkreniMaticni(JMBG);
    int niz[13]; int n, br, velicina = 0;
    while(testni != 0){
        testni /= 10;
        velicina ++;
    }
    if(velicina < 12) return false;
    if(velicina == 12){
        niz[0] = 0;
        br = 1;
    }else{
        br = 0;
    }
    for(int i = br; i < 13; i++){
        n = okrenuti % 10;
        niz[i] = n;
        okrenuti /= 10;
    }
    int c13 = 11 - (7 * (niz[0] + niz[6]) + 6 * (niz[1] + niz[7]) + 5 * (niz[2] + niz[8]) + 4 * (niz[3] + niz[9]) + 3 * (niz[4] + niz[10]) + 2 * (niz[5] + niz[11])) % 11;
    if (c13 == 11) c13 = 0;
    int dani = (10 * niz[0]) + niz[1];
    int mjesec = (10 * niz[2]) + niz[3];
    int godina = (100 * niz[4]) + (10 * niz[5]) + niz[6];
    int sifra_regije = (10 * niz[7]) + niz[8];
    int isti_dan = (100 * niz[9]) + (10 * niz[10]) + niz[11];

    if(godina >= 100 && godina <= 999){
        godina += 1000;
    }
    if(godina >= 0 && godina <= 99){
        godina += 2000;
    }
    if(c13 != niz[12]) return false;
    if(c13 == 10) return false;
    if(sifra_regije < 0 || sifra_regije > 99) return false;
    if(dani < 1 || dani > 31) return false;
    if(mjesec < 1 || mjesec > 12) return false;
    if(godina < 1921 || godina > 2021) return false;

    int DaniMjeseca[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    bool PrestupnaGodina = (godina % 4 == 0 && godina % 100 != 0) || (godina % 400 == 0);
    if(PrestupnaGodina && mjesec == 2){
        DaniMjeseca[1] = 29;
    }
    if(dani < 1 || dani > DaniMjeseca[mjesec - 1]) return false;
return true;
}

GradjaninBiH::GradjaninBiH(std::string ImePrezime, long long int JMBG) : ImePrezime(ImePrezime), JMBG(JMBG), prethodni(posljednji){
    if(ImePrezime.empty()) throw std::logic_error("Ime ne moze biti prazno!");
    if(!JeValidanJMBG(JMBG)) throw std::logic_error("JMBG nije validan");
    if(!JedinstvenJMBG(JMBG)) throw std::logic_error("Vec postoji gradjanin sa istim JMBG");
    posljednji = this;
}

bool GradjaninBiH::ValidanJe(int dan, int mjesec, int godina, int sifra_regije){
    if(godina >= 100 && godina <= 999) godina += 1000;
    if(godina >= 0 && godina <= 99) godina += 2000;

    if(godina < 1921 || godina > 2021) return false;
    if(sifra_regije < 0 || sifra_regije > 99) return false;
    if(dan < 1 || dan > 31) return false;
    if(mjesec < 1 || mjesec > 12) return false;

    int DaniMjeseca[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    bool PrestupnaGodina = (godina % 4 == 0 && godina % 100 != 0) || (godina % 400 == 0);
    if(PrestupnaGodina && mjesec == 2){
        DaniMjeseca[1] = 29;
    }
    if(dan < 1 || dan > DaniMjeseca[mjesec - 1]) return false;

return true;
}

std::string GradjaninBiH::DajDatumIRegiju(long long int broj){
    std::string jmbg_s = std::to_string(broj);
    std::string temp;
    for(int i = 0; i < 9; i++){
        temp += jmbg_s.at(i);
    }
return temp;
} 

int NajmanjiNedostajuci(int niz[], int velicina, GradjaninBiH::Pol spol){
    int pocetak = (spol == GradjaninBiH::Musko) ? 0 : 500;
    int kraj = (spol == GradjaninBiH::Musko) ? 499 : 999;

    bool prisutan[500] = {false};

    for(int i = 0; i < velicina; i++){
        if(niz[i] >= pocetak && niz[i] <= kraj){
            prisutan[niz[i] - pocetak] = true; 
        }
    }
    for(int i = 0; i < (kraj - pocetak + 1); i++){
        if(!prisutan[i]) return i + pocetak;
    }
    return -1;
}

int GradjaninBiH::C13(long long int parcijalni_jmbg){
    long long int okrenuti = OkreniMaticni(parcijalni_jmbg);
    int niz[12]; int c13, n;
    for(int i = 0; i < 12; i++){
        n = okrenuti % 10;
        niz[i] = n;
        okrenuti /= 10;
    }
    c13 = 11 - (7 * (niz[0] + niz[6]) + 6 * (niz[1] + niz[7]) + 5 * (niz[2] + niz[8]) + 4 * (niz[3] + niz[9]) + 3 * (niz[4] + niz[10]) + 2 * (niz[5] + niz[11])) % 11;
    if(c13 == 11) c13 = 0;
return c13;
}

GradjaninBiH::GradjaninBiH(std::string ImePrezime, int dan, int mjesec, int godina, int sifra_regije, Pol spol) : JMBG(0), ImePrezime(""), prethodni(posljednji){
    if(ImePrezime.empty()) throw std::logic_error("Ime ne moze biti prazno!");
    if(!ValidanJe(dan, mjesec, godina, sifra_regije)) throw std::logic_error("Neispravni podaci");

    int god = godina % 1000; std::string godinaS;

    if(god >= 0 && god <= 9){
        godinaS = "00" + std::to_string(god);
    }else if(god >= 10 && god <= 99){
        godinaS = "0" + std::to_string(god);
    }else{
        godinaS = std::to_string(god);
    }
    std::string danS = std::to_string(dan);
    std::string mjesecS = (mjesec < 10) ? "0" + std::to_string(mjesec) : std::to_string(mjesec);
    std::string sifra_regije_S = std::to_string(sifra_regije);

    std::string jmbg = danS + mjesecS + godinaS + sifra_regije_S;

    int niz[30];
    int i = 0;

    for(GradjaninBiH *p = posljednji; p != nullptr; p = p->prethodni){
        if(p->prethodni != nullptr && DajDatumIRegiju(p->JMBG) == jmbg && prethodni->DajPol() == spol){
            niz[i] = DajKod(p->JMBG);
            i++;
        }
    }
    int mali = NajmanjiNedostajuci(niz, i, spol);
    std::string kod;
    if(mali <= 9){
        jmbg += "00" + std::to_string(mali);
    }else if(mali >= 0 && mali <= 99){
        jmbg += "0" + std::to_string(mali);
    }else{
        jmbg += std::to_string(mali);
    }
    int c13 = GradjaninBiH::C13(Stoll(jmbg));
    jmbg += std::to_string(c13);
    if(!JedinstvenJMBG(Stoll(jmbg))) throw std::logic_error("Vec postoji gradjanin sa istim JMBG");
    GradjaninBiH::JMBG = Stoll(jmbg);
    GradjaninBiH::ImePrezime = ImePrezime;

    prethodni = posljednji;
    posljednji = this;
}

GradjaninBiH::~GradjaninBiH(){
    if(posljednji == this){
        posljednji = prethodni;
    }else{
        for(GradjaninBiH *p = posljednji; p != nullptr; p = p->prethodni){
            if(p->prethodni == this){
                p->prethodni = prethodni;
                break;
            }
        }
    }
}

int GradjaninBiH::DajDanRodjenja() const {
    long long int jmbg = JMBG;
    jmbg /= 100000000000;
    return jmbg;
}
int GradjaninBiH::DajMjesecRodjenja() const {
    long long int jmbg = JMBG;
    jmbg /= 1000000000;
    return jmbg % 100;
}

int GradjaninBiH::DajGodinuRodjenja() const {
    long long int jmbg = JMBG;
    jmbg /= 1000000;
    int godina = jmbg % 1000;
    if(godina >= 100 && godina <= 999) godina += 1000;
    if(godina >= 0 && godina <= 99) godina += 2000;
    return godina;
}

int GradjaninBiH::DajSifruRegije() const {
    long long int jmbg = JMBG;
    jmbg /= 10000;
    return jmbg % 100;
}
GradjaninBiH &GradjaninBiH::PromijeniImeIPrezime(std::string DrugoImePrezime){
    if(DrugoImePrezime.empty()) throw std::logic_error("Ime ne moze biti prazno!");
    GradjaninBiH::ImePrezime = DrugoImePrezime;
    return *this;
}

GradjaninBiH *GradjaninBiH::posljednji = nullptr;

int main ()
{
    int n, dan, mjesec, godina, sifra, spol_int;
    std::string ime_prezime;
    GradjaninBiH::Pol spol;
    std::cout << "Unesite broj gradjana:" << std::endl;
    std::cin >> n; int i = 0;
    GradjaninBiH **gradjani = nullptr;
    try{
        gradjani = new GradjaninBiH *[n]{};
        while(i < n){
            std::cin.clear(); std::cin.ignore(10000, '\n');
            std::cout << "Unesite podatke za " << i + 1 << ". gradjanina: " << std::endl;
            std::cout << "Ime i prezime: ";
            std::getline(std::cin, ime_prezime);
            std::cout << "Datum rodjenja: ";
            std::cin >> dan >> mjesec >> godina;

            std::cout << "Sifra regije: ";
            std::cin >> sifra;

            std::cout << "Pol ( 1-Musko, 2-Zensko ):" << std::endl;
            std::cin >> spol_int;
            if(spol_int == 1){
                spol = GradjaninBiH::Musko;
            }else if(spol_int == 2){
                spol = GradjaninBiH::Zensko;
            }else{
                throw std::logic_error("Neispravan unos za pol.");
            }
            try{
                gradjani[i] = new GradjaninBiH(ime_prezime, dan, mjesec, godina, sifra, spol);
                i++;
            }catch(std::logic_error err){
                std::cout << err.what() << ". Molimo ponovite unos." << std::endl;
                std::cout << std::endl;
            }
        }
        std::sort(gradjani, gradjani + n, [](GradjaninBiH *a, GradjaninBiH *b){
            if(a->DajGodinuRodjenja() != b->DajGodinuRodjenja())
            return a->DajGodinuRodjenja() < b->DajGodinuRodjenja();
            if(a->DajMjesecRodjenja() != b->DajMjesecRodjenja())
            return a->DajMjesecRodjenja() < b->DajMjesecRodjenja();
        return a->DajDanRodjenja() < b->DajDanRodjenja();
        });
        std::cout << "Gradjani sortirani po starosti: " << std::endl;
        for(int i = 0; i < n; i++){
            std::cout << gradjani[i]->DajImeIPrezime() << " " << gradjani[i]->DajDanRodjenja() << "." << gradjani[i]->DajMjesecRodjenja() << "." << gradjani[i]->DajGodinuRodjenja() << " JMBG: " << gradjani[i]->DajJMBG() << std::endl;
        }

    }catch(...){
        std::cout << "Problemi sa memorjom!";
        for(int i = 0; i < n; i++) delete gradjani[i];
        delete[] gradjani;
    }
    for(int i = 0; i < n; i++) delete gradjani[i];
    delete[] gradjani;
	return 0;
}
 