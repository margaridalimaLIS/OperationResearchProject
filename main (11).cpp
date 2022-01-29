#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include "dados.h"

using namespace std;

int main() {

//ir buscar dados ao ficheiro
  dados prob;
  ifstream dados{"C:/Users/megss/Desktop/dados.txt"};
  if(!dados) {
          cerr<<"Erro na leitura do ficheiro!" <<endl;
      }
      else {
      dados>>prob;
      cout << "Ficheiro lido!" <<endl;
      dados.close();
      }

 //Heuristica Silver-Meal
  heuristica(prob);

 //Algoritmo recursivo
  algoritmo(prob);


  return 0;

}
