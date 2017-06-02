// Lukas Pelanek
// xpelan03

/* c206.c **********************************************************}
{* T�ma: Dvousm�rn� v�zan� line�rn� seznam
**
**                   N�vrh a referen�n� implementace: Bohuslav K�ena, ��jen 2001
**                            P�epracovan� do jazyka C: Martin Tu�ek, ��jen 2004
**                                            �pravy: Bohuslav K�ena, ��jen 2014
**
** Implementujte abstraktn� datov� typ dvousm�rn� v�zan� line�rn� seznam.
** U�ite�n�m obsahem prvku seznamu je hodnota typu int.
** Seznam bude jako datov� abstrakce reprezentov�n prom�nnou
** typu tDLList (DL znamen� Double-Linked a slou�� pro odli�en�
** jmen konstant, typ� a funkc� od jmen u jednosm�rn� v�zan�ho line�rn�ho
** seznamu). Definici konstant a typ� naleznete v hlavi�kov�m souboru c206.h.
**
** Va��m �kolem je implementovat n�sleduj�c� operace, kter� spolu
** s v��e uvedenou datovou ��st� abstrakce tvo�� abstraktn� datov� typ
** obousm�rn� v�zan� line�rn� seznam:
**
**      DLInitList ...... inicializace seznamu p�ed prvn�m pou�it�m,
**      DLDisposeList ... zru�en� v�ech prvk� seznamu,
**      DLInsertFirst ... vlo�en� prvku na za��tek seznamu,
**      DLInsertLast .... vlo�en� prvku na konec seznamu, 
**      DLFirst ......... nastaven� aktivity na prvn� prvek,
**      DLLast .......... nastaven� aktivity na posledn� prvek, 
**      DLCopyFirst ..... vrac� hodnotu prvn�ho prvku,
**      DLCopyLast ...... vrac� hodnotu posledn�ho prvku, 
**      DLDeleteFirst ... zru�� prvn� prvek seznamu,
**      DLDeleteLast .... zru�� posledn� prvek seznamu, 
**      DLPostDelete .... ru�� prvek za aktivn�m prvkem,
**      DLPreDelete ..... ru�� prvek p�ed aktivn�m prvkem, 
**      DLPostInsert .... vlo�� nov� prvek za aktivn� prvek seznamu,
**      DLPreInsert ..... vlo�� nov� prvek p�ed aktivn� prvek seznamu,
**      DLCopy .......... vrac� hodnotu aktivn�ho prvku,
**      DLActualize ..... p�ep�e obsah aktivn�ho prvku novou hodnotou,
**      DLSucc .......... posune aktivitu na dal�� prvek seznamu,
**      DLPred .......... posune aktivitu na p�edchoz� prvek seznamu, 
**      DLActive ........ zji��uje aktivitu seznamu.
**
** P�i implementaci jednotliv�ch funkc� nevolejte ��dnou z funkc�
** implementovan�ch v r�mci tohoto p��kladu, nen�-li u funkce
** explicitn� uvedeno n�co jin�ho.
**
** Nemus�te o�et�ovat situaci, kdy m�sto leg�ln�ho ukazatele na seznam 
** p�ed� n�kdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodn� komentujte!
**
** Terminologick� pozn�mka: Jazyk C nepou��v� pojem procedura.
** Proto zde pou��v�me pojem funkce i pro operace, kter� by byly
** v algoritmick�m jazyce Pascalovsk�ho typu implemenov�ny jako
** procedury (v jazyce C procedur�m odpov�daj� funkce vracej�c� typ void).
**/

#include "c206.h"

int solved;
int errflg;

void DLError() {
/*
** Vytiskne upozorn�n� na to, �e do�lo k chyb�.
** Tato funkce bude vol�na z n�kter�ch d�le implementovan�ch operac�.
**/	
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;             /* glob�ln� prom�nn� -- p��znak o�et�en� chyby */
    return;
}

