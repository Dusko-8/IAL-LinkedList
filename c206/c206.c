/* ******************************* c206.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c206 - Dvousměrně vázaný lineární seznam                            */
/*  Návrh a referenční implementace: Bohuslav Křena, říjen 2001               */
/*  Vytvořil: Martin Tuček, říjen 2004                                        */
/*  Upravil: Kamil Jeřábek, září 2020                                         */
/*           Daniel Dolejška, září 2021                                       */
/*           Daniel Dolejška, září 2022                                       */
/* ************************************************************************** */
/*
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int. Seznam bude jako datová
** abstrakce reprezentován proměnnou typu DLList (DL znamená Doubly-Linked
** a slouží pro odlišení jmen konstant, typů a funkcí od jmen u jednosměrně
** vázaného lineárního seznamu). Definici konstant a typů naleznete
** v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu s výše
** uvedenou datovou částí abstrakce tvoří abstraktní datový typ obousměrně
** vázaný lineární seznam:
**
**      DLL_Init ........... inicializace seznamu před prvním použitím,
**      DLL_Dispose ........ zrušení všech prvků seznamu,
**      DLL_InsertFirst .... vložení prvku na začátek seznamu,
**      DLL_InsertLast ..... vložení prvku na konec seznamu,
**      DLL_First .......... nastavení aktivity na první prvek,
**      DLL_Last ........... nastavení aktivity na poslední prvek,
**      DLL_GetFirst ....... vrací hodnotu prvního prvku,
**      DLL_GetLast ........ vrací hodnotu posledního prvku,
**      DLL_DeleteFirst .... zruší první prvek seznamu,
**      DLL_DeleteLast ..... zruší poslední prvek seznamu,
**      DLL_DeleteAfter .... ruší prvek za aktivním prvkem,
**      DLL_DeleteBefore ... ruší prvek před aktivním prvkem,
**      DLL_InsertAfter .... vloží nový prvek za aktivní prvek seznamu,
**      DLL_InsertBefore ... vloží nový prvek před aktivní prvek seznamu,
**      DLL_GetValue ....... vrací hodnotu aktivního prvku,
**      DLL_SetValue ....... přepíše obsah aktivního prvku novou hodnotou,
**      DLL_Previous ....... posune aktivitu na předchozí prvek seznamu,
**      DLL_Next ........... posune aktivitu na další prvek seznamu,
**      DLL_IsActive ....... zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce explicitně
* uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako procedury
** (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c206.h"

int error_flag;
int solved;

/**
 * Vytiskne upozornění na to, že došlo k chybě.
 * Tato funkce bude volána z některých dále implementovaných operací.
 */
void DLL_Error()
{
  printf("*ERROR* The program has performed an illegal operation.\n");
  error_flag = TRUE;
}

/**
 * Provede inicializaci seznamu list před jeho prvním použitím (tzn. žádná
 * z následujících funkcí nebude volána nad neinicializovaným seznamem).
 * Tato inicializace se nikdy nebude provádět nad již inicializovaným seznamem,
 * a proto tuto možnost neošetřujte.
 * Vždy předpokládejte, že neinicializované proměnné mají nedefinovanou hodnotu.
 *
 * @param list Ukazatel na strukturu dvousměrně vázaného seznamu
 */
void DLL_Init(DLList *list)
{
  list->activeElement = NULL;
  list->firstElement = NULL;
  list->lastElement = NULL;
}

/**
 * Zruší všechny prvky seznamu list a uvede seznam do stavu, v jakém se nacházel
 * po inicializaci.
 * Rušené prvky seznamu budou korektně uvolněny voláním operace free.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Dispose(DLList *list)
{
  struct DLLElement *tmpElement;
  while (list->lastElement != NULL)
  {
    tmpElement = list->firstElement;

    if (list->firstElement != list->lastElement)
    {
      list->firstElement = tmpElement->nextElement;
    }
    else
    {
      list->firstElement = NULL;
      list->lastElement = NULL;
    }
    free(tmpElement);
  }

  list->activeElement = NULL;
}

/**
 * Vloží nový prvek na začátek seznamu list.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na začátek seznamu
 */
