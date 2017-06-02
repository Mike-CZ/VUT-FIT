// Lukas Pelanek
// xpelan03

/* c201.c *********************************************************************}
{* Téma: Jednosmìrný lineární seznam
**
**                     Návrh a referenèní implementace: Petr Pøikryl, øíjen 1994
**                                          Úpravy: Andrea Nìmcová listopad 1996
**                                                   Petr Pøikryl, listopad 1997
**                                Pøepracované zadání: Petr Pøikryl, bøezen 1998
**                                  Pøepis do jazyka C: Martin Tuèek, øíjen 2004
**	                                      Úpravy: Bohuslav Køena, øíjen 2014
**
** Implementujte abstraktní datový typ jednosmìrný lineární seznam.
** U¾iteèným obsahem prvku seznamu je celé èíslo typu int.
** Seznam bude jako datová abstrakce reprezentován promìnnou typu tList.
** Definici konstant a typù naleznete v hlavièkovém souboru c201.h.
** 
** Va¹ím úkolem je implementovat následující operace, které spolu s vý¹e
** uvedenou datovou èástí abstrakce tvoøí abstraktní datový typ tList:
**
**      InitList ...... inicializace seznamu pøed prvním pou¾itím,
**      DisposeList ... zru¹ení v¹ech prvkù seznamu,
**      InsertFirst ... vlo¾ení prvku na zaèátek seznamu,
**      First ......... nastavení aktivity na první prvek,
**      CopyFirst ..... vrací hodnotu prvního prvku,
**      DeleteFirst ... zru¹í první prvek seznamu,
**      PostDelete .... ru¹í prvek za aktivním prvkem,
**      PostInsert .... vlo¾í nový prvek za aktivní prvek seznamu,
**      Copy .......... vrací hodnotu aktivního prvku,
**      Actualize ..... pøepí¹e obsah aktivního prvku novou hodnotou,
**      Succ .......... posune aktivitu na dal¹í prvek seznamu,
**      Active ........ zji¹»uje aktivitu seznamu.
**
** Pøi implementaci funkcí nevolejte ¾ádnou z funkcí implementovaných v rámci
** tohoto pøíkladu, není-li u dané funkce explicitnì uvedeno nìco jiného.
**
** Nemusíte o¹etøovat situaci, kdy místo legálního ukazatele na seznam 
** pøedá nìkdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodnì komentujte!
**
** Terminologická poznámka: Jazyk C nepou¾ívá pojem procedura.
** Proto zde pou¾íváme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**/

#include "c201.h"

int solved;
int errflg;

void Error() {
/*
** Vytiskne upozornìní na to, ¾e do¹lo k chybì.
** Tato funkce bude volána z nìkterých dále implementovaných operací.
**/	
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;                      /* globální promìnná -- pøíznak chyby */
}

void InitList (tList *L) {
/*
** Provede inicializaci seznamu L pøed jeho prvním pou¾itím (tzn. ¾ádná
** z následujících funkcí nebude volána nad neinicializovaným seznamem).
** Tato inicializace se nikdy nebude provádìt nad ji¾ inicializovaným
** seznamem, a proto tuto mo¾nost neo¹etøujte. V¾dy pøedpokládejte,
** ¾e neinicializované promìnné mají nedefinovanou hodnotu.
**/

	L->First = NULL;				//inicializace
	L->Act = NULL;

}

void DisposeList (tList *L) {
/*
** Zru¹í v¹echny prvky seznamu L a uvede seznam L do stavu, v jakém se nacházel
** po inicializaci. Ve¹kerá pamì» pou¾ívaná prvky seznamu L bude korektnì
** uvolnìna voláním operace free.
***/

	L->Act = NULL;
	tElemPtr tmp = NULL;

	while (L->First != NULL){					// cyklus probiha dokud neni seznam prazdny
		tmp = L->First->ptr;					// do pomocne promenne nahraji druhy prvek
												// pokud existuje prvni prvek, tak ho zrusim
		free(L->First);
		L->First = tmp;							// uvolnim prvni prvek a na jeho pozici ulozim prvek druhy
	}
	
}

void InsertFirst (tList *L, int val) {
/*
** Vlo¾í prvek s hodnotou val na zaèátek seznamu L.
** V pøípadì, ¾e není dostatek pamìti pro nový prvek pøi operaci malloc,
** volá funkci Error().
**/
 
	tElemPtr first;

	if ((first = (tElemPtr) malloc(sizeof(struct tElem))) == NULL){
		Error();					// Pokud se nepovedlo alokovat pamet, vytiskneme chybove hlaseni
		return;
	}

	first->data = val;				//vlozeni prvku na prvni misto
	first->ptr = L->First;		

	L->First = first;
 
}

