/* 
 * Learning of single neuron
 * File:   main.c
 * Author: metatron
 *
 */

/*
 * Include liblaries
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
 * Quasi static pre defined wariables
 */
static const int RANGE_MIN = 0;
static const int RANGE_MAX = 1;
static const int STATES_AMOUNT = 4;

/*
 * Initialize pre defined variables
 */
//tables of truth
int truthTables[4][4] =
{
    { 0, 0, 0, 1 }, /* and */
    { 0, 1, 1, 1 }, /* or */
    { 1, 1, 1, 0 }, /* nand */
    { 0, 1, 1, 0 }  /* xor */
};

//logical getways states array
int statesArray[4][2] = 
{
        {0, 0},
        {0, 1},
        {1, 0},
        {1, 1}
};

//selection, eras amoint, error report freqency, results array size
int selectedTable, erasAmount, reportFreq,  resultsArraySize = 0;

//bias level, eta value, threshold 
float bias, eta, threshold = 0;

/*
 * Pre declared headers of used functions
 */
//main teaching function
void teachNeuron(int selection, int truthTable[4][4], int statesArray[4][2], int erasAmount, int reportFreq, float bias, float eta, float threshold, int statesAmount) ;
//define bias function
void biasDef(float *bias, int min, int max) ;
//define eta function
void etaDef(float *eta) ;
//threshold define function
void thresholdDef(float *threshold, int min, int max);
//generate random float weight
float randWeight(int min, int max);


/*
 * Main function initing all others
 */
int main(int argc, char** argv) {
    srand(time(NULL));
   
    //select truth table for further processing
    printf("Wybierz funkcje logiczna dla uczenia neuronu\n");
    printf("1. AND\n2. OR\n3. NAND\n4. XOR\n");
    scanf( "%d", &selectedTable );    
    
    //define amount of learning eras
    printf("Zdefiniuj ilosc epok nauczania\n");
    printf("Zalecana ilosc epok to > 1000\n");
    scanf("%d", &erasAmount);
    
    //define error amount frequency
    printf("Podaj co ile epok raportowac wielkosc bledu\n");
    scanf("%d", &reportFreq);
        
    //define bias level    
    biasDef(&bias, RANGE_MIN, RANGE_MAX);
    
    //define threshold    
    thresholdDef(&threshold, RANGE_MIN, RANGE_MAX);
    
    //define eta value   
    etaDef(&eta);
    
    //init teaching
    teachNeuron(selectedTable, truthTables, statesArray, erasAmount, reportFreq, bias, eta, threshold, STATES_AMOUNT);       
    
    return (EXIT_SUCCESS);
}


/*
 * Additional functions
 */
//main teaching function
void teachNeuron(int selection, int truthTable[4][4], int statesArray[4][2], int erasAmount, int reportFreq, float bias, float eta, float threshold, int statesAmount) {
    /**
     * variables in function definition
     */
    int i, j, wartoscOczekiwana, x0, x1 = 0, min = -2, max = 2;
    float w0, w1, w2, dw0, dw1, dw2, max_err, min_err, adder, blad = 0.0;
    
    //proper selection value
    --selection;
        
    //set first random weight
    w0 = randWeight(min,max);
    w1 = randWeight(min,max);
    w2 = randWeight(min,max);
    
    //diagnostic info
    printf("Wylosowane wagi poczatkowe to: w0 = %2.2f, w1 = %2.2f, w2 = %2.2f\n", w0, w1, w2);    
    
    //learning process loops
    //eras count
    for (; i < erasAmount; i++) {        
        //steps count
        for(; j < statesAmount; j++) {
            adder = 0;
            w0 = w0 + dw0;
            w1 = w1 + dw1;
            w2 = w2 + dw2;
            x0 = statesArray[j][0];
            x1 = statesArray[j][1];
            wartoscOczekiwana = truthTable[selection][j];
            adder = w0*bias+w1*x0+w2*x1;
            blad =  wartoscOczekiwana - adder;
            dw0 = eta * blad * bias;
            dw1 = eta * blad * x0;
            dw2 = eta * blad * x1;
            
            //check max and min error
            if(blad > max_err) {
                max_err = blad;
            }
            if(blad < min_err) {
                min_err = blad;
            }
            
            //print error info
            if(i%reportFreq == 0) {
                printf("Zarejestrowany blad dla ostatnich %d epok to: %f\n", reportFreq, max_err-min_err);
                min_err = 0;
                max_err = 0;
            }
            
        }        
        j = 0;
    }
    
    //diagnostic info
    printf("Dobrane wagi koncowe to: w0 = %2.2f, w1 = %2.2f, w2 = %2.2f\n", w0, w1, w2);
    
//    return returnArray;
        
}

//bias define function
void biasDef(float *bias, int min, int max) {
    printf("Podaj wartosc bias:\n");
    printf("Wartosc bias powinna byc z przedzialu %d - %d\n", min, max);
    
        do {
            scanf("%f", &*bias);        
            if(*bias > (float)max || *bias < (float)min) {
                printf("\nPodales liczbe spoza zakresu\n");
            }
        } while(*bias > (float)max || *bias < (float)min);
}

//eta define function
void etaDef(float *eta) {
    printf("Podaj wartosc wspolczynnika eta\n");    
    scanf("%f", &*eta);

    if(*eta > 3) {
        printf("Podales niebezpiecznie wysoki wspolczynnik zmian\n");
    } 
    
    if(*eta <= 0 ) {
        printf("Przy podanych wartosciach wspolczynnika zmian nie nastapia zadne zmiany\n");
    }
    
}

//threshold define function
void thresholdDef(float *threshold, int min, int max) {
    printf("Podaj wartosc progu\n");    
    printf("Wartosc progu powinna byc z przedzialu %d - %d\n", min, max);
    scanf("%f", &*threshold);

    if(*threshold > (float)max || *threshold < (float)min) {
        printf("Wartos progu moze byc nieprawidlowa\n");
    } 
    
}

//generate random float weight
float randWeight(int min, int max) {     
            float result = (rand())/ ((double)RAND_MAX + 1)*(max - min) + min;
            do {
                if(result > min && result < max)
                    break;
                
                if(result > max)
                    result = result/rand();
                
                if(result < min)
                    result = result+max/rand();
                
            } while (result > max || result < min);
            
            return result;
}