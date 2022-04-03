/*  Assegnamento Sistemi Operativi e in Tempo Reale
	Alice Cipriani mat. 340403						*/
  
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "list.h"

/************* Funzioni locali *********************************/

/* Confronta due elementi della lista:
   - ritorna un valore >0 se item1 > item2;
   - ritorna un valore <0 se item1 < item2;
   - ritorna un valore ==0 se item1 == item2.
  NB: puo' essere utilizzata nelle funzioni di ricerca e/o ordinamento
*/
int itemCompare( itemType item1, itemType item2 )
{
  /*if ( item1.id > item2.id )  
    return 1;
  else if ( item1.id < item2.id )  
    return -1;
  else
    return 0;*/
    return strcmp(item1.id, item2.id);
}



/* alloca un nuovo nodo per la lista, copiandone all'interno l'elemento item */
NODE * createNode( itemType item )
{
  NODE * p = (NODE *)malloc(sizeof(NODE));
  assert( p != NULL );

  p->item = item;
  p->next = NULL;
  return p;
}



/* dealloca il nodo p */
void deleteNode( NODE * p )
{
  free(p);
}





/********** Funzioni standard *******************************/

 /* Inizializza una nuova lista */
LIST NewList()
{
  return NULL;
}



/* Azzera la lista */
LIST DeleteList( LIST l )
{
  LIST tmp = l;

  /* dealloca tutti i nodi della lista */
  while ( !isEmpty(tmp) )
  {
    NODE * todel = tmp;
    tmp = tmp -> next;
    deleteNode( todel );
  }

  /* la lista e' ora vuota, e puo' essere utilizzata di nuovo */
  return NewList();
}



/* ritorna TRUE se la lista e' vuota */
BOOL isEmpty( LIST l )
{
  return l == NULL;
}



/* ritorna il numero di elementi presenti nella lista */
int getLength(LIST l)
{ 
  int size = 0;
  LIST tmp = l; 

  while ( ! isEmpty(tmp) )
  {
    ++size;
    tmp = tmp -> next;
  }

  return size;
}



/* Restituisce l'elemento in testa alla lista, senza modificarla */
itemType getHead(LIST l) 
{
  assert( !isEmpty(l) );
  return l->item;
}



/* Restituisce l'elemento in coda alla lista, senza modificarla */
itemType getTail(LIST l)
{
  NODE * tmp = l;
  assert( !isEmpty(l) );

  while( !isEmpty(tmp->next) )
      tmp = tmp->next;

  return tmp->item;
}



  /* Cerca l'elemento item nella lista e ne restituisce il puntatore
   ritorna NULL se non lo trova */
itemType* Find( LIST l, itemType item )
{
	LIST tmp = l;

  	while(!isEmpty(tmp) && itemCompare(tmp->item, item) != 0){
  		tmp = tmp->next;
  	}

	if(isEmpty(tmp)){
		return NULL;
	}else{
		return &(tmp->item);
	}

  return NULL;
}



/* Inserisce un elemento nella prima posizione della lista */
LIST EnqueueFirst (LIST l, itemType item)
{
  NODE * new_node = createNode(item);
  new_node->next = l;
  return new_node;
}



/* Inserisce un elemento nell'ultima posizione della lista */
LIST EnqueueLast (LIST l, itemType item) 
{
  NODE * new_node = createNode(item);

  if ( isEmpty( l ) )
  {
    /* Lista vuota: inserimento in testa */
    l = new_node;
  }
  else
  {
    LIST tmp = l;
    while ( !isEmpty( tmp -> next ) )
      tmp = tmp -> next;
    tmp -> next = new_node;
  }

  return l;
}



/* Inserisce un elemento mantenendo la lista ordinata */
LIST EnqueueOrdered(LIST l, itemType item )
{
  NODE * new_node = createNode(item);


  if ( isEmpty( l ) || item.served_n < l->item.served_n){
    /* Empty list or item with higher priority than the head item: inserting with maximum priority */
    l = EnqueueFirst(l, item);
  }
  else{
    LIST tmp = l;
    /* scan the list until it's empty */
    while ( !isEmpty( tmp -> next ) ){
      if(item.served_n >= tmp->next->item.served_n)
        /* item has lower or equal priority than the element in the list: continue scrolling*/
        tmp = tmp -> next;
      else{
        /* item has a higher priority than the element in the list: insert*/
        NODE * new_next = tmp -> next;
        tmp -> next = new_node;
        new_node->next = new_next;
        return l;
      }
    }

    tmp -> next = new_node;
  }

  return l;
}



 /* Toglie il primo elemento della lista (se non e' vuota) */
LIST DequeueFirst( LIST l )
{
  if ( !isEmpty( l ) )
  {
    NODE * todel = l;
    l = l -> next;
    deleteNode( todel );
  }
  return l;
}



/* Toglie l'ultimo elemento della lista (se non e' vuota) */
LIST DequeueLast( LIST l )
{
  if ( !isEmpty( l ) )
  {
    assert(FALSE);

    /* TODO */

  }

  return l;
}



/* Toglie l'elemento item dalla lista (se e' presente) */
LIST Dequeue( LIST l, itemType item )
{
  if ( ! isEmpty( l ) )
  {
    if ( itemCompare( l -> item, item ) == 0 )
    {
      /* eliminazione dell'elemento in testa */
      NODE * todel = l;
      l = l -> next;
      deleteNode( todel );
    }
    else
    {
      LIST tmp = l;

      while ( ! isEmpty( tmp -> next ) && itemCompare( tmp -> next -> item, item ) != 0 )
          tmp = tmp -> next;

      if ( ! isEmpty( tmp -> next ) )
      {
        /* l'elemento e' stato trovato nella lista (viene eliminato) */
        NODE * todel = tmp -> next;
        tmp -> next = tmp -> next -> next;
        deleteNode( todel );
      }
    }
  }
  return l;
}



/* Stampa a video un elemento della lista */
void PrintItem( itemType item )
{
  if(item.type == ACTUATOR){
    printf( "\tActuator id: %s is listening to: %s\n", item.id, item.sensor );
    printf( "\t\tTgoal: %f\n", item.temp );
  }else{
    printf( "\tSensor id: %s\n", item.id );
    printf( "\t\tTemp: %f °C\n", item.temp );
  }
}



/* Stampa a video la lista (esegue PrintItem() su tutti gli elementi) */
void PrintList( LIST l )
{
  LIST tmp = l;
  printf("\n\n\n--------------------------------");
  if(tmp->item.type == ACTUATOR){
      printf("\n\n---Printing actuators list: ----\n");
  }else{
      printf("\n\n---Printing sensors list: ---\n");
  }

  while ( !isEmpty(tmp) ) {
    PrintItem( tmp->item );
    tmp = tmp -> next;

    if ( ! isEmpty(tmp) )
      printf(" ");
  }
  printf("--------------------------------\n\n\n");
}

void PrintPriority( LIST l )
{
  LIST tmp = l;
  printf("\n\n\n--------------------------------");
  printf("\n\n---Printing sensors priority list: ---\n");
  while ( !isEmpty(tmp) ) {
    printf( "\tSensor id: %s\n", tmp->item.id );
    printf( "\t\tTimes served: %i\n", tmp->item.served_n );
    tmp = tmp -> next;

    if ( ! isEmpty(tmp) )
      printf(" ");
  }
  printf("--------------------------------\n\n\n");
}



