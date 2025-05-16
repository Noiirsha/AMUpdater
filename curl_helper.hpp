#pragma once

#include <curl/curl.h>
#include <iostream>

#include "utils.hpp"

CURL* curl;

void CurlDebugStringA(std::string str) {
#ifdef _DEBUG
    std::cout << "[ CURL] " + str << std::endl;
#endif // _DEBUG
}

void initCurl() {
    curl = curl_easy_init();
}

size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t total_size = size * nmemb;
    output->append((char*)contents, total_size);
    return total_size;
}

std::string Curl_Get(std::string url) {

    std::string response = "";

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        CURLcode res = curl_easy_perform(curl);

        if (res == CURLE_OK) {
            CurlDebugStringA(url + " : " + response);
            return response;
        }
        else {
            CurlDebugStringA(url + " -X> " + curl_easy_strerror(res));
        }
    }

    return response;
}

static size_t WriteFileCallback(void* ptr, size_t size, size_t nmemb, FILE* stream) {
    return fwrite(ptr, size, nmemb, stream);
}

struct progress {
    char* p;
    size_t size;
};

int progress_callback(void* clientp,
    double dltotal,
    double dlnow,
    double ultotal,
    double ulnow) {

    struct progress* memory = (struct progress*) clientp;

    if (dlnow > 0)
    {
        int downloadPrecentage = (dlnow / dltotal * 100);
        precentage = downloadPrecentage * 0.7;
        content_strings.overwriteLatest(outputNetworkStringA("File Download", std::to_string(downloadPrecentage) + "% (" + std::to_string(dlnow) + " / " + std::to_string(dltotal) + ")"));

        CurlDebugStringA("Download Progress: " + std::to_string(downloadPrecentage) + "% (" + std::to_string(dlnow) + " / " + std::to_string(dltotal) + ")");
    }

    return 0;
}

int Curl_GetFile(std::string url, std::string filePath) {

    if (curl) {
        struct progress data;
        content_strings.push(outputNetworkStringA("File Download", ""));

        FILE* file = fopen(filePath.c_str(), "wb");
        if (!file) return -1;

        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteFileCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);          

        curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, &data);
        curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, progress_callback);
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
        

        CURLcode res = curl_easy_perform(curl);

        long httpStatus = 0;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpStatus);
        if (httpStatus != 200 || res != CURLE_OK || (httpStatus >= 400 && httpStatus < 600)) {
            fclose(file);
            remove(filePath.c_str());
            content_strings.overwriteLatest(outputNetworkStringA("File Download", "Download Error"));
            curl_easy_cleanup(curl);
            return -1;
        }

        fclose(file);

        if (res != CURLE_OK) {
            remove(filePath.c_str()); 
            content_strings.overwriteLatest(outputNetworkStringA("File Download", "Download Error"));
            curl_easy_cleanup(curl);
            return -1;
        }
        else {
            content_strings.overwriteLatest(outputNetworkStringA("File Download", "Download Completed"));
        }

        curl_easy_cleanup(curl);
        return 0;

    }
    else {
        return -1;
    }

}