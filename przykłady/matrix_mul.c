#include <stdio.h>
#include <stdlib.h>

void mnoz(double**A, int a, int b, double**B, int c, int d, double**C)
{
    int i, j, k;
    double s;
    for(i=0; i<a; i++)
    {
        for(j=0; j<d; j++)
        {
            s = 0;
            for(k=0; k<b; k++)
            {
                s+=A[i][k]*B[k][j];
            }
            C[i][j] = s;
        }

    }
}

print_matrix(double**A, int m, int n)
{
    int i, j;
    printf("[");
    for(i =0; i< m; i++)
    {
        for(j=0; j<n; j++)
        {
            printf("%f ", A[i][j]);
        }
        printf("\n");
    }
    printf("]\n");
}
int main()
{
    FILE *fpa;
    FILE *fpb;
    double **A;
    double **B;
    double **C;
    int ma, mb, na, nb;
    int i, j;
    double x;

    fpa = fopen("A.txt", "r");
    fpb = fopen("B.txt", "r");
    if( fpa == NULL || fpb == NULL )
    {
        perror("błąd otwarcia pliku");
        exit(-10);
    }

    fscanf (fpa, "%d", &ma);
    fscanf (fpa, "%d", &na);


    fscanf (fpb, "%d", &mb);
    fscanf (fpb, "%d", &nb);

    printf("pierwsza macierz ma wymiar %d x %d, a druga %d x %d\n", ma, na, mb, nb);

    if(na != mb)
    {
        printf("Złe wymiary macierzy!\n");
        return EXIT_FAILURE;
    }
    
    /*Alokacja pamięci*/
    A = malloc(ma*sizeof(double));
    for(i=0; i< ma; i++)
    {
        A[i] = malloc(na*sizeof(double));
    }

    B = malloc(mb*sizeof(double));
    for(i=0; i< mb; i++)
    {
        B[i] = malloc(nb*sizeof(double));
    }

    /*Macierz na wynik*/
    C = malloc(ma*sizeof(double));
    for(i=0; i< ma; i++)
    {
        C[i] = malloc(nb*sizeof(double));
    }

    printf("Rozmiar C: %dx%d\n", ma, nb);
    for(i =0; i< ma; i++)
    {
        for(j = 0; j<na; j++)
        {
            fscanf( fpa, "%lf", &x );
            A[i][j] = x;
        }
    }

    printf("A:\n");
    print_matrix(A, ma, mb);

    for(i =0; i< mb; i++)
    {
        for(j = 0; j<nb; j++)
        {
            fscanf( fpb, "%lf", &x );
            B[i][j] = x;
        }
    }

    printf("B:\n");
    print_matrix(B, mb, nb);

    mnoz(A, ma, na, B, mb, nb, C);

    printf("C:\n");
    print_matrix(C, ma, nb);


    for(i=0; i<na; i++)
    {
        free(A[i]);
    }
    free(A);

    for(i=0; i<nb; i++)
    {
        free(B[i]);
    }
    free(B);

    for(i=0; i<nb; i++)
    {
        free(C[i]);
    }
    free(C);

   
    fclose(fpa);
    fclose(fpb);


    return 0;
}