+++
title = "N-dimensional arrays in C"
date = "2012-04-09"
+++

The grammar of C is messy and some constructions can be counter-intuitive for 
beginners: this is the case of multidimensional arrays (non-programmers would 
call them tensors).

## Introduction

Someone new to low-level programming would be tempted to declare arrays with 
multiple dimensions in the following way:

```c
float*** make_3d_float_array(size_t x, size_t y, size_t z) {
    float*** A = malloc(x*sizeof(float**));

    for (...) {
        A[i] = malloc(... // I think you know the rest...
```

Even though the syntax for accessing cells is the same as for static arrays, 
that's not the best thing to do: unless you're planning to replace entire rows 
or planes more often than you do random accesses, this is inefficient. We need 
contiguous arrays without having to explicitly compute the indexes.
 
While learning C and experimenting with metaprogramming, I wrote [a few 
helpers](ndarray.h) using the preprocessor for dynamic 
multidimensional arrays. If you take a look at it without much knowledge of the 
C preprocessor, you'll probably see it as mostly voodoo. Fortunately C99 comes 
with syntactic sugar that makes this unnecessary. **Read on !**

## Static arrays

As a reminder, static arrays means we are putting the size of the array in its 
type. A static 2D arrays of integers can be declared and used as such:

```c
void foo(int x[4][2]) {
    // Do something with x.
}

int main() {
    int x[4][2];

    for (int i = 0; i < sizeof(x) / sizeof(x[0]); i++)
        for (int j = 0; j < sizeof(x) / sizeof(x[0][0]); j++)
            x[i][j] = rand();

    foo(x);
    // Don't be fooled by the syntax of foo's argument,
    // there's no copy of x involved!

    return 0;
}
```

Although the syntax may be deceiving, **the array is not copied when calling the 
function**. And there's no way to ask for an implicit copy.

What about dynamic arrays, for which the size may not be known at compile time ?

## Proper dynamic arrays

In C99, variable length arrays can be declared and allocated as follow:

```c
int m = rand() % 10 + 1;
int n = rand() % 10 + 1;

printf("m = %d, n = %d\n", m, n);

int x[m][n];
int (*y)[n] = malloc(m * n * sizeof(y[0][0]));
int (*z)[m][n] = malloc(sizeof(*z));
```

`x` is a dynamic (i.e. variable-length) array allocated on the stack. `y` and 
`z` are both pointers to chunks of memory allocated in the heap (which are the 
size of `x`) but the semantic slightly differs between the two: the type of the 
data `y` points to is *an array of `n` integers*, while `z` points to *an array 
of `m` arrays of `n` integers*.

If you're not comfortable with pointer arithmetic, you may want to read [this 
tutorial](http://www.cs.umd.edu/class/sum2003/cmsc311/Notes/BitOp/pointer.html).

Cell accesses are written as such:

```c
x[i][j] = 42;
y[i][j] = 42;
(*z)[i][j] = 42; // same as z[0][i][j] ;)
```

Even though the semantic of `z` is, in some way, closer to what we're 
expressing, its usage is less practical.

How can we write functions taking dynamic arrays as arguments ? This is were the 
real syntax tricks hide: we can put the lengths as arguments and use them in the 
type of the array. For more details on it, read this [GCC man 
page](https://gcc.gnu.org/onlinedocs/gcc/Variable-Length.html).

In plain C99:

```c
void foo(int m, int n, int x[m][n]) {
    // Do something with x.
}

// ...

foo(m, n, x);
foo(m, n, y);
foo(m, n, *z);
```

Again, the arrays are not copied.

Finally, GCC provides an elegant extension to put the size arguments after the 
array, which may lead to more readable code in some cases:

```c
void bar(int m, int n; int x[m][n], int m, int n) {
    // Do something with x.
}

// ...

bar(x, m, n);
```

A full working example of this is available [here](arrays.c).
