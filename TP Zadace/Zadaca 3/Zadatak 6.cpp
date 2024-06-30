//TP 2023/2024: Zadaća 3, Zadatak 6
#include <iostream>
#include <cmath>
#include <vector>
#include <stdexcept>
#include <memory>

struct Distrikt{
    int broj_distrikta;
    std::shared_ptr<Distrikt> sljedeci;
};

std::vector<int> Razbrajanje(int N, int M){
    std::vector<int> izlazni;

    std::shared_ptr<Distrikt> pocetak = nullptr, prethodni = nullptr, novi = nullptr;
    for(int i = 0; i < N; i++){
        novi = std::make_shared<Distrikt>();
        novi->broj_distrikta = i + 1; novi->sljedeci = nullptr;
        if(!pocetak) pocetak = novi;
        else if (i == N - 1) {prethodni->sljedeci = novi; novi->sljedeci = novi;}
        else prethodni->sljedeci = novi;
        prethodni = novi;
    } 
    int brojac = 0;
    while(izlazni.size() != N){
        for(int i = 0; i < brojac; i++){
            prethodni = pocetak;
            pocetak = pocetak->sljedeci;
        }
        brojac = M - 1;
        izlazni.push_back(pocetak->broj_distrikta);
        prethodni->sljedeci = pocetak->sljedeci;
        pocetak = pocetak->sljedeci;
    }
    prethodni->sljedeci = nullptr;
    novi->sljedeci = nullptr;
    return izlazni;
}

int OdabirKoraka(int N, int K){
    int M = 0;

    if(N < K || K == 0) throw std::domain_error("Broj blokova i redni broj bloka su pozitivni cijeli brojevi i redni broj bloka ne moze biti veci od broja blokova");

    if(N / K == 1){
        M = 1;
        return M;
    }
    if(K == 1){
        M = 0;
        return M;
    }
    if(K <= 0){
        return M;
    }
    for(;;){
        std::vector<int> probni = Razbrajanje(N, M);
        int elem = probni.at(N - 1);
        if(elem == K) break;
        M++;
    }
    return M;
}

int main(){
    try{
        int N, K;
        std::cout << "Unesite broj distrikta u gradu: ";
        std::cin >> N;
        std::cout << "Unesite redni broj distrikta u kojem se nalazi restoran: ";
        std::cin >> K;
        std::cout << "Trazeni korak: " << OdabirKoraka(N, K);
    }catch(std::domain_error izuzetak){
        std::cout << izuzetak.what();
    }
    return 0;
}