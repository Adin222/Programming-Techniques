// TP 2023/2024: Zadaća 2, Zadatak 2
#include <deque>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <type_traits>
#include <vector>


template <typename T> int NajveciEl(std::vector<std::vector<T>> mat) {
    T max_width = 0;
    for (int i = 0; i < mat.size(); i++) {
      for (int j = 0; j < mat.at(i).size(); j++) {
        int br = mat.at(i).at(j);
        int sirina = 0;
        sirina = std::to_string(br).length();
        if (sirina > max_width) {
          max_width = sirina;
        }
      }
    }
    return max_width + 1;
}

template <typename A, typename B, typename F>
auto GeneraliziraniKroneckerovProizvod(A &kont_1, B &kont_2, F funk)
    -> std::vector<std::vector<decltype(funk(kont_1[0][0], kont_2[0][0]))>> {
  int m = 0, n = 0, p = 0, q = 0;
  for (auto it = std::begin(kont_1); it != std::end(kont_1); it++) {
    m++;
  }
  if(m > 0){
    for (auto it = std::begin(kont_1[0]); it != std::end(kont_1[0]); it++) n++;
  }else{
      n = 0;
  }
  
  for (auto it = std::begin(kont_2); it != std::end(kont_2); it++) {
    p++;
  }
  if(p > 0){
     for (auto it = std::begin(kont_2[0]); it != std::end(kont_2[0]); it++) q++;
  }else{
      q = 0;
  }
  
  bool prvi_par = false, drugi_par = false;
  if(n == 0 || q == 0) throw std::domain_error("Parametri nemaju formu matrice");

  for (int i = 0; i < m; i++) {
    int brojac = 0;
    for (auto it = std::begin(kont_1[i]); it != std::end(kont_1[i]); it++) {
      brojac++;
    }
    if (brojac != n) {
      prvi_par = true;
    }
  }
  for (int i = 0; i < p; i++) {
    int brojac_2 = 0;
    for (auto it = std::begin(kont_2[i]); it != std::end(kont_2[i]); it++) {
      brojac_2++;
    }
    if (brojac_2 != q) {
      drugi_par = true;
    }
  }
  if (prvi_par && drugi_par == false) {
    throw std::domain_error("Prvi parametar nema formu matrice");
  }
  if (drugi_par && prvi_par == false) {
    throw std::domain_error("Drugi parametar nema formu matrice");
  }
  if (prvi_par && drugi_par) {
    throw std::domain_error("Parametri nemaju formu matrice");
  }
  try {
    std::vector<std::vector<decltype(funk(kont_1[0][0], kont_2[0][0]))>> kont_3(
        m * p, std::vector<decltype(funk(kont_1[0][0], kont_2[0][0]))>(n * q));
    for (int i = 0; i < m; i++) {
      for (int j = 0; j < n; j++) {
        for (int k = 0; k < p; k++) {
          for (int l = 0; l < q; l++) {
            auto result = funk(kont_1[i][j], kont_2[k][l]);
            kont_3.at(i * p + k).at(j * q + l) = result;
          }
        }
      }
    }
    return kont_3;
  } catch (...) {
    throw std::runtime_error("Neocekivani problemi pri racunanju");
  }
}

int main() {
  try {
    int m, n, p, q;
    double broj1, broj2;
    std::cout << "Unesite dimenzije prve matrice: ";
    std::cin >> m >> n;
    std::vector<std::deque<double>> prvi(m, std::deque<double>(n));
    std::cout << "Unesite elemente prve matrice: ";
    for (int i = 0; i < m; i++) {
      for (int j = 0; j < n; j++) {
        std::cin >> prvi.at(i).at(j);
      }
    }
    std::cout << "Unesite dimenzije druge matrice: ";
    std::cin >> p >> q;
    std::vector<std::deque<double>> drugi(p, std::deque<double>(q));
    std::cout << "Unesite elemente druge matrice: ";
    for (int i = 0; i < p; i++) {
      for (int j = 0; j < q; j++) {
        std::cin >> drugi.at(i).at(j);
      }
    }
    auto funkcija = [](double x, double y) { return x * y; };
    std::cout << "Njihov Kroneckerov proizvod glasi: " << std::endl;
    auto rezultat = GeneraliziraniKroneckerovProizvod(prvi, drugi, funkcija);
    for (int i = 0; i < rezultat.size(); i++) {
      for (int j = 0; j < rezultat.at(i).size(); j++) {
        std::cout << std::setw(NajveciEl(rezultat)) << rezultat.at(i).at(j);
      }
      std::cout << std::endl;
    }
  } catch (std::domain_error izuzetak) {
    std::cout << izuzetak.what() << std::endl;
  } catch (std::runtime_error izuzetak) {
    std::cout << izuzetak.what() << std::endl;
  }
  return 0;
}
