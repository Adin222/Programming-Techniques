//TP 2023/2024: Zadaća 1, Zadatak 4
#include <iostream>
#include <string>
#include <stdexcept>

bool JeSlovo(char znak){
    return (znak >= 'a' && znak <= 'z') || (znak >= 'A' && znak <= 'Z');
}

bool DaLiJeMalo(char znak){
    return (znak >= 'a' && znak <= 'z');
}
bool ProvjeriKljuc(std::string kljuc){
    if(kljuc.size() == 0) return true;
    for(int i = 0; i < kljuc.size(); i++){
        if(!(kljuc.at(i) >= 'A' && kljuc.at(i) <= 'Z')){
            return true;
        }
    }
    return false;
}
std::string ViginereSifriranje(std::string poruka, std::string kljuc){
    if(ProvjeriKljuc(kljuc)) throw std::domain_error("Neispravan kljuc");
    std::string izlazni;
    int duzina_kljuca = kljuc.length();
    int brojac = 0;

    for(int i = 0; i < poruka.size(); i++){
        char znak = poruka.at(i);
     if(JeSlovo(znak)){
        char pomakni = kljuc.at(brojac % duzina_kljuca) - 'A';
        if(DaLiJeMalo(znak)){
            izlazni.push_back('a' + (znak - 'a' + pomakni) % 26);
        }else{
            izlazni.push_back('A' + (znak - 'A' + pomakni) % 26);
        }
        brojac++;
     }else{
            izlazni.push_back(znak);
        }
    }
    return izlazni;
}

std::string ViginereDesifriranje(std::string poruka, std::string kljuc){
    if(ProvjeriKljuc(kljuc)) throw std::domain_error("Neispravan kljuc");
    std::string izlazni;
    int duzina_kljuca = kljuc.length();
    int brojac = 0;

    for(int i = 0; i < poruka.size(); i++){
        char znak = poruka.at(i);
        if(JeSlovo(znak)){
            char pomakni = kljuc.at(brojac % duzina_kljuca) - 'A';
            if(DaLiJeMalo(znak)){
                izlazni.push_back('a' + (znak - 'a' - pomakni + 26) % 26);
            }else {
                izlazni.push_back('A' + (znak - 'A' - pomakni + 26) % 26);
            }
            brojac++;
        }else{
            izlazni.push_back(znak);
        }
    }
    return izlazni;
}

int main ()
{
    try{
    std::string s, kljuc;
    do{
     std::cout << "Unesite kljuc: ";
     std::getline(std::cin, kljuc);
     if(ProvjeriKljuc(kljuc)) std::cout << "Neispravan kljuc, ponovite unos: " << std::endl;
     std::cin.clear();
    }while(ProvjeriKljuc(kljuc));
    std::cout << "Kljuc prihvacen. Unesite poruku: ";
    std::getline(std::cin, s);
    std::string sifrirana = ViginereSifriranje(s, kljuc);
    std::cout << "Sifrirana poruka: ";
    for(char x : sifrirana){
        std::cout << x;
    }
    std::cout << std::endl;
    std::cout << std::endl;
    std::string kljuc2;
    do{
     std::cout << "Pokusajte pogoditi kljuc: ";
     std::getline(std::cin, kljuc2);
     if(ProvjeriKljuc(kljuc2)){
         std::cout << "Niste pogodili kljuc, probajte ponovo!" << std::endl;
         continue;
     }
     if(!ProvjeriKljuc(kljuc2)){
         std::string desifriranje = ViginereDesifriranje(sifrirana, kljuc2);
         std::cout << "Pokusaj desifriranja: ";
         for(char x : desifriranje){
             std::cout << x;
         }
         std::cout << std::endl;
         if(s == desifriranje){
             std::cout << "Bravo, pogodili ste kljuc!" << std::endl;
             break;
         }
         std::cout << "Niste pogodili kljuc, probajte ponovo!" << std::endl;
         continue;
     }
    }while(true);
    }catch(std::domain_error e){
        std::cout << e.what() << std::endl;
    }


	return 0;
}
