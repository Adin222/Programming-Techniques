//TP 2023/2024: Zadaća 4, Zadatak 1
#include <iostream>
#include <cmath>
#include <utility>
#include <algorithm>
#include <cmath>
#include <math.h>

const double epsilon = 0.00001;

typedef std::pair<double, double> Tacka;
enum Pozicija {GoreLijevo, GoreDesno, DoljeLijevo, DoljeDesno};
enum Smjer {Nalijevo, Nadesno};

class Pravougaonik{
    Tacka goreLijevo, goreDesno, doleLijevo, doleDesno;
    double hor, vert;
    void Okreni(Tacka &tacka, const Tacka &t, Smjer s);
public:
    Pravougaonik(const Tacka &t1, const Tacka &t2);
    void Postavi(const Tacka &t1, const Tacka &t2);
    void Postavi(Pozicija p, const Tacka &t);
    void Centriraj(const Tacka &t);
    Tacka DajTjeme(Pozicija p) const;
    Tacka DajCentar() const;
    double DajHorizontalnu() const;
    double DajVertikalnu() const;
    double DajObim() const;
    double DajPovrsinu() const;
    static Pravougaonik Presjek(const Pravougaonik &p1, const Pravougaonik &p2);
    void Transliraj (double delta_x, double delta_y);
    void Rotiraj(const Tacka &t, Smjer s);
    void Ispisi() const;
    friend bool DaLiSePoklapaju(const Pravougaonik &p1, const Pravougaonik &p2);
    friend bool DaLiSuPodudarni(const Pravougaonik &p1, const Pravougaonik &p2);
    friend bool DaLiSuSlicni(const Pravougaonik &p1, const Pravougaonik &p2);
};

Pravougaonik::Pravougaonik(const Tacka &t1, const Tacka &t2){
    Postavi(t1, t2);
}

void Pravougaonik::Postavi(const Tacka &t1, const Tacka &t2){
    if((t1.first - t2.first > epsilon) && (t1.second - t2.second > epsilon)){
        goreDesno.first = t1.first; goreDesno.second = t1.second;
        doleLijevo.first = t2.first; doleLijevo.second = t2.second;
        doleDesno.first = goreDesno.first; doleDesno.second = doleLijevo.second;
        goreLijevo.first = doleLijevo.first; goreLijevo.second = goreDesno.second;
    }
    if((t2.first - t1.first > epsilon) && (t2.second - t1.second > epsilon)){
        doleLijevo.first = t1.first; doleLijevo.second = t1.second;
        goreDesno.first = t2.first; goreDesno.second = t2.second;
        doleDesno.first = goreDesno.first; doleDesno.second = doleLijevo.second;
        goreLijevo.first = doleLijevo.first; goreLijevo.second = goreDesno.second;
    }
    if((t2.first - t1.first > epsilon) && (t1.second - t2.second > epsilon)){
        goreLijevo.first = t1.first; goreLijevo.second = t1.second;
        doleDesno.first = t2.first; doleDesno.second = t2.second;
        doleLijevo.first = goreLijevo.first; doleLijevo.second = doleDesno.second;
        goreDesno.first = doleDesno.first; goreDesno.second = goreLijevo.second;
    }
    if((t1.first - t2.first > epsilon) && (t2.second - t1.second > epsilon)){
        doleDesno.first = t1.first; doleDesno.second = t1.second;
        goreLijevo.first = t2.first; goreLijevo.second = t2.second;
        doleLijevo.first = goreLijevo.first; doleLijevo.second = doleDesno.second;
        goreDesno.first = doleDesno.first; goreDesno.second = goreLijevo.second;
    }
    if((t2.first - t1.first > epsilon) && (std::fabs(t1.second - t2.second) < epsilon)){
        goreLijevo.first = t1.first; goreLijevo.second = t1.second;
        doleLijevo.first = t1.first; doleLijevo.second = t1.second;
        goreDesno.first = t2.first; goreDesno.second = t2.second;
        doleDesno.first = t2.first; doleDesno.second = t2.second;
    }
    if((t1.first - t2.first > epsilon) && (std::fabs(t1.second - t2.second) < epsilon)){
        goreLijevo.first = t2.first; goreLijevo.second = t2.second;
        doleLijevo.first = t2.first; doleLijevo.second = t2.second;
        goreDesno.first = t1.first; goreDesno.second = t1.second;
        doleDesno.first = t1.first; doleDesno.second = t1.second;
    }
    if((std::fabs(t1.first - t2.first) < epsilon) && (t1.second - t2.second > epsilon)){
        goreLijevo.first = t1.first; goreLijevo.second = t1.second;
        goreDesno.first = t1.first; goreDesno.second = t1.second;
        doleLijevo.first = t2.first; doleLijevo.second = t2.second;
        doleDesno.first = t2.first; doleDesno.second = t2.second;
    }
    if((std::fabs(t1.first - t2.first) < epsilon) && (t2.second - t1.second > epsilon)){
        goreLijevo.first = t2.first; goreLijevo.second = t2.second;
        goreDesno.first = t2.first; goreDesno.second = t2.second;
        doleLijevo.first = t1.first; doleLijevo.second = t1.second;
        doleDesno.first = t1.first; doleDesno.second = t1.second;
    }
    if((std::fabs(t1.first - t2.first) < epsilon) && (std::fabs(t1.second - t2.second) < epsilon)){
        goreLijevo.first = t2.first; goreLijevo.second = t2.second;
        goreDesno.first = t2.first; goreDesno.second = t2.second;
        doleLijevo.first = t1.first; doleLijevo.second = t1.second;
        doleDesno.first = t1.first; doleDesno.second = t1.second;
    }
    hor = abs(goreDesno.first - goreLijevo.first);
    vert = abs(goreLijevo.second - doleLijevo.second);
}
void Pravougaonik::Postavi(Pozicija p, const Tacka &t){
    if(p == DoljeDesno){
        Tacka T1(goreLijevo.first, goreLijevo.second);
        Postavi(t, T1);
    }else if(p == DoljeLijevo){
        Tacka T1(goreDesno.first, goreDesno.second);
        Postavi(t, T1);
    }else if(p == GoreDesno){
        Tacka T1(doleLijevo.first, doleLijevo.second);
        Postavi(t, T1);
    }else if(p == GoreLijevo){
        Tacka T1(doleDesno.first, doleDesno.second);
        Postavi(t, T1);
    }
    hor = abs(goreDesno.first - goreLijevo.first);
    vert = abs(goreLijevo.second - doleLijevo.second);
}

