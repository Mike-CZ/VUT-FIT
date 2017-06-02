/*
 * Soubor:  proj3.c
 * Datum:   2013/12/10
 * Autor:   Lukas Pelanek, xpelan03@stud.fit.vutbr.cz
 * Projekt: Hledani obrazcu, projekt c. 3 pro predmet IZP
 * Popis:   Program v danem monochromatickem obrazku hleda nejdelsi vodorovne a svisle usecky a nejvetsi ctverce.
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

/* Napoveda */

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
               "Program v danem monochromatickem obrazku hleda nejdelsi vodorovne\na sivsle usecky a nejvetsi ctverce.\n"
               "Obrazek je ulozen jako bitmapa v textovem souboru ve forme\nobdelnikove matice nul a jednicek.\n"
               "Program vypise pocatecni a koncove souradnice nalezene usecky nebo ctverce \n\n"
               "Pouziti: ./proj2 --help\n"
               "         ./proj2 --test 'nazev_souboru.txt'\n"
               "         ./proj2 --vline 'nazev_souboru.txt'\n"
               "         ./proj2 --hline 'nazev_souboru.txt'\n"
               "         ./proj2 --square 'nazev_souboru.txt'\n\n"
               "Pocet argumentu musi odpovidat vyse uvedenym pripadum pouziti!\n\n"
               "Argumenty: --help vypise napovedu\n"
               "           --test otestuje, zda dany soubor obsahuje platnou bitmapu\n"
               "           --vline najde a vypise souradnice prvni nejdelsi vertikalni usecky\n"
               "           --hline najde a vypise souradnice prvni nejdelsi horizontalni usecky\n"
               "           --square najde a vypise souradnice prvniho nejvetsiho ctverce\n");
    }
    else
    {
        printf("Nespravne zadane argumenty!\nSpustte program s argumentem --help pro vypsani napovedy.\n");     /* Zavola se pokud argumenty neodpovidaji, viz main */
    }
}
typedef struct bitmap
{
  int rows;
  int cols;
  char *cells;
}Bitmap;

Bitmap my_bitmap;                       /* Globalni promennou jsem pouzil proto, ze hodnoty jednou zapisi a uz je nemenim, ve funkcich se pote k temto hodnotam snadneji pristupuje */

/* Funkce pro overeni validity bitmapy */

int isbitmap(char *argv[])
{
int char_ = 0;
int counter = 0;
int test = 0;
FILE *file;
file = fopen(argv[2], "r");

if ((file = fopen(argv[2],"r")) == NULL)
{                                                               /* Test, zda dany soubor existuje */
    return -1;
}

fscanf(file, "%d %d", &my_bitmap.rows, &my_bitmap.cols);


while(1)
{
    test = fscanf(file, "%d", &char_);

    if(test == -1)
    {                                                           /* Konec souboru == konec cyklu */
        break;
    }

    if(char_ == 1 || char_ == 0)                                /* Pokud je to platny znak, navysi se pocitadlo */
    {
        counter++;
    }
    else
    {
        fclose(file);
        return -1;                                              /* Neplatny znak -> chyba */
    }
    if(test == 0)
    {
        fclose(file);                                           /* Pokud fscanf neprecte zadnou hodnotu -> chyba */
        return -1;
    }
}
fclose(file);


if ((my_bitmap.rows * my_bitmap.cols) == counter && counter != 0)
{
    return 0;      /* V pripade platne bitmapy vrati 0 */
}

return -1;
}

/*Funkce pro nacitani bitmapy do pole */

void getbitmap(char *argv[])
{
my_bitmap.cells = malloc(my_bitmap.rows * my_bitmap.cols);    /* Alokace pameti */

FILE *file;
file = fopen(argv[2], "r");
int char_ = 0;
int index = 0;

while (index != my_bitmap.rows * my_bitmap.cols)      /* Cyklus probiha do te doby, dokud se pocitadlo nerovna poctu prvku v matici */
{
    fscanf(file, "%d", &char_);
    if(char_ == 0 || char_ == 1)
    {
        my_bitmap.cells[index] = char_;     /* Pokud je to platne cislo, ulozim ho do pole */
        ++index;
    }
}
fclose(file);
}

/* Funkce pro zjisteni "barvy" */

char getcolor(Bitmap *bitmap, int x, int y)
{
    return bitmap->cells[(bitmap->cols * x) + y];      /* Prevzato z demonstracniho cviceni */
}

/* Funkce pro hledani vertikalni usecky */

