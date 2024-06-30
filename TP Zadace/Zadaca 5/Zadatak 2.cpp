//TP 2023/2024: Zadaća 5, Zadatak 2
#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <functional>
#include <iomanip>
#include <stdexcept>

class Berza{
    int max_cijena, min_cijena;
    std::vector<int> dnevne_dionice;
public:
    explicit Berza(int max_cijena);
    Berza(int min_cijena, int max_cijena);
    void RegistrirajCijenu(int cijena);
    int DajBrojRegistriranihCijena() const { return dnevne_dionice.size(); }
    void BrisiSve() { dnevne_dionice.clear(); }
    int DajMinimalnuCijenu() const;
    int DajMaksimalnuCijenu() const;
    bool operator !();
    int operator [](int indeks) const;
    Berza &operator ++();
    Berza operator ++(int);
    Berza &operator --();
    Berza operator --(int);
    Berza operator -();
    Berza &operator +=(int broj);
    Berza &operator +=(const Berza &berza);
    Berza &operator -=(int broj);
    Berza &operator -=(const Berza &berza);
    int DajBrojCijenaVecihOd(int cijena);
    void Ispisi() const;
    friend Berza operator +(Berza &berza, int broj);
    friend Berza operator -(Berza berza, int broj);
    friend Berza operator +(int broj, Berza &berza);
    friend Berza operator -(int broj, Berza berza);
    friend Berza operator -(Berza berza1, Berza berza2);
    friend Berza operator +(Berza berza1, Berza berza2);
    friend bool operator ==(const Berza &b1, const Berza &b2);
    friend bool operator !=(const Berza &b1, const Berza &b2);
};

bool operator ==(const Berza &b1, const Berza &b2){
  if(b1.dnevne_dionice.size() == b2.dnevne_dionice.size() && std::equal(b1.dnevne_dionice.begin(), b1.dnevne_dionice.end(), b2.dnevne_dionice.begin())){
      return true;
  }
  return false;
}

bool operator !=(const Berza &b1, const Berza &b2){ return !(b1 == b2); }

Berza &Berza::operator +=(int broj){
    return *this = *this + broj;
}

Berza &Berza::operator +=(const Berza &b){
    return *this = *this + b;
}

Berza &Berza::operator -=(int broj){
    return *this = *this - broj;
}

Berza &Berza::operator -=(const Berza &berza){
    return *this = *this - berza;
}

Berza operator +(Berza berza1, Berza berza2){
    if(berza1.dnevne_dionice.size() != berza2.dnevne_dionice.size()) throw std::domain_error("Nesaglasni operandi");
    if((berza1.max_cijena != berza2.max_cijena) || (berza1.min_cijena != berza2.min_cijena)) throw std::domain_error("Nesaglasni operandi");
    std::transform(berza1.dnevne_dionice.begin(), berza1.dnevne_dionice.end(), berza2.dnevne_dionice.begin(), berza1.dnevne_dionice.begin(), std::plus<int>());
    if(std::find_if(berza1.dnevne_dionice.begin(), berza1.dnevne_dionice.end(), std::bind(std::greater<int>(), std::placeholders::_1, berza1.max_cijena)) != berza1.dnevne_dionice.end()){
        throw std::domain_error("Prekoracen dozvoljeni opseg cijena");
    }
    return berza1;
}

Berza operator -(Berza berza1, Berza berza2){
    if(berza1.dnevne_dionice.size() != berza2.dnevne_dionice.size()) throw std::domain_error("Nesaglasni operandi");
    if((berza1.max_cijena != berza2.max_cijena) || (berza1.min_cijena != berza2.min_cijena)) throw std::domain_error("Nesaglasni operandi");
    std::transform(berza1.dnevne_dionice.begin(), berza1.dnevne_dionice.end(), berza2.dnevne_dionice.begin(), berza1.dnevne_dionice.begin(), std::minus<int>());
    if(std::find_if(berza1.dnevne_dionice.begin(), berza1.dnevne_dionice.end(), std::bind(std::less<int>(), std::placeholders::_1, berza1.min_cijena)) != berza1.dnevne_dionice.end()){
        throw std::domain_error("Prekoracen dozvoljeni opseg cijena");
    }
    return berza1;
}


Berza operator -(int broj, Berza berza){
    if(std::find_if(berza.dnevne_dionice.begin(), berza.dnevne_dionice.end(), std::bind(std::less<int>(), broj - berza.min_cijena, std::placeholders::_1)) != berza.dnevne_dionice.end()){
        throw std::domain_error("Prekoracen dozvoljeni opseg cijena");
    }
    if(std::find_if(berza.dnevne_dionice.begin(), berza.dnevne_dionice.end(), std::bind(std::greater<int>(), broj - berza.max_cijena, std::placeholders::_1)) != berza.dnevne_dionice.end()){
        throw std::domain_error("Prekoracen dozvoljeni opseg cijena");
    }
    std::transform(berza.dnevne_dionice.begin(), berza.dnevne_dionice.end(), berza.dnevne_dionice.begin(), std::bind(std::minus<int>(), broj, std::placeholders::_1));
    return berza;
}

Berza operator +(int broj, Berza &berza){
    return berza + broj;
}

Berza operator +(Berza &berza, int broj){
    auto preko = std::bind(std::greater<int>(), std::placeholders::_1, berza.max_cijena);

    if(std::any_of(berza.dnevne_dionice.begin(), berza.dnevne_dionice.end(), std::bind(preko, std::bind(std::plus<int>(), std::placeholders::_1, broj)))){
        throw std::domain_error("Prekoracen dozvoljeni opseg cijena");
    }
    std::transform(berza.dnevne_dionice.begin(), berza.dnevne_dionice.end(), berza.dnevne_dionice.begin(), std::bind(std::plus<int>(), std::placeholders::_1, broj));
    return berza;
}

