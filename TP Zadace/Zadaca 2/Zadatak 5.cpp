#include <iostream>
#include <deque>
#include <new>
#include <stdexcept>
#include <type_traits>
#include <vector>

template <typename A, typename B>
auto **KreirajTablicuMnozenja(A &SekA, B &SekB) {
  int brojac = 0, brojac_2 = 0;
  for (auto it = std::begin(SekA); it != std::end(SekA); it++) {
    brojac++;
  }
  int suma = 0;
  for (int i = 0; i < brojac; i++) {
    suma += brojac - i;
  }
  for (auto it = std::begin(SekB); it != std::end(SekB); it++) {
    brojac_2++;
  }
  if (brojac != brojac_2)
    throw std::range_error("Kontejneri nisu iste duzine");
  typedef std::remove_reference_t<decltype(*std::begin(SekA) *
                                           *std::begin(SekB))>
      noviTip;
  try {
    noviTip **mat = new noviTip *[brojac] {};  
    mat[0] = new noviTip[suma];
    for (int i = 1; i < brojac; i++) mat[i] = mat[i - 1] + i;
    auto itA = std::begin(SekA);
    for (int i = 0; i < brojac; i++) {
      auto itB = std::begin(SekB);
      for (int j = 0; j <= i; j++) {
        if (*itA * *itB != *itB * *itA) {
          delete[] mat[0];
          delete[] mat;
          throw std::logic_error(
              "Nije ispunjena pretpostavka o komutativnosti");
        }
        mat[i][j] = *itA * *itB;
        itB++;
      }
      itA++;
    }
    return mat;
  } catch (std::bad_alloc) {
    throw std::range_error("Nema dovoljno memorije");
  }
}

int main() {
  int duzina;
  double broj, broj_2;
  std::cout << "Duzina sekvenci: ";
  std::cin >> duzina;
  std::vector<double> vek;
  std::deque<double> dek;
  std::cout << "Elementi prve sekvence: ";
  for (int i = 0; i < duzina; i++) {
    std::cin >> broj;
    vek.push_back(broj);
  }
  std::cout << "Elementi druge sekvence: ";
  for (int i = 0; i < duzina; i++) {
    std::cin >> broj_2;
    dek.push_back(broj_2);
  }
  double **mat = nullptr;
  try {
    mat = KreirajTablicuMnozenja(vek, dek);
    std::cout << "Tablica mnozenja: " << std::endl;
    for (int i = 0; i < duzina; i++) {
      for (int j = 0; j <= i; j++) {
        std::cout << mat[i][j] << " ";
      }
      std::cout << std::endl;
    }
    delete[] mat[0];
    delete[] mat;
  } catch (std::range_error e) {
    std::cout << e.what() << std::endl;
  } catch (std::logic_error err) {
    std::cout << err.what() << std::endl;
  }
  return 0;
}
