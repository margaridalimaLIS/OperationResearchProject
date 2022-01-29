#ifndef DADOS_H
#define DADOS_H
#include <vector>
#include <fstream>
using namespace std;

class dados
{
public:
    dados();
    dados(double, double, double, double, vector <double>);
    size_t n; //n periodos
    double k;  //custo fixo de produção
    double h; //custo armazenagem
    double cu; //custo unitario producao
    vector <double> ri; //procura para cada periodo
    double c(size_t, size_t);
    vector <double> C(size_t);

};

istream& operator >>(std::istream& is, dados& m);
void algoritmo(dados prob);
void heuristica(dados prob);

#endif // DADOS_H


