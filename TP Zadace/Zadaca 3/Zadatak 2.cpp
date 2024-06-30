// TP 2023/2024: Zadaća 3, Zadatak 2
#include <algorithm>
#include <cctype>
#include <iostream>
#include <map>
#include <set>
#include <vector>


typedef std::map<std::string, std::vector<std::string>> Knjiga;
typedef std::map<std::string, std::set<std::tuple<std::string, int, int>>>
    IndeksPojma;

bool Validno(const std::string &stringov) {
  for (char c : stringov) {
    if (!std::isalnum(c))
      return false;
  }
  return true;
}

std::string PovecajRijec(std::string rijec){
    std::string izlazni;
    for(char x : rijec){
        izlazni += std::toupper(x);
    }
    return izlazni;
}

IndeksPojma KreirajIndeksPojmova(Knjiga knjiga){
    IndeksPojma index;
    Knjiga::iterator it;

    for(it = knjiga.begin(); it != knjiga.end(); it++){
        int br_str = 0;
        std::string temp;
        int key = -1;
        int novija;
        for(std::vector<std::string>::iterator it_str = (*it).second.begin(); it_str != (*it).second.end(); it_str++){
            br_str++;
            for(int i = 0; i < (*it_str).length() + 1; i++){
                if((key == -1) && (((*it_str)[i] < '0') || ((*it_str)[i] > '9')) && (((*it_str)[i] < 'A') || (((*it_str)[i] > 'Z') && ((*it_str)[i] < 'a')) || ((*it_str)[i] > 'z'))) continue;

                if((key == -1) && ((((*it_str)[i] >= '0') && ((*it_str)[i] <= '9')) || ((((*it_str)[i] >= 'A') && ((*it_str)[i] <= 'Z')) || (((*it_str)[i] >= 'a') && ((*it_str)[i] <= 'z'))))){
                    temp.push_back((*it_str)[i]);
                    novija = i + 1;
                    key = 0;
                    continue;
                }

                if((key == 0) && ((((*it_str)[i] >= '0') && ((*it_str)[i] <= '9')) || ((((*it_str)[i] >= 'A') && ((*it_str)[i] <= 'Z')) || (((*it_str)[i] >= 'a') && ((*it_str)[i] <= 'z')))) && ((*it_str)[i] != '\0')){
                    temp.push_back((*it_str)[i]);
                    continue;
                }
                if((key == 0) && ((((((*it_str)[i] < '0') || ((*it_str)[i] > '9'))) && (((*it_str)[i] < 'A') || (((*it_str)[i] > 'Z') && ((*it_str)[i] < 'a')) || ((*it_str)[i] > 'z'))) || ((*it_str)[i] == '\0'))){
                    for(int j = 0; j < temp.length(); j++){
                        if((temp[j] >= 'a') && (temp[j] <= 'z')) temp[j] = temp[j] + 'A' - 'a';
                    }
                    std::set<std::tuple<std::string, int, int>> priv;
                    priv.insert(std::tie((*it).first, br_str, novija));

                    IndeksPojma::iterator pronadji = index.find(temp);
                    if(pronadji == index.end()) index.insert({temp, priv});
                    else (*pronadji).second.insert(std::tie((*it).first, br_str, novija)); 

                    temp.clear();
                    key = -1;
                }
            }
        }
    }
    return index;
}



std::set<std::tuple<std::string, int, int>> PretraziIndeksPojmova(std::string rijec, IndeksPojma index) {
  if (!Validno(rijec))
    throw std::domain_error("Neispravna rijec");

  rijec = PovecajRijec(rijec);

  IndeksPojma::iterator pronadjena = index.find(rijec);

  if (pronadjena == index.end())
    return {};
  else
    return pronadjena->second;
}

void IspisiIndeksPojmova(IndeksPojma ulazni) {
  std::string a;
  int b, c;
  for (auto it = ulazni.begin(); it != ulazni.end(); it++) {
    std::cout << it->first << ": ";
    for (auto k = (it->second).begin(); k != (it->second).end(); k++) {
      std::tie(a, b, c) = *k;
      if (next(k) != (it->second).end()) {
        std::cout << a << "/" << b << "/" << c << ", ";
      } else {
        std::cout << a << "/" << b << "/" << c;
      }
    }
    std::cout << std::endl;
  }
}

int main() {
  Knjiga knjiga;
  std::string poglavlja;
  std::string privrem_pog;
  std::vector<std::string> temp_str;

  int i = 0;
  for (;;) {
    std::cout << "Unesite naziv poglavlja: ";
    std::getline(std::cin, poglavlja);

    if (poglavlja.empty())
      break;

    i = 0;
    for (;;) {
      std::cout << "Unesite sadrzaj stranice " << i + 1 << ": ";
      std::getline(std::cin, privrem_pog);
      if (privrem_pog.empty())
        break;
      temp_str.push_back(privrem_pog);
      i++;
    }
    knjiga.insert({poglavlja, temp_str});
    temp_str.clear();
  }
  IndeksPojma indeksi = KreirajIndeksPojmova(knjiga);

  std::cout << std::endl;
  std::cout << "Kreirani indeks pojmova: " << std::endl;
  IspisiIndeksPojmova(indeksi);
  std::cout << std::endl;

  std::string rijec;
  std::string a;
  int b, c;

  while (true) {
    std::cout << "Unesite rijec: ";
    std::getline(std::cin, rijec);
    if (rijec.empty()) {
      std::cout << "Dovidjenja!" << std::endl;
      break;
    }
    std::transform(rijec.begin(), rijec.end(), rijec.begin(),
                   [](unsigned char c) { return std::toupper(c); });
    try {
      std::set<std::tuple<std::string, int, int>> temp =
          PretraziIndeksPojmova(rijec, indeksi);
      if (temp.empty()) {
        std::cout << "Rijec nije nadjena!";
      } else {
        std::cout << "Rijec nadjena na pozicijama: ";
      }
      for (auto it = temp.begin(); it != temp.end(); it++) {
        std::tie(a, b, c) = *it;
        std::cout << a << "/" << b << "/" << c << " ";
      }
    } catch (std::domain_error izuzetak) {
      std::cout << izuzetak.what() << "!";
    }
    std::cout << std::endl;
  }

  return 0;
}