void DLInitList (tDLList *L) {
/*
** Provede inicializaci seznamu L p�ed jeho prvn�m pou�it�m (tzn. ��dn�
** z n�sleduj�c�ch funkc� nebude vol�na nad neinicializovan�m seznamem).
** Tato inicializace se nikdy nebude prov�d�t nad ji� inicializovan�m
** seznamem, a proto tuto mo�nost neo�et�ujte. V�dy p�edpokl�dejte,
** �e neinicializovan� prom�nn� maj� nedefinovanou hodnotu.
**/
	L->First = NULL;
	L->Act = NULL;											//inicializace
	L->Last = NULL;
}

void DLDisposeList (tDLList *L) {
/*
** Zru�� v�echny prvky seznamu L a uvede seznam do stavu, v jak�m
** se nach�zel po inicializaci. Ru�en� prvky seznamu budou korektn�
** uvoln�ny vol�n�m operace free. 
**/
	if (L->First == NULL){
		return;
	}
	tDLElemPtr tmp = NULL;
	while (L->First != NULL){							//cyklus probiha dokud neni seznam prazdny
		tmp = L->First->rptr;
		free(L->First);
		L->First = tmp;
	}
	L->First = NULL;
	L->Last = NULL;										//uvedeni seznamu do stavu po inicializaci
	L->Act = NULL;
}

void DLInsertFirst (tDLList *L, int val) {
/*
** Vlo�� nov� prvek na za��tek seznamu L.
** V p��pad�, �e nen� dostatek pam�ti pro nov� prvek p�i operaci malloc,
** vol� funkci DLError().
**/
	tDLElemPtr tmp;
	if ((tmp = (tDLElemPtr) malloc(sizeof(struct tDLElem))) == NULL){
		DLError();
		return;
	}

	tmp->rptr = NULL;
	tmp->lptr = NULL;								//nastaveni ukazatelu na null a prirazeni hodnoty
	tmp->data = val;

	if (L->First == NULL){
		L->First = tmp;								//pokud je seznam prazdny, stava se prvek prvnim a poslednim zaroven
		L->Last = tmp;
	}
	else{
		tmp->rptr = L->First;
		L->First->lptr = tmp;						//jinak vlozim prvek pred prvniho
		L->First = tmp;
	}

}

void DLInsertLast(tDLList *L, int val) {
/*
** Vlo�� nov� prvek na konec seznamu L (symetrick� operace k DLInsertFirst).
** V p��pad�, �e nen� dostatek pam�ti pro nov� prvek p�i operaci malloc,
** vol� funkci DLError().
**/ 	
	tDLElemPtr tmp;

	if ((tmp = (tDLElemPtr)malloc(sizeof(struct tDLElem))) == NULL){
		DLError();
		return;
	}
	tmp->lptr = NULL;
	tmp->rptr = NULL;								//nastaveni ukazatelu na null a prirazeni hodnoty
	tmp->data = val;

	if (L->First == NULL){
		L->First = tmp;								//pokud je seznam prazdny, prvek se stane prvnim a poslednim soucasne
		L->Last = tmp;
	}
	else{
		tmp->lptr = L->Last;
		L->Last->rptr = tmp;						//vlozeni za posledniho
		L->Last = tmp;
	}
}

void DLFirst (tDLList *L) {
/*
** Nastav� aktivitu na prvn� prvek seznamu L.
** Funkci implementujte jako jedin� p��kaz (nepo��t�me-li return),
** ani� byste testovali, zda je seznam L pr�zdn�.
**/
	L->Act = L->First;
}

void DLLast (tDLList *L) {
/*
** Nastav� aktivitu na posledn� prvek seznamu L.
** Funkci implementujte jako jedin� p��kaz (nepo��t�me-li return),
** ani� byste testovali, zda je seznam L pr�zdn�.
**/
	L->Act = L->Last;
}

void DLCopyFirst (tDLList *L, int *val) {
/*
** Prost�ednictv�m parametru val vr�t� hodnotu prvn�ho prvku seznamu L.
** Pokud je seznam L pr�zdn�, vol� funkci DLError().
**/
	if (L->First == NULL){
		DLError();
		return;
	}
	*val = L->First->data;
}

