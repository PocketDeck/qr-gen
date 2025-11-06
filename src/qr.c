#include <stdio.h>

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <string>\n", argv[0]);
        return 1;
    }

    fprintf(stderr, "Input string: %s\n", argv[1]);

    return 0;
}