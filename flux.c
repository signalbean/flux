#include "flux.h"
#include "utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

FluxTool* create(void) {
    FluxTool* tool = malloc(sizeof(FluxTool));
    if (!tool) return NULL;
    
    curl_global_init(CURL_GLOBAL_DEFAULT);
    tool->curl = curl_easy_init();
    
    if (!tool->curl) {
        free(tool);
        return NULL;
    }
    
    return tool;
}

void destroy(FluxTool* tool) {
    if (tool) {
        if (tool->curl) curl_easy_cleanup(tool->curl);
        free(tool);
    }
    curl_global_cleanup();
}

bool download(FluxTool* tool, const char* url, const char* output_file, bool resume) {
    if (!tool || !tool->curl) return false;

    FILE* file;
    curl_off_t resume_from = 0;
    
    if (resume) {
        struct stat st;
        if (stat(output_file, &st) == 0) {
            resume_from = st.st_size;
            file = fopen(output_file, "ab");
            printf("Resuming from %lld bytes\n", (long long)resume_from);
        } else {
            file = fopen(output_file, "wb");
        }
    } else {
        file = fopen(output_file, "wb");
    }
    
    if (!file) return false;

    ProgressData progress_data = {0, resume_from};

    curl_easy_setopt(tool->curl, CURLOPT_URL, url);
    curl_easy_setopt(tool->curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(tool->curl, CURLOPT_WRITEDATA, file);
    curl_easy_setopt(tool->curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(tool->curl, CURLOPT_XFERINFOFUNCTION, progress_callback);
    curl_easy_setopt(tool->curl, CURLOPT_XFERINFODATA, &progress_data);
    curl_easy_setopt(tool->curl, CURLOPT_NOPROGRESS, 0L);
    curl_easy_setopt(tool->curl, CURLOPT_USERAGENT, "flux/1.0");
    
    if (resume_from > 0) {
        curl_easy_setopt(tool->curl, CURLOPT_RESUME_FROM_LARGE, resume_from);
    }

    printf("Downloading: %s\n", url);

    CURLcode res = curl_easy_perform(tool->curl);
    printf("\n");
    fclose(file);

    if (res != CURLE_OK) {
        printf("Download failed: %s\n", curl_easy_strerror(res));
        return false;
    }

    long response_code;
    curl_easy_getinfo(tool->curl, CURLINFO_RESPONSE_CODE, &response_code);
    
    if (response_code >= 200 && response_code < 300) {
        printf("Download completed!\n");
        return true;
    } else {
        printf("HTTP error: %ld\n", response_code);
        return false;
    }
}

bool upload(FluxTool* tool, const char* local_file, const char* url) {
    if (!tool || !tool->curl) return false;

    struct stat st;
    if (stat(local_file, &st) != 0) {
        printf("File not found: %s\n", local_file);
        return false;
    }

    FILE* file = fopen(local_file, "rb");
    if (!file) return false;

    ProgressData progress_data = {0, 0};

    curl_easy_setopt(tool->curl, CURLOPT_URL, url);
    curl_easy_setopt(tool->curl, CURLOPT_UPLOAD, 1L);
    curl_easy_setopt(tool->curl, CURLOPT_READFUNCTION, read_callback);
    curl_easy_setopt(tool->curl, CURLOPT_READDATA, file);
    curl_easy_setopt(tool->curl, CURLOPT_INFILESIZE_LARGE, st.st_size);
    curl_easy_setopt(tool->curl, CURLOPT_XFERINFOFUNCTION, progress_callback);
    curl_easy_setopt(tool->curl, CURLOPT_XFERINFODATA, &progress_data);
    curl_easy_setopt(tool->curl, CURLOPT_NOPROGRESS, 0L);
    curl_easy_setopt(tool->curl, CURLOPT_USERAGENT, "flux/1.0");

    printf("Uploading: %s\n", local_file);

    CURLcode res = curl_easy_perform(tool->curl);
    printf("\n");
    fclose(file);

    if (res != CURLE_OK) {
        printf("Upload failed: %s\n", curl_easy_strerror(res));
        return false;
    }

    long response_code;
    curl_easy_getinfo(tool->curl, CURLINFO_RESPONSE_CODE, &response_code);
    
    if (response_code >= 200 && response_code < 300) {
        printf("Upload completed!\n");
        return true;
    } else {
        printf("HTTP error: %ld\n", response_code);
        return false;
    }
}
