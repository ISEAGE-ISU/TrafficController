#include "FileOps.h"

size_t CDC::FileOps::write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    return fwrite(ptr, size, nmemb, stream);
}

bool CDC::FileOps::tftp_download(const char *url, const char *save_file) {
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    if (curl) {

        FILE *fp;
        fp = fopen(save_file, "wb");
        if (fp == NULL) {
            curl_easy_cleanup(curl);
            return false;
        }
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, FileOps::write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            return false;
        }
        curl_easy_cleanup(curl);
        fclose(fp);
    }
    return true;
}