Tacka Pravougaonik::DajTjeme(Pozicija p) const {
    if(p == Pozicija::DoljeDesno){
        return doleDesno;
    }else if(p == Pozicija::DoljeLijevo){
        return doleLijevo;
    }else if(p == Pozicija::GoreDesno){
        return goreDesno;
    }else{
        return goreLijevo;
    }
    return std::make_pair(0, 0);
}

Tacka Pravougaonik::DajCentar() const {
    double x = (doleLijevo.first + doleDesno.first) / 2;
    double y = (doleLijevo.second + goreLijevo.second) / 2;
    Tacka centar = std::make_pair(x, y);
    return centar;
}

double Pravougaonik::DajHorizontalnu() const {return hor;}
double Pravougaonik::DajVertikalnu() const {return vert;}
double Pravougaonik::DajObim() const {return (2 * DajHorizontalnu() + 2 * DajVertikalnu());}
double Pravougaonik::DajPovrsinu() const {return (DajHorizontalnu() * DajVertikalnu());}

bool DaLiSePoklapaju(const Pravougaonik &p1, const Pravougaonik &p2){
    Tacka A1 = p1.doleDesno;
    Tacka B1 = p1.goreDesno;
    Tacka C1 = p1.goreLijevo;
    Tacka D1 = p1.doleLijevo;

    Tacka A2 = p2.doleDesno;
    Tacka B2 = p2.goreDesno;
    Tacka C2 = p2.goreLijevo;
    Tacka D2 = p2.doleLijevo;

    bool prva = (A1.first == A2.first) && (A1.second == A2.second);
    bool druga = (B1.first == B2.first) && (B1.second == B2.second);
    bool treca = (C1.first == C2.first) && (C1.second == C2.second);
    bool cetvrta = (D1.first == D2.first) && (D1.second == D2.second);

    return (prva && druga && treca && cetvrta);
}

bool DaLiSuPodudarni(const Pravougaonik &p1, const Pravougaonik &p2){
    return (p1.DajHorizontalnu() == p2.DajHorizontalnu() && p1.DajVertikalnu() == p2.DajVertikalnu()) || (p1.DajHorizontalnu() == p2.DajVertikalnu() && p1.DajVertikalnu() == p2.DajHorizontalnu());
}

bool DaLiSuSlicni(const Pravougaonik &p1, const Pravougaonik &p2){
    double a = p1.DajVertikalnu();
    double b = p1.DajHorizontalnu();
    double c = p2.DajVertikalnu();
    double d = p2.DajHorizontalnu();

    if(std::fabs(((a / b)) - ((c / d))) < epsilon) return true;

    return ((a * b) == (c * d)) || ((a * c) == (b * d));
}

void Pravougaonik::Centriraj(const Tacka &t){
    double x, y;
    x = std::fabs(goreDesno.first - goreLijevo.first);
    x = x / 2;
    y = std::fabs(goreDesno.second - doleDesno.second);
    y = y / 2;
    
    doleLijevo.first = t.first - x;
    doleLijevo.second = t.second - y;

    goreLijevo.first = t.first - x;
    goreLijevo.second = t.second + y;

    doleDesno.first = t.first + x;
    doleDesno.second = t.second - y;

    goreDesno.first = t.first + x;
    goreDesno.second = t.second + y;
}

