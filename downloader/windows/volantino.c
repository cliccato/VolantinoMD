/*
Compiler instruction: gcc -Wall -o curl volantino.c -l curl
Skidded in part from https://dev.to/hi_artem/using-libcurl-in-c-c-application-4668
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

#ifndef UTIL_H
#define UTIL_H

struct MemoryStruct {
    char *memory;
    size_t size;
};

size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);

#endif

size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    char *ptr = realloc(mem->memory, mem->size + realsize + 1);

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

int main() {

    CURL *curl_handle;
    CURLcode res;
    const char url[100] = "https://mdspa2.cdn.hugecloud.eu/upload/presente/volantini/nord-p-g/volantino.pdf";

    struct MemoryStruct chunk;
    chunk.memory = malloc(1);  
    chunk.size = 0;

    curl_handle = curl_easy_init();
    curl_easy_setopt(curl_handle, CURLOPT_URL, url);
    curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);
    curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

    res = curl_easy_perform(curl_handle);

    if(res != CURLE_OK) {
        fprintf(stderr, "error: %s\n", curl_easy_strerror(res));
    } else {
        printf("Download eseguito");
    }
    curl_easy_cleanup(curl_handle);
    free(chunk.memory);

  return 0;
}