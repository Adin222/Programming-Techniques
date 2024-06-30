//TP 2023/2024: Zadaća 5, Zadatak 1
#include <cctype>
#include <iostream>
#include <initializer_list>
#include <stdexcept>
#include <limits>
#include <cmath>
#include <string>
#include <sstream>
#include <cctype>

class Razlomak{
    long long int brojnik, nazivnik;
    static long long int NZD(long long int a, long long int b);
    bool JeLiPrekoracioSabiranje(long long int a, long long int b);
    bool JeLiPrekoracioOduzimanje(long long int a, long long int b);
    bool JeLiPrekoracioDijeljenje(long long int a, long long int b);
    bool JeLiPrekoracioMnozenje(long long int a, long long int b);
    static bool JeInt(std::string &str);
public:
    Razlomak() : brojnik(0), nazivnik(1) {}
    Razlomak(long long int brojnik) : brojnik(brojnik), nazivnik(1) {}
    Razlomak(long long int brojnik, long long int nazivnik);
    Razlomak(std::initializer_list<long long int> lista);
    long long int DajBrojnik() const { return brojnik; }
    long long int DajNazivnik() const { return nazivnik; }
    operator long double() const { return static_cast<long double>(brojnik) / static_cast<long double>(nazivnik); }
    Razlomak &operator +=(const Razlomak &razlomak);
    Razlomak &operator -=(const Razlomak &razlomak);
    Razlomak &operator /=(const Razlomak &razlomak);
    Razlomak &operator *=(const Razlomak &razlomak);
    Razlomak operator +() const { return *this;}
    Razlomak &operator -();
    friend bool operator <(const Razlomak &raz1, const Razlomak &raz2);
    friend bool operator >(const Razlomak &raz1, const Razlomak &raz2);
    friend bool operator <=(const Razlomak &raz1, const Razlomak &raz2);
    friend bool operator >=(const Razlomak &raz1, const Razlomak &raz2);
    friend bool operator ==(const Razlomak &raz1, const Razlomak &raz2);
    friend bool operator !=(const Razlomak &raz1, const Razlomak &raz2);
    friend Razlomak operator +(const Razlomak &raz1, const Razlomak &raz2);
    friend Razlomak operator -(const Razlomak &raz1, const Razlomak &raz2);
    friend Razlomak operator *(const Razlomak &raz1, const Razlomak &raz2);
    friend Razlomak operator /(const Razlomak &raz1, const Razlomak &raz2);
    friend Razlomak &operator ++(Razlomak &razlomak);
    friend Razlomak operator ++(Razlomak &razlomak, int);
    friend Razlomak &operator --(Razlomak &razlomak);
    friend Razlomak operator --(Razlomak &razlomak, int);
    friend std::ostream &operator <<(std::ostream &tok, const Razlomak &razlomak);
    friend std::istream &operator >>(std::istream &tok, Razlomak &razlomak);
};

bool Razlomak::JeInt(std::string &s){
    if(s.empty()) return false;

    std::size_t i = 0;

    if(s[0] == '-' || s[0] == '+'){
        if(s.size() == 1) return false;
        i = 1;
    }
    for(int i = 0; i < s.size(); i++){
        if(!std::isdigit(s[i])) return false;
    }
    return true;
}

std::ostream &operator <<(std::ostream &tok, const Razlomak &razlomak){
    if(razlomak.DajNazivnik() == 1){
        tok << razlomak.DajBrojnik();
    }else{
        tok << razlomak.DajBrojnik() << "/" << razlomak.DajNazivnik();
    }
    return tok;
}

std::istream &operator >>(std::istream &tok, Razlomak &razlomak){
    std::string input;
    tok >> input;
    
    std::size_t nadjen = input.find("/");
    if(nadjen != std::string::npos){
        std::string brojnik_str = input.substr(0, nadjen);
        std::string nazivnik_str = input.substr(nadjen + 1);
        if(!Razlomak::JeInt(brojnik_str) || !Razlomak::JeInt(nazivnik_str)){
            tok.setstate(std::ios::failbit);
            return tok;
        }
        try{
            int brojnik_1 = std::stoi(brojnik_str);
            int nazivnik_1 = std::stoi(nazivnik_str);

            if(nazivnik_1 == 0){
                tok.setstate(std::ios::failbit);
                return tok;
            }
            razlomak = Razlomak(static_cast<long long int>(brojnik_1), static_cast<long long int>(nazivnik_1));
        }catch(...){
            tok.setstate(std::ios::failbit);
        }
    }else{
        if(!Razlomak::JeInt(input)){
            tok.setstate(std::ios::failbit);
            return tok;
        }
        try{
            int brojnik_1 = std::stoi(input);
            razlomak = Razlomak(static_cast<long long int>(brojnik_1));
        }catch(...){
            tok.setstate(std::ios::failbit);
        }

    }
    return tok;
}

bool operator <(const Razlomak &raz1, const Razlomak &raz2){
return static_cast<long double>(raz1) < static_cast<long double>(raz2);
}

