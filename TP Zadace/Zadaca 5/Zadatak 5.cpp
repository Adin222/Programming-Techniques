// TP 2023/2024: Zadaća 5, Zadatak 5
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>

template <typename TipEl> class Matrica {
  int br_redova, br_kolona;
  TipEl **elementi;
  char ime_matrice;
  static TipEl **AlocirajMemoriju(int br_redova, int br_kolona);
  static void DealocirajMemoriju(TipEl **elementi, int br_redova);
  void KopirajElemente(TipEl **elementi);

public:
  Matrica(int br_redova, int br_kolona, char ime = 0);
  Matrica(const Matrica &m);
  Matrica(Matrica &&m);
  Matrica(std::string ImeDatoteke, bool value);
  ~Matrica() { DealocirajMemoriju(elementi, br_redova); }
  Matrica &operator=(const Matrica &m);
  Matrica &operator=(Matrica &&m);
  template <typename Tip2>
  friend std::istream &operator>>(std::istream &tok, Matrica<Tip2> &m);
  template <typename Tip2>
  friend std::ostream &operator<<(std::ostream &tok, const Matrica<Tip2> &m);
  template <typename Tip2>
  friend Matrica<Tip2> operator+(const Matrica<Tip2> &m1, const Matrica<Tip2> &m2);
  template <typename Tip2>
  friend Matrica<Tip2> operator-(const Matrica<Tip2> &m1, const Matrica<Tip2> &m2);
  template <typename Tip2>
  friend Matrica<Tip2> operator*(const Matrica<Tip2> &m1, const Matrica<Tip2> &m2);
  template <typename Tip2>
  friend Matrica<Tip2> operator*(const Matrica<Tip2> &m, Tip2 d);
  template <typename Tip2>
  friend Matrica<Tip2> operator*(Tip2 d, const Matrica<Tip2> &m);
  Matrica &operator+=(const Matrica &m);
  Matrica &operator-=(const Matrica &m);
  Matrica &operator*=(const Matrica &m);
  Matrica &operator*=(TipEl d);
  TipEl *operator[](int i) { return elementi[i]; }
  const TipEl *operator[](int i) const { return elementi[i]; }
  TipEl &operator()(int i, int j);
  const TipEl &operator()(int i, int j) const;
  operator std::string() const;
  void SacuvajUTekstualnuDatoteku(std::string str);
  void SacuvajUBinarnuDatoteku(std::string str);
  void ObnoviIzTekstualneDatoteke(std::string str);
  void ObnoviIzBinarneDatoteke(std::string str);
};

template <typename TipEl>
Matrica<TipEl>::Matrica(std::string ImeDatoteke, bool value){
    if(value){
        std::ifstream ulazni_tok(ImeDatoteke, std::ios::binary);
        if(!std::ifstream(ImeDatoteke)) throw std::logic_error("Trazena datoteka ne postoji");

        DealocirajMemoriju(elementi, br_redova);

        ulazni_tok.read(reinterpret_cast<char*>(&br_redova), sizeof(br_redova));
        ulazni_tok.read(reinterpret_cast<char*>(&br_kolona), sizeof(br_kolona));

        elementi = new TipEl *[br_redova];

        for(int i = 0; i < br_redova; i++){
            ulazni_tok.read(reinterpret_cast<char*>(elementi[i]), br_kolona * sizeof(TipEl));
        }
        if(!ulazni_tok){
            for(int i = 0; i < br_redova; i++){
                delete[] elementi[i];
            }
            delete[] elementi;
            throw std::logic_error("Problemi pri citanju datoteke");
        }
        ulazni_tok.close();
    }else{
        std::ifstream ulazni_tok(ImeDatoteke);
        if(!std::ifstream(ImeDatoteke)) throw std::logic_error("Trazena datoteka ne postoji");
        std::string linija;
        std::vector<std::vector<TipEl>> temp;
        while(std::getline(ulazni_tok, linija)){
            std::istringstream linijski_tok(linija);
            std::vector<TipEl> red;
            TipEl element;
            while(linijski_tok >> element){
                red.push_back(element);
                if(linijski_tok.peek() == ',') linijski_tok.ignore();
            }
            if(!red.empty()) temp.push_back(red);
        }
        ulazni_tok.close();
        if(temp.empty()) throw std::logic_error("Datoteka sadrzi besmislene podatke");
        DealocirajMemoriju(elementi, br_redova);
        size_t broj_kolona = temp[0].size();
        for(auto red : temp){
            if(red.size() != broj_kolona) throw std::logic_error("Datoteka sadrzi besmislene podatke");
        }
        br_redova = temp.size();
        br_kolona = broj_kolona;
        for(int i = 0; i < br_redova; i++){
            elementi[i] = new TipEl[br_kolona];
            for(int j = 0; j < br_kolona; j++){
                elementi[i][j] = temp[i][j];
            }
        }
    }
}