Berza operator -(Berza berza, int broj){
    auto ispod = std::bind(std::less<int>(), std::placeholders::_1, berza.min_cijena);

    if(std::any_of(berza.dnevne_dionice.begin(), berza.dnevne_dionice.end(), std::bind(ispod, std::bind(std::minus<int>(), std::placeholders::_1, broj)))){
        throw std::domain_error("Prekoracen dozvoljeni opseg cijena");
    }
    std::transform(berza.dnevne_dionice.begin(), berza.dnevne_dionice.end(), berza.dnevne_dionice.begin(), std::bind(std::minus<int>(), std::placeholders::_1, broj));
    return berza;
}

Berza Berza::operator -(){
    Berza kopija(*this);
    std::transform(kopija.dnevne_dionice.begin(), kopija.dnevne_dionice.end(), kopija.dnevne_dionice.begin(), std::bind(std::minus<int>(), std::placeholders::_1, (kopija.DajMaksimalnuCijenu() + kopija.DajMinimalnuCijenu())));
    return kopija;
}

Berza &Berza::operator ++(){
    if(std::find_if(dnevne_dionice.begin(), dnevne_dionice.end(), std::bind(std::greater<int>(), std::placeholders::_1, max_cijena - 100)) != dnevne_dionice.end()){
        throw std::range_error("Prekoracen dozvoljeni opseg cijena");
    }
    std::transform(dnevne_dionice.begin(), dnevne_dionice.end(), dnevne_dionice.begin(), std::bind(std::plus<int>(), std::placeholders::_1, 100));
    return *this;
}

Berza Berza::operator ++(int){
    if(std::find_if(dnevne_dionice.begin(), dnevne_dionice.end(), std::bind(std::greater<int>(), std::placeholders::_1, max_cijena - 100)) != dnevne_dionice.end()){
        throw std::range_error("Prekoracen dozvoljeni opseg cijena");
    }
    std::transform(dnevne_dionice.begin(), dnevne_dionice.end(), dnevne_dionice.begin(), std::bind(std::plus<int>(), std::placeholders::_1, 100));
    return *this;
}

Berza &Berza::operator --(){
    if(std::find_if(dnevne_dionice.begin(), dnevne_dionice.end(), std::bind(std::less<int>(), std::placeholders::_1, min_cijena + 100)) != dnevne_dionice.end()){
        throw std::range_error("Prekoracen dozvoljeni opseg cijena");
    }
    std::transform(dnevne_dionice.begin(), dnevne_dionice.end(), dnevne_dionice.begin(), std::bind(std::minus<int>(), std::placeholders::_1, 100));
    return *this;
}

Berza Berza::operator --(int){
    if(std::find_if(dnevne_dionice.begin(), dnevne_dionice.end(), std::bind(std::less<int>(), std::placeholders::_1, min_cijena + 100)) != dnevne_dionice.end()){
        throw std::range_error("Prekoracen dozvoljeni opseg cijena"); 
    }
    std::transform(dnevne_dionice.begin(), dnevne_dionice.end(), dnevne_dionice.begin(), std::bind(std::minus<int>(), std::placeholders::_1, 100));
    return *this;
}

int Berza::operator [](int indeks) const {
    if(indeks > dnevne_dionice.size() || indeks < 1) throw std::range_error("Neispravan indeks");
    return dnevne_dionice[indeks - 1];
}

int Berza::DajBrojCijenaVecihOd(int cijena){
    if(DajBrojRegistriranihCijena() == 0) throw std::range_error("Nema registriranih cijena");
    int broj = std::count_if(dnevne_dionice.begin(), dnevne_dionice.end(), std::bind(std::greater<int>(), std::placeholders::_1, cijena));
    return broj;
}

bool Berza::operator !(){
    if(DajBrojRegistriranihCijena() == 0) return true;
return false;
}

int Berza::DajMinimalnuCijenu() const {
    if(DajBrojRegistriranihCijena() == 0) throw std::range_error("Nema registriranih cijena");
    return *std::min_element(dnevne_dionice.begin(), dnevne_dionice.end());
}

int Berza::DajMaksimalnuCijenu() const {
    if(DajBrojRegistriranihCijena() == 0) throw std::range_error("Nema registriranih cijena");
    return *std::max_element(dnevne_dionice.begin(), dnevne_dionice.end());
}

Berza::Berza(int max_cijena){
    if(max_cijena < 0) throw std::range_error("Ilegalne granicne cijene");
    Berza::max_cijena = max_cijena;
    Berza::min_cijena = 0;
}

Berza::Berza(int min_cijena, int max_cijena){
    if(max_cijena < 0 || min_cijena < 0 || min_cijena > max_cijena) throw std::range_error("Ilegalne granicne cijene");
    Berza::max_cijena = max_cijena;
    Berza::min_cijena = min_cijena;
}

void Berza::RegistrirajCijenu(int cijena){
    if(cijena > max_cijena || cijena < min_cijena) throw std::range_error("Ilegalna cijena");
    dnevne_dionice.push_back(cijena);
}

void Berza::Ispisi() const {
    std::vector<double> kopija(dnevne_dionice.begin(), dnevne_dionice.end());
    std::sort(kopija.begin(), kopija.end(), std::greater<int>());
    std::transform(kopija.begin(), kopija.end(), std::ostream_iterator<double>(std::cout<<std::setprecision(2) << std::fixed, "\n"), std::bind(std::divides<double>(), std::placeholders::_1, 100.));
}


int main ()
{
	return 0;
}
