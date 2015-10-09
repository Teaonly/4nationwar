#include "cpu.h"
#include "randman.h"
#include "simpleman.h"


CPU* CPU::createCPU(std::string &name, void *parameter) {
    name = name;
    parameter = parameter;
    CPU *cpu = new SimpleMan();
    return cpu;
}

