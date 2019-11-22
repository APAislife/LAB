#include <stdio.h>
#include <string.h>
#include <ctype.h>

//Codifica dei comandi mediante tipo enum (punto = 0 ... err = 5)
typedef enum{punto, notquadre, quadre, minusc, maiusc, err}exp;

char *cercaRegexp(char *src, char *regexp);
exp isMetaC(char* regexp, int i);


int main(){
    //Stringa sorgente di prova
    char src[50]="foto noto moto morO amore AmorE Amarena";
    //Espressione regolare di prova che contiene tutti i metacaratteri (In questo caso l'unica sottostringa valida
    //all'interno di src è "Amarena"
    char reg[50]="\\A[^fn].[r]\\a";
    char *c;
    //La funzione cercaRegexp ritorna un puntatore ( a carattere ) alla prima occorrenza dell'espressione regolare nella stringa sorgente
    c=cercaRegexp(src, reg);
    printf("%s", c);
    return 0;
}

char *cercaRegexp(char *src, char *regexp){
    int i,j, lun_src, lun_reg;
    //Il puntatore a carattere tmp viene inizializzato a NULL in modo da ritornare un puntatore a NULL nel caso in cui la regExp non occorra nella stringa sorgente
    char * tmp=NULL;
    int flag, found=0;
    int count=0;

    lun_src=strlen(src);
    lun_reg=strlen(regexp);

    //Prima ciclo for che itera sulla stringa sorgente e si ferma se raggiunge la fine o se è stato trovata un'occorrenza della regExp
    for(j=0; j<lun_src && tmp==NULL; j++) {

        flag=1;

        //Secondo ciclo che itera sull'espressione regolare e si ferma se raggiunge la fine o se il flag=0 (Il ciclo finisce prima se
        for (i = 0; i < lun_reg && flag; i++) {

            //Switch che chiama la funzione isMetaC per controllare se l'elemento in posizione i della regExp è un metacarattere o meno
            switch (isMetaC(regexp, i)) {
                //Se l'i-esimo carattere della regExp è un punto ogni carattere della stringa sorgente va bene, quindi si
                //aggiorna la variabile count ( trovata una corrispondenza tra stringa e regExp ) e se la variabile tmp = NULL
                //questa diventa il puntatore al j-esimo carattere della stringa sorgente [Prima occorrenza della regExp nella stringa sorgente]
                case punto:
                    if(tmp==NULL)
                        tmp=&src[j];
                    count++;
                    break;

                //Se viene trovato il metacarattere [...] qualsiasi carattere all'interno delle parentesi è accettabile
                //all'interno della stringa, quindi viene posto il flag found = 0 e un ciclo scorre a partire dalla prima
                //parentesi sino alla parentesi chiusa, controllando per ogni elemento se questo è uguale al j+count-esimo
                //carattere della stringa sorgente. In caso positivo found = 1 e il controllo fuori dal ciclo aggiornerà count
                //e assegnerà a tmp il j-esimo [prima occorrenza] carattere della stringa sorgente se tmp=NULL [ cioè se
                //non sono state trovate altre occorrenze prima di quel momento. In caso negativo (found = 0) a tmp verrà
                //assegnato NULL e count tornerà al valore 0.
                case quadre:

                    i++; //Salta il carattere '[' nella regexp
                    found=0;
                    while (regexp[i] != ']') {
                        if(src[j+count]==regexp[i])
                            found = 1;
                        i++;
                    }

                    if(found){
                        if(tmp==NULL)
                            tmp=&src[j];
                        count++;
                    } else {
                        tmp=NULL;
                        count=0;
                    }
                    break;

                //Se viene trovato il metacarattere [^...] nessun carattere all'interno delle parentesi è accettabile
                //all'interno della stringa, quindi viene posto il flag found = 0 e un ciclo scorre sino al carattere ']'
                //nella regexp controllando se il j+count-esimo carattere della stringa sorgente sia uguale all'i-esimo
                //carattere della regexp. In caso positivo il flag found = 1, cioè è stato trovato un carattere che non è
                //accettabile. Se alla fine del ciclo found = 1 al puntatore tmp viene assegnato NULL e count ritorna = 0
                //mentre in caso contrario si aumenta count e se tmp = NULL gli viene assegnato il puntatore al j-esimo carattere
                //nella regexp.
                case notquadre:

                    i+=2; //salta i caratteri '[^' nella regexp
                    found=0;
                    while (regexp[i] != ']') {
                        if(src[j+count]==regexp[i])
                            found=1;
                        i++;
                    }

                    if(found){
                        tmp=NULL;
                        count=0;
                    } else {
                        if(tmp==NULL)
                            tmp=&src[j];
                        count++;
                    }
                    break;

                //Se viene trovato il metacarattere \a qualsiasi carattere minuscolo è accettabile. In questo caso si
                //controlla che il j+count-esimo carattere della stringa sorgente sia minuscolo tramite islower(). In caso
                //positivo si aggiorna count e se tmp = NULL gli viene assegnato il puntatore al j-esimo carattere della
                //stringa sorgente. In caso negativo a tmp viene assegnato il puntatore NULL e count ritorna 0.
                case minusc:
                    i++; //salta il carattere '\a' [considerato come carattere singolo]
                    if(islower(src[j+count])){
                        if(tmp==NULL)
                            tmp=&src[j];
                        count++;
                    } else{
                        tmp=NULL;
                        count=0;
                    }

                    break;

                //Analogo al caso minusc
                case maiusc:
                    i++;
                    if(isupper(src[j+count])){
                        if(tmp==NULL)
                            tmp=&src[j];
                        count++;
                    } else{
                        tmp=NULL;
                        count=0;
                    }
                    break;

                //Se l'i-esimo carattere di regexp non è un metacarattere (carattere standard o spazio) si controlla che
                //il j+count-esimo elemento della stringa sorgente sia uguale all'i-esimo elemento della regexp. In caso
                //positivo si aggiorna count e se tmp = NULL gli viene assegnato il puntatore al j-esimo carattere della
                //stringa sorgente. In caso negativo a tmp viene assegnato NULL e count ritorna a 0.
                default:
                    if(src[j+count]==regexp[i]){
                        if(tmp==NULL)
                            tmp=&src[j];
                        count++;
                    } else{
                        tmp=NULL;
                        count=0;
                    }
                    break;
            }

            //Se dopo lo switch (controllo dei metacaratteri) il puntatore tmp = NULL allora flag e count ritornano uguali
            //a 0. In caso contrario flag viene posto = 1 (Espressione trovata all'interno della stringa).
            if(tmp==NULL){
                flag=0;
                count=0;
            } else {
                flag=1;
            }
        }
    }

    //La funzione ritorna al programma chiamante la variabile tmp che sarà NULL o il puntatore alla prima occorrenza di
    //regexp all'interno della stringa sorgente.
    return tmp;
}

exp isMetaC(char* regexp, int i){
    //Viene creata una tabella di 5 elementi contenente i metacaratteri (o una parte nel caso delle quadre)
    char *caratteri[err]={".", "[^", "[", "\\a", "\\A"};
    //Alla variabile meta di tipo enum exp viene assegnato err (5) che corrisponde a tutto ciò che non è un metacarattere
    exp meta=err;
    int j;

    //Se l'i-esimo carattere di regexp non è alfanumerico allora si controlla per ogni metacarattere che questo appaia
    //esattamente nell'i-esimo carattere della regexp [si controlla che la prima occorrenza del metacarattere si trovi a partire
    //da l'i-esimo carattere della regexp e non dopo (risolve il caso in cui ci siano più metacaratteri nella regexp) ]
    if(!isalnum(regexp[i]))
        for(j=0; j<err; j++){
            if(strstr(regexp+i, caratteri[j])==regexp+i)
                return (meta=j);
        }

    return meta;

}


//
// Created by Gabriele Cirotto on 04/11/2019.
//

