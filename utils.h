#pragma once

#include <stdio.h>
#include <curl/curl.h>

// File I/O callbacks
size_t write_callback(void* contents, size_t size, size_t nmemb, void* file);
size_t read_callback(char* buffer, size_t size, size_t nitems, void* file);

// Progress tracking
typedef struct {
    long last_time;
    curl_off_t last_bytes;
} ProgressData;

int progress_callback(void* clientp, curl_off_t dltotal, curl_off_t dlnow, 
                     curl_off_t ultotal, curl_off_t ulnow);
