#include <stdio.h>

int majority(int *a, int N);

int maggioritario(int *a, int l, int r);

int contaMaj(int *a, int num, int l, int r);

int main() {
    int N1 = 7, N2 = 8;

    //Caso elemento maggioritario = 3
    int v1[7] = {3, 3, 9, 4, 3, 5, 3};
    int x1 = majority(v1, N1);
    if (x1 == -1) {
        printf("Non ci sono elementi maggioritari.\n");
    } else {
        printf("L'elemento maggioritario e': %d\n", x1);
    }

    //Caso in cui non esiste l'elemento maggioritario
    int v2[8] = {0, 1, 0, 2, 3, 4, 0, 5};
    int x2 = majority(v2, N2);
    if (x2 == -1) {
        printf("Non ci sono elementi maggioritari.\n");
    } else {
        printf("L'elemento maggioritario e': %d\n", x2);
    }


    return 0;
}

int majority(int *a, int N) {
    int l = 0, r = N - 1;
    int cnt = 0;
    int x = maggioritario(a, l, r);

    //La funzione maggioritario ritorna solo il numero che ha più occorrenze nel vettore, quindi è necessario scorrerlo (al massimo) tutto per vedere se questo è l'elemento maggioritario
    for (int i = 0; i < N; i++) {
        if (x == a[i]) {
            cnt++;
        }
        if (cnt > N / 2) {
            return x;
        }
    }

    //Se non ci sono elementi maggioritari la funzione ritorna -1
    return -1;

}

int maggioritario(int *a, int l, int r) {

    //Caso semplice: in un vettore da un solo elemento, questo sarà quello maggioritario
    if (l == r) {
        return a[l];
    }

    //Ricorsione sul sottovettore sinistro e destro
    int q = (l + r) / 2;
    int left = maggioritario(a, l, q);
    int right = maggioritario(a, q + 1, r);

    //Se le due metà hanno lo stesso elemento maggioritario lo ritornano
    if (left == right) {
        return left;
    }

    //In caso contrario conta quanti elementi maggioritari ci sono nel sottovettore sinistro e quanti nel sottovettore destro
    int ContaLeft = contaMaj(a, left, l, r);
    int ContaRight = contaMaj(a, right, l, r);

    //Ritorna il vincitore tra i due possibili maggioritari(se hanno lo stesso numero di occorrenze ritorna il primo che ha trovato, cioè il sinistro)
    return ContaLeft >= ContaRight ? left : right;

}

int contaMaj(int *a, int num, int l, int r) {
    int i, cnt = 0;
    //Conta quante volte occorre il presunto elemento maggioritario all'interno del sottovettore
    for (i = l; i <= r; i++) {
        if (a[i] == num) {
            cnt++;
        }
    }
    return cnt;
}

//
// Created by Gabriele Cirotto on 02/11/2019.
//

