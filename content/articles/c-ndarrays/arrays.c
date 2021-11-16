#include <stdlib.h>
#include <stdio.h>

// Note: this is standard C99
void foo(int m, int n, int x[m][n]) {
    // memset would be more appropriate to set the entire array to zero
    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++) {
            x[i][j] = 0;
        }
}

// Same as above but the length parameters are forward-declared
// Warning: that's a GCC extension
void bar(int m, int n; int x[m][n], int m, int n) {
    int k = m < n ? m : n;

    for (int i = 0; i < k; i++)
        x[i][i] = 1;
}

void print(int m, int n; int x[m][n], int m, int n) {
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++) {
            printf("% 4d ", x[i][j]);
        }
        printf("\n");
    }
}

void baz(int x[2][2]) {
    x[0][0] = 42;
}


int main() {
    int m = rand() % 10 + 1;
    int n = rand() % 10 + 1;

    printf("m = %d, n = %d\n", m, n);

    int x[m][n]; // allocated on the stack, equivalent to an alloca()
    int (*y)[n] = malloc(m * n * sizeof(y[0][0])); // this one is allocated in the heap
    int (*z)[m][n] = malloc(sizeof(*z)); // here for exhaustivity, not exactly the same semantic, nor the same usage

    // m == sizeof(x) / sizeof(x[0])
    // n == sizeof(x[0]) / sizeof(x[0][0])
    int size = sizeof(x) / sizeof(x[0][0]);

    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++) {
            int val = i * n + j;

            x[i][j] = val;
            y[i][j] = size + val;
            (*z)[i][j] = 2*size + val;
        }

    printf("x = \n");
    print(x, m, n);

    foo(m, n, x);
    printf("foo(x) =>\n");
    print(x, m, n);

    printf("y = \n");
    print(y, m, n);

    bar(y, m, n);
    printf("bar(y) =>\n");
    print(y, m, n);

    printf("z = \n");
    print(*z, m, n);

    foo(m, n, *z);
    bar(*z, m, n);
    printf("bar(foo(z)) =>\n");
    print(*z, m, n);

    free(y);
    free(z);

    return 0;
}