void DLCopyLast (tDLList *L, int *val) {
/*
** Prost�ednictv�m parametru val vr�t� hodnotu posledn�ho prvku seznamu L.
** Pokud je seznam L pr�zdn�, vol� funkci DLError().
**/
	if (L->First == NULL){
		DLError();
		return;
	}
	*val = L->Last->data;

}

void DLDeleteFirst (tDLList *L) {
/*
** Zru�� prvn� prvek seznamu L. Pokud byl prvn� prvek aktivn�, aktivita 
** se ztr�c�. Pokud byl seznam L pr�zdn�, nic se ned�je.
**/
	if (L->First == NULL){
		return;
	}
	if (L->First == L->Act){				//pokud je ruseny prvek soucasne aktualni, zrusim aktivitu
		L->Act = NULL;
	}
	
	tDLElemPtr tmp = L->First;

	if (L->First == L->Last){				//pokud seznam obsahuje jeden prvek zrusim ukazatele na prvni i posledni prvek
		L->Last = NULL;
		L->First = NULL;
	}
	else{
		L->First = tmp->rptr;				//druhy prvek vlozim na prvni pozici a zrusim ukazatel na prvni prvek
		L->First->lptr = NULL;
	}
	free(tmp);
}	

void DLDeleteLast (tDLList *L) {
/*
** Zru�� posledn� prvek seznamu L. Pokud byl posledn� prvek aktivn�,
** aktivita seznamu se ztr�c�. Pokud byl seznam L pr�zdn�, nic se ned�je.
**/ 
	if (L->First == NULL){
		return;
	}
	if (L->Last == L->Act){				//pokud je ruseny prvek soucasne aktualni, zrusim aktivitu
		L->Act = NULL;
	}

	tDLElemPtr tmp = L->Last;

	if (L->First == L->Last){			//pokud seznam obsahuje jeden prvek zrusim ukazatele na prvni i posledni prvek
		L->First = NULL;
		L->Last = NULL;
	}
	else{
		L->Last = tmp->lptr;			//predposledni prvek vlozim na posledni pozici a zrusim ukazatel na posledni prvek
		L->Last->rptr = NULL;
	}
	free(tmp);						
}

void DLPostDelete (tDLList *L) {
/*
** Zru�� prvek seznamu L za aktivn�m prvkem.
** Pokud je seznam L neaktivn� nebo pokud je aktivn� prvek
** posledn�m prvkem seznamu, nic se ned�je.
**/
	if ((L->Act == NULL) || (L->Act == L->Last)){
		return;
	}
	tDLElemPtr tmp = L->Act->rptr;

	if (tmp->rptr == NULL){					//pokud za rusenym prvkem jiz zadny neni, aktualni prvek se stane poslednim
		L->Act->rptr = NULL;
		L->Last = L->Act;
	}
	else{
		L->Act->rptr = tmp->rptr;			//jinak zrusim prvek a nastavim ukazatele
		tmp->rptr->lptr = L->Act;
	}
		free(tmp);


}

void DLPreDelete(tDLList *L) {
	/*
	** Zru�� prvek p�ed aktivn�m prvkem seznamu L .
	** Pokud je seznam L neaktivn� nebo pokud je aktivn� prvek
	** prvn�m prvkem seznamu, nic se ned�je.
	**/
	if ((L->Act == NULL) || (L->Act == L->First)){
		return;
	}
	tDLElemPtr tmp = L->Act->lptr;

	if (tmp->lptr == NULL){					//pokud pred rusenym prvkem jiz zadny neni, aktualni prvek se stane prvnim
		L->Act->lptr = NULL;
		L->First = L->Act;
	}
	else{
		L->Act->lptr = tmp->lptr;			//jinak zrusim prvek a nastavim ukazatele
		tmp->lptr->rptr = L->Act;
	}
		free(tmp);


}

