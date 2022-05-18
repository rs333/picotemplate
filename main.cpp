#include "Runner.h"

int main(int argc, char **argv)
{
    Runner &run = Runner::getInstance();

    run.init({.step_size_us = 10000});

    return run.run();
}
