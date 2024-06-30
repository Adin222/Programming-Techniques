//TP 2023/2024: Zadaća 1, Zadatak 1
#include <iostream>
#include <cmath>
#include <vector>
#include <array>
#include <stdexcept>

enum class TretmanNegativnih {IgnorirajZnak, Odbaci, TretirajKao0, PrijaviGresku};

int BrojKvadrata(int n){
    if(n < 0) throw std::domain_error("Broj mora biti nenegativan");
    if(n <= 3) return n;
    if(sqrt(n) == int(sqrt(n))) return 1;
    std::vector<int> vektor(n + 1, n);
    vektor.at(0) = 0;
    vektor.at(1) = 1;
    vektor.at(2) = 2;
    vektor.at(3) = 3;
for(int i = 4; i <=n; i++){
    for(int j = 1; j <= sqrt(i); j++){
        int tmp = j * j;
        if(tmp > i){
            break;
        }else{
            int nova_vrijednost = 1 + vektor.at(i - tmp);
            if(nova_vrijednost < vektor.at(i)){
                vektor.at(i) = nova_vrijednost;
            }
        }
    }
}
int povratni = vektor.at(n);
return povratni;
}

std::array<std::vector<int>, 5> RazvrstajPoBrojuKvadrata(std::vector<int> v, TretmanNegativnih tret){
    std::array<std::vector<int>, 5> izlazni;
    for(int i = 0; i < v.size(); i++){
        int element = v.at(i);
    if(element < 0){
        if(tret == TretmanNegativnih::IgnorirajZnak){
            if(element < 0){
            izlazni.at(BrojKvadrata(-element)).push_back(element);
            }
        }else if(tret == TretmanNegativnih::Odbaci){
            continue;
        }else if(tret == TretmanNegativnih::TretirajKao0){
            if(element < 0){
              element = -element;
              izlazni.at(0).push_back(-element);  
            }else{
              izlazni.at(0).push_back(element);  
            }
        }else if(tret == TretmanNegativnih::PrijaviGresku){
            throw std::domain_error("Nije predvidjeno razvrstavanje negativnih brojeva");
        }
    }
    if(v.at(i) < 0){
        continue;
    }
    int rez = BrojKvadrata(element);
    izlazni.at(rez).push_back(element);
    }
return izlazni;
}



int main ()
{
   int broj;
   std::vector<int> ulazni;
   std::cout << "Unesite brojeve (bilo koji ne-broj oznacava kraj): ";
   while(std::cin >> broj){
       if(broj < 0){
           std::cout << "Nije podrzano razvrstavanje negativnih brojeva!" << std::endl;
           return -1;
       }
    ulazni.push_back(broj);
   }
std::cout << "Rezultati razvrstavanja po broju kvadrata: " << std::endl;
try{
    std::array<std::vector<int>, 5> izlazni = RazvrstajPoBrojuKvadrata(ulazni, TretmanNegativnih::TretirajKao0);
    for(int i = 0; i < 5; i++){
        if(izlazni.at(i).size() == 0) continue;
            std::cout << i << ": ";
            for(int x : izlazni.at(i)){
                std::cout << x << " ";
            }
            std::cout << std::endl;
    }
}catch(std::domain_error izuzetak){
    std::cout << izuzetak.what() << std::endl;
}
	return 0;
}
