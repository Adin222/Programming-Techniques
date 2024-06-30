//TP 2023/2024: Zadaća 1, Zadatak 3
#include <cctype>
#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <limits>

bool ImaLiZnak(char znak){
    std::string znakovi = ".,;!?)/";
    for(int i = 0; i < znakovi.size(); i++){
        if(znakovi.at(i) == znak) return true;
    }
return false;
}
bool JeLiSlovo(char znak){
    return (znak >= 'a' && znak <= 'z') || (znak >= 'A' && znak <= 'Z');
}
bool ProvjeriRijec(std::string rijec){
    for(int i = 0; i < rijec.size(); i++){
        if(JeLiSlovo(rijec.at(i))){
            return false;
        }
    }
    for(int i = 0; i < rijec.size(); i++){
        if(rijec.at(i) >= '0' && rijec.at(i) <= '9'){
            if(ImaLiZnak(rijec.at(rijec.size()-1))){
                return true;
            }
        }
    }
    for(int i = 0; i < rijec.size(); i++){
        if(rijec.at(i) >= '0' && rijec.at(i) <= '9'){
            return true;
        }
    }
    return false;
}
long long int Stoll(std::string znak){
    long long int rezultat = 0;
    bool negativan = false;
    int indeks = 0;
    if(znak.at(0) == '-'){
        negativan = true;
        indeks = 1;
    }
    for(int i = indeks; i < znak.size(); i++){
        if(znak.at(i) >= '0' && znak.at(i) <= '9'){
            rezultat = rezultat * 10 + (znak.at(i) - '0');
        }
    }
    if(negativan){
        rezultat = -1 * rezultat;
    }
    int brojac = 0;
    long long int tmp = rezultat;
    while(tmp != 0){
        tmp /= 10;
        brojac++;
    }
    int brojac_br = 0;
    for(int i = 0; i < znak.size(); i++){
        if(znak.at(i) >= '0' && znak.at(i) <= '9'){
            brojac_br++;
        }
    }
    long long int mali = std::numeric_limits<long long int>::min() + 1;
    if(rezultat == 0) brojac++;
    if(brojac != brojac_br) throw std::range_error("Prevelik broj");
    if(rezultat < mali ) throw;
    return rezultat;
}

std::vector<long long int> BrojeviUStringu(std::string s){
    std::vector<long long int> izlazni;
    if(s.size() == 0){
        return izlazni;
    }
    int i = 0;
    while(s.at(i) == ' ' && i < s.size()){
        s.erase(s.begin() + i);
    }
    int k = s.size() - 1;
    while(s.at(k) == ' ' && k > 0){
        s.erase(s.begin() + k);
        k--;
    }
    std::string rijec;
    int l = 0;
    for(int i = 0; i < s.size(); i++){
        rijec = "";
        while(l < s.size() && s.at(l) != ' '){
            rijec += s.at(l);
            l++;
        }
        if(ProvjeriRijec(rijec)){
            izlazni.push_back(Stoll(rijec));
        }
        if(l < s.size() && s.at(l) == ' '){
            l++;
        }
    }
    return izlazni;
}
std::string MalaSlova(std::string s){
    for(int i = 0; i <s.size(); i++){
        if(s.at(i) >= 'A' && s.at(i) <= 'Z'){
            s.at(i) = s.at(i) - 'A' + 'a';
        }
    }
    return s;
}

int main ()
{
    try{
    std::string s;
    std::cout << "Unesite string: ";
    std::getline(std::cin, s);
    if(s.size() == 0){
        std::cout << "String je prazan" << std::endl;
        return 0;
    } 
    std::vector<long long int> v = BrojeviUStringu(s);
    if(v.size() == 0){
      std::cout << "Uneseni string ne sadrzi nijedan broj!"; 
      return 0;
    } 
    std::cout << "Brojevi unutar stringa: ";
    for(int i = 0; i < v.size(); i++){
        std::cout << v.at(i) << " ";
    }
    }catch(std::range_error e){
        std::cout << "PROBLEM: Uneseni string sadrzi " << MalaSlova(e.what()) << "!!!" <<std::endl;
    }catch(...){
        std::cout << "Premali broj" << std::endl;
    }
	return 0;
}
