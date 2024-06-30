//TP 2023/2024: Zadaća 1, Zadatak 2
#include <iostream>
#include <vector>
#include <iomanip>
#include <stdexcept>

typedef std::vector<std::vector<double>> matrix;

double NadjiMedijanu(std::vector<double> vek){
    for(int i = 0; i < vek.size(); i++){
        for(int j = i + 1; j < vek.size(); j++){
            if(vek.at(i) > vek.at(j)){
                int temp = vek.at(i);
                vek.at(i) = vek.at(j);
                vek.at(j) = temp;
            }
        }
    }

    double medijan;
    if(vek.size() % 2 == 1){
     medijan = vek.at((vek.size() + 1) / 2 - 1);   
    }else if(vek.size() % 2 == 0){
        int n = vek.size();
        medijan = (vek.at(n / 2 - 1) + vek.at(n / 2)) / 2;
    }
return medijan;
}

matrix MedijanskiFilter(matrix m, int N){
    if(N < 0) throw std::domain_error("Neispravan red filtriranja");
    bool uglasta = true;
    for(int i = 0; i < m.size(); i++){
        for(int j = 0; j < m.at(i).size(); j++){
            if(m.at(i).size() != i + 1){
                uglasta = false;
            } 
        }
    }
    int sirina_reda = 0;
    for(int i = 0; i < m.size(); i++){
        for(int j = 0; j < m.at(i).size(); j++){
            if(m.at(i).size() > sirina_reda) {
               sirina_reda = m.at(i).size(); 
            }
        }
    }
    matrix mat(m.size(), std::vector<double>(sirina_reda, 0.));
    
    for(int i = 0; i < m.size(); i++){
        for(int j = 0; j < m.at(i).size(); j++){
            std::vector<double> drzac;
            for(int s = i - N; s <= i + N; s++){
                if(s < 0 || s >= m.size()){
                    continue;
                }
                for(int k = j - N; k <= j + N; k++){
                    if(k < 0 || k >= m.at(s).size()){
                        continue;
                    }
                    drzac.push_back(m.at(s).at(k));
                }
            }
            mat.at(i).at(j) = NadjiMedijanu(drzac);
        }
    }
    if(uglasta){
        for(int i = 0; i < m.size(); i++){
            mat.at(i).resize(i + 1);
        }
    }
    return mat;
}


int main ()
{
    try{
        int sirina, visina, red_filtera;
        std::cout << "Unesite broj redova i kolona matrice: ";
        std::cin >> visina >> sirina;
        if(sirina < 0) throw std::domain_error("Neispravan red filtriranja");
        matrix mat(visina, std::vector<double>(sirina, 0.));
        std::cout << "Unesite elemente matrice: " << std::endl;
        for(int i = 0; i < visina; i++){
            for(int j = 0; j < sirina; j++){
                std::cin >> mat.at(i).at(j);
            }
        }
        std::cout << "Unesite red filtriranja: ";
        std::cin >> red_filtera;
        matrix rez = MedijanskiFilter(mat, red_filtera);
        std::cout << std::endl;
        std::cout << "Matrica nakon filtriranja: " << std::endl;
        for(int i = 0; i < rez.size(); i++){
            for(int j = 0; j < rez.at(i).size(); j++){
                std::cout << std::setw(7) << std::setprecision(2) << std::fixed << rez.at(i).at(j); 
            }
            std::cout << std::endl;
        }
    }catch(std::domain_error izuzetak){
        std::cout << "GRESKA: " << izuzetak.what() << "!" << std::endl;
    }

	return 0;
}