template <typename TipEl>
void Matrica<TipEl>::SacuvajUTekstualnuDatoteku(std::string NazivDat) {
    std::ofstream izlazni_tok(NazivDat);
    if(!izlazni_tok) throw std::logic_error("Problemi sa upisom u datoteku");

    for(int i = 0; i < br_redova; i++){
        for(int j = 0; j < br_kolona; j++){
            if(j < br_kolona - 1){
                izlazni_tok << elementi[i][j] << ",";
            }else{
                izlazni_tok << elementi[i][j];
            }
            if(!izlazni_tok) throw std::logic_error("Problemi sa upisom u datoteku");
        }
        izlazni_tok << std::endl;
    }
}

template <typename TipEl>
void Matrica<TipEl>::SacuvajUBinarnuDatoteku(std::string NazivDat) {
    std::ofstream izlazni_tok(NazivDat);
    if(!izlazni_tok) throw std::logic_error("Problemi sa upisom u datoteku");

    izlazni_tok.write(reinterpret_cast<char*>(&br_redova), sizeof(br_redova));
    izlazni_tok.write(reinterpret_cast<char*>(&br_kolona), sizeof(br_kolona));

    for(int i = 0; i < br_redova; i++){
        izlazni_tok.write(reinterpret_cast<char *>(elementi[i]), br_kolona * sizeof(TipEl));
        if(!izlazni_tok) throw std::logic_error("Problemi sa upisom u datoteku");
    }
}

template <typename TipEl>
 void Matrica<TipEl>::ObnoviIzTekstualneDatoteke(std::string NazivDat){
    std::ifstream ulazni_tok(NazivDat);

    if(!std::ifstream(NazivDat)) throw std::logic_error("Trazena datoteka ne postoji");

    std::string linija;
    std::vector<std::vector<TipEl>> temp;

    while(std::getline(ulazni_tok, linija)){
        std::istringstream linijski_tok(linija);
        std::vector<TipEl> red;
        TipEl element;
        while(linijski_tok >> element){
            red.push_back(element);
            if(linijski_tok.peek() == ',') linijski_tok.ignore();
        }
        if(!red.empty()) temp.push_back(red);
    }
    
    ulazni_tok.close();
    if(temp.empty()) throw std::logic_error("Datoteka sadrzi besmislene podatke");


    DealocirajMemoriju(elementi, br_redova);

    size_t broj_kolona = temp[0].size();
    for(auto red : temp){
        if(red.size() != broj_kolona) throw std::logic_error("Datoteka sadrzi besmislene podatke");
    }
    br_redova = temp.size();
    br_kolona = broj_kolona;
    elementi = new TipEl *[br_redova];
    for(int i = 0; i < br_redova; i++){
        elementi[i] = new TipEl[br_kolona];
        for(int j = 0; j < br_kolona; j++){
            elementi[i][j] = temp[i][j];
        }
    }
    for(int i = 0; i < br_redova; i++){
        for(int j = 0; j < br_kolona; j++){
            std::cout << elementi[i][j];
        }
        std::cout << std::endl;
    }
}