int find_vline(Bitmap *bitmap, int *x1, int *y1, int *x2, int *y2)
{
    int inc_coords = 0;
    int length = 0;
    int old_length = 0;
    int a1 = 0;
    int b1 = 0;
    int a2 = 0;
    int b2 = 0;

        for(int j = 0; j < my_bitmap.rows; ++j)         /* projizdim po radcich */
        {
         for(int k = 0; k < my_bitmap.cols; ++k)
         {
             inc_coords = j;
             while(getcolor(bitmap, inc_coords, k) == 1)          /* Pokud najdu platny znak, divam se smerem dolu a hledam nejdelsi usecku */
             {
             ++inc_coords;
             ++length;
             }

            a1 =(length>old_length)? j : a1;
            b1 =(length>old_length)? k  : b1;                           /* Pokud nalezena usecka je delsi nez predchozi, ulozim nove souradnice */
            a2 =(length>old_length)? j + length - 1 : a2;
            b2 =(length>old_length)? k : b2;
            old_length = (length>old_length)? length : old_length;
            length = 0;
         }
        }

free(my_bitmap.cells);

if(a1 == a2 && b1 == b2)
{
    return 0;
}

*x1 = a1;
*y1 = b1;       /* Predani souradnic pomoci ukazatele */
*x2 = a2;
*y2 = b2;

if(a1 > 0 || b1 > 0 || a2 >0 || b2 > 0)
{
    return 1;                               /* Pokud byla nalezena usecka, funkce vrati 1 */
}
return 0;
}

/* Funkce pro hledani horizontalni usecky */

int find_hline(Bitmap *bitmap, int *x1, int *y1, int *x2, int *y2)   /* Tato funkce je implementovana jinak, protoze jsem zjistil, ze hledani vertikalni usecky bylo implementovano spatne, tak jsem ji musel predelat.*/
{                                                                    /* Po zkusenostech se ctvercem jsem predchozi funkci navrhl odlisne (efektivneji?). Jelikoz tato funkce funguje spravne, jiz jsem ji nepredelaval */

    int length = 0;
    int coords_1 = 0;
    int coords_2 = 0;
    int previous_length = 0;
    int a1 = 0;
    int b1 = 0;
    int a2 = 0;
    int c1 = 0;
    int d1 = 0;
    int c2 = 0;
    int d2 = 0;
    int test = 0;
    int previous = 0;


        for(int j = 0; j < my_bitmap.rows * my_bitmap.cols; ++j)
        {
            ++coords_2;
          if(getcolor(bitmap, coords_1, coords_2 -1) == 1)       /* Matici projizdim po radcich */
          {
            ++length;
          }
          else
          {
              previous_length = length;
              length = 0;
          }

          a1 = (length > previous_length)? coords_2 - length : a1;
          a2 = (length > previous_length)? coords_2 - 1: a2;
          b1 = (length > previous_length)? coords_1 : b1;
          test = (test < a2 - a1 + 1)? a2 - a1 + 1 : test;              /* Porovnavani vzdalenosti nalezenych usecek */
          c1 = (previous < a2 - a1 + 1)? a1 : c1;
          c2 = (previous < a2 - a1 + 1)? a2 : c2;
          d1 = (previous < a2 - a1 + 1)? b1 : d1;
          d2 = d1;
          previous = test;

        if(coords_2 == my_bitmap.cols)                          /* Kdyz dosahnu konce radku */
        {
          length = 0;
          coords_2 = 0;
          a1 =0;
          a2 =0;                                            /* Nulovani pocitadel pro dalsi radek */
          previous_length = 0;
          coords_1++;                                       /* Inkrementace souradnice pro posunuti na dalsi radek */
        }
        }
 free(my_bitmap.cells);



*x1 = d1;
*y1 = c1;
*x2 = d2;
*y2 = c2;

 if(c1 > 0 || d1 > 0 || c2 >0 || d2 > 0)
{
    return 1;                               /* Pokud byla nalezena usecka, funkce vrati 1 */
}
return 0;
}

/* Funkce pro hledani ctverce */

