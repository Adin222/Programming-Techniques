// TP 2023/2024: Zadaća 4, Zadatak 3
#include <algorithm>
#include <initializer_list>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <tuple>
#include <utility>


class Datum {
  int dan, mjesec, godina;

public:
  Datum(int dan, int mjesec, int godina) { Postavi(dan, mjesec, godina); }
  void Postavi(int dan, int mjesec, int godina);
  std::tuple<int, int, int> Ocitaj() const {
    return std::make_tuple(dan, mjesec, godina);
  }
  void Ispisi() const { std::cout << dan << "/" << mjesec << "/" << godina; }
};

void Datum::Postavi(int d, int m, int g) {
  int broj_dana[12]{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  if (g % 4 == 0 && g % 100 != 0 || g % 400 == 0)
    broj_dana[1]++;
  if (g < 1 || d < 1 || m < 1 || m > 12 || d > broj_dana[m - 1])
    throw std::domain_error("Neispravan datum");
  dan = d;
  mjesec = m;
  godina = g;
}

class Vrijeme {
  int sati, minute;

public:
  Vrijeme(int sati, int minute) { Postavi(sati, minute); }
  void Postavi(int sati, int minute);
  std::pair<int, int> Ocitaj() const { return std::make_pair(sati, minute); }
  void Ispisi() const;
};

void Vrijeme::Postavi(int sati, int minute) {
  if (sati < 0 || sati > 23)
    throw std::domain_error("Neispravno vrijeme");
  if (minute < 0 || minute > 59)
    throw std::domain_error("Neispravno vrijeme");
  Vrijeme::sati = sati;
  Vrijeme::minute = minute;
}

void Vrijeme::Ispisi() const {
  std::string ss =
      (sati < 10) ? "0" + std::to_string(sati) : std::to_string(sati);
  std::string mm =
      (minute < 10) ? "0" + std::to_string(minute) : std::to_string(minute);
  std::cout << ss << ":" << mm;
}

class Pregled {
  std::string ImePacijenta;
  Datum datum;
  Vrijeme vrijeme;

public:
  Pregled(std::string ImePacijenta, Datum datum, Vrijeme vrijeme)
      : ImePacijenta(ImePacijenta), datum(datum), vrijeme(vrijeme) {}
  Pregled(std::string ImePacijenta, int dan, int mjesec, int godina, int sati,
          int minute)
      : ImePacijenta(ImePacijenta), datum(dan, mjesec, godina),
        vrijeme(sati, minute) {}
  Pregled &PromijeniPacijenta(std::string NovoIme) {
    ImePacijenta = NovoIme;
    return *this;
  }
  Pregled &PromijeniDatum(Datum NoviDatum) {
    datum = NoviDatum;
    return *this;
  }
  Pregled &PromijeniVrijeme(Vrijeme NovoVrijeme) {
    vrijeme = NovoVrijeme;
    return *this;
  }
  void PomjeriDanUnaprijed();
  void PomjeriDanUnazad();
  std::string DajImePacijenta() const { return ImePacijenta; }
  Datum DajDatumPregleda() const { return datum; }
  Vrijeme DajVrijemePregleda() const { return vrijeme; }
  void Ispisi() const;
  static bool DolaziPrije(Pregled &A, Pregled &B);
};

bool Pregled::DolaziPrije(Pregled &A, Pregled &B) {
  int a1, b1, c1, a2, b2, c2, ss1, mm1, ss2, mm2;
  std::tie(a1, b1, c1) = A.datum.Ocitaj();
  std::tie(a2, b2, c2) = B.datum.Ocitaj();
  std::tie(ss1, mm1) = A.vrijeme.Ocitaj();
  std::tie(ss2, mm2) = B.vrijeme.Ocitaj();

  if (c1 < c2)
    return true;
  if (c1 > c2)
    return false;

  if (b1 < b2)
    return true;
  if (b1 > b2)
    return false;

  if (a1 < a2)
    return true;
  if (a1 > a2)
    return false;

  if (ss1 < ss2)
    return true;
  if (ss1 > ss2)
    return false;
  return (mm1 < mm2);
}

void Pregled::PomjeriDanUnaprijed() {
  int dan, mjesec, godina;
  std::tie(dan, mjesec, godina) = datum.Ocitaj();
  dan++;

  int broj_dana[12]{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  if (godina % 4 == 0 && godina % 100 != 0 || godina % 400 == 0)
    broj_dana[1]++;

  if (dan > broj_dana[mjesec - 1]) {
    dan = 1;
    mjesec++;
    if (mjesec > 12) {
      mjesec = 1;
      godina++;
    }
  }
  datum.Postavi(dan, mjesec, godina);
}

void Pregled::Ispisi() const {
  std::cout << std::left << std::setw(30) << ImePacijenta;
  datum.Ispisi();
  std::cout << " ";
  vrijeme.Ispisi();
  std::cout << std::endl;
}

void Pregled::PomjeriDanUnazad() {
  int dan, mjesec, godina;
  std::tie(dan, mjesec, godina) = datum.Ocitaj();
  dan--;

  int broj_dana[12]{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  if (godina % 4 == 0 && godina % 100 != 0 || godina % 400 == 0)
    broj_dana[1]++;

  if (dan < 1) {
    mjesec--;
    if (mjesec < 1) {
      mjesec = 12;
      godina--;
    }
    dan = broj_dana[mjesec - 1];
  }
  datum.Postavi(dan, mjesec, godina);
}

class Pregledi {
  int MaxPregleda;
  int BrojPregleda;
  Pregled **niz_pregleda;

public:
  explicit Pregledi(int MaxPregleda)
      : MaxPregleda(MaxPregleda), BrojPregleda(0),
        niz_pregleda(new Pregled *[MaxPregleda] {}) {}
  Pregledi(std::initializer_list<Pregled> lista_pregleda);
  ~Pregledi();
  Pregledi(const Pregledi &l);
  Pregledi(Pregledi &&l);
  Pregledi &operator=(const Pregledi &l);
  Pregledi &operator=(Pregledi &&l);
  void RegistrirajPregled(std::string ImePacijenta, Datum datum,
                          Vrijeme vrijeme);
  void RegistrirajPregled(std::string ImePacijenta, int dan, int mjesec,
                          int godina, int sati, int minute);
  void RegistrirajPregled(Pregled *pregled);
  int DajBrojPregleda() const { return BrojPregleda; }
  int DajBrojPregledaNaDatum(Datum dat) const;
  Pregled DajNajranijiPregled() const;
  Pregled &DajNajranijiPregled();
  void IsprazniKolekciju();
  void ObrisiNajranijiPregled();
  void ObrisiPregledePacijenta(std::string pacijent);
  void IspisiPregledeNaDatum(Datum dat) const;
  void IspisiSvePreglede() const;
};

Pregledi::Pregledi(std::initializer_list<Pregled> lista_pregleda)
    : MaxPregleda(lista_pregleda.size()), BrojPregleda(lista_pregleda.size()),
      niz_pregleda(new Pregled *[lista_pregleda.size()] {}) {
  try {
    int i = 0;
    for (const Pregled &pregled : lista_pregleda)
      niz_pregleda[i++] = new Pregled(pregled);
  } catch (...) {
    for (int i = 0; i < BrojPregleda; i++)
      delete niz_pregleda[i];
    delete[] niz_pregleda;
    throw;
  }
}

Pregledi::~Pregledi() {
  for (int i = 0; i < BrojPregleda; i++)
    delete niz_pregleda[i];
  delete[] niz_pregleda;
}

Pregledi::Pregledi(const Pregledi &l)
    : MaxPregleda(l.MaxPregleda), BrojPregleda(l.BrojPregleda),
      niz_pregleda(new Pregled *[l.MaxPregleda] {}) {
  try {
    for (int i = 0; i < l.BrojPregleda; i++) {
      niz_pregleda[i] = new Pregled(*l.niz_pregleda[i]);
    }
  } catch (...) {
    for (int i = 0; i < l.BrojPregleda; i++)
      delete niz_pregleda[i];
    delete[] niz_pregleda;
  }
}

Pregledi::Pregledi(Pregledi &&l)
    : MaxPregleda(l.MaxPregleda), BrojPregleda(l.BrojPregleda),
      niz_pregleda(l.niz_pregleda) {
  l.BrojPregleda = 0;
  l.niz_pregleda = nullptr;
}

Pregledi &Pregledi::operator=(const Pregledi &l) {
  if (l.MaxPregleda != MaxPregleda)
    throw std::logic_error("Nesaglasni kapaciteti pregleda");

  if (l.BrojPregleda > BrojPregleda) {
    try {
      for (int i = BrojPregleda; i < l.BrojPregleda; i++)
        niz_pregleda[i] = new Pregled(*l.niz_pregleda[i]);
      for (int i = 0; i < BrojPregleda; i++)
        *niz_pregleda[i] = *l.niz_pregleda[i];
    } catch (...) {
      for (int i = BrojPregleda; i < l.BrojPregleda; i++) {
        delete niz_pregleda[i];
        niz_pregleda[i] = nullptr;
      }
      throw;
    }
  } else {
    for (int i = l.BrojPregleda; i < BrojPregleda; i++) {
      delete niz_pregleda[i];
      niz_pregleda[i] = nullptr;
    }
    for (int i = 0; i < l.BrojPregleda; i++)
      *niz_pregleda[i] = *l.niz_pregleda[i];
  }
  BrojPregleda = l.BrojPregleda;
  return *this;
}

Pregledi &Pregledi::operator=(Pregledi &&l) {
  if (l.MaxPregleda != MaxPregleda)
    throw std::logic_error("Nesaglasni kapaciteti pregleda");
  std::swap(MaxPregleda, l.MaxPregleda);
  std::swap(niz_pregleda, l.niz_pregleda);
  return *this;
}

void Pregledi::RegistrirajPregled(std::string ImePacijenta, Datum datum,
                                  Vrijeme vrijeme) {
  if (BrojPregleda >= MaxPregleda)
    throw std::range_error("Dostignut maksimalni broj pregleda");
  niz_pregleda[BrojPregleda++] = new Pregled(ImePacijenta, datum, vrijeme);
}

void Pregledi::RegistrirajPregled(std::string ImePacijenta, int dan, int mjesec,
                                  int godina, int sati, int minute) {
  if (BrojPregleda >= MaxPregleda)
    throw std::range_error("Dostignut maksimalni broj pregleda");
  niz_pregleda[BrojPregleda++] =
      new Pregled(ImePacijenta, dan, mjesec, godina, sati, minute);
}

void Pregledi::RegistrirajPregled(Pregled *pregled) {
  if (BrojPregleda >= MaxPregleda)
    throw std::range_error("Dostignut maksimalni broj pregleda");
  niz_pregleda[BrojPregleda++] = pregled;
}

int Pregledi::DajBrojPregledaNaDatum(Datum dat) const {
  int dan2, mjesec2, godina2;
  std::tie(dan2, mjesec2, godina2) = dat.Ocitaj();

  int br_pregleda = std::count_if(
      niz_pregleda, niz_pregleda + BrojPregleda,
      [dan2, mjesec2, godina2](Pregled *A) {
        int dan1, mjesec1, godina1;
        std::tie(dan1, mjesec1, godina1) = A->DajDatumPregleda().Ocitaj();
        return (dan1 == dan2 && mjesec1 == mjesec2 && godina1 == godina2);
      });
  return br_pregleda;
}

Pregled Pregledi::DajNajranijiPregled() const {
  if (BrojPregleda == 0)
    throw std::domain_error("Nema registriranih pregleda");
  Pregled *najraniji = *std::min_element(
      niz_pregleda, niz_pregleda + BrojPregleda, [](Pregled *p1, Pregled *p2) {
        auto datum1 = p1->DajDatumPregleda().Ocitaj();
        auto datum2 = p2->DajDatumPregleda().Ocitaj();
        if (datum1 != datum2) {
          return datum1 < datum2;
        }
        auto vrijeme1 = p1->DajVrijemePregleda().Ocitaj();
        auto vrijeme2 = p2->DajVrijemePregleda().Ocitaj();
        return vrijeme1 < vrijeme2;
      });
  return *najraniji;
}

Pregled &Pregledi::DajNajranijiPregled() {
  if (BrojPregleda == 0)
    throw std::domain_error("Nema registriranih pregleda");
  Pregled *najraniji = *std::min_element(
      niz_pregleda, niz_pregleda + BrojPregleda, [](Pregled *p1, Pregled *p2) {
        auto datum1 = p1->DajDatumPregleda().Ocitaj();
        auto datum2 = p2->DajDatumPregleda().Ocitaj();
        if (datum1 != datum2) {
          return datum1 < datum2;
        }
        auto vrijeme1 = p1->DajVrijemePregleda().Ocitaj();
        auto vrijeme2 = p2->DajVrijemePregleda().Ocitaj();
        return vrijeme1 < vrijeme2;
      });
  return *najraniji;
}

void Pregledi::IsprazniKolekciju() {
  for (int i = 0; i < BrojPregleda; i++)
    delete niz_pregleda[i];
  delete[] niz_pregleda;
  niz_pregleda = new Pregled *[MaxPregleda];
  BrojPregleda = 0;
}

void Pregledi::ObrisiNajranijiPregled() {
  if (BrojPregleda == 0)
    throw std::range_error("Prazna kolekcija");

  Pregled &najraniji_pregled = this->DajNajranijiPregled();

  auto it = std::find_if(
      niz_pregleda, niz_pregleda + BrojPregleda,
      [&najraniji_pregled](Pregled *p) { return p == &najraniji_pregled; });
  if (it != niz_pregleda + BrojPregleda) {
    delete *it;
    *it = nullptr;
    std::swap(*it, niz_pregleda[BrojPregleda - 1]);
    BrojPregleda--;
  }
}

void Pregledi::ObrisiPregledePacijenta(std::string pacijent) {
  int brojac = 0;
  for (int i = 0; i < BrojPregleda; i++) {
    if (niz_pregleda[i]->DajImePacijenta() == pacijent) {
      delete niz_pregleda[i];
      niz_pregleda[i] = nullptr;
      brojac++;
    }
  }
  std::remove(niz_pregleda, niz_pregleda + BrojPregleda, nullptr);
  BrojPregleda -= brojac;
}

void Pregledi::IspisiPregledeNaDatum(Datum dat) const {
  std::sort(niz_pregleda, niz_pregleda + BrojPregleda,
            [](Pregled *a, Pregled *b) {
              auto vrijeme1 = a->DajVrijemePregleda().Ocitaj();
              auto vrijeme2 = b->DajVrijemePregleda().Ocitaj();
              return vrijeme1 < vrijeme2;
            });
  int d1, m1, g1, d2, m2, g2;
  std::tie(d1, m1, g1) = dat.Ocitaj();
  for (int i = 0; i < BrojPregleda; i++) {
    std::tie(d2, m2, g2) = niz_pregleda[i]->DajDatumPregleda().Ocitaj();
    if (d1 == d2 && m1 == m2 && g1 == g2) {
      niz_pregleda[i]->Ispisi();
    }
  }
}

void Pregledi::IspisiSvePreglede() const {
  std::sort(niz_pregleda, niz_pregleda + BrojPregleda,
            [](Pregled *a, Pregled *b) {
              auto datum1 = a->DajDatumPregleda().Ocitaj();
              auto datum2 = b->DajDatumPregleda().Ocitaj();
              if(datum1 != datum2){
                  return datum1 < datum2;
              }else{
              auto vrijeme1 = a->DajVrijemePregleda().Ocitaj();
              auto vrijeme2 = b->DajVrijemePregleda().Ocitaj();
              return vrijeme1 < vrijeme2;
              }
            });
  for (int i = 0; i < BrojPregleda; i++)
    niz_pregleda[i]->Ispisi();
}

int main() {
  std::cout << "Unesite maksimalni broj pregleda: ";
  int max_pr;
  std::cin >> max_pr;
  Pregledi pregledi(max_pr);

  for (;;) {
    std::cout << "1. Registriraj Pregled" << std::endl;
    std::cout << "2. Daj Broj Pregleda" << std::endl;
    std::cout << "3. Daj Broj Pregleda Na Datum" << std::endl;
    std::cout << "4. Daj Najraniji Pregled" << std::endl;
    std::cout << "5. Obrisi Najraniji Pregled" << std::endl;
    std::cout << "6. Obrisi Preglede Pacijenta" << std::endl;
    std::cout << "7. Ispisi Preglede Na Datum" << std::endl;
    std::cout << "8. Ispisi Sve Preglede" << std::endl;
    std::cout << "9. Isprazni Kolekciju" << std::endl;
    std::cout << "10. Izlaz" << std::endl;
    std::cout << "Unesite izbor: ";

    int rezim;
    std::cin >> rezim;

    if (rezim == 10)
      break;

    switch (rezim) {
    case 1: {
      std::string ImePac;
      int dan, mjesec, godina, sati, minute;
      std::cout << "Unesite ime pacijenta: ";
      std::cin.ignore(10000, '\n');
      std::getline(std::cin, ImePac);
      std::cout << "Unesite datum i vrijeme pregleda (dan mjesec godina sati "
                   "minute): ";
      std::cin >> dan >> mjesec >> godina >> sati >> minute;
      try {
        pregledi.RegistrirajPregled(ImePac, dan, mjesec, godina, sati, minute);
      } catch (const std::range_error &greska) {
        std::cout << greska.what() << std::endl;
      }
      break;
    }
    case 2: {
      std::cout << "Ukupan broj pregleda: " << pregledi.DajBrojPregleda()
                << std::endl;
      break;
    }
    case 3: {
      int dan, mjesec, godina;
      std::cout << "Unesite datum (dan mjesec godina): ";
      std::cin >> dan >> mjesec >> godina;
      Datum datum(dan, mjesec, godina);
      std::cout << "Broj pregleda na datum: "
                << pregledi.DajBrojPregledaNaDatum(datum) << std::endl;
      break;
    }
    case 4: {
      try {
        Pregled najraniji = pregledi.DajNajranijiPregled();
        najraniji.Ispisi();
      } catch (const std::range_error &greska) {
        std::cout << greska.what() << std::endl;
      }
      break;
    }
    case 5: {
      try {
        pregledi.ObrisiNajranijiPregled();
        std::cout << "Najraniji pregled je obrisan." << std::endl;
      } catch (const std::range_error &greska) {
        std::cout << greska.what() << std::endl;
      }
      break;
    }
    case 6: {
      std::string ImePacijenta;
      std::cout << "Unesite ime pacijenta: ";
      std::cin.ignore(10000, '\n');
      std::getline(std::cin, ImePacijenta);
      pregledi.ObrisiPregledePacijenta(ImePacijenta);
      std::cout << "Pregledi za pacijenta " << ImePacijenta << " su obrisani."
                << std::endl;
      break;
    }
    case 7: {
      int dan, mjesec, godina;
      std::cout << "Unesite datum: ";
      std::cin >> dan >> mjesec >> godina;
      Datum datum(dan, mjesec, godina);
      pregledi.IspisiPregledeNaDatum(datum);
      break;
    }
    case 8: {
      std::cout << "Svi pregledi: " << std::endl;
      pregledi.IspisiSvePreglede();
      break;
    }
    case 9: {
      pregledi.IsprazniKolekciju();
      std::cout << "Kolekcija ispraznjena." << std::endl;
      break;
    }
    default: {
      std::cout << "Nepoznat izbor" << std::endl;
      break;
    }
    }
  }
  std::cout << "Kraj programa." << std::endl;
  return 0;
}
