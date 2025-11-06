#pragma once

#include <stdbool.h>

void usage(const char* program_name);
int parse(int argc, char* argv[], char** command, char** arg1, char** arg2, bool* resume);
