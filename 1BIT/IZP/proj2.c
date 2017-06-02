/*
 * Soubor:  proj2.c
 * Datum:   2013/11/24
 * Autor:   Lukas Pelanek, xpelan03@stud.fit.vutbr.cz
 * Projekt: Iteracni vypocty, projekt c. 2 pro predmet IZP
 * Popis:   Program vypocita a vypise druhou odmocninu, arcus sinus nebo uhly obecneho trojuhelniku.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define PI 3.141592653589
#define eps 0.000000000000000001

/*Napoveda*/

void help(int argc, char *argv[])
{
    if(argc == 1)
    {
         printf("Nezadal jste zadne argumenty!\nSpustte program s argumentem --help pro vypsani napovedy.\n");
    }

    else if (argc ==2 && (strcmp("--help", argv[1]) == 0))
    {
        printf("Vitejte v napovede.\n\n"
               "Autor: Lukas Pelanek, xpelan03@stud.fit.vutbr.cz\n\n"
               "Program vypocita a vypise druhou odmocninu, arcus sinus\nnebo uhly obecneho trojuhelniku.\n\n"
               "Pouziti: ./proj2 --help\n"
               "         ./proj2 --sqrt X\n"
               "         ./proj2 --asin X\n"
               "         ./proj2 --triangle AX AY BX BY CX CY\n\n"
               "Pocet argumentu musi odpovidat vyse uvedenym pripadum pouziti!\n\n"
               "Argumenty: --help vypise napovedu\n"
               "           --sqrt vypocita a vypise druhou odmocninu z cisla X. Interval <0,inf)\n"
               "           --asin vypocita a vypise arcus sinus z cisla X. Interval <-1,1>\n"
               "           --triangle vypocita a vypise uhly trojuhelniku, kde AX AY BX BY\n"
               "           CX CY jsou souradnice jednotlivych bodu.\n\n");
    }
    else
    {
        printf("Nespravne zadane argumenty!\nSpustte program s argumentem --help pro vypsani napovedy.\n");     /* Zavola se pokud argumenty neodpovidaji, viz main */
    }
}
/*Funkce pro absolutni hodnotu*/

double my_abs(double value)
{
    if (value>=0)
    {
        return value;
    }

    return -value;   /*Pokud je value zaporne cislo, vrati cislo s opacnym znamenkem ---> kladne cislo */
}

/*Funkce pro odmocninu*/

double my_sqrt(double x)
{
    double another = 0;

    if(x==0)
    {
        another = 0;             /* Odmocnina z nuly = 0 */
    }

    else
    {
    double previous = 1.0;
    another = ((x/previous) + previous)/2;              /* Prvni vypocet mimo cyklus */

    while((my_abs(another - previous) >= eps))          /* Cyklus opakuji dokud rozdil mezi novym vypoctem a predchozim neni vetsi nebo roven eps */
    {
        previous = another;                             /* Do previous priradim prvni vypocet */
        another = ((x/previous) + previous)/2;
    }

    }

    return another;
}
/*Funkce pro arcus sinus */

double my_asin(double x)
{

double div_1 = 1.0;
double div_2 = 2.0;
double denominator_1 = 3.0;
double previous_multiply = 1.0;
double numerator_1 = x*x*x;
double divide = div_1/div_2;
double multiply = previous_multiply*divide;                                             /* Prvni vypocet mimo cyklus */
double previous = x;
double another = previous  + (multiply * (numerator_1/denominator_1));

while(my_abs(previous  - another) >= eps)                                               /* Cyklus se provadi dokud rozdil mezi predchozi a novou hodnotou neni vetsi nebo roven eps */
{
    previous  = another;                                                                /* Predchozi vypocet */
    numerator_1 = numerator_1*x*x;
    previous_multiply = multiply;
    denominator_1 = denominator_1+2;
    div_1 = div_1+2;                                                                    /* Uprava promennych, aby vypocet odpovidal Taylorove rade */
    div_2 = div_2+2;
    divide = div_1/div_2;
    multiply = previous_multiply*divide;
    another = previous  + (multiply * (numerator_1/denominator_1));
   if(x < (-0.9))
    {
        another = (-PI/2) + my_asin(my_sqrt(1-(x*x)));                                  /* Vypocet pro krajni hodnoty byl nepresny, proto vyuzivam vztahu pi/2 = arcsin 1 */
    }
     if(x > 0.9)
    {
        another = (PI/2) - my_asin(my_sqrt(1-(x*x)));
    }
}

return another;

}


