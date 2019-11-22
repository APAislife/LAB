#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAXC 31 //Numero massimo di caratteri per parola del file corse.txt (30 + '\0')
#define MAXRIGHE 1000 //Numero massimo di righe nel file corse.txt

//Codifica dei comandi mediante tipo enum (r_date = 0 ..... r_err = 7 )
typedef enum {
    r_stampa, r_ord_data, r_ord_tratta, r_ord_partenza, r_ord_arrivo, r_partenza, r_fine, r_err
} comando_e;

typedef struct riga{             //Definizione di una struct riga che è composta da 6 stringhe e un intero
    char codice_tratta[MAXC];
    char partenza[MAXC];
    char destinazione[MAXC];
    char data[MAXC];
    char ora_partenza[MAXC];
    char ora_arrivo[MAXC];
    int ritardo;
}riga;

comando_e leggiComando(void);
void selezionaDati(riga **el1, riga **el2, riga **el3, riga **el4, riga **el5, int numero_tratte);
void strToLower(char parola[]);
void stampa(riga **elenco, int numero_tratte);
void ordData(riga **elenco, int numero_tratte);
void ordTratta(riga **elenco, int numero_tratte);
void ordPartenza(riga **elenco, int numero_tratte);
void ordArrivo(riga **elenco, int numero_tratte);
int confrDate(char s1[], char s2[]);
int confrOra(char s1[], char s2[]);
void CercaPartenzaLin(riga **elenco, int numero_tratte, char *parola);
void CercaPartenzaBin(riga **elenco, int numero_tratte, char *parola);

int main(){

    FILE* fpin; //Puntatore al file in input
    int dim_tabella;
    int i;
    char rig[MAXC]; //
    riga tab[MAXRIGHE]; //Vettore di struct di 1000 elementi
    riga *eldata[MAXRIGHE], *elpart[MAXRIGHE], *elarr[MAXRIGHE], *eltrat[MAXRIGHE], *elenco[MAXRIGHE];

    //Apertura file con eventuale segnalazione di errori
    if((fpin=fopen("corse.txt", "r"))==NULL){
        fprintf(stderr, "Errore nell'apertura del file corse.txt\n");
        return EXIT_FAILURE;
    }

    //Lettura prima riga del file
    fscanf(fpin, "%d", &dim_tabella);

    //Creazione tabella per contenere tutte le righe del file
    for(i=0; i<dim_tabella; i++){
        fscanf(fpin, "%s%s%s%s%s%s%d", tab[i].codice_tratta, tab[i].partenza, tab[i].destinazione, tab[i].data, tab[i].ora_partenza, tab[i].ora_arrivo, &tab[i].ritardo);
        eldata[i] = elpart[i]= elarr[i] = eltrat[i]= elenco[i] = &tab[i];
    }
    fclose(fpin);


    selezionaDati(elenco, eldata, elpart, elarr, eltrat, dim_tabella);

    return 0;
}

comando_e leggiComando(void){
    comando_e c;
    char command[MAXC]; //Stringa che conterrà il comando immesso da tastiera

    //Vettore che contiene puntatori alle stringhe costanti che saranno i comandi immessi dall'utente
    char *tabella[r_err] ={"stampa", "ord_data", "ord_tratta", "ord_partenza", "ord_arrivo", "partenza", "fine"};

    printf("Comando (stampa/ord_data/ord_tratta/ord_partenza/ord_arrivo/partenza/fine): ");
    scanf("%s", command);
    printf("\n");
    strToLower(command); //Comando che rende minuscole tutte le lettere del comando immesso dall'utente
    c=r_stampa; // Parte dal primo comando
    while(c<r_err && strcmp(command, tabella[c])!=0){
        c++; //Finchè il comando immesso non è uguale a quello della tabella o non finiscono i comandi si aumenta c
    }
    return (c);
}

void strToLower(char parola[]){ //Funzione che rende minuscola un'intera parola
    int i;
    for (i=0; parola[i]!='\0'; i++){
        parola[i]=tolower(parola[i]);
    }
}

void selezionaDati(riga **el1, riga **el2, riga **el3, riga **el4, riga **el5, int numero_tratte){
    comando_e codiceComando;
    int flag=0;
    int continua=1;
    char parola[MAXC];

    while(continua){
        codiceComando=leggiComando();
        riga **attuale=el1;

        switch (codiceComando){
            case r_stampa:

                stampa(attuale, numero_tratte);
                printf("\n");


                break;

            case r_ord_data:

                ordData(el2, numero_tratte);
                printf("Ordinamento attuale: per data.\n");
                flag=0;


                break;

            case r_ord_tratta:

                ordTratta(el5, numero_tratte);
                printf("Ordinamento attuale: per tratta.\n");
                flag=0;


                break;

            case r_ord_partenza:

                ordPartenza(el3, numero_tratte);
                printf("Ordinamento attuale: per partenza.\n");
                flag=1;

                break;

            case r_ord_arrivo:

                ordArrivo(el4, numero_tratte);
                printf("Ordinamento attuale: per arrivo.\n");
                flag=0;

                break;

            case r_partenza:

                printf("Inserire luogo di partenza: ");
                scanf("%s", parola);
                printf("\n\n");

                if(flag==0){
                    //Ricerca lineare
                    CercaPartenzaLin(attuale, numero_tratte, parola);
                } else {
                    //Ricerca dicotomica
                    CercaPartenzaBin(attuale, numero_tratte, parola);
                }

                break;

            case r_fine: continua=0;
                break;

            case r_err:
            default: printf("Comando errato!\n");
        }
    }
}

