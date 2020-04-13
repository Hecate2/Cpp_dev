//-------------------------------------------------------------------------------------------------------------
// Quine-McCluskey Algorithm
// =========================
//-------------------------------------------------------------------------------------------------------------
// English:
//-------------------------------------------------------------------------------------------------------------
// Description: Application to simplify boolean functions with Quine-McCluskey algorithm
// Date: 05/16/2012
// Author: Stefan Moebius (mail@stefanmoebius.de)
// Modified by Hong Xu (hong@topbug.net) to use as a library on 11/16/2014
// Licence: Can be used freely (Public Domain)
//-------------------------------------------------------------------------------------------------------------
// German:
//-------------------------------------------------------------------------------------------------------------
// Beschreibung: Programm zur Vereinfachung von Booleschen Funktionen mit hilfe des Quine�McCluskey Verfahrens.
// Datum: 16.05.2012
// Author: Stefan Moebius (mail@stefanmoebius.de)
// Modified by Hong Xu (hong@topbug.net) to use as a library
// Lizenz: darf frei verwendet werden (Public Domain)
//-------------------------------------------------------------------------------------------------------------
#include <iostream>
#include <sstream>
#include <vector>
#include <set>
using namespace std;
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>

// Adjust MAXVARS and MAX if you have more input to handle.
#define MAXVARS 7
#define MAX 2048
// #define MAXVARS 10
// #define MAX 12288L

//Global fields: / Globale Felder:
static int minterm[MAX][MAX];
static int mask[MAX][MAX];      // mask of minterm  /  Maske des Minterm
static int used[MAX][MAX];      // minterm used  /  Minterm wurde verwendet
static int result[MAX];     // results  /  Ergebnisse
static int primmask[MAX];       // mask for prime implicants  /  Maske f�r Primimplikant
static int prim[MAX];           // prime implicant  /  Primimplikant
static bool wprim[MAX];          // essential prime implicant (true/false)  /  wesentlicher Primimplikant (true/false)
static int nwprim[MAX];     // needed not essential prime implicant  /  ben�tigter unwesentlicher Primimplikant
static int out_size;

//Count all set bits of the integer number  /  Z�hlen der gesetzen Bits in einer Integerzahl
static int popCount(unsigned int x) { // Taken from book "Hackers Delight"  / Aus dem Buch "Hackers Delight"
    x = x - ((x >> 1) & 0x55555555);
    x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
    x = (x + (x >> 4)) & 0x0F0F0F0F;
    x = x + (x >> 8);
    x = x + (x >> 16);
    return x & 0x0000003F;
}

//Calculate hamming weight/distance of two integer numbers  /  Berechnung der Hammingdistanz von 2 Integerzahlen
static int hammingWeight(unsigned int v1, unsigned int v2) {
    return popCount(v1 ^ v2);
}

//Output upper part of term in console  /  Oberer Teil des Terms in der Konsole ausgeben
static void upperTerm(int bitfield, int mask, int num) {
    if (mask) {
        int z;
        for ( z = 0; z < num; z++) {
            if (mask & (1 << z)) {
                if (bitfield & (1 << z))
                    printf("_");
                else
                    printf(" ");
            }
        }
    }
}

//Output lower part of term in console  /  Unterer Teil des Terms in der Konsole ausgeben
static void lowerTerm(int mask, int num) {
    if (mask) {
        int z;
        for (z = 0; z < num; z++) {
            if (mask & (1 << z)) {
                printf("%c", 'z' - (num - 1) + z);
            }
        }
    }
}

static void writeOutput(int bitfield, int mask, int num, vector<int>::iterator out) {
    if (mask) {
        int z;
        for ( z = 0; z < num; z++) {
            if (mask & (1 << z)) {
                if (bitfield & (1 << z))
                    out[z] = 0;
                else
                    out[z] = 1;
            } else {
                out[z] = -1;
            }
        }
    }
}

//Output a term to console  /  Ausgabe eines Terms in der Konsole
static void outputTerm(int bitfield, int mask, int num) {
    upperTerm(bitfield, mask, num);
    if (mask) printf("\n");
    lowerTerm(mask, num);
}