/*Funkce pro vypocet uhlu trojuhelniku*/

double my_triangle(double ax, double ay, double bx, double by, double cx, double cy)
{

double a1 = bx - ax;
double a2 = by - ay;                                            /*delka strany c*/
double c = my_sqrt((a1*a1) + (a2*a2));

double b1 = cx - bx;
double b2 = cy - by;                                            /*delka strany a*/
double a = my_sqrt((b1*b1) + (b2*b2));

double c1 = cx - ax;
double c2 = cy - ay;                                            /*delka strany b*/
double b = my_sqrt((c1*c1) + (c2*c2));

if(ax == ay && bx == by && cx == cy)                            /* na merlinovi v urcitych pripadech trojuhelnikova nerovnost nefungovala a i pres to to pocitalo, proto pokud si jsou souradnice rovny --> nan */
{
printf("nan\nnan\nnan\n");
}

else if((my_abs(a + b) > my_abs(c)) && (my_abs(a + c) > my_abs(b)) && (my_abs(b + c) > my_abs(a)))
{
double angle_A= ((b*b) + (c*c) - (a*a)) / (2*b*c);
double angle_B= ((a*a) + (c*c) - (b*b)) / (2*a*c);              /* velikosti uhlu */
double angle_C= ((a*a) + (b*b) - (c*c)) / (2*a*b);

angle_A = (PI/2) - my_asin(angle_A);
angle_B = (PI/2) - my_asin(angle_B);                             /* arccos */
angle_C = (PI/2) - my_asin(angle_C);


printf("%.10e\n", angle_A);
printf("%.10e\n", angle_B);
printf("%.10e\n", angle_C);
}

else
{
    printf("nan\nnan\nnan\n");
}

return 0;
}



int main(int argc, char *argv[])
{
    char *check;                                                    /* Ukazatel pro strtod. Detekce, zda zadany vstup je platne cislo */

    if(argc==3 && strcmp("--asin", argv[1]) ==0)
    {
    double x = strtod(argv[2], &check);
        if(*check == 0)
        {
            if(x <=1 && x >=(-1))                                   /*Vstup musi byt v intervalu <-1, 1> */
            {
                printf("%.10e\n", my_asin(x));
            }
            else
            {
                printf("nan\n");
            }
        }
        else
        {
           printf("nan\n");
        }
    }


    else if(argc==3 && strcmp("--sqrt", argv[1]) ==0)
    {
    double x = strtod(argv[2], &check);

        if(*check == 0)
        {

            if(x<0)                                             /* Vstup nesmi byt mensi nez nula */
            {
                printf("nan\n");
            }
            else
            {
                printf("%.10e\n", my_sqrt(x));
            }
        }
        else
        {
            printf("nan\n");
        }
    }


    else if(argc==8 && strcmp("--triangle", argv[1]) ==0)
    {
      int test = 0;                                               /* Pomocne pocitadlo, kazdou souradnici overuji zvlast, pokud je nektera neplatna, pocitadlo se zvysi */
      double ax = strtod(argv[2], &check);
      (*check == 0) ? test=test : ++test;
      double ay = strtod(argv[3], &check);
      (*check == 0) ? test=test : ++test;
      double bx = strtod(argv[4], &check);
      (*check == 0) ? test=test : ++test;
      double by = strtod(argv[5], &check);
      (*check == 0) ? test=test : ++test;
      double cx = strtod(argv[6], &check);
      (*check == 0) ? test=test : ++test;
      double cy = strtod(argv[7], &check);
      (*check == 0) ? test=test : ++test;

        if(test == 0)                                           /* Pocita pouze v pripade, ze vsechny souradnice jsou platna cisla */
        {
        my_triangle(ax, ay, bx, by, cx, cy);
        }
        else
        {
           printf("nan\nnan\nnan\n");
        }

    }



    else
    {
    help(argc, argv);
    }



    return 0;
}