template <typename TipEl>
void Matrica<TipEl>::ObnoviIzBinarneDatoteke(std::string NazivDat){
    std::ifstream ulazni_tok(NazivDat, std::ios::binary);
    if(!std::ifstream(NazivDat)) throw std::logic_error("Trazena datoteka ne postoji");

    DealocirajMemoriju(elementi, br_redova);

    ulazni_tok.read(reinterpret_cast<char*>(&br_redova), sizeof(br_redova));
    ulazni_tok.read(reinterpret_cast<char*>(&br_kolona), sizeof(br_kolona));

    elementi = new TipEl *[br_redova];
    for(int i = 0; i < br_redova; i++){
        elementi[i] = new TipEl[br_kolona];
    }
    for(int i = 0; i < br_redova; i++){
        ulazni_tok.read(reinterpret_cast<char *>(elementi[i]), br_kolona * sizeof(TipEl));
    }
    if(!ulazni_tok){
        for(int i = 0; i < br_redova; i++){
            delete[] elementi[i];
        }
        delete[] elementi;
        throw std::logic_error("Problemi pri citanju datoteke");
    }
    ulazni_tok.close();
    for(int i = 0; i < br_redova; i++){
        for(int j = 0; j < br_kolona; j++){
            std::cout << elementi[i][j];
        }
        std::cout << std::endl;
    }
}

template <typename TipEl>
TipEl **Matrica<TipEl>::AlocirajMemoriju(int br_redova, int br_kolona) {
  TipEl **elementi = new TipEl *[br_redova] {};
  try {
    for (int i = 0; i < br_redova; i++)
      elementi[i] = new TipEl[br_kolona];
  } catch (...) {
    DealocirajMemoriju(elementi, br_redova);
    throw;
  }
  return elementi;
}

template <typename TipEl>
void Matrica<TipEl>::DealocirajMemoriju(TipEl **elementi, int br_redova) {
  for (int i = 0; i < br_redova; i++)
    delete[] elementi[i];
  delete[] elementi;
}

template <typename TipEl>
Matrica<TipEl>::Matrica(int br_redova, int br_kolona, char ime)
    : br_redova(br_redova), br_kolona(br_kolona), ime_matrice(ime),
      elementi(AlocirajMemoriju(br_redova, br_kolona)) {}
template <typename TipEl>
void Matrica<TipEl>::KopirajElemente(TipEl **elementi) {
  for (int i = 0; i < br_redova; i++)
    for (int j = 0; j < br_kolona; j++)
      Matrica::elementi[i][j] = elementi[i][j];
}
template <typename TipEl>
Matrica<TipEl>::Matrica(const Matrica &m)
    : br_redova(m.br_redova), br_kolona(m.br_kolona),
      ime_matrice(m.ime_matrice),
      elementi(AlocirajMemoriju(m.br_redova, m.br_kolona)) {
  KopirajElemente(m.elementi);
}
template <typename TipEl>
Matrica<TipEl>::Matrica(Matrica &&m)
    : br_redova(m.br_redova), br_kolona(m.br_kolona), elementi(m.elementi),
      ime_matrice(m.ime_matrice) {
  m.br_redova = 0;
  m.elementi = nullptr;
}
template <typename TipEl>
Matrica<TipEl> &Matrica<TipEl>::operator=(const Matrica &m) {
  if (br_redova < m.br_redova || br_kolona < m.br_kolona) {
    TipEl **novi_prostor = AlocirajMemoriju(m.br_redova, m.br_kolona);
    DealocirajMemoriju(elementi, br_redova);
    elementi = novi_prostor;
  } else if (br_redova > m.br_redova)
    for (int i = m.br_redova; i < br_redova; i++)
      delete elementi[i];
  br_redova = m.br_redova;
  br_kolona = m.br_kolona;
  ime_matrice = m.ime_matrice;
  KopirajElemente(m.elementi);
  return *this;
}
template <typename TipEl>
Matrica<TipEl> &Matrica<TipEl>::operator=(Matrica &&m) {
  std::swap(br_redova, m.br_redova);
  std::swap(br_kolona, m.br_kolona);
  std::swap(ime_matrice, m.ime_matrice);
  std::swap(elementi, m.elementi);
  return *this;
}

