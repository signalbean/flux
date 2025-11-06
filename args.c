#include "args.h"
#include <stdio.h>
#include <string.h>

void usage(const char* program_name) {
    printf("Usage: %s [OPTIONS] <command> <url> [output_file]\n\n", program_name);
    printf("Commands:\n");
    printf("  download, -d <url> [output_file]  Download file from URL\n");
    printf("  upload, -u <file> <url>           Upload file to URL\n\n");
    printf("Options:\n");
    printf("  --no-resume, -n               Disable resume functionality\n");
    printf("  --version, -v                 Show version information\n");
    printf("  --help, -h                    Show this help message\n");
}

void version(void) {
    printf("flux version %s\n", FLUX_VERSION);
}

int parse(int argc, char* argv[], char** command, char** arg1, char** arg2, bool* resume) {
    if (argc < 3) {
        usage(argv[0]);
        return 1;
    }

    *resume = true;
    int arg_offset = 1;

    // Parse options
    while (arg_offset < argc && argv[arg_offset][0] == '-') {
        // Check for command aliases first
        if (strcmp(argv[arg_offset], "-d") == 0 || strcmp(argv[arg_offset], "-u") == 0) {
            break; // This is a command alias, not an option
        } else if (strcmp(argv[arg_offset], "--no-resume") == 0 || strcmp(argv[arg_offset], "-n") == 0) {
            *resume = false;
        } else if (strcmp(argv[arg_offset], "--version") == 0 || strcmp(argv[arg_offset], "-v") == 0) {
            version();
            return 0;
        } else if (strcmp(argv[arg_offset], "--help") == 0 || strcmp(argv[arg_offset], "-h") == 0) {
            usage(argv[0]);
            return 0;
        } else {
            printf("Unknown option: %s\n", argv[arg_offset]);
            usage(argv[0]);
            return 1;
        }
        arg_offset++;
    }

    if (argc - arg_offset < 2) {
        usage(argv[0]);
        return 1;
    }

    // Handle command aliases
    char* cmd = argv[arg_offset];
    if (strcmp(cmd, "-d") == 0) {
        *command = "download";
    } else if (strcmp(cmd, "-u") == 0) {
        *command = "upload";
    } else {
        *command = cmd;
    }
    
    *arg1 = argv[arg_offset + 1];
    *arg2 = (argc - arg_offset >= 3) ? argv[arg_offset + 2] : NULL;

    return -1; // Continue processing
}
