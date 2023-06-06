#include <stdio.h>

int main() {

    long double a = 0.1;
    long double b = 0.2;

    long double c = a + b;
    printf("%.160Lf\n",c);
    return 0;
}