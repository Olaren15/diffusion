#include "engine.h"

#include <stdio.h>

int main(int argc, char** argv) {
    (void)argc;
    (void)argv;

    engine_t engine;

    bool success = engine_init(&engine);
    if (!success) {
        printf("An error occurred while initializing the engine\n");
        return 1;
    }

    engine_run(&engine);
    engine_destroy(&engine);

    return 0;
}
