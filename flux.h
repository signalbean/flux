#pragma once

#include <curl/curl.h>
#include <stdbool.h>

typedef struct {
    CURL* curl;
} FluxTool;

// Tool management
FluxTool* flux_create(void);
void flux_destroy(FluxTool* tool);

// Operations
bool download_file(FluxTool* tool, const char* url, const char* output_file, bool resume);
bool upload_file(FluxTool* tool, const char* local_file, const char* url);
