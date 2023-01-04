#include <array>
#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <dlfcn.h>

typedef enum
{
    first_contract,
    second_contract,
} contracts;

contracts contract = first_contract;
int (*PrimeCount)(int, int);
float (*E)(int);
void *library_handle;

void load_library(contracts contract)
{
    const char *first_library_name = "./libd1_dynamic.so";
    const char *second_library_name = "./libd2_dynamic.so";
    const char *library_name;
    switch (contract)
    {
    case first_contract:
    {
        library_name = first_library_name;
        break;
    }
    case second_contract:
    {
        library_name = second_library_name;
        break;
    }
    }
    library_handle = dlopen(library_name, RTLD_LAZY);

    if (library_handle == NULL)
    {
        perror("dlopen error");
        exit(EXIT_FAILURE);
    }
    PrimeCount = (int (*)(int, int))dlsym(library_handle, "PrimeCount");
    printf("Count: %d\n", PrimeCount(5, 8));
    E = (float (*)(int))dlsym(library_handle, "E");
}

void change_contract()
{
    dlclose(library_handle);
    switch (contract)
    {
    case first_contract:
    {
        contract = second_contract;
        break;
    }
    case second_contract:
    {
        contract = first_contract;
        break;
    }
    }
    load_library(contract);
}

int main()
{
    load_library(contract);
    int command = 0;
    printf("To compute count of prime number on [A, B] enter -- 1.Args: start finish\n");
    printf("To compute E enter -- 2.Args: argument\n");
    printf("To change contract enter -- 0\n");
    while (scanf("%d", &command) != EOF)
    {
        switch (command)
        {
        case 0:
        {
            change_contract();
            printf("Contract has been changed\n");
            switch (contract)
            {
            case first_contract:
            {
                printf("Contract is first\n");
                break;
            }
            case second_contract:
            {
                printf("Contract is second\n");
                break;
            }
            }
            break;
        }
        case 1:
        {
            int start, finish;
            if (scanf("%d %d", &start, &finish) == 2)
            {
                printf("Count: %d\n", PrimeCount(start, finish));
            }
            break;
        }
        case 2:
        {
            int argument;
            if (scanf("%d", &argument) == 1)
            {
                printf("E is: %f\n", (*E)(argument));
            }
            break;
        }
        default:
        {
            printf("This command is not supported, enter 1 or 2 or 0\n");
        }
        }
    }
    return 0;
}
