#pragma once

#include <stdbool.h>

void print_usage(const char* program_name);
int parse_args(int argc, char* argv[], char** command, char** arg1, char** arg2, bool* resume);