bool operator >(const Razlomak &raz1, const Razlomak &raz2){
return static_cast<long double>(raz1) > static_cast<long double>(raz2);
}

bool operator ==(const Razlomak &raz1, const Razlomak &raz2){
return static_cast<long double>(raz1) == static_cast<long double>(raz2);
}

bool operator >=(const Razlomak &raz1, const Razlomak &raz2){
return(raz1 > raz2 || raz1 == raz2);
}

bool operator <=(const Razlomak &raz1, const Razlomak &raz2){
return (raz1 < raz2 || raz1 == raz2);
}

bool operator !=(const Razlomak &raz1, const Razlomak &raz2){
    return !(raz1 == raz2);
}

Razlomak &operator ++(Razlomak &razlomak){
    razlomak += 1;
    return razlomak;
}

Razlomak operator ++(Razlomak &razlomak, int){
    Razlomak pomocni = razlomak;
    ++razlomak;
    return pomocni;
}

Razlomak &operator --(Razlomak &razlomak){
    razlomak -= 1;
    return razlomak;
}

Razlomak operator --(Razlomak &razlomak, int){
    Razlomak pomocni = razlomak;
    --razlomak;
    return pomocni;
}


Razlomak &Razlomak::operator -(){
    if(brojnik == std::numeric_limits<long long int>::min()) throw std::overflow_error("Greska");
    Razlomak::brojnik *= -1;
    return *this;
}

bool Razlomak::JeLiPrekoracioSabiranje(long long int a, long long int b){
    if(b > 0 && a > std::numeric_limits<long long int>::max() - b) return true;
    if(b < 0 && a < std::numeric_limits<long long int>::min() - b) return true;
    return false;
}

bool Razlomak::JeLiPrekoracioOduzimanje(long long int a, long long int b){
    if(b < 0 && a > std::numeric_limits<long long int>::max() + b) return true;
    if(b > 0 && a < std::numeric_limits<long long int>::min() + b) return true;
    return false;
}

bool Razlomak::JeLiPrekoracioMnozenje(long long int a, long long int b){
    long long int M = std::numeric_limits<long long int>::max();
    long long int m = std::numeric_limits<long long int>::min();

    if(b > 0){
        if(a > M / b || a < m / b) return true;
    }else if(b < 0){
        if(a > 0){
            if(b < m / a || b > M / a) return true;
        }else{
            if(a < 0){
                if(a + M < 0 || b + M < 0) return true;
                if(-a > M / (-b) || -a < m / (-b)) return true;
            }
        }
    }
    return false;
}

bool Razlomak::JeLiPrekoracioDijeljenje(long long int a, long long int b){
    if(b < 0){
        if(a > 0 && b >= (a - 1) / std::numeric_limits<long long int>::min()) return true;
        if(a < 0 && b >= (a + 1) / std::numeric_limits<long long int>::max()) return true;
    }

    if(a == std::numeric_limits<long long int>::min() && b == -1) return true;

    return false;
}

long long int Razlomak::NZD(long long int a, long long int b){
    if(b == 0) return std::abs(a);
    return NZD(b, a % b);
}

Razlomak &Razlomak::operator /=(const Razlomak &razlomak){
    if(JeLiPrekoracioMnozenje(DajBrojnik(), razlomak.DajBrojnik() || JeLiPrekoracioMnozenje(DajNazivnik(), razlomak.DajNazivnik()))){
        throw std::overflow_error("Nemoguce dobiti tacan rezultat");
    }

    long long int brojnik_novi = DajBrojnik() * razlomak.DajNazivnik();
    long long int nazivnik_novi = DajNazivnik() * razlomak.DajBrojnik();

    if(JeLiPrekoracioDijeljenje(brojnik_novi, nazivnik_novi)) throw std::overflow_error("Nemoguce dobiti tacan rezultat");

    *this = Razlomak(brojnik_novi, nazivnik_novi);
    return *this;
}

Razlomak operator /(const Razlomak &raz1, const Razlomak &raz2){
    Razlomak raz = raz1;
    raz /= raz2;
    return raz;
}

Razlomak &Razlomak::operator *=(const Razlomak &razlomak){
    long long int s = Razlomak::NZD(DajBrojnik(), razlomak.DajNazivnik());
    long long int t = Razlomak::NZD(DajBrojnik(), razlomak.DajNazivnik());

    long long int u = Razlomak::NZD(DajNazivnik(), razlomak.DajBrojnik());

    if(JeLiPrekoracioMnozenje((DajBrojnik() / s), (razlomak.DajBrojnik() / t)) || JeLiPrekoracioMnozenje(DajNazivnik() / t, razlomak.DajNazivnik() / s)){
        throw std::overflow_error("Nemoguce dobiti tacan rezultat");
    }

    long long int brojnik_novi = DajBrojnik() * (razlomak.DajBrojnik() / u);
    long long int nazivnik_novi = DajNazivnik() * razlomak.DajNazivnik();

    if(JeLiPrekoracioDijeljenje(brojnik_novi, nazivnik_novi)) throw std::overflow_error("Nemoguce dobiti tacan rezultat");

    *this = Razlomak(brojnik_novi, nazivnik_novi);
    return *this;
}

