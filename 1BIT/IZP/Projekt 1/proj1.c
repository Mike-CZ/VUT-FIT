/*
 * Soubor:  proj1.c
 * Datum:   2013/11/14
 * Autor:   Lukas Pelanek, xpelan03@stud.fit.vutbr.cz
 * Projekt: Pocitani slov, projekt c. 1 pro predmet IZP
 * Popis:   Program ve vstupnim textu spocita slova obsahujici uzivatelem specifikovany znak a jejich pocet vypise.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Napoveda */

void help(int argc,char *argv[])
{
    if (argc == 1) /* Pokud nejsou zadany zadne argumenty */
        {
        printf("Nezadal jste zadne argumenty. Pro vypsani napovedy spustte program s argumentem --help.\n");
        }
    else if (argc > 1 && strcmp("--help", argv[1]) ==0)
        {
        printf("Vitejte v napovede.\n\n"
               "Autor: Lukas Pelanek\n\n"
               "Program ve vstupnim textu spocita slova obsahujici uzivatelem specifikovany\n znak a jejich pocet vypise.\n"
               "Pouziti: ./proj1 --help\n"
               "         ./proj1 X [N] [-d]\n\n"
               "Argumenty musi byt zadany v danem poradi !\n\n"
               "Argumenty: --help vypise napovedu\n"
               "           X reprezentuje hledany znak. Specialni znak : symbolizuje libovolne\n cislo 0-9, znak ^ symbolizuje libovolne velke pismeno A-Z a znak . symbolizuje\n libovolny znak.\n"
               "           Volitelny argument N vyjadruje pozici hledaneho znaku.\n"
               "           Volitelny argument -d zapina ladici rezim programu. V ladicim rezimu program vypisuje kazde slovo na novy radek.\n\n\n");
        }

}
/*Funkce pro prevod argumentu na cislo */

int prevod(int argc, char *argv[])
{
    if(argc>2) /* Provede pouze, pokud jsou zadany alespon 2 argumenty. Pri zadani mene argumentu program padal */
{
    int cislo =0;
    int i;
    int test = argv[2][0];

    if (test >= '0' && test <= '9')
    {
  for (i = 0; argv[2][i] != '\0'; i++)
            {
        cislo = cislo * 10;
        cislo = cislo + argv[2][i] - '0';
            }
    return cislo;
    }
    else
    {
        return 0; /* Pokud argument neni cislo, navrati nulu, kterou pozdeji porovnavam */
    }
}
    else
    {
        return 0; /* Pokud argument neni zadan => navrati nulu */
    }
}


    void hledej(int argc, char *argv[])
    {
        int hledany_znak = argv[1][0];
        int pozice = 0;
        int znak = 0;
        int pomocne_pocitadlo = 0;
        int hledana_pozice = prevod(argc, argv);
        int pocet_znaku = 0;
        int oddelovac_pocitadlo = 0;
        int s_pozici = 0;
        int bez_pozice = 0;

       while ((znak = getchar()) != EOF)
        {
            if ((znak >=  'a' && znak <= 'z') || (znak>= 'A' && znak <='Z') || (znak>= '0' && znak<= '9') || (znak== '-') || (znak== '_' ))
            {
                if (((argc == 2) && (strcmp("-d", argv[1]) ==0)) || ((argc == 3) && (strcmp("-d", argv[2]) ==0)) || ((argc == 4) && (strcmp("-d", argv[3]) ==0)))    /* Podminka pro vypsani debug modu */
                {
                    if(pozice <=79)      /* Omezeni maximalni delky slova v debug modu na 80 znaku */
                    {
            putchar(znak);
                    }
                }
            }

        else
        {
            znak = '\n';                /* Pokud je zadan neplatny znak  ==> oddelovac */
        }
        if (znak == '\n')
            {
                oddelovac_pocitadlo++;   /* Pocita kolik neplatych znaku bylo zadano */
            }
        if (znak != '\n')
            {
                oddelovac_pocitadlo=0;      /* Pokud narazi na platny znak, vynuluje pocitadlo */
            }

        if (oddelovac_pocitadlo == 1 && pozice != 0) /* Vypise pouze jeden oddelovac a zaroven nesmi byt pozice 0, protoze by jinak tiskl prazdny radek pokud prvni znak je oddelovac */
            {
                if (((argc == 2) && (strcmp("-d", argv[1]) ==0)) || ((argc == 3) && (strcmp("-d", argv[2]) ==0)) || ((argc == 4) && (strcmp("-d", argv[3]) ==0)))
                {
                putchar(znak);
                }
            }


        if ((znak>=  'a' && znak <= 'z') || (znak>= 'A' && znak <='Z') || (znak>= '0' && znak<= '9') || (znak== '-') || (znak== '_' ))
        {
            if (hledany_znak == znak && hledana_pozice == 0)
            {
            pocet_znaku++;          /* Pocet vyskytu hledaneho znaku specifikovaneho argumentem  */
            }
            else
            {
            pozice++;              /* Pocita pozici vsech platnych znaku v danem slove */
            }
            if (pozice == hledana_pozice && hledany_znak ==znak)
            {                                                                           /* Hledani znaku specifikovaneho argumentem na specifikovane pozici  */
                s_pozici++;
            }
            if(hledany_znak == '.' && hledana_pozice ==0)
            {
                bez_pozice++;                                                           /* Hledani vsech platnych znaku */
            }
            if(hledany_znak == '.' && pozice == hledana_pozice)
            {
                s_pozici++;
            }
            if(hledany_znak == '^' && hledana_pozice==0)
            {
                if(znak>= 'A' && znak <='Z')                                            /* Hledani velkych pismen */
                {
                bez_pozice++;
                }
            }
            if(hledany_znak == '^' && pozice==hledana_pozice)
            {
                if(znak>= 'A' && znak <='Z')
                {
               s_pozici++;
                }
            }
            if(hledany_znak == ':' && hledana_pozice==0)
            {                                                                           /* Hledani cisel */
                if(znak>= '0' && znak <='9')
                {
                bez_pozice++;
                }
            }

                if(hledany_znak == ':' && pozice==hledana_pozice)
            {
                if(znak>= '0' && znak <='9')
                {
               s_pozici++;
                }
            }

        }

        /* Nulovani pocitadel a inkrementovani pomocneho pocitadla*/

        if (znak == '\n' && pocet_znaku>0)
        {
            pomocne_pocitadlo++;
            pocet_znaku=0;
        }
        if (znak == '\n')
        {
            pozice =0;
        }
        if (znak == '\n' && bez_pozice>0)
        {
            pomocne_pocitadlo++;
            bez_pozice=0;
        }

    }

    if ((argc == 2) && (strcmp("-d", argv[1]) ==0))
        {
            /* Pokud je zadan samotny argument -d, zadny pocet nevypisuje */
        }
        else
    {

        if (hledana_pozice ==0 )
    {
        printf("%d\n", pomocne_pocitadlo);          /* Pokud neni specifikovana pozice, vypise pocet slov obsahujici hledany znak */
    }
        if (hledana_pozice > 0)
        {
        printf("%d\n", s_pozici);                   /* Pokud je specifikovana pozice, vypise pocet znaku na dane pozici */
        }
    }

    }

int main(int argc, char *argv[])

{
       if (argc > 1 && strcmp("--help", argv[1]) !=0)
       {
        hledej(argc, argv);
       }

        help(argc, argv);

   return 0;
}
