// TP 2023/2024: Zadaća 3, Zadatak 3
#include <algorithm>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <new>
#include <stdexcept>


enum class Smjer { Desno, Dolje, Dijagonalno };

template <typename TipElementa> struct Matrica {
  char ime_matrice;
  int br_redova, br_kolona;
  TipElementa **elementi = nullptr;
};

template <typename TipElementa> void UnistiMatricu(Matrica<TipElementa> &mat) {
  if (!mat.elementi) return;
  for (int i = 0; i < mat.br_redova; i++) delete[] mat.elementi[i];
  delete[] mat.elementi;
  mat.elementi = nullptr;
}

template <typename TipElemenata> void UnesiMatricu(Matrica<TipElemenata> &mat) {
  for (int i = 0; i < mat.br_redova; i++)
    for (int j = 0; j < mat.br_kolona; j++) {
      std::cout << mat.ime_matrice << "(" << i + 1 << "," << j + 1 << ") = ";
      std::cin >> mat.elementi[i][j];
    }
}

template <typename TipElementa>
Matrica<TipElementa> StvoriMatricu(int br_redova, int br_kolona, char ime = 0) {
  Matrica<TipElementa> mat;
  mat.br_redova = br_redova;
  mat.br_kolona = br_kolona;
  mat.ime_matrice = ime;
  mat.elementi = new TipElementa *[br_redova] {};
  try {
    for (int i = 0; i < br_redova; i++)
      mat.elementi[i] = new TipElementa[br_kolona];
  } catch (...) {
    UnistiMatricu(mat);
    throw;
  }
  return mat;
}

template <typename TipElemenata>
Matrica<TipElemenata> ZbirMatrica(const Matrica<TipElemenata> &m1, const Matrica<TipElemenata> &m2) {
  if (m1.br_redova != m2.br_redova || m1.br_kolona != m2.br_kolona) {
    throw std::domain_error("Matrice nemaju jednake dimenzije!");
  }
  auto m3 = StvoriMatricu<TipElemenata>(m1.br_redova, m1.br_kolona);
  for (int i = 0; i < m1.br_redova; i++)
    for (int j = 0; j < m1.br_kolona; j++)
      m3.elementi[i][j] = m1.elementi[i][j] + m2.elementi[i][j];
  return m3;
}

template <typename TipElemenata>
void IspisiMatricu(const Matrica<TipElemenata> &mat, int sirina_ispisa, int sirina = 4, bool treba_brisati = false) {
  for (int i = 0; i < mat.br_redova; i++) {
    for (int j = 0; j < mat.br_kolona; j++)
      std::cout << std::setw(sirina_ispisa) << std::setprecision(sirina) << mat.elementi[i][j];
    std::cout << std::endl;
  }
  if (treba_brisati) {
    for(int i = 0; i < mat.br_redova; i++) delete [] mat.elementi[i];
    delete [] mat.elementi;
  }
}


