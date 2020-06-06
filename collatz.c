#include <stdio.h>
#include <cs50.h>

int collatz(int n);

int main(void)
{
    int n = get_int("n: ");
    printf("No. of steps: %d\n", collatz(n));

}

int collatz (int n)
{
    // base case
    if (n == 1) {
        return 0;
    }
    // even numbers
    else if (n%2 == 0){
        return 1 + collatz(n/2);
    // odd numbers
    } else {
        return 1 + collatz(3*n+1);
    }

}