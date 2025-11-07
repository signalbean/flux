#pragma once

#include <stdbool.h>

#define VERSION "0.1.0"

void usage(const char* program_name);
void version(void);
int parse(int argc, char* argv[], char** command, char** arg1, char** arg2, bool* resume);