void Pravougaonik::Ispisi() const {std::cout << "{" << "{" << goreLijevo.first << "," << goreLijevo.second << "}" << "," << "{" << doleDesno.first << "," << doleDesno.second << "}" << "}";}

void Pravougaonik::Transliraj(double delta_x, double delta_y){
    doleLijevo.first += delta_x;
    doleLijevo.second += delta_y;

    goreLijevo.first += delta_x;
    goreLijevo.second += delta_y;

    doleDesno.first += delta_x;
    doleDesno.second += delta_y;

    goreDesno.first += delta_x;
    goreDesno.second += delta_y;
}

void Pravougaonik::Rotiraj(const Tacka &t, Smjer s){
    if(s == Nalijevo){
        double x1, y1, x2, y2, x3, y3, x4, y4;
        x1 = goreDesno.first; y1 = goreDesno.second;
        x2 = doleLijevo.first; y2 = doleLijevo.second;
        x1 = x1 - t.first; y1 = y1 - t.second;
        x2 = x2 - t.first; y2 = y2 - t.second;
        x3 = (-1) * y1; y3 = x1;
        x4 = (-1) * y2; y4 = x2;
        x3 = x3 + t.first; y3 = y3 + t.second;
        x4 = x4 + t.first; y4 = y4 + t.second;
        Tacka T1(x3, y3);
        Tacka T2(x4, y4);
        Postavi(T1, T2);
    }

        if(s == Nadesno){
            double x1, y1, x2, y2, x3, y3, x4, y4;
            x1 = goreDesno.first; y1 = goreDesno.second;
            x2 = doleLijevo.first; y2 = doleLijevo.second;
            x1 = x1 - t.first; y1 = y1 - t.second;
            x2 = x2 - t.first; y2 = y2 - t.second;
            x3 = y1; y3 = (-1) * x1;
            x4 = y2; y4 = (-1) * x2;
            x3 = x3 + t.first; y3 = y3 + t.second;
            x4 = x4 + t.first; y4 = y4 + t.second;
            Tacka T1(x3, y3);
            Tacka T2(x4, y4);
            Postavi(T1, T2);
        }
}

Pravougaonik Pravougaonik::Presjek(const Pravougaonik &p1, const Pravougaonik &p2){
    double Xmin = std::max(p1.doleLijevo.first, p2.doleLijevo.first);
    double Ymin = std::max(p1.doleLijevo.second, p2.doleLijevo.second);
    double Xmax = std::min(p1.goreDesno.first, p2.goreDesno.first);
    double Ymax = std::min(p1.goreDesno.second, p2.goreDesno.second);

    if(Xmin > Xmax || Ymin > Ymax) throw std::domain_error("Pravougaonici se ne presjecaju");
    return Pravougaonik(std::make_pair(Xmin, Ymax), std::make_pair(Xmax, Ymin));
}

int main ()
{
    int n;
    std::cout << "Unesite n: ";
    std::cin >> n;
    double t11, t12, t21, t22;

    int brojac = 0;

    Pravougaonik **dinam_niz = new Pravougaonik*[n]();
    try{
        for(int i = 0; i < n; i++){
            std::cout << "Unesite 1. tjeme pravougaonika " << i + 1 << ": ";
            std::cin >> t11 >> t12;
            std::cout << "Unesite 2. tjeme pravougaonika " << i + 1 << ": ";
            std::cin >> t21 >> t22;

            dinam_niz[brojac++] = new Pravougaonik(std::make_pair(t11, t12), std::make_pair(t21, t22)); 
        }
    }catch(...){
        std::cout << "Problemi sa memorijom";
        for(int i = 0; i < brojac; i++) delete dinam_niz[i];
        delete[] dinam_niz;
    }
    int delta_x, delta_y;

    std::cout << "Unesite podatke za transliranje (dx dy): ";
    std::cin >> delta_x >> delta_y;

    std::transform(dinam_niz, dinam_niz + brojac, dinam_niz, [delta_x, delta_y](Pravougaonik *pravougao){
        pravougao->Transliraj(delta_x, delta_y);
        pravougao->Rotiraj(pravougao->DajCentar(), Nalijevo);
        return pravougao;
    });

    std::cout << "Pravougaonici, nakon transformacija: ";

    std::for_each(dinam_niz, dinam_niz + brojac, [](Pravougaonik *prav){
        std::cout << std::endl;
        prav->Ispisi();
    });

    auto NajvecaPovrsina = *std::max_element(dinam_niz, dinam_niz + brojac, [](Pravougaonik *A, Pravougaonik *B){
        return A->DajPovrsinu() < B->DajPovrsinu();
    });
    std::cout << std::endl;
    std::cout << "Pravougaonik s najvecom povrsinom: ";
    NajvecaPovrsina->Ispisi();

    for(int i = 0; i < brojac; i++) delete dinam_niz[i];
    delete[] dinam_niz;
	return 0;
}