void DLL_InsertFirst(DLList *list, int data)
{
  struct DLLElement *tmpElement = malloc(sizeof(struct DLLElement));

  if (tmpElement != NULL)
  {
    tmpElement->data = data;
    tmpElement->previousElement = NULL;
    if (list->firstElement != NULL)
    {
      list->firstElement->previousElement = tmpElement;
      tmpElement->nextElement = list->firstElement;
    }
    else
    {
      list->lastElement = tmpElement;
      tmpElement->nextElement = NULL;
    }

    list->firstElement = tmpElement;
  }
  else
  {
    DLL_Error();
  }
}

/**
 * Vloží nový prvek na konec seznamu list (symetrická operace k
 * DLL_InsertFirst). V případě, že není dostatek paměti pro nový prvek při
 * operaci malloc, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na konec seznamu
 */
void DLL_InsertLast(DLList *list, int data)
{
  struct DLLElement *tmpElement = malloc(sizeof(struct DLLElement));

  if (tmpElement != NULL)
  {
    tmpElement->data = data;
    tmpElement->nextElement = NULL;

    if (list->lastElement != NULL)
    {
      list->lastElement->nextElement = tmpElement;
      tmpElement->previousElement = list->lastElement;
    }
    else
    {
      list->firstElement = tmpElement;
      tmpElement->previousElement = NULL;
    }

    list->lastElement = tmpElement;
  }
  else
  {
    DLL_Error();
  }
}

/**
 * Nastaví první prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz, aniž byste testovali,
 * zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_First(DLList *list)
{
  list->activeElement = list->firstElement;
}

/**
 * Nastaví poslední prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz, aniž byste testovali,
 * zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Last(DLList *list)
{
  list->activeElement = list->lastElement;
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu prvního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetFirst(DLList *list, int *dataPtr)
{
  if (list->firstElement != NULL)
  {
    *dataPtr = list->firstElement->data;
  }
  else
  {
    DLL_Error();
  }
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu posledního prvku seznamu
 * list. Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetLast(DLList *list, int *dataPtr)
{
  if (list->firstElement != NULL)
  {
    *dataPtr = list->lastElement->data;
  }
  else
  {
    DLL_Error();
  }
}

/**
 * Zruší první prvek seznamu list.
 * Pokud byl první prvek aktivní, aktivita se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteFirst(DLList *list)
{
  if (list->firstElement != NULL)
  {
    struct DLLElement *tmpElement ;

    tmpElement = list->firstElement;

    if (list->firstElement == list->activeElement)
    {
      list->activeElement = NULL;
    }
    if (tmpElement->nextElement != NULL)
    {
      list->firstElement = tmpElement->nextElement;
      list->firstElement->previousElement = NULL;
    }
    else
    {
      list->firstElement = NULL;
      list->lastElement = NULL;
    }

    free(tmpElement);
  }
}

/**
 * Zruší poslední prvek seznamu list.
 * Pokud byl poslední prvek aktivní, aktivita seznamu se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteLast(DLList *list)
{
  if (list->lastElement != NULL)
  {
    struct DLLElement *tmpElement ;

    tmpElement = list->lastElement;

    if (list->lastElement == list->activeElement)
    {
      list->activeElement = NULL;
    }
    if (tmpElement->previousElement != NULL)
    {
      list->lastElement = tmpElement->previousElement;
      list->lastElement->nextElement = NULL;
    }
    else
    {
      list->firstElement = NULL;
      list->lastElement = NULL;
    }

    free(tmpElement);
  }
}

/**
 * Zruší prvek seznamu list za aktivním prvkem.
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * posledním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteAfter(DLList *list)
{
  struct DLLElement *tmpElement;
  if (list->activeElement != NULL && list->activeElement != list->lastElement)
  {
    tmpElement = list->activeElement->nextElement;

    if (tmpElement == list->lastElement)
    {
      list->lastElement = list->activeElement;
    }

    if (tmpElement->nextElement != NULL)
    {
      list->activeElement->nextElement = tmpElement->nextElement;
      tmpElement->nextElement->previousElement = list->activeElement;
    }
    else
    {
      list->activeElement->nextElement = NULL;
    }

    free(tmpElement);
  }
}

/**
 * Zruší prvek před aktivním prvkem seznamu list .
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * prvním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteBefore(DLList *list)
{
  struct DLLElement *tmpElement;
  if (list->activeElement != NULL && list->activeElement != list->firstElement)
  {

    tmpElement = list->activeElement->previousElement;

    if (tmpElement == list->firstElement)
    {
      list->firstElement = list->activeElement;
    }

    if (tmpElement->previousElement != NULL)
    {
      list->activeElement->previousElement = tmpElement->previousElement;
      tmpElement->previousElement->nextElement = list->activeElement;
    }
    else
    {
      list->activeElement->previousElement = NULL;
    }

    free(tmpElement);
  }
}

/**
 * Vloží prvek za aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu za právě aktivní prvek
 */
