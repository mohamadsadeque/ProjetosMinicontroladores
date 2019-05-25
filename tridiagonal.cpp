#include <iostream>
#include <cmath>]
#include <cstdlib>
#include <time.h>
using namespace std;

int elemento(int i,int j);
void calcS(float v[], float b[], int n);
void gerarV(float v[], int n);
void printV(float v[], int n);
//void jacobi(float v[], int it);
#define TAM 8
int main()
{
   float b[TAM] = {0,0,0,0,0,0,0,0};
  //  float v[TAM] = {-1,1,-1,-1,-1,-1,1,-1};  // vetor  usado no exemplo do projeto
  float v[TAM];
       srand(time(NULL));
    for(int i =0 ; i< 20; i++){
    gerarV(v,TAM);
    printV(v,TAM);
    };
    calcS(v,b,TAM);
    printV(b,TAM);

    return 0;
}

int elemento(int i, int j){
    if(i == j) return 4;
    else if(abs(i-j) == 1) return -1;
    else return 0;
}

void calcS(float v[], float b[], int n)
{

    for(int i = 0; i<n; i++)
    {
        for(int j = 0; j<n; j++)
        {
            b[i] += elemento(i,j) * v[j];


        }
        cout << b[i] << endl;
    }
}

/*
void jacobi(float a[],float b[],float x[],int n; int it)
{
    int soma = 0;
    for(int k = 0; k<it; k++)
    {
        cout << "Vetor solução da iteracao:" << it << endl;
            printV(x[],n);
        for(int i = 0; i<n; i++)
        {
            for(int j = 0; j<n; j++)
            {
                if(j != i) soma += elemento(i,j)*x[j];
            }

        }
    }
}
*/

void gerarV(float v[], int n){
    int a;

    for(int i = 0; i<n; i++){
        a = rand()%100;
        if(a%2 == 0) v[i] = 1;
        else v[i] = -1;
    }

}


void printV(float v[], int n){
for(int i =0 ; i<n;i++){
    cout<< v[i] << " ";

}
cout << endl;

}
