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
    
    if (total > 0 && (now - progress_data->last_time) >= 1) {
        double percentage = ((double)current / total) * 100.0;
        
        // Clear the entire line first
        printf("\r\033[K");
        
        printf("[");
        int pos = (int)(30 * percentage / 100.0);
        for (int i = 0; i < 30; ++i) {
            if (i < pos) printf("=");
            else if (i == pos) printf(">");
            else printf(" ");
        }
        
        const char* units[] = {"B", "KB", "MB", "GB"};
        double size = (double)current;
        int unit = 0;
        while (size >= 1024.0 && unit < 3) {
            size /= 1024.0;
            unit++;
        }
        
        printf("] %.1f%% %.1f %s", percentage, size, units[unit]);
        fflush(stdout);
        
        progress_data->last_time = now;
    }
    return 0;
}
