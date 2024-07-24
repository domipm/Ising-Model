#include<cmath>
#include<iostream>
#include"gsl_rng.h"

#define SEED 1649176165

using namespace std;

gsl_rng *tau;

// ! ESTRATEGIA ALTERNATIVA: HACEMOS 20 PASOS MONTECARLO
// ! CADA PASO MONTECARLO SON 30*30 ITERACIONES
// ! ESCRIBIMOS LA MATRIZ NUEVA SOLO TRAS CADA PASO MONTECARLO
// ! NO ESCRIBIMOS LA MATRIZ EN CADA ITERACION

int dim = 30, iter = 200;
float T = 0.01;

//RANDOM INT GENERATOR (Numero -1 o 1 aleatoriamente)
int rnd_int() {
    float num = gsl_rng_uniform_int(tau, 2); //NUMERO ALEATORIO ENTRE (0 y 1)
    if (num == 0) return -1;
    else if (num == 1) return 1;    
    return 0;
}

int main() {

    //DEFINIMOS LA FUNCIÓN EXTERNA GSL_RNG Y ESTABLECEMOS LA SEMILLA (A partir de ahora, los numeros generados seguirán siempre los de la semilla dada)
    extern gsl_rng *tau;
    tau = gsl_rng_alloc(gsl_rng_taus);
    gsl_rng_set(tau, SEED);

    //ABRIMOS EL FICHERO CORRESPONDIENTE
    FILE *data;
    data = fopen("1649176165_30_0.01_200.txt", "w");

    //CREAMOS MATRIZ DIMENSIONES DIMxDIM (Spin)
    int s[dim][dim];

    // * C O N F I G U R A C I O N  I N I C I A L * //

    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            s[i][j] = rnd_int(); //VALOR MATRIZ PARA i,j in [0, dim-1)
            //PERIODICIDAD FILAS Y COLUMNAS
            s[dim-1][dim-1] = s[0][0];
            s[i][dim-1] = s[i][0];
            s[dim-1][j] = s[0][j];
            //GUARDAMOS VALORES EN FICHERO
            fprintf(data, "%d\t", s[i][j]);
        }
        if (i != dim-1) fprintf(data, "\n");
    }

    // * A L G O R I T M O * //

    //Hacemos ITER pasos montecarlo
    for (int k = 0; k < iter; k++) {
        //Bucle de cada paso montecarlo (DIMxDIM)
        for (int l = 0; l < dim*dim; l++) {

            int i = gsl_rng_uniform_int(tau, dim);
            int j = gsl_rng_uniform_int(tau, dim);

            int E = 2*s[i][j] * ( s[ ( dim + (i+1) % dim ) % dim ][j] + s[ ( dim + (i-1) % dim) % dim ][j] + s[i][ ( dim + (j+1) % dim) % dim ] + s[i][ ( dim + (j-1) % dim ) % dim ] );

            float p = min(float(1.0), exp(-E/T) );

            float r = gsl_rng_uniform(tau);
            if (r < p) {
                s[i][j] *= -1; //CAMBIAMOS EL VALOR
                //CONDICIONES PERIODICAS
                for (int i = 0; i < dim; i++) {
                    for (int j = 0; j < dim; j++) {
                        s[dim-1][dim-1] = s[0][0];
                        s[i][dim-1] = s[i][0];
                        s[dim-1][j] = s[0][j];
                    }
                }
            }

        }

        //TRAS CADA PASO MONTECARLO, ESCRIBIMOS LA MATRIZ NUEVA
        for (int i = 0; i < dim; i++) {
            fprintf(data, "\n");
            for (int j = 0; j < dim; j++) {
                fprintf(data, "%d\t", s[i][j]);
            }
            //fprintf(data, "\n");
        }

    }

    return 0;

}