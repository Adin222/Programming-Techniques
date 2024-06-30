//TP 2023/2024: Zadaća 3, Zadatak 4
#include <iostream>
#include <list>
#include <vector>
#include <algorithm>

std::vector<int> Razbrajanje(int N, int M){
    std::list<int> polazna;
    std::vector<int> izlazni;

    for(int i = 0; i < N; i++){
        polazna.push_back(i + 1);
    }
    auto iter = polazna.begin();
    izlazni.push_back(*iter);
    iter = polazna.erase(iter);

    while(!polazna.empty()){
        for(int i = 0; i < M - 1; i++){
            iter++;
            if(iter == polazna.end()){
                iter = polazna.begin();
            }
        }
        izlazni.push_back(*iter);
        iter = polazna.erase(iter);
        if(iter == polazna.end()){
            iter = polazna.begin();
        }
    }
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


int main ()
{
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
