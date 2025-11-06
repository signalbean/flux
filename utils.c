#include "utils.h"
#include <stdio.h>
#include <time.h>

// File I/O callbacks
size_t write_callback(void* contents, size_t size, size_t nmemb, void* file) {
    return fwrite(contents, size, nmemb, (FILE*)file);
}

size_t read_callback(char* buffer, size_t size, size_t nitems, void* file) {
    return fread(buffer, size, nitems, (FILE*)file);
}

// Progress tracking
int progress_callback(void* clientp, curl_off_t dltotal, curl_off_t dlnow, 
                     curl_off_t ultotal, curl_off_t ulnow) {
    ProgressData* progress_data = (ProgressData*)clientp;
    long now = time(NULL);
    
    curl_off_t total = (dltotal > 0) ? dltotal : ultotal;
    curl_off_t current = (dlnow > 0) ? dlnow : ulnow;
    
    // Add resume offset to current progress for accurate total
    curl_off_t actual_current = current + progress_data->resume_from;
    curl_off_t actual_total = total + progress_data->resume_from;
    
    if (actual_total > 0 && (now - progress_data->last_time) >= 1) {
        double percentage = ((double)actual_current / actual_total) * 100.0;
        
        // Clear the entire line first
        printf("\r\033[K");
        
        printf("[");
        int pos = (int)(30 * percentage / 100.0);
        for (int i = 0; i < 30; ++i) {
            if (i < pos) printf("*");
            else printf(".");
        }
        
        // Format current size
        const char* units[] = {"B", "KB", "MB", "GB"};
        double current_size = (double)actual_current;
        int current_unit = 0;
        while (current_size >= 1024.0 && current_unit < 3) {
            current_size /= 1024.0;
            current_unit++;
        }
        
        // Format total size
        double total_size = (double)actual_total;
        int total_unit = 0;
        while (total_size >= 1024.0 && total_unit < 3) {
            total_size /= 1024.0;
            total_unit++;
        }
        
        printf("] %.1f%% %.1f %s / %.1f %s", 
               percentage, current_size, units[current_unit], 
               total_size, units[total_unit]);
        fflush(stdout);
        
        progress_data->last_time = now;
    }
    return 0;
}
