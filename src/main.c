#include <stdio.h>

int main(int argc, char *argv[])
{
    #pragma omp parallel default(none)
    {
        printf("Hello, World!\n");
    }
    return 0;
}
