//TP 2023/2024: Zadaća 3, Zadatak 1
#include <iostream>
#include <cmath>
#include <utility>
#include <vector>
#include <functional>
#include <algorithm>
#include <stdexcept>
#include <tuple>

bool jeBroj(const std::string str){
    try{
        std::stod(str);
        return true;
    }catch(...){
        return false;
    }
}

std::function<double(double)> KubnaInterpolacija(std::vector<std::pair<double, double>> vek){
    std::sort(std::begin(vek), std::end(vek), [](std::pair<double, double> x, std::pair<double, double> y){
        return x.first < y.first;
    });
    for(int i = 0; i < vek.size() - 1; i++){
        if(vek.at(i).first == vek.at(i + 1).first){
            throw std::domain_error("Neispravni cvorovi");
        }
    }
    if(vek.size() < 4) throw std::domain_error("Nedovoljan broj cvorova");
    std::vector<double> x, y;
    for(auto p : vek){
        x.push_back(p.first);
        y.push_back(p.second);
    }
    return [x, y](double x_vr){
        double rez = 0.0;
        if(x_vr < x.at(0) || x_vr > x.at(x.size() - 1)){
            throw std::range_error("Argument izvan opsega");
        }
        for(int i = 0; i < x.size() - 1; i++){
            if(x_vr == x.at(i)) return y.at(i);

            if(x_vr >= x.at(i) && x_vr < x.at(i + 1)){
                int pocetak = i - 1 >= 0 ? i - 1 : 0;
                int kraj = i + 2 <= x.size() - 1 ? i + 2 : x.size() - 1;
                if(i == 0){
                    pocetak = 0;
                    kraj = i + 3 <= x.size() - 1 ? i + 3 : x.size() - 1;
                }
                if(i == x.size() - 2){
                    pocetak = i - 2 >= 0 ? i - 2 : 0;
                    kraj = i + 1;
                }
                for(int k = pocetak; k <= kraj; k++){
                    double vk = y.at(k);
                    for(int j = pocetak; j <= kraj; j++){
                        if(j != k){
                            vk *= (x_vr - x.at(j)) / (x.at(k) - x.at(j));
                        }
                    }
                    rez += vk;
                }
                break;
            }
        }
        if(x_vr == x.at(x.size() - 1)) return y.at(y.size() - 1);
       return rez; 
    };
}
std::function<double(double)> KubnaInterpolacija(std::function<double(double)> InterFunk, int Xmin, int Xmax, double DeltaX){
    if((Xmax < Xmin) || DeltaX <= 0) throw std::domain_error("Nekorektni parametri");
    std::vector<std::pair<double, double>> vek_par;
    int maksimum = (fabs(std::fmod((Xmax - Xmin), DeltaX)) < 0.000001) ? Xmax + DeltaX : Xmax;
    for(double i = Xmin; i <= maksimum; i += DeltaX){
        vek_par.push_back(std::make_pair(i, InterFunk(i)));
    }
    return KubnaInterpolacija(vek_par);
}

double CiljanaFunkcija(double x){
    return x * x + sin(x) + log(x + 1);
}

int main ()
{
    int izbor;
    std::cout << "Odaberite opciju (1 - unos cvorova, 2 - aproksimacija): ";
    std::cin >> izbor;
    if(izbor == 1){
        int cvorovi;
        std::vector<std::pair<double, double>> pairs;
        double prvi_par, drugi_par;
        std::cout << "Unesite broj cvorova: ";
        std::cin >> cvorovi;

        if(cvorovi <= 0){
            std::cout << "Broj cvorova mora biti pozitivan broj!";
            return 1;
        }
        double Xmin(0), Xmax(0);
        std::cout << "Unesite cvorove kao parove x y: ";
        for(int i = 0; i < cvorovi; i++){
            std::cin >> prvi_par >> drugi_par;

            if(i == 0){
                Xmin = prvi_par;
                Xmax = prvi_par;
            }else{
                if(prvi_par < Xmin){
                    Xmin = prvi_par;
                }
                if(prvi_par > Xmax){
                    Xmax = prvi_par;
                }
            }
            pairs.push_back(std::make_pair(prvi_par, drugi_par));
        }
        try{
            auto fun = KubnaInterpolacija(pairs);
            std::string brojevi;
            for(;;){
                std::cout << "Unesite argument (ili \"kraj\" za kraj): ";
                std::cin >> brojevi;
                if(!jeBroj(brojevi)){
                    break;
                } 
                if(std::stod(brojevi) < Xmin || std::stod(brojevi) > Xmax){
                    std::cout << "Argument izvan opsega!" << std::endl;
                
                }else{
                    std::cout << "f(" + brojevi + ") = " << fun(std::stod(brojevi)) << std::endl;
                }
            }
        }catch(std::domain_error izuzetak){
            std::cout << izuzetak.what() << std::endl;
            return 1;
        }
    }
    if(izbor == 2){
      double Xmin, Xmax, Dx;
      std::cout << "Unesite krajeve intervala i korak: ";
      std::cin >> Xmin >> Xmax >> Dx;
      try{
          auto fun = KubnaInterpolacija(CiljanaFunkcija, Xmin, Xmax, Dx);
          std::string brojevi;
          for(;;){
              std::cout << "Unesite argument (ili \"kraj\" za kraj): ";
              std::cin >> brojevi;

              if(!jeBroj(brojevi)) break;

              if(std::stod(brojevi) < Xmin || std::stod(brojevi) > Xmax){
                  std::cout << "Argument izvan opsega!" << std::endl;
              }else{
                  std::cout << "f(" + brojevi + ") = " << CiljanaFunkcija(std::stod(brojevi)) << " fapprox(" + brojevi + ") = " << fun(std::stod(brojevi)) << std::endl;
              }
          }
      }catch(std::domain_error izuzetak){
          std::cout << izuzetak.what() << std::endl;
          return 1;
      }
    }

	return 0;
}