template <typename TipEl>
std::istream &operator>>(std::istream &tok, Matrica<TipEl> &m) {
  for (int i = 0; i < m.br_redova; i++)
    for (int j = 0; j < m.br_kolona; j++) {
      if (&tok == &std::cin)
        std::cout << m.ime_matrice << "(" << i + 1 << "," << j + 1 << ") = ";
      tok >> m.elementi[i][j];
    }
  return tok;
}

template <typename TipEl>
std::ostream &operator<<(std::ostream &tok, const Matrica<TipEl> &m) {
  int sirina_ispisa = tok.width();
  for (int i = 0; i < m.br_redova; i++) {
    for (int j = 0; j < m.br_kolona; j++)
      std::cout << std::setw(sirina_ispisa) << m.elementi[i][j];
    std::cout << std::endl;
  }
  return tok;
}

template <typename TipEl>
Matrica<TipEl> operator+(const Matrica<TipEl> &m1, const Matrica<TipEl> &m2) {
  if (m1.br_redova != m2.br_redova || m1.br_kolona != m2.br_kolona)
    throw std::domain_error("Matrice nemaju jednake dimenzije!");
  Matrica<TipEl> m3(m1.br_redova, m1.br_kolona);
  for (int i = 0; i < m1.br_redova; i++)
    for (int j = 0; j < m1.br_kolona; j++)
      m3.elementi[i][j] = m1.elementi[i][j] + m2.elementi[i][j];
  return m3;
}

template <typename TipEl>
Matrica<TipEl> operator-(const Matrica<TipEl> &m1, const Matrica<TipEl> &m2) {
  if (m1.br_redova != m2.br_redova || m1.br_kolona != m2.br_kolona)
    throw std::domain_error("Matrice nemaju jednake dimenzije!");
  Matrica<TipEl> m3(m1.br_redova, m1.br_kolona);
  for (int i = 0; i < m1.br_redova; i++)
    for (int j = 0; j < m1.br_kolona; j++)
      m3.elementi[i][j] = m1.elementi[i][j] - m2.elementi[i][j];
  return m3;
}

template <typename TipEl>
Matrica<TipEl> operator*(const Matrica<TipEl> &m1, const Matrica<TipEl> &m2) {
  if (m1.br_kolona != m2.br_redova)
    throw std::domain_error("Matrice nisu saglasne za mnozenje");
  Matrica<TipEl> m3(m1.br_redova, m2.br_kolona);
  for (int i = 0; i < m1.br_redova; i++)
    for (int j = 0; j < m2.br_kolona; j++) {
      m3.elementi[i][j] = TipEl();
      for (int k = 0; k < m1.br_kolona; k++)
        m3.elementi[i][j] += m1.elementi[i][k] * m2.elementi[k][j];
    }
  return m3;
}

template <typename TipEl>
Matrica<TipEl> operator*(const Matrica<TipEl> &m, TipEl d) {
  Matrica<TipEl> m2(m.br_redova, m.br_kolona);
  for (int i = 0; i < m.br_redova; i++)
    for (int j = 0; j < m.br_kolona; j++)
      m2.elementi[i][j] = m.elementi[i][j] * d;
  return m2;
}

template <typename TipEl>
inline Matrica<TipEl> operator*(TipEl d, const Matrica<TipEl> &m) {
  return m * d;
}