void First (tList *L) {
/*
** Nastaví aktivitu seznamu L na jeho první prvek.
** Funkci implementujte jako jediný pøíkaz, ani¾ byste testovali,
** zda je seznam L prázdný.
**/

	L->Act = L->First;					// prvni prvek se stava aktivnim

}

void CopyFirst (tList *L, int *val) {
/*
** Prostøednictvím parametru val vrátí hodnotu prvního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci Error().
**/
	if (L->First == NULL){
		Error();
		return;
	}
	*val = L->First->data;			// pokud neni seznam prazdny, vratime hodnotu prvniho prvku seznamu
}

void DeleteFirst (tList *L) {
/*
** Zru¹í první prvek seznamu L a uvolní jím pou¾ívanou pamì».
** Pokud byl ru¹ený prvek aktivní, aktivita seznamu se ztrácí.
** Pokud byl seznam L prázdný, nic se nedìje.
**/
	tElemPtr tmp = NULL;

	if (L->First == NULL){				//pokud je seznam prazdny nic se nedeje
		return;
	}

	if (L->First == L->Act){			// pokud je prvni prvek zaroven aktivni, ztraci se aktivita seznamu
		L->Act = NULL;
	}

	tmp = L->First;	
	L->First = L->First->ptr;			// smazani prvniho prvku
	free(tmp);


}	

void PostDelete (tList *L) {
/* 
** Zru¹í prvek seznamu L za aktivním prvkem a uvolní jím pou¾ívanou pamì».
** Pokud není seznam L aktivní nebo pokud je aktivní poslední prvek seznamu L,
** nic se nedìje.
**/
	if ((L->Act == NULL) || (L->Act->ptr == NULL)){
		return;
	}
	tElemPtr temp = L->Act->ptr;			// Do pomocne promenne nahraji dalsi prvek na seznamu
	L->Act->ptr = temp->ptr;				// Do naslednika ulozim odkaz na na naslednika nasledovnika

	free(temp);								// Uvolnim nasledovnika

}

void PostInsert (tList *L, int val) {
/*
** Vlo¾í prvek s hodnotou val za aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se nedìje!
** V pøípadì, ¾e není dostatek pamìti pro nový prvek pøi operaci malloc,
** zavolá funkci Error().
**/

	if (L->Act == NULL){
		return;							// Pokud neni seznam aktivni nic nedelam
	}
	tElemPtr tmp;

	if ((tmp = (tElemPtr) malloc(sizeof(struct tElem))) == NULL){
		Error();									// Alokace pameti pro dalsi prvek
		return;
	}
	tmp->data = val;
	tmp->ptr = L->Act->ptr;							// Prvek ukazuje na naslednika aktualniho prvku

	L->Act->ptr = tmp;								// Tento prvek se stava naslednikem 

}

void Copy (tList *L, int *val) {
/*
** Prostøednictvím parametru val vrátí hodnotu aktivního prvku seznamu L.
** Pokud seznam není aktivní, zavolá funkci Error().
**/
	
	if (L->Act == NULL){
		Error();
		return;
	}

	*val = L->Act->data;				// Pokud je seznam aktivni, zkopirujeme hodnotu prvku
}

void Actualize (tList *L, int val) {
/*
** Pøepí¹e data aktivního prvku seznamu L hodnotou val.
** Pokud seznam L není aktivní, nedìlá nic!
**/
	
	if (L->Act == NULL){
		return;
	}

	L->Act->data = val;					// pokud je seznam aktivni, aktualizujeme hodnotu aktivniho prvku
}

void Succ (tList *L) {
/*
** Posune aktivitu na následující prvek seznamu L.
** V¹imnìte si, ¾e touto operací se mù¾e aktivní seznam stát neaktivním.
** Pokud seznam L není aktivní, nedìlá nic.
**/

	if (L->Act == NULL){
		return;
	}

	L->Act = L->Act->ptr;

}

int Active (tList *L) {		
/*
** Je-li seznam L aktivní, vrací TRUE. V opaèném pøípadì vrací FALSE.
** Tuto funkci implementujte jako jediný pøíkaz return. 
**/
	return L->Act != NULL;
}

/* Konec c201.c */
