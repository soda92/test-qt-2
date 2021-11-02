#include <stdio.h>
#include <curl/curl.h>

void write_callback(void *data, size_t size, size_t nmemb, void *ptr)
{
    fwrite(data, size, nmemb, (FILE *)ptr);
}

int main()
{
    curl_global_init(CURL_GLOBAL_ALL);
    CURL *handle = curl_easy_init();
    // curl_easy_setopt(handle, CURLOPT_CUSTOMREQUEST, "NLST");
    curl_easy_setopt(handle, CURLOPT_DIRLISTONLY, 1L);

    // curl_easy_setopt(handle, CURLOPT_VERBOSE, 1L);
    curl_easy_setopt(handle, CURLOPT_NOPROXY, "*");
    curl_easy_setopt(handle, CURLOPT_URL, "ftp://127.0.0.1/lamp_sample/");
    curl_easy_setopt(handle, CURLOPT_USERNAME, "user");
    curl_easy_setopt(handle, CURLOPT_PASSWORD, "12345");
    FILE *f = fopen("D:/out.txt", "w");
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, f);

    curl_easy_perform(handle);
    fclose(f);
    curl_global_cleanup();
    return 0;
}