Razlomak operator *(const Razlomak &raz1, const Razlomak &raz2){
    Razlomak raz = raz1;
    raz *= raz2;
    return raz;
}

Razlomak &Razlomak::operator +=(const Razlomak &razlomak){
    long long int r = Razlomak::NZD(DajNazivnik(), razlomak.DajNazivnik());

    if(JeLiPrekoracioMnozenje(DajBrojnik(), (razlomak.DajNazivnik() / r)) || JeLiPrekoracioMnozenje(razlomak.DajBrojnik(), (DajNazivnik() / r)) || JeLiPrekoracioMnozenje(DajNazivnik(), (razlomak.DajNazivnik() / r))){
        throw std::overflow_error("Nemoguce dobiti tacan rezultat");
    }

    if(JeLiPrekoracioSabiranje(DajBrojnik() * (razlomak.DajNazivnik() / r), razlomak.DajBrojnik() * (DajNazivnik() / r))){
        throw std::overflow_error("Nemoguce dobiti tacan rezultat");
    }

    long long int brojnik_novi = DajBrojnik() * (razlomak.DajNazivnik() / r) + razlomak.DajBrojnik() * (DajNazivnik() / r);
    long long int nazivnik_novi = DajNazivnik() * (razlomak.DajNazivnik() / r);

    if(JeLiPrekoracioDijeljenje(brojnik_novi, nazivnik_novi)) throw std::overflow_error("Nemoguce dobiti tacan rezultat");

    *this = Razlomak(brojnik_novi, nazivnik_novi);
    return *this;
}

Razlomak operator +(const Razlomak &raz1, const Razlomak &raz2){
    Razlomak raz = raz1;
    raz += raz2;
    return raz;
}

Razlomak &Razlomak::operator -=(const Razlomak &razlomak){
    long long int r = Razlomak::NZD(DajNazivnik(), razlomak.DajNazivnik());

    if(JeLiPrekoracioMnozenje(DajBrojnik(), (razlomak.DajNazivnik() / r)) || JeLiPrekoracioMnozenje(razlomak.DajBrojnik(), (DajNazivnik() / r)) || JeLiPrekoracioMnozenje(DajNazivnik(), (razlomak.DajNazivnik() / r))){
        throw std::overflow_error("Nemoguce dobiti tacan rezultat");
    }

    if(JeLiPrekoracioOduzimanje(DajBrojnik() * (razlomak.DajNazivnik() / r), razlomak.DajBrojnik() * (DajNazivnik() / r))){
        throw std::overflow_error("Nemoguce dobiti tacan rezultat");
    }

    long long int brojnik_novi = DajBrojnik() * (razlomak.DajNazivnik() / r) - razlomak.DajBrojnik() * (DajNazivnik() / r);
    long long int nazivnik_novi = DajNazivnik() * (razlomak.DajNazivnik() / r);

    if(JeLiPrekoracioDijeljenje(brojnik_novi, nazivnik_novi)) throw std::overflow_error("Nemoguce dobiti tacan rezultat");

    *this = Razlomak(brojnik_novi, nazivnik_novi);
    return *this;
}

Razlomak operator -(const Razlomak &raz1, const Razlomak &raz2){
    Razlomak raz = raz1;
    raz -= raz2;
    return raz;
}

Razlomak::Razlomak(long long int brojnik, long long int nazivnik){
    if(nazivnik == 0) throw std::logic_error("Nekorektan razlomak");
    long long int nzd = NZD(brojnik, nazivnik);

    if(brojnik > 0 && nazivnik > 0){
        Razlomak::brojnik = brojnik / nzd;
        Razlomak::nazivnik = nazivnik / nzd;
    }
    if(brojnik < 0 && nazivnik > 0){
        Razlomak::brojnik = brojnik / nzd;
        Razlomak::nazivnik = nazivnik / nzd;
    }
    if(brojnik > 0 && nazivnik < 0){
        Razlomak::brojnik = -1 * (brojnik / nzd);
        Razlomak::nazivnik = -1 * (nazivnik / nzd);
    }
    if(brojnik < 0 && nazivnik < 0){
        Razlomak::brojnik = -1 * (brojnik / nzd);
        Razlomak::nazivnik = -1 * (nazivnik / nzd);
    }
}

Razlomak::Razlomak(std::initializer_list<long long int> lista){
    if(lista.size() > 2) throw std::domain_error("Previse parametara");
    if(lista.size() == 0){
        *this = Razlomak();
    }else if(lista.size() == 1){
        *this = Razlomak(*lista.begin());
    }else if (lista.size() == 2){
        auto it = lista.begin();
        *this = Razlomak(*it, *(std::next(it)));
    }
}


int main ()
{
    try{
        Razlomak r1(2337, 3740), r2(4014, 5225);
        Razlomak r3;
        r3 = r1 + r2;
        std::cout << r3.DajBrojnik() << " " << r3.DajNazivnik();
    }catch(std::overflow_error err){
        std::cout << "IZUZETAK: " << err.what();
    }
	return 0;
}