void DLL_InsertAfter(DLList *list, int data)
{
  if (list->activeElement != NULL)
  {
    struct DLLElement *tmpElement = malloc(sizeof(struct DLLElement));

    if (tmpElement != NULL)
    {
      tmpElement->data = data;

      if (list->activeElement->nextElement != NULL)
      {
        tmpElement->nextElement = list->activeElement->nextElement;
        list->activeElement->nextElement->previousElement = tmpElement;
      }
      else
      {
        tmpElement->nextElement = NULL;
        list->lastElement = tmpElement;
      }
      tmpElement->previousElement = list->activeElement;
      list->activeElement->nextElement = tmpElement;
    }
    else
    {
      DLL_Error();
    }
  }
}

/**
 * Vloží prvek před aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu před právě aktivní prvek
 */
void DLL_InsertBefore(DLList *list, int data)
{
  struct DLLElement *tmpElement = malloc(sizeof(struct DLLElement));

  if (tmpElement != NULL)
  {
    tmpElement->data = data;

    if (list->activeElement != list->firstElement)
    {
      tmpElement->previousElement = list->activeElement->previousElement;
      list->activeElement->previousElement->nextElement = tmpElement;
    }
    else
    {
      tmpElement->previousElement = NULL;
      list->firstElement = tmpElement;
    }
    
    tmpElement->nextElement = list->activeElement;
    list->activeElement->previousElement = tmpElement;
  }
  else
  {
    DLL_Error();
  }
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, volá funkci DLL_Error ().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetValue(DLList *list, int *dataPtr)
{
  if (list->activeElement != NULL)
  {
    *dataPtr = list->activeElement->data;
  }
  else
  {
    DLL_Error();
  }
}

/**
 * Přepíše obsah aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, nedělá nic.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Nová hodnota právě aktivního prvku
 */
void DLL_SetValue(DLList *list, int data)
{
  if (list->activeElement != NULL)
  {
    list->activeElement->data = data;
  }
}

/**
 * Posune aktivitu na následující prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Next(DLList *list)
{
  if (list->activeElement != NULL)
  {
    if (list->activeElement->nextElement == NULL)
    {
      list->activeElement = NULL;
    }
    else
    {
      list->activeElement = list->activeElement->nextElement;
    }
  }
}

/**
 * Posune aktivitu na předchozí prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Previous(DLList *list)
{
  if (list->activeElement != NULL)
  {
    if (list->activeElement->previousElement == NULL)
    {
      list->activeElement = NULL;
    }
    else
    {
      list->activeElement = list->activeElement->previousElement;
    }
  }
}

/**
 * Je-li seznam list aktivní, vrací nenulovou hodnotu, jinak vrací 0.
 * Funkci je vhodné implementovat jedním příkazem return.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 *
 * @returns Nenulovou hodnotu v případě aktivity prvku seznamu, jinak nulu
 */
int DLL_IsActive(DLList *list)
{
  return list->activeElement == NULL ? 0 : 1;
}

/* Konec c206.c */
