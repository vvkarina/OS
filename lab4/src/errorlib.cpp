#include "errorlib.h"

int Oerror(const char * error, int id) {
    write(STDERR_FILENO, error, strlen(error));
    exit(id);
}
