#pragma once

#include <curl/curl.h>
#include <stdbool.h>

typedef struct {
    CURL* curl;
} FluxTool;

// Tool management
FluxTool* create(void);
void destroy(FluxTool* tool);

// Operations
bool download(FluxTool* tool, const char* url, const char* output_file, bool resume);
bool upload(FluxTool* tool, const char* local_file, const char* url);