Matrica<int> ProsiriPremaFunkcijama(Matrica<int> &mat, std::map<Smjer, std::function<int(int)>> smjer, int n) {
    if(n < 1) throw std::domain_error("Besmislen parametar");
  Matrica<int> copy_mat = StvoriMatricu<int>(mat.br_redova, mat.br_kolona);
  for(int i = 0; i < mat.br_redova; i++){
      for(int j = 0; j < mat.br_kolona; j++){
          copy_mat.elementi[i][j] = mat.elementi[i][j];
      }
  }
  if(smjer.empty()) return copy_mat;
  bool desno = false, dolje = false, dijagonalno = false;
  for (auto it = smjer.begin(); it != smjer.end(); it++) {
    if (it->first == Smjer::Desno)
      desno = true;
    if (it->first == Smjer::Dolje)
      dolje = true;
    if (it->first == Smjer::Dijagonalno)
      dijagonalno = true;
  }
  for (int i = 0; i < n - 1; i++) {
    if (desno && !dolje && !dijagonalno) {
      Matrica<int> NovaMatrica = StvoriMatricu<int>(copy_mat.br_redova, copy_mat.br_kolona * 2);
      for (int i = 0; i < copy_mat.br_redova; i++) {
        for (int j = 0; j < copy_mat.br_kolona; j++) {
          NovaMatrica.elementi[i][j] = copy_mat.elementi[i][j];
          NovaMatrica.elementi[i][j + copy_mat.br_kolona] = smjer[Smjer::Desno](copy_mat.elementi[i][j]);
        }
      }
      UnistiMatricu(copy_mat);
      copy_mat = NovaMatrica;
    }
    if (dolje && !desno && !dijagonalno) {
      Matrica<int> NovaMatrica = StvoriMatricu<int>(copy_mat.br_redova * 2, copy_mat.br_kolona);
      for (int i = 0; i < copy_mat.br_redova; i++) {
        for (int j = 0; j < copy_mat.br_kolona; j++) {
          NovaMatrica.elementi[i][j] = copy_mat.elementi[i][j];
          NovaMatrica.elementi[i + copy_mat.br_redova][j] = smjer[Smjer::Dolje](copy_mat.elementi[i][j]);
        }
      }
      UnistiMatricu(copy_mat);
      copy_mat = NovaMatrica;
    }
    if (dijagonalno && !desno && !dolje) {
      Matrica<int> NovaMatrica = StvoriMatricu<int>(copy_mat.br_redova * 2, copy_mat.br_kolona * 2);
      for (int i = 0; i < copy_mat.br_redova; i++) {
        for (int j = 0; j < copy_mat.br_kolona; j++) {
          NovaMatrica.elementi[i][j] = copy_mat.elementi[i][j];
          NovaMatrica.elementi[i + copy_mat.br_redova][j] = copy_mat.elementi[i][j];
          NovaMatrica.elementi[i][j + copy_mat.br_kolona] = copy_mat.elementi[i][j];
          NovaMatrica.elementi[i + copy_mat.br_redova][j + copy_mat.br_kolona] =  smjer[Smjer::Dijagonalno](copy_mat.elementi[i][j]);
        }
      }
      UnistiMatricu(copy_mat);
      copy_mat = NovaMatrica;
    }
    if (dijagonalno && desno && dolje) {
      Matrica<int> NovaMatrica = StvoriMatricu<int>(copy_mat.br_redova * 2, copy_mat.br_kolona * 2);
      for (int i = 0; i < copy_mat.br_redova; i++) {
        for (int j = 0; j < copy_mat.br_kolona; j++) {
          NovaMatrica.elementi[i][j] = copy_mat.elementi[i][j];
          NovaMatrica.elementi[i][j + copy_mat.br_kolona] = smjer[Smjer::Desno](copy_mat.elementi[i][j]);
          NovaMatrica.elementi[i + copy_mat.br_redova][j] = smjer[Smjer::Dolje](copy_mat.elementi[i][j]);
          NovaMatrica.elementi[i + copy_mat.br_redova][j + copy_mat.br_kolona] = smjer[Smjer::Dijagonalno](copy_mat.elementi[i][j]);
        }
      }
      UnistiMatricu(copy_mat);
      copy_mat = NovaMatrica;
    }
    if (desno && dolje && !dijagonalno) {
      Matrica<int> NovaMatrica = StvoriMatricu<int>(copy_mat.br_redova * 2, copy_mat.br_kolona * 2);
      for (int i = 0; i < copy_mat.br_redova; i++) {
        for (int j = 0; j < copy_mat.br_kolona; j++) {
          NovaMatrica.elementi[i][j] = copy_mat.elementi[i][j];
          NovaMatrica.elementi[i + copy_mat.br_redova][j] = smjer[Smjer::Dolje](copy_mat.elementi[i][j]);
          NovaMatrica.elementi[i][j + copy_mat.br_kolona] = smjer[Smjer::Desno](copy_mat.elementi[i][j]);
          NovaMatrica.elementi[i + copy_mat.br_redova][j + copy_mat.br_kolona] = copy_mat.elementi[i][j];
        }
      }
      UnistiMatricu(copy_mat);
      copy_mat = NovaMatrica;
    }
    if (desno && !dolje && dijagonalno) {
      Matrica<int> NovaMatrica = StvoriMatricu<int>(copy_mat.br_redova * 2, copy_mat.br_kolona * 2);
      for (int i = 0; i < copy_mat.br_redova; i++) {
        for (int j = 0; j < copy_mat.br_kolona; j++) {
          NovaMatrica.elementi[i][j] = copy_mat.elementi[i][j];
          NovaMatrica.elementi[i + copy_mat.br_redova][j] = copy_mat.elementi[i][j];
          NovaMatrica.elementi[i][j + copy_mat.br_kolona] = smjer[Smjer::Desno](copy_mat.elementi[i][j]);
          NovaMatrica.elementi[i + copy_mat.br_redova][j + copy_mat.br_kolona] = smjer[Smjer::Dijagonalno](copy_mat.elementi[i][j]);
        }
      }
      UnistiMatricu(copy_mat);
      copy_mat = NovaMatrica;
    }
    if (!desno && dolje && dijagonalno) {
      Matrica<int> NovaMatrica = StvoriMatricu<int>(copy_mat.br_redova * 2, copy_mat.br_kolona * 2);
      for (int i = 0; i < copy_mat.br_redova; i++) {
        for (int j = 0; j < copy_mat.br_kolona; j++) {
          NovaMatrica.elementi[i][j] = copy_mat.elementi[i][j];
          NovaMatrica.elementi[i + copy_mat.br_redova][j] =smjer[Smjer::Dolje](copy_mat.elementi[i][j]);
          NovaMatrica.elementi[i][j + copy_mat.br_kolona] = copy_mat.elementi[i][j];
          NovaMatrica.elementi[i + copy_mat.br_redova][j + copy_mat.br_kolona] = smjer[Smjer::Dijagonalno](copy_mat.elementi[i][j]);
        }
      }
      UnistiMatricu(copy_mat);
      copy_mat = NovaMatrica;
    }
  }
  return copy_mat;
}

int main() {
    Matrica<int> A, mat;
  try {
    int visina, sirina;
    std::cout << "Unesi broj redova i kolona matrice: ";
    std::cin >> visina >> sirina;
    A = StvoriMatricu<int>(visina, sirina, 'M');
    UnesiMatricu(A);
    std::map<Smjer, std::function<int(int)>> m{
        {Smjer::Desno, [](int x) { return x + 1; }},
        {Smjer::Dolje, [](int x) { return x + 2; }},
        {Smjer::Dijagonalno, [](int x) { return x + 3; }}};
    int n;
    std::cout << std::endl;
    std::cout << "Unesite n: " << std::endl;
    std::cin >> n;
    mat = ProsiriPremaFunkcijama(A, m, n);
    IspisiMatricu(mat, 5);
  } catch (std::domain_error izuz) {
    std::cout << "IZUZETAK: " << izuz.what() << std::endl;
  }
  UnistiMatricu(A);
  UnistiMatricu(mat);
  return 0;
}

//Samo jedan test pada