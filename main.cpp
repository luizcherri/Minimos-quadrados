/*
 * File:   main.cpp
 * Author: Ana claudia Molina Zaqueu
 *         Isabela Rita Perez
 *         Luiz Henrique Cherri
 *
 * Created on 20 de Setembro de 2009, 11:34
 */

#include <iostream>
#include <stdlib.h>
#include <math.h>
#define pi 3.14159265358979323846264338327950288
using namespace std;
#define maxERROR 1400


typedef struct Dados {
        double dia;
        double temp;
} dados;

/*
 * As funcoes fi sao criadas para facilitar a mudanca da base...
 * Pode-se adicionar mais funcoes...
*/

// Funcoes fi1 ... fi5
double fi(int opt, double x){
    if (opt == 1)
        return (1);
    if (opt == 2)
        return (sin(2*pi*x/365));
    if (opt == 3)
        return (cos(2*pi*x/365));
    if (opt == 4)
        return (sin(2*pi*x));
    if (opt == 5)
        return (cos(2*pi*x));

    else {
        cout << "ERRO... função inexistente" << endl;
    }
}

double sumA(int i, int j, dados *dat, int size){
    double soma = 0;
    for (int k = 0 ; k < size ; k++){
        soma = soma + fi(i+1, dat[k].dia)*fi(j+1, dat[k].dia);
    }
    return (soma);
}

double sumb (int i, dados *dat, int size) {
    int soma = 0;
    for (int k = 0 ; k < size ; k++){
        soma = soma + fi(i+1, dat[k].dia)*dat[k].temp;
    }
    return (soma);
}

void geraA (double **A, int n, dados *dat, int size){
    for (int i = 0 ; i< n ; i++){
        for (int j = 0 ; j < n ; j ++){
            A[i][j] = sumA (i, j, dat, size);
        }
    }
}

void gerab (double *b, int n, dados *dat, int size){
    for (int i = 0 ; i < n ; i++){
        b[i] = sumb (i, dat, size);
    }
}

void forwardSubstitution(int n, double **a, double *b) {
	int i, j, k, max;
	float t;
	for (i = 0; i < n; ++i) {
		max = i;
		for (j = i + 1; j < n; ++j)
			if (a[j][i] > a[max][i])
				max = j;

		for (j = n; j >= i; --j)
			for (k = i + 1; k < n; ++k){
                            if (j!=n)
                                a[k][j] -= a[k][i]/a[i][i] * a[i][j];
                            else
                                b[k] -= a[k][i]/a[i][i] * b[i]; ;
                        }
	}
}

void reverseElimination(int n, double **a, double *b) {
	int i, j;
	for (i = n - 1; i >= 0; --i) {
		b[i] = b[i] / a[i][i];
		a[i][i] = 1;
		for (j = i - 1; j >= 0; --j) {
			b[j] -= a[j][i] * b[i];
			a[j][i] = 0;
		}
	}
}

void gauss(int n, double **a, double *b) {
	int i, j;

	forwardSubstitution(n,a,b);
	reverseElimination(n,a,b);
}

// Retorna o valor da funcao obtida no ponto
double Spto (dados *dat, int i, double *b){
    return (b[0]*fi(1, dat[i].dia) + b[1]*fi(2, dat[i].dia) + b[2]*fi(3, dat[i].dia)+ b[3]*fi(4, dat[i].dia) + b[4]*fi(5, dat[i].dia));
}


// Calculo do erro
//void erro (double *b, dados *dat, int size){
//    int i = 1;
//    double soma = 0;
//    int j = 0;
//    int k = 0;
//    while (i < size){
//        k = i;
//        while (int(dat[i].dia) == int(dat[i-1].dia)){
//            soma = soma + ( dat[i-1].temp - Spto (dat, i-1, b) )*( dat[i-1].temp - Spto (dat, i-1, b) );
//            i++;
//        }
//        i++;
//
//
//        if (soma > maxERROR){
//            j++;
//            cout << "ERRO\t" << j << " [" << dat[k].dia << ":"<< dat[i].dia << "]" << "\tSOMA = "<<soma<< endl;
//
//        }
//        soma = 0;
//    }
//    cout << "\n\nNumero de anomalias climaticas: " << j << endl;
//}

int main(int argc, char** argv) {

    FILE * arq;
    int size = 0;
    dados *dat  = (dados*) malloc (sizeof (dados));



    if ( arq = fopen ("temp.dat","r") ){

        while (!feof(arq)) {
            fscanf (arq, "%lf", &dat[size].dia);
            fscanf (arq, "%lf", &dat[size].temp);
            dat  = (dados*) realloc (dat  , (++size+1)*sizeof(dados));

        }

        // fim da leitua

        // Gerando matriz A

        int n = 4;

        double **A = new double*[n];
        for (int i = 0 ; i < n ; i++){
            A[i] = new double[n];
        }

        geraA (A, n, dat, size);

//        cout << endl;
//        for (int i = 0 ; i< n ; i++){
//            for (int j = 0 ; j < n ; j ++){
//                printf ("%.6lf ", A[i][j]);
//            }
//            cout << endl;
//        }

        // Gerando vetor b. ta zuado o somatorio do gerab
        double *b = new double[n];
        gerab (b, n, dat, size);

//        printf("\n"); printf("\n");
//        for (int i = 0; i < n; i++) {
//            printf("%.6f\t", b[i]);
//        }
//
//        cout << endl;

        gauss (n,A,b);



//        printf("\n"); printf("\n");
//        for (int i = 0; i < n; i++) {
//            printf("%.6f\t", b[i]);
//        }

        cout << "Funcao obtida (formato gnuPLOT)\n" ;
        cout << b[0] << " + " << b[1] << " * sin(2*pi*x/365) + " << b[2] << "*cos(2*pi*x/365) + " << b[3] << " * sin(2*pi*x) + " << b[4] << "*cos(2*pi*x)" << endl;
        cout << endl <<endl;


//        cout << "Erros encontrados" << endl;
//        erro (b ,dat, size);
    }
    else
        cout << "ERRO - arquivo nao encontrado...";

}