template <typename TipEl>
Matrica<TipEl> &Matrica<TipEl>::operator+=(const Matrica &m) {
  if (br_redova != m.br_redova || br_kolona != m.br_kolona)
    throw std::domain_error("Matrice nemaju jednake dimenzije!");
  for (int i = 0; i < br_redova; i++)
    for (int j = 0; j < br_kolona; j++)
      elementi[i][j] += m.elementi[i][j];
  return *this;
}

template <typename TipEl>
Matrica<TipEl> &Matrica<TipEl>::operator-=(const Matrica &m) {
  if (br_redova != m.br_redova || br_kolona != m.br_kolona)
    throw std::domain_error("Matrice nemaju jednake dimenzije!");
  for (int i = 0; i < br_redova; i++)
    for (int j = 0; j < br_kolona; j++)
      elementi[i][j] -= m.elementi[i][j];
  return *this;
}

template <typename TipEl>
Matrica<TipEl> &Matrica<TipEl>::operator*=(const Matrica &m) {
  if (br_kolona != m.br_redova)
    throw std::domain_error("Matrice nisu saglasne za mnozenje");
  Matrica<TipEl> m1 = *this;
  for (int i = 0; i < br_redova; i++)
    for (int j = 0; j < m.br_kolona; j++) {
      elementi[i][j] = TipEl();
      for (int k = 0; k < br_kolona; k++)
        elementi[i][j] += m1.elementi[i][k] * m.elementi[k][j];
    }
  return *this;
}

template <typename TipEl> Matrica<TipEl> &Matrica<TipEl>::operator*=(TipEl d) {
  for (int i = 0; i < br_redova; i++)
    for (int j = 0; j < br_kolona; j++)
      elementi[i][j] *= d;
  return *this;
}

template <typename TipEl> TipEl &Matrica<TipEl>::operator()(int i, int j) {
  if (i < 1 || i > br_redova || j < 1 || j > br_kolona)
    throw std::range_error("Neispravan indeks");
  return elementi[i - 1][j - 1];
}

template <typename TipEl>
const TipEl &Matrica<TipEl>::operator()(int i, int j) const {
  if (i < 1 || i > br_redova || j < 1 || j > br_kolona)
    throw std::range_error("Neispravan indeks");
  return elementi[i - 1][j - 1];
}

template <typename TipEl> Matrica<TipEl>::operator std::string() const {
  using std::to_string;
  std::string s = "{";
  for (int i = 0; i < br_redova; i++) {
    s += "{";
    for (int j = 0; j < br_kolona; j++) {
      s += to_string(elementi[i][j]);
      if (j != br_kolona - 1)
        s += ",";
    }
    s += "}";
    if (i != br_redova - 1)
      s += ",";
  }
  return s += "}";
}

int main() {
    int m, n;
    std::cout << "Unesite broj redova i kolona za matricu: ";
    std::cin >> m >> n;

    std::cin.ignore(100000, '\n');
  try {
    Matrica<double> a(m, n);
    std::cout << "Unesite elemente matrice: " << std::endl;
    std::cin >> a;
    std::cin.ignore(1000000, '\n');

    std::cout << "Unesite ime tekstualne datoteke: ";
    std::string ime1;
    std::getline(std::cin, ime1);
    a.SacuvajUTekstualnuDatoteku(ime1);

    std::cout << "Unesite ime binarne datoteke: ";
    std::string ime2;
    std::getline(std::cin, ime2);
    a.SacuvajUBinarnuDatoteku(ime2);

    std::cout << "Ucitana matrica iz tekstualne datoteke: " << std::endl;
    a.ObnoviIzTekstualneDatoteke(ime1);
    
    std::cout << "Ucitana matrica iz binarne datoteke: " << std::endl;
    a.ObnoviIzBinarneDatoteke(ime2);
  
  } catch (...) {
    std::cout << "Neki problem" << std::endl;
  }
  return 0;
}


//Prolaze samo 2 testa hahahah