//Determines whether "value" contains "part"  /  Bestimmt, ob "value" "part" beinhaltet
static int contains(int value, int mask, int part, int partmask) {
    if ((value & partmask) == (part & partmask)) {
        if ((mask & partmask) ==  partmask)
            return true;
    }
    return false;
}

// free memory alocated by qmc_simplify
void qmc_free(void * p)
{
    free(p);
}

// @num count of variables
// @out_size will point to the number clauses
// *out is in the form of DNF. (*out) is an array which has @num * @out_size elements.
// Each consecutive @num elements is a clause.
vector<int> qmc_simplify(const bool * truthtable, int num_of_variables/*, int ** out, int* out_size*/) {
    // int num = 0; // Number of Variables  /  Anzahl Eing�nge
    int pos = 0;
    int cur = 0;
    int reduction = 0; //reduction step  / Reduktionsschritt
    int maderedction = false;
    int prim_count = 0;
    int term = 0;
    int termmask = 0;
    int found = 0;
    int x = 0;
    int y = 0;
    int z = 0;
    int count = 0;
    int lastprim = 0;
    int res = 0; // actual result  /  Ist-Ausgabe
    int length_of_truthtable = 1 << num_of_variables;

    // return and set out_size to 0 if all is true or all is false
    for (x = 1; x < length_of_truthtable; ++ x)
    {
        if (truthtable[0] != truthtable[x])
            break;
    }

    if (x == length_of_truthtable) // All entries are equal, return now
    {
        out_size = 0;
        return vector<int>{};
    }

    // Fill all arrays with default values / Alle Arrays mit Standardwert auff�llen
    for (x = 0; x < MAX; x++) {
        primmask[x] = 0;
        prim[x] = 0;
        wprim[x] = false;
        //nwprim[x] = false;
        result[x] = false;
        nwprim[x] = true; //unwesentliche Primimplikaten als ben�tigt markieren
        for (y = 0; y < MAX; y++) {
            mask[x][y] = 0;
            minterm[x][y] = 0;
            used[x][y] = false;
        }
    }

    // printf("Enter number of variables: ");
    // scanf(" %d", &num_of_variables);
    if (num_of_variables > MAXVARS) {
        // fprintf(stderr, "ERROR: Number of variables too big!\n");
        return vector<int> {};
    }
    if (num_of_variables < 1) {
        // fprintf(stderr, "ERROR: Number of variables must be at least 1!\n");
        return vector<int> {};
    }

    pos = (1 << num_of_variables);  // 2 ^ num_of_variables
    // printf("Please enter desired results: ( 0 or 1)\n\n");

    cur = 0;
    for ( x=0; x < pos; x++) {
        // int value = 0;
        // outputTerm(x, pos - 1, num_of_variables);
        // printf(" = ");
        // scanf(" %d", &value);
        if (truthtable[~x & (pos - 1)]) {
            mask[cur][0] = ((1 << num_of_variables)- 1);
            minterm[cur][0] = x;
            cur++;
            result[x] = 1;
        }
        // printf("\n");
    }

    for (reduction = 0; reduction < MAX; reduction++) {
        cur = 0;
        maderedction = false;
        for (y=0; y < MAX; y++) {
            for (x=0; x < MAX; x++) {
                if ((mask[x][reduction]) && (mask[y][reduction])) {
                    if (popCount(mask[x][reduction]) > 1) { // Do not allow complete removal (problem if all terms are 1)  /  Komplette aufhebung nicht zulassen (sonst problem, wenn alle Terme = 1)
                        if ((hammingWeight(minterm[x][reduction] & mask[x][reduction], minterm[y][reduction] & mask[y][reduction]) == 1) && (mask[x][reduction] == mask[y][reduction])) { // Simplification only possible if 1 bit differs  /  Vereinfachung nur m�glich, wenn 1 anderst ist
                            term = minterm[x][reduction]; // could be mintern x or y /  egal ob mintern x oder minterm y
                            //e.g.:
                            //1110
                            //1111
                            //Should result in mask of 1110  /  Soll Maske von 1110 ergeben
                            termmask = mask[x][reduction]  ^ (minterm[x][reduction] ^ minterm[y][reduction]);
                            term  &= termmask;

                            found = false;
                            for ( z=0; z<cur; z++) {
                                if ((minterm[z][reduction+1] == term) && (mask[z][reduction+1] == termmask) ) {
                                    found = true;
                                }
                            }

                            if (found == false) {
                                minterm[cur][reduction+1] = term;
                                mask[cur][reduction+1] = termmask;
                                cur++;
                            }
                            used[x][reduction] = true;
                            used[y][reduction] = true;
                            maderedction = true;
                        }
                    }
                }
            }
        }
        if (maderedction == false)
            break; //exit loop early (speed optimisation)  /  Vorzeitig abbrechen (Geschwindigkeitsoptimierung)
    }

    prim_count = 0;
    //printf("\nprime implicants:\n");
    for ( reduction = 0 ; reduction < MAX; reduction++) {
        for ( x=0 ;x < MAX; x++) {
            //Determine all not used minterms  /  Alle nicht verwendeten Minterme bestimmen
            if ((used[x][reduction] == false) && (mask[x][reduction]) ) {
                //Check if the same prime implicant is already in the list  /  �berpr�fen, ob gleicher Primimplikant bereits in der Liste
                found = false;
                for ( z=0; z < prim_count; z++) {
                    if (((prim[z] & primmask[z]) == (minterm[x][reduction] & mask[x][reduction])) &&  (primmask[z] == mask[x][reduction]) )
                        found = true;
                }
                if (found == false) {
                    //outputTerm(minterm[x][reduction], mask[x][reduction], num_of_variables);
                    //printf("\n");
                    primmask[prim_count] = mask[x][reduction];
                    prim[prim_count] = minterm[x][reduction];
                    prim_count++;
                }
            }
        }
    }

    //find essential and not essential prime implicants  /  wesentliche und unwesentliche Primimplikanten finden
    //all alle prime implicants are set to "not essential" so far  /  Primimplikanten sind bisher auf "nicht wesentlich" gesetzt
    for (y=0; y < pos; y++) { //for all minterms  /  alle Minterme durchgehen
        count = 0;
        lastprim = 0;
        if (mask[y][0]) {
            for (x=0; x < prim_count; x++ ) { //for all prime implicants  /  alle Primimplikanten durchgehen
                if (primmask[x]) {
                    // Check if the minterm contains prime implicant  /  the �berpr�fen, ob der Minterm den Primimplikanten beinhaltet
                    if (contains(minterm[y][0], mask[y][0], prim[x], primmask[x])) {
                        count++;
                        lastprim = x;
                    }
                }
            }
            // If count = 1 then it is a essential prime implicant /  Wenn Anzahl = 1, dann wesentlicher Primimplikant
            if (count == 1) {
                wprim[lastprim] = true;
            }
        }
    }

    // successively testing if it is possible to remove prime implicants from the rest matrix  /  Nacheinander testen, ob es m�gich ist, Primimplikaten der Restmatrix zu entfernen
    for ( z=0; z < prim_count; z++) {
        if (primmask[z] ) {
            if (wprim[z] == false) { // && (rwprim[z] == true))
                nwprim[z] = false; // mark as "not essential" /  als "nicht ben�tigt" markiert
                for ( y=0; y < pos; y++) { // for all possibilities  /  alle M�glichkeiten durchgehen
                    res = 0;
                    for ( x=0; x < prim_count; x++) {
                        if ( (wprim[x] == true) || (nwprim[x] == true)) {  //essential prime implicant or marked as required  /  wesentlicher Primimplikant oder als ben�tigt markiert
                            if ((y & primmask[x]) == (prim[x] & primmask[x])) { //All bits must be 1  /  Es m�ssen alle Bits auf einmal auf 1 sein (da And-Verkn�pfung)
                                res = 1;
                                break;
                            }
                        }
                    }
                    //printf(" %d\t%d\n", result, result[y]);
                    if (res == result[y]) {  // compare calculated result with input value /  Berechnetes ergebnis mit sollwert vergleichen
                        //printf("not needed\n"); //prime implicant not required  /  Primimplikant wird nicht ben�tigt
                    }
                    else {
                        //printf("needed\n");
                        nwprim[z] = true; //prime implicant required  /  Primimplikant wird doch ben�tigt
                    }
                }
            }
        }
    }
    // printf("\nResult:\n\n");

    // *out = (int *) calloc(sizeof(int), prim_count * num_of_variables);
    vector<int> out(prim_count * num_of_variables, 0);
    vector<int>::iterator out_begin = out.begin();

    // Output of essential and required prime implicants / Ausgabe der wesentlichen und restlichen ben�tigten Primimplikanten:
    count = 0;
    for ( x = 0 ; x < prim_count; x++) {
        if (wprim[x] == true) {
            // if (count > 0) printf("   ");
            writeOutput(prim[x], primmask[x], num_of_variables, out_begin + x * num_of_variables);
            count++;
        }
        else if ((wprim[x] == false) && (nwprim[x] == true)) {
            // if (count > 0) printf("   ");
            writeOutput(prim[x], primmask[x], num_of_variables, out_begin + x * num_of_variables);
            count++;
        }
    }

    out_size = prim_count;

#if 0
    // Output of essential and required prime implicants / Ausgabe der wesentlichen und restlichen ben�tigten Primimplikanten:
    count = 0;
    for ( x = 0 ; x < prim_count; x++) {
        if (wprim[x] == true) {
            // if (count > 0) printf("   ");
            upperTerm(prim[x], primmask[x], num_of_variables);
            count++;
        }
        else if ((wprim[x] == false) && (nwprim[x] == true)) {
            // if (count > 0) printf("   ");
            upperTerm(prim[x], primmask[x], num_of_variables);
            count++;
        }
    }
    // printf("\n");
    count = 0;
    for ( x = 0 ; x < prim_count; x++) {
        if (wprim[x] == true) {
            // if (count > 0) printf(" + ");
            lowerTerm(primmask[x], num_of_variables);
            count++;
        }
        else if ((wprim[x] == false) && (nwprim[x] == true)) {
            // if (count > 0) printf(" + ");
            lowerTerm(primmask[x], num_of_variables);
            count++;
        }
    }
    printf("\n");
#endif
    return out;
}

std::string truthtable_to_expr(const std::string& truth_table){
    int s = truth_table.size();
    int num_of_variables = int(log(s)/log(2)+0.5);
    bool truthtable[s];
    // int* out;
    //int out_size;
    int i, j;

    /* the truth truthtable -- 3 inputs 1 output
     * 
     * C  B  A  o
     * -------
     * 0  0  0  1
     * 0  0  1  1
     * 0  1  0  1
     * 0  1  1  1
     * 1  0  0  1
     * 1  0  1  1
     * 1  1  0  0
     * 1  1  1  0
     */

    // truthtable[0] = true;
    // truthtable[1] = true;
    // truthtable[2] = true;
    // truthtable[3] = true;
    // truthtable[4] = true;
    // truthtable[5] = true;
    // truthtable[6] = false;
    // truthtable[7] = false;

    for(i=0; i<s; ++i){
        if(truth_table[i]=='1'){truthtable[i]=true;}
        else if(truth_table[i]=='0'){truthtable[i]=false;}
        else{
            //TODO:exceptions
        }
    }

    /* simplify the truthtable */
    vector<int> out = qmc_simplify(truthtable, num_of_variables);

    /* print the output */
    for (i = 0; i < out_size; ++ i)
    {
        if (i > 0)
            printf(" + ");

        for (j = 0; j < num_of_variables; ++ j)
        {
            switch (out[i * num_of_variables + j])
            {
            case 0:    /* the literal is false */
                printf("~");
            case 1:    /* the literal is true */
                printf("%c", 'A'+j);
                break;
            }
        }
    }

    if (out_size == 0) /* All truthtable entries are true or false */
        if (truthtable[0])
            printf("true");
        else
            printf("false");

    printf("\n");

    // qmc_free(out);

    return "0";
}

int main(){
    printf("输入真值表字符串，输出逻辑表达式字符串\n\n");
    printf("示例：01 -> A\n");
    int effective_operands;
    string truthtable;
    while(1){
        printf(">>> ");
        cin >> truthtable;
        while(cin.fail()) {
            cin.clear();
            cin.ignore();
            printf("输入错误。请输入一个正整数和一个逻辑表达式字符串。\n正整数表示逻辑表达式字符串中不同变量的个数\n");
            cin >> truthtable;
        }
        truthtable_to_expr(truthtable);
    }
    return 0;
}