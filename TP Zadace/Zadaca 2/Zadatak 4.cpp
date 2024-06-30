//TP 2023/2024: Zadaća 2, Zadatak 4
#include <iostream>
#include <vector>
#include <algorithm>

template <typename T>
bool Kriterij(const std::vector<T> &prvi, const std::vector<T> &drugi){
    T maks_a = *std::max_element(prvi.begin(), prvi.end());
    T maks_b = *std::max_element(drugi.begin(), drugi.end());
    if(maks_a != maks_b){
        return maks_a > maks_b; 
    }
    return prvi > drugi;
}

template<typename T>
void SortirajPoDobrotiRedova(std::vector<std::vector<T>> &mat){
    std::sort(mat.begin(), mat.end(), Kriterij<T>);
}

int main ()
{
    std::vector<std::vector<int>> mat;
    std::string unos;

    std::cout << "Unesite elemente (* za kraj reda, * na pocetku reda za kraj unosa):\n";
    while(true){
        std::getline(std::cin, unos);
        if(!isdigit(unos.at(0))) break;
        std::vector<int> red;
        int broj = 0;
        bool ima_br = false;
        for(char c : unos){
            if(isdigit(c)){
                broj = broj * 10 + (c - '0');
                ima_br = true;
            }else if(c == ' ' && ima_br){
                red.push_back(broj);
                broj = 0;
                ima_br = false;
            }
        }
        if(ima_br) red.push_back(broj);
        mat.push_back(red);
    }
    SortirajPoDobrotiRedova(mat);

    std::cout << "Matrica nakon sortiranja:\n";

    for(int i = 0; i < mat.size(); i++){
        for(int j = 0; j < mat.at(i).size(); j++){
            std::cout << mat.at(i).at(j) << " ";
        }
        std::cout << std::endl;
    }
    std::vector<int> vek; int broj;
    std::cout << "Unesite elemente sekvence koja se trazi (* za kraj reda): ";
    while(std::cin >> broj){
        vek.push_back(broj);
    }
    auto it = std::lower_bound(mat.begin(), mat.end(), vek, Kriterij<int>);
    
    if(it != mat.end() && *it == vek){
        std::cout << "Trazena sekvenca se nalazi u " << it - mat.begin() + 1 << ". redu (nakon sortiranja)\n";
    }else{
        std::cout << "Trazena sekvenca se ne nalazi u matrici\n";
    }

	return 0;
}
