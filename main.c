#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "flux.h"
#include "args.h"

int main(int argc, char* argv[]) {
    char *command, *arg1, *arg2;
    bool resume;
    
    int parse_result = parse_args(argc, argv, &command, &arg1, &arg2, &resume);
    if (parse_result != -1) {
        return parse_result;
    }

    FluxTool* tool = flux_create();
    if (!tool) {
        printf("Failed to initialize flux tool\n");
        return 1;
    }

    int result = 1;
    
    if (strcmp(command, "download") == 0) {
        const char* output_file = arg2;
        
        if (!output_file) {
            // Extract filename from URL
            const char* pos = strrchr(arg1, '/');
            output_file = pos ? pos + 1 : "downloaded_file";
        }
        
        result = download_file(tool, arg1, output_file, resume) ? 0 : 1;
        
    } else if (strcmp(command, "upload") == 0) {
        if (!arg2) {
            printf("Upload command requires local file and URL\n");
        } else {
            result = upload_file(tool, arg1, arg2) ? 0 : 1;
        }
        
    } else {
        printf("Unknown command: %s\n", command);
        print_usage(argv[0]);
    }
    
    flux_destroy(tool);
    return result;
}