void DLPostInsert (tDLList *L, int val) {
/*
** Vlo�� prvek za aktivn� prvek seznamu L.
** Pokud nebyl seznam L aktivn�, nic se ned�je.
** V p��pad�, �e nen� dostatek pam�ti pro nov� prvek p�i operaci malloc,
** vol� funkci DLError().
**/
	if (L->Act == NULL){
		return;
	}

	tDLElemPtr tmp;
	if ((tmp = (tDLElemPtr)malloc(sizeof(struct tDLElem))) == NULL){
		DLError();
		return;
	}

	if (L->Act->rptr == NULL){				//pokud aktualni prvek je soucasne poslednim, vlozim novy prvek za tento prvek a nastavim jej jako posledni
		L->Act->rptr = tmp;
		tmp->lptr = L->Act;
		tmp->rptr = NULL;
		L->Last = tmp;
	}
	else{
		tmp->rptr = L->Act->rptr;			//jinak vlozim prvek za aktualni prvek a nastavim ukazatele
		tmp->lptr = L->Act;
		L->Act->rptr->lptr = tmp;
		L->Act->rptr = tmp;
	}
	tmp->data = val;
}

void DLPreInsert (tDLList *L, int val) {
/*
** Vlo�� prvek p�ed aktivn� prvek seznamu L.
** Pokud nebyl seznam L aktivn�, nic se ned�je.
** V p��pad�, �e nen� dostatek pam�ti pro nov� prvek p�i operaci malloc,
** vol� funkci DLError().
**/	
	if (L->Act == NULL){
		return;
	}

	tDLElemPtr tmp;
	if ((tmp = (tDLElemPtr)malloc(sizeof(struct tDLElem))) == NULL){
		DLError();
		return;
	}

	if (L->Act->lptr == NULL){			//pokud aktualni prvek je soucasne prvnim, vlozim novy prvek pred tento prvek a nastavim jej jako prvni
		L->Act->lptr = tmp;
		tmp->rptr = L->Act;
		tmp->lptr = NULL;
		L->First = tmp;
	}
	else{
		tmp->lptr = L->Act->lptr;		//jinak vlozim prvek pred aktualni prvek a nastavim ukazatele
		tmp->rptr = L->Act;
		L->Act->lptr->rptr = tmp;
		L->Act->lptr = tmp;
	}
	tmp->data = val;
}

void DLCopy (tDLList *L, int *val) {
/*
** Prost�ednictv�m parametru val vr�t� hodnotu aktivn�ho prvku seznamu L.
** Pokud seznam L nen� aktivn�, vol� funkci DLError ().
**/
	if (L->Act == NULL){
		DLError();
		return;
	}
	*val = L->Act->data;
}

void DLActualize (tDLList *L, int val) {
/*
** P�ep�e obsah aktivn�ho prvku seznamu L.
** Pokud seznam L nen� aktivn�, ned�l� nic.
**/
	if (L->Act == NULL){
		return;
	}
	L->Act->data = val;
}

void DLSucc (tDLList *L) {
/*
** Posune aktivitu na n�sleduj�c� prvek seznamu L.
** Nen�-li seznam aktivn�, ned�l� nic.
** V�imn�te si, �e p�i aktivit� na posledn�m prvku se seznam stane neaktivn�m.
**/
	if (L->Act == NULL){
		return;
	}
	L->Act = L->Act->rptr;
}


void DLPred (tDLList *L) {
/*
** Posune aktivitu na p�edchoz� prvek seznamu L.
** Nen�-li seznam aktivn�, ned�l� nic.
** V�imn�te si, �e p�i aktivit� na prvn�m prvku se seznam stane neaktivn�m.
**/
	if (L->Act == NULL){
		return;
	}
	L->Act = L->Act->lptr;
}

int DLActive (tDLList *L) {
/*
** Je-li seznam aktivn�, vrac� true. V opa�n�m p��pad� vrac� false.
** Funkci implementujte jako jedin� p��kaz.
**/
	return (L->Act != NULL);
}

/* Konec c206.c*/
