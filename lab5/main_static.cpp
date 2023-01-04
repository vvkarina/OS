#include "..//lab5/include/lib.h"
#include <stdio.h>

int main() {
    int command = 0;
    printf("To compute count of prime number on [A, B] enter -- 1.Args: start finish\n");
    printf("To compute E enter -- 2.Args: argument\n");

    while (scanf("%d", &command) != EOF) {
        switch (command) {
            case 1: {
                int start, finish;

                if (scanf("%d %d", &start, &finish) == 2) {
                    printf("Count: %d\n", PrimeCount(start, finish));
                }

                break;
            }
            case 2: {
                int argument;

                if (scanf("%d", &argument) == 1) {
                    printf("E is: %f\n", E(argument));
                }

                break;
            }
            default: {
                printf("This command is not supported, enter 1 or 2\n");
            }
        }
    }

    return 0;
}