int find_square(Bitmap *bitmap, int *x1, int *y1, int *x2, int *y2)
{
    int coords_1 = 0;
    int coords_2 = 0;
    int coords_3 = 0;
    int coords_4 = 0;
    int a1 = 0;
    int a2 = 0;
    int d1 = 0;
    int d2 = 0;
    int length = 0;
    int length_2 = 0;
    int length_3 = 0;
    int old_length = 0;
    int side_1 = 0;
    int side_2 = 0;

    for(coords_3 = 0; coords_3 < my_bitmap.rows - 1; ++coords_3)       /* Projizdim po radcich */
    {

     for(coords_4 = 0; coords_4 < my_bitmap.cols - 1 ; ++coords_4)     /* -1 protoze nemusim kontrolovat posledni radek ani sloupec, ctverec se tam nemuze vyskytnout */
     {
         coords_1 = coords_3;
         coords_2 = coords_4;

         while(getcolor(bitmap, coords_1, coords_2) == 1)
         {
             ++coords_1;                                                /* Pokud narazim na platny znak, pocitam delku strany smerem dolu */
             ++length ;

         }

       while( length  > 1)
        {

         while(getcolor(bitmap, coords_3+length - 1, length_2 + coords_4) == 1)
         {

            length_2++;                                                                 /* Dokud je delka strany vetsi nez 1, jdu doprava a merim delku druhe strany */
         }


        if(length  <= length_2)                                 /* Pokud je delka druhe strany vetsi nebo rovna prvni, musim overit dalsi dve strany */
        {


        for(int u = 0; u < length; u++)                          /* cyklus se provede tolikrat, jak dlouha je prvni strana */
        {
         if(getcolor(bitmap, coords_3, coords_4 + u) == 1)
         {

             length_3++;                                  /* Delka treti strany */

         }
         else
         {
             length_3 = 0;                                  /* Pokud narazim na neplatny znak, konec cyklu a vynulovani pocitadla */
             break;
         }

        }

        side_1 = (length_3 == length )? side_1 + 1 : side_1;   /* Pokud delka strany odpovida delce strany prvni, zvedne se hodnota o 1 */

        length_3 = 0;

   for(int u = 0; u < length ; u++)
        {
         if(getcolor(bitmap, coords_3+u, coords_4+length - 1) == 1)
         {

             length_3++;                                 /* Delka ctvrte strany */

         }
         else
        {
            length_3 = 0;                                   /* Pokud narazim na neplatny znak, konec cyklu a vynulovani pocitadla */
            break;
        }

        }
        side_2 = (length_3 == length )? side_2 + 1 : side_2;  /* Pokud delka strany odpovida delce strany prvni, zvedne se hodnota o 1 */

            if(side_2 == 1 && side_1 == 1)
            {

            a1 = (length_3 > old_length)? coords_3 : a1;
            a2 = (length_3 > old_length)? coords_4 : a2;              /* Pokud byly nalezeny zbyvajici dve strany, porovnam delku stran a pokud je delsi nez predchozi, ulozim nove souradnice */
            d1 = (length_3 > old_length)? a1 + length_3 - 1 : d1;
            d2 = (length_3 > old_length)? a2 + length_3 - 1: d2;
            old_length = (length_3 > old_length)? length_3 : old_length;
            }
            side_2=0;
            side_1=0;
            length_3 = 0;

        }

            length_2=0;
            --length ;              /* Dekrementace delky strany, abych mohl zjistit, zda se v danem sloupci nenachazi mensi ctverec */
        }

length  = 0;
length_2 = 0;
coords_1 = 0;
coords_2 = 0;
    }
}
 free(my_bitmap.cells);

*x1 = a1;
*y1 = a2;       /* Predani souradnic pomoci ukazatele */
*x2 = d1;
*y2 = d2;

 if(a1 > 0 || a2 > 0 || d1 >0 || d2 > 0)
{
    return 1;           /* Pokud byl nalezen ctverec, funkce vrati 1 */
}
return 0;
}

int main(int argc,char *argv[])
{
int x1=0;
int y1=0;
int x2=0;
int y2=0;

if (argc == 3 && (strcmp("--test", argv[1]) == 0))
{

if(isbitmap(argv) == 0)
{
    printf("Valid\n");                          /* Validace bitmapy */
}
else
{
    printf("Invalid\n");
}
}

 else if (argc == 3 && (strcmp("--hline", argv[1]) == 0))
{
    if(isbitmap(argv) == 0)
    {
getbitmap(argv);
if(find_hline(&my_bitmap, &x1, &y1, &x2, &y2) == 1)         /* Pokud funkce navratila 1, vypise souradnice */
{
printf("%d %d %d %d\n", x1, y1, x2, y2);
}
else
{
    printf("Nenalezena zadna usecka.\n");                   /* Pokud funkce nevratila 1, vypise hlaseni */
}
    }
    else
    {
        printf("Invalid\n");                                /* Pokud matice neni platna, vypise hlaseni */
    }
}

else if (argc == 3 && (strcmp("--vline", argv[1]) == 0))
{
    if(isbitmap(argv) == 0)
    {
getbitmap(argv);
if(find_vline(&my_bitmap, &x1, &y1, &x2, &y2) == 1)         /* Pokud funkce navratila 1, vypise souradnice */
{
printf("%d %d %d %d\n", x1, y1, x2, y2);
}
else
{
    printf("Nenalezena zadna usecka.\n");               /* Pokud funkce nevratila 1, vypise hlaseni */
}
    }
    else
    {
        printf("Invalid\n");                                /* Pokud matice neni platna, vypise hlaseni */
    }
}

 else if (argc == 3 && (strcmp("--square", argv[1]) == 0))
{
    if(isbitmap(argv) == 0)
    {
getbitmap(argv);
if(find_square(&my_bitmap, &x1, &y1, &x2, &y2) == 1)        /* Pokud funkce navratila 1, vypise souradnice */
{
printf("%d %d %d %d\n", x1, y1, x2, y2);
}
else
{
    printf("Nenalezen zadny ctverec.\n");                   /* Pokud funkce nevratila 1, vypise hlaseni */
}
    }
    else
    {
        printf("Invalid\n");                                /* Pokud matice neni platna, vypise hlaseni */
    }
}
else
{
    help(argc, argv);
}

return 0;
}
