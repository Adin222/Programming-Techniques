//TP 2023/2024: Zadaća 2, Zadatak 3
#include <iostream>
#include <deque>
#include <vector>

template <typename T1, typename T2, typename T3>
T3 UnijaBlokova(T1 p1, T1 p2, T2 p3, T2 p4, T3 p5){
    T3 oznacen = p5;

    while(p1 != p2){
        bool pronadjen = false;

        T3 temp_p = p5;
        while(temp_p != oznacen){
            if(*p1 == *temp_p){
                pronadjen = true;
                break;
            }
            temp_p++;
        }
        if(!pronadjen){
            *oznacen = *p1;
             oznacen++;
        }
        p1++;
    }

    while(p3 != p4){
        bool pronadjen = false;

        T3 temp_p = p5;
        while(temp_p != oznacen){
            if(*p3 == *temp_p){
                pronadjen = true;
                break;
            }
            temp_p++;
        }
        if(!pronadjen){
            *oznacen = *p3;
             oznacen++;
        }
        p3++;
    }
    return oznacen;
}


int main ()
{
    int n, m;
    std::cout << "Unesite broj elemenata niza (max. 100): ";
    std::cin >> n;
    double niz[100];
    std::cout << "Unesite elemente niza: ";
    for(int i = 0; i < n; i++){
        std::cin >> niz[i];
    }
    std::cout << "Unesite broj elemenata deka: ";
    std::cin >> m;
    std::cout << "Unesite elemente deka: ";
    std::deque<int> dek(m);
    for(int i = 0; i < m; i++){
        std::cin >> dek.at(i);
    }
    std::vector<int> PocetakBloka(100);
    auto KrajBloka = UnijaBlokova(niz, niz + n, dek.begin(), dek.end(), PocetakBloka.begin());
    int brojac = 0;
    std::cout << "Njihova unija glasi: ";
    for(auto it = std::begin(PocetakBloka); it != KrajBloka; it++){
        brojac++;
        std::cout << *it << " ";
    }
    PocetakBloka.resize(brojac);
	return 0;
}
