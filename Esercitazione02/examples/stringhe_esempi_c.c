/* File: stringhe.c */
/* Questo programma illustra l'uso delle stringhe in C */

#include<stdio.h>
#include<stdlib.h> /* per array dinamici */
#include<string.h> /* libreria che contiene le funzioni sulle strighe:
                      qui la utiliziamo per la funzione strcmp() */

int main(void)
{

  int i = 0;

  /* Una stringa e' un array di caratteri che contiene la sequenza di caratteri
     che forma la stringa seguita dal carattere speciale di fine stringa: '\0'.
   */

  char stringa1[10] = {'p','i','p','p','o','\0'};

  /* Invece il seguente array di caratteri non e' una stringha perche'
     non termina con '\0' */

  char non_stringa1[2] = {'p','i'};

  /* Una stringa puo' essere inizializzata utilizzando una stringa letterale */

  char stringa2[6] = "pippo";

  /* Si noti che stringa2 e' un array *statico* di 6 caratteri:
     'p','i','p','p','o','\0' */

  /* E' ovviamente possibile memorizzare una stringa in un array di caratteri
     dinamico */

  char* buffer = malloc(80*sizeof(char));

  /* In questo caso, come per tutti gli array dinamici, non possiamo
     inizializzare l'array contestualmente alla sua creazione */


  /* Per stampare una stringa si deve utilizzare il formattatore "%s" */

  printf("%s\n", stringa1);
  printf("%s\n", non_stringa1);    /* nota: non e' una stringa! */
  printf("%s\n", stringa2);
  

  int len=0;
  // la funzione strlen ritorna il numero dei caratteri contenuti nella stringa
  // escluso il carattere terminatore '\0'
  len=strlen(stringa2);
  printf("La stringa2 %s contiene %d caratteri\n",stringa2,len);

  char stringa[20];
  strcpy(stringa,"esempio di stringa");
  printf("%s\n", stringa);


  /* Per leggere una stringa si deve utilizzare il formattatore "%s". */

  scanf("%s", buffer);

  /* Con questa istruzione viengono letti da input i caratteri in sequenza fino
     a trovare il primo carattere bianco, oppure un tab, un
     interlinea,. ecc. questi carattesi vengono messi dentro il vettore
     buffer, al posto del carattere bianco, ecc, viene posto il carattere
     '\0'. Il vettore deve essere sufficiantemente grande da contenere tutti i
     caratteri letti.  */

  /* Nota non si usa &buffer ma direttamente buffer! questo perche' buffer
     essendo di tipo char* e' gia' un indirizzo!! */

  printf("%s\n", buffer);



  /* Per manipolare la stringa si deve accedere ai singoli caratteri
     singolarmente. Ad esempio: */

  while (buffer[i] != '\0') {
    /* fai qualcosa con buffer[i], ad esempio: */
    printf("%c\n", buffer[i]);
    i++;
  }



  /* Per verificare l'uguaglianza tra due stringhe non si puo' usare == perche'
     questo confronta i puntatori. Si devono necessariamente scadire le due
     stringhe. E' anche possibile usare la funzione di libreria per le
     stringhe: int strcmp(char* st1, char* st2), che restituisce, 0 se st1 =
     st2, <0 se st2 < st2, >0 se st1 > st2.  */

  if (strcmp(buffer,stringa1) == 0)
    printf("uguali\n");
  else printf("diverse\n");

  /* Esistono diverse funzioni di libreria sulle stringhe, si consulti un
     manuale di C per maggiori informazioni.  */



  return 0;
}

