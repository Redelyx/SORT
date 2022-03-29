#include <stdio.h>

main(){
    float *x, *y, *z;  /* tre variabili di tipo  puntatore a float*/
    float a, b, c;     /* tre variabili di tipo float */
    
    x = &a ;  /* x contiene il puntatore (indirizzo) di a */
    *x =  1.1;  /* a contiene 1.1 */
    
    y = &b ;  /* y contiene il puntatore (indirizzo) di b */
    b =  1.1;  /* b contiene 1.1 */
     
    if(a == b) printf("i contenuti di a e b sono uguali\n");
    
    if(*x == *y) printf("i contenuti dei puntatori contenuti in x e y sono uguali\n");
       
    if(x == y) printf("i puntatori a (indirizzi di) a e a b sono uguali PROBLEMA!\n");
    else printf("i puntatori a (indirizzi di) a e a b sono diversi!\n");
    
    c = a + *y;
    printf("il valore di c e\' %f e deve essere 2.2\n",c);
    
    y = &c;
    c = a + *y;
    printf("il valore di c e\' %f e deve essere 3.3\n",c);
   
	int intero = 10;
    char ch = '_';
	int* p_intero;
	char* pc;

    /* sizeof di puntatori */

    printf("%lu\n", sizeof(char*));  /* la dimensione dei punt e' ovviamente */
    printf("%lu\n", sizeof(int*));   /* la stessa per tutti i tipi */
    printf("%lu\n", sizeof(double*));


    /* aritmetica dei puntatori */
    p_intero = &intero;
    printf("%p = &intero = %p\n", p_intero, &intero);
    p_intero = p_intero + 1;  /* incremento di un sizeof(int) */
    printf("%p\n", p_intero);
}
