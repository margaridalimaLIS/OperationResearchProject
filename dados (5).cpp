#include "dados.h"
#include "algorithm"
#include <iostream>
using namespace std;

dados::dados()
{}

dados::dados(double a, double b, double c, double d, vector <double> e) {
    n=a;
    k=b;
    cu=c;
    h=d;
    ri=e;
}

vector <double> dados::C(size_t a) {
    size_t b=n-a+1; //numero de elementos do vetor final (numero de custos)
    vector <double> s;
    for(size_t i=0; i<b; i++) {
        s.push_back(c(a,a+i));
    }
    vector <double> ss=s;
    sort(ss.begin(),ss.end());
    vector <double> sol;
    sol.push_back(ss[0]); //vai buscar o custo minimo
    sol.push_back(a);
    size_t w=0, l=0;
    size_t kont=n-b;
    while(w==0) {
        if(s[l]==ss[0]) {
            w=l+1;
        }
        l++;
    }
    sol.push_back(w+kont);
    return sol; //(custo min, p inicial, p final)
}


double dados::c(size_t i, size_t j) {
    double d=0;
    if(i==j) {
        if(i!=n) {d= C(i+1)[0]+k;}
        if(i==n) {d=k; }
    }
    if(j>i) {
       d=k;
       size_t e=1;
       for(size_t l=i;l<j; l++) {
           d+=e*h*ri[l];
           e++;
           }

       if(j<n) {
           d+=C(j+1)[0];
       }
    }
    return d;
}

istream& operator >>(std::istream& is, dados& m) {
    size_t n; //n periodos
    double k;  //custo fixo de produção
    double h; //custo armazenagem
    double cu; //custo unitario
    vector <double> ri; //procura para cada periodo
    double procuras;
    is>>n  >>k >>cu >>h;
    for(size_t i=0; i<n; i++) {
        is>>procuras;
        ri.push_back(procuras);
    }
    dados b(n,k,cu,h,ri);
    m=b;
    return is;
}

void algoritmo(dados prob) {
cout<<"ALGORITMO RECURSIVO" <<endl;

size_t i=prob.n;
vector <vector <double>> ct; //[customin, i, periodo q minimiza o custo]
while(i>0) {
   ct.push_back(prob.C(i));   //vai buscar [700 12 12] [770 11 12] [890 10 12] [1050 9 12] ...
   cout<< "C(" <<i <<")=" <<prob.C(i)[0] <<" (produzir de " <<prob.C(i)[1] <<" até " <<prob.C(i)[2] <<")" <<endl;
   i=i-1; //vai buscar o periodo anterior aquele onde comecou a producao
}

vector <vector <double>> sol; //[3860 1 3] [2490 4 7] [1250 8 12]
double q=0; //descobrir a quantidade produzida
size_t d=ct[prob.n-1][2]+1;  //periodo a seguir a acabar a producao (3+1)
sol.push_back(ct[prob.n-1]); //vai buscar a producao no periodo 1
while(d!=prob.n+1) {
    for(size_t r=0; r<prob.n; r++) {
        if(ct[r][1]==d) {
            sol.push_back(ct[r]);
            d=ct[r][2]+1;
        }
    }
}

cout<< "Plano de producao: " <<endl;
for(size_t u=0; u<sol.size(); u++) {
    for(size_t o=sol[u][1]; o<sol[u][2]+1; o++) {  //somar as quantidades produzidas em cada periodo
        q+=prob.ri[o-1];
    }
    cout<<"Produzir " <<q <<" no periodo " <<sol[u][1] <<" , que corresponde a procura ate ao periodo " <<sol[u][2] <<endl;
    q=0;
}

double custos=sol[0][0];
cout <<"Custo total sem custos unitarios: " <<custos <<endl;
double sum=0;
for(size_t i=0; i<prob.ri.size(); i++) {
    sum+=prob.ri[i];
}
cout<<"Custo total: "<< custos+sum*prob.cu <<endl <<endl;

}

void heuristica(dados prob) {

cout<<"HEURISTICA DE SILVER-MEAL" <<endl;
size_t min=0;
size_t i=1;
vector <double> ml; //minimos locais
while(i<prob.n+1) {
    vector <double> TCU;
    for(size_t j=i; j<prob.n+1; j++) {
        double tcu;
        if(j!=prob.n) {tcu=(prob.c(i,j)-prob.C(j+1)[0])/(j-i+1); }
        else tcu=prob.c(i,j)/(j-i+1);
        TCU.push_back(tcu);
        if(j!=prob.n && j>i) {
            if(TCU.size()>=3) {
                size_t m=1;
                while(min==0) {
                    if(TCU[m]<=TCU[m-1] && TCU[m]<=TCU[m+1]) {
                        min=TCU[m];
                    }
                    if(TCU.size()==m+2) {break;}    //para nao chegar a ultima posicao
                    else {m++;}
                }
            }

        }
        else if(j==prob.n) {
            vector <double> copia=TCU;
            sort(copia.begin(),copia.end());
            size_t n;
            for(size_t z=0; z<TCU.size(); z++) {
                if(copia[0]==TCU[z]) {
                  n=z+1;
                  break;
                }
            }
            double q=0;  //descobrir a quantidade produzida
            for(size_t s=i;s<i+n;s++) {
                q+=prob.ri[s-1];
            }
            cout<<"Custo minimo local " <<TCU[n-1] <<" que corresponde a producao de "<<q
               <<" no periodo " <<i <<" (corresponde a procura ate ao periodo " <<i+n-1 <<")" <<endl;

            if(i+n-1!=j) {

                ml.push_back(prob.c(i,i+n-1)-prob.C(i+n)[0]);
                i=i+n;
            }     //se o periodo nao e o final, avançamos
            else {
                ml.push_back(prob.C(i)[0]);
                i=j+1;
            }
            break;
        }
        if(min!=0) {
            double q=0;  //descobrir a quantidade produzida
            for(size_t s=i;s<j;s++) {
                q+=prob.ri[s-1];
            }
            cout<<"Custo minimo local " <<min <<" que corresponde a producao de "<<q
               <<" no periodo " <<i <<" (corresponde a procura ate ao periodo " <<j-1 <<")" <<endl;
            ml.push_back(prob.c(i,j-1)-prob.C(j)[0]);
            min=0;
            i=j;
            TCU.erase(TCU.begin(),TCU.begin()+TCU.size()-1);
            break;
       }
    }
}
double ct=0;
double procura=0;
for(size_t y=0;y<prob.ri.size();y++) {
    procura+=prob.ri[y];
}
procura=procura*prob.cu;
ml.push_back(procura);
for(size_t g=0; g<ml.size(); g++) {
    ct+=ml[g];
}
cout <<"custo total: " <<ct <<endl <<endl;

}
