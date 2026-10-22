#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int x;
    int y;
} Square;

typedef struct {
    Square c[8];
    int nb;
} Liste;

int moveX[8] = {2, 1, -1, -2, -2, -1, 1, 2};
int moveY[8] = {1, 2, 2, 1, -1, -2, -2, -1};

Liste disponibles(int n, Square p)
{
    Liste l;
    int k, nx, ny;
    l.nb = 0;
    for (k = 0; k < 8; k++) {
        nx = p.x + moveX[k];
        ny = p.y + moveY[k];
        if (nx >= 0 && nx < n && ny >= 0 && ny < n) {
            l.c[l.nb].x = nx;
            l.c[l.nb].y = ny;
            l.nb++;
        }
    }
    return l;
}

int estVisitee(Square pile[], int top, Square p)
{
    int i;
    for (i = 0; i <= top; i++) {
        if (pile[i].x == p.x && pile[i].y == p.y)
            return 1;
    }
    return 0;
}

int compterSuivants(int n, Square p, Square pile[], int top)
{
    Liste l;
    int i, compteur;
    l = disponibles(n, p);
    compteur = 0;
    for (i = 0; i < l.nb; i++) {
        if (!estVisitee(pile, top, l.c[i]))
            compteur++;
    }
    return compteur;
}

Liste nonVisitees(int n, Square p, Square pile[], int top)
{
    Liste tous, res;
    int i, j, k;
    int nbSuivants[8];
    Square tmpSquare;
    int tmpNb;
    tous = disponibles(n, p);
    res.nb = 0;
    for (i = 0; i < tous.nb; i++) {
        if (!estVisitee(pile, top, tous.c[i])) {
            res.c[res.nb] = tous.c[i];
            nbSuivants[res.nb] = compterSuivants(n, tous.c[i], pile, top);
            res.nb++;
        }
    }
    for (j = 1; j < res.nb; j++) {
        tmpSquare = res.c[j];
        tmpNb = nbSuivants[j];
        k = j - 1;
        while (k >= 0 && nbSuivants[k] > tmpNb) {
            res.c[k + 1] = res.c[k];
            nbSuivants[k + 1] = nbSuivants[k];
            k--;
        }
        res.c[k + 1] = tmpSquare;
        nbSuivants[k + 1] = tmpNb;
    }
    return res;
}

void supprimerPremier(Liste *l)
{
    int i;
    for (i = 0; i < l->nb - 1; i++)
        l->c[i] = l->c[i + 1];
    l->nb--;
}

int main()
{
    Square *pile1;
    Liste *pile2;
    int *t;
    int top1, top2;
    int n, i, j;
    Square depart, suivant;

    printf("Enter the board size n: ");
    scanf("%d", &n);
    printf("Enter the starting row and column (1 to %d): ", n);
    scanf("%d %d", &depart.x, &depart.y);
    depart.x--;
    depart.y--;

    if (n < 1 || depart.x < 0 || depart.x >= n || depart.y < 0 || depart.y >= n) {
        printf("Invalid input\n");
        return 1;
    }

    pile1 = malloc(n * n * sizeof(Square));
    pile2 = malloc(n * n * sizeof(Liste));
    t = malloc(n * n * sizeof(int));
    if (pile1 == NULL || pile2 == NULL || t == NULL) {
        printf("Not enough memory\n");
        return 1;
    }

    printf("Searching for a solution...\n");

    top1 = 0;
    top2 = 0;
    pile1[0] = depart;
    pile2[0] = nonVisitees(n, depart, pile1, top1);

    while (top1 >= 0 && top1 < n * n - 1) {
        if (pile2[top2].nb > 0) {
            suivant = pile2[top2].c[0];
            top1++;
            pile1[top1] = suivant;
            top2++;
            pile2[top2] = nonVisitees(n, suivant, pile1, top1);
        } else {
            top1--;
            top2--;
            if (top2 >= 0)
                supprimerPremier(&pile2[top2]);
        }
    }

    if (top1 < 0) {
        printf("No solution found\n");
    } else {
        printf("Success: the knight visited all %d squares\n", n * n);
        for (i = 0; i < n * n; i++)
            t[i] = 0;
        for (i = 0; i <= top1; i++)
            t[pile1[i].x * n + pile1[i].y] = i + 1;
        for (i = 0; i < n; i++) {
            for (j = 0; j < n; j++)
                printf("%4d", t[i * n + j]);
            printf("\n");
        }
    }

    free(pile1);
    free(pile2);
    free(t);
    return 0;
}