void stampa(riga **elenco, int numero_tratte){
    int i;
    for(i=0; i<numero_tratte; i++){
    printf("%s %s %s %s %s %s %d\n", elenco[i]->codice_tratta, elenco[i]->partenza, elenco[i]->destinazione, elenco[i]->data,
                                                elenco[i]->ora_partenza, elenco[i]->ora_arrivo, elenco[i]->ritardo);
    }

}

void ordData (riga **elenco, int numero_tratte) {
    riga *temp;
    int i, j, flag=1, res;

    for(i=0; i<numero_tratte-1 && flag==1; i++){
        flag=0;
        for(j=0; j<numero_tratte-1-i; j++){
            res=confrDate(elenco[j]->data, elenco[j+1]->data);
            if(res==0){
                if(confrOra(elenco[j]->ora_partenza, elenco[j+1]->ora_partenza)>0){
                    flag=1;
                    temp=elenco[j];
                    elenco[j]=elenco[j+1];
                    elenco[j+1]=temp;
                }
            } else if (res<0){
                flag=1;
                temp=elenco[j];
                elenco[j]=elenco[j+1];
                elenco[j+1]=temp;
            }

        }
    }


}

int confrDate(char s1[], char s2[]){
    int a1, m1, g1, a2, m2, g2;
    sscanf(s1, "%d/%d/%d", &g1, &m1, &a1);
    sscanf(s2, "%d/%d/%d", &g2, &m2, &a2);

    if(a1==a2){
        if(m1==m2){
            if(g1==g2){
                return 0;
            } else if(g1>g2){
                return -1;
            } else {
                return 1;
            }
        } else if (m1>m2){
            return -1;
        } else {
            return 1;
        }
    } else if (a1>a2){
        return -1;
    } else {
        return 1;
    }
}

int confrOra(char s1[], char s2[]){
    int h1, m1, h2, m2;
    sscanf(s1, "%d:%d", &h1, &m1);
    sscanf(s2, "%d:%d", &h2, &m2);

    h1=h1*60;
    h2=h2*60;

    return (h1+m1)-(h2+m2);
}

void ordTratta(riga **elenco, int numero_tratte){
    riga *temp;
    int i,j;

    for(i=1; i<numero_tratte; i++){
        temp=elenco[i];
        j=i-1;
        while(j>=0 && strcmp(temp->codice_tratta, elenco[j]->codice_tratta)<0){
            elenco[j+1]=elenco[j];
            j--;
        }
        elenco[j+1]=temp;
    }
}

void ordPartenza(riga **elenco, int numero_tratte){
    riga *temp;
    int i,j;

    for(i=1; i<numero_tratte; i++){
        temp=elenco[i];
        j=i-1;
        while(j>=0 && strcmp(temp->partenza, elenco[j]->partenza)<0){
            elenco[j+1]=elenco[j];
            j--;
        }
        elenco[j+1]=temp;
    }
}

void ordArrivo(riga **elenco, int numero_tratte){
    riga *temp;
    int i,j;

    for(i=1; i<numero_tratte; i++){
        temp=elenco[i];
        j=i-1;
        while(j>=0 && strcmp(temp->destinazione, elenco[j]->destinazione)<0){
            elenco[j+1]=elenco[j];
            j--;
        }
        elenco[j+1]=temp;
    }
}

void CercaPartenzaLin(riga **elenco, int numero_tratte, char *parola){
    int i=0;
    int found=0;

    while(i<numero_tratte && found!=1){
        if(strstr(elenco[i]->partenza, parola)!=NULL) {
            printf("%s %s %s %s %s %s %d\n", elenco[i]->codice_tratta, elenco[i]->partenza, elenco[i]->destinazione, elenco[i]->data,
                                                elenco[i]->ora_partenza, elenco[i]->ora_arrivo, elenco[i]->ritardo);
            found=1;
        }else {
            i++;
        }
    }

}

void CercaPartenzaBin(riga **elenco, int numero_tratte, char *parola){
    int m;
    int l=0, r=numero_tratte-1;

    while(l<=r){
        m=(l+r)/2;
        if(strstr(elenco[m]->partenza, parola)!=NULL){
            printf("%s %s %s %s %s %s %d\n", elenco[m]->codice_tratta, elenco[m]->partenza, elenco[m]->destinazione, elenco[m]->data,
                   elenco[m]->ora_partenza, elenco[m]->ora_arrivo, elenco[m]->ritardo);
        }
        if(strcmp(elenco[m]->partenza, parola)<0){
            l=m+1;
        } else {
            r=m-1;
        }
    }
}









//
// Created by Gabriele Cirotto on 07/11/2019.
//

