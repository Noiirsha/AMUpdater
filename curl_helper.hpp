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

int Curl_GetFile(std::string url, std::string filePath) {

    if (curl) {

        content_strings.push(outputNetworkStringA("File Download", ""));

        FILE* file = fopen(filePath.c_str(), "wb");
        if (!file) return -1;

        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteFileCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);                 

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

        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
        curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, [](void*, double dlTotal, double dlNow, double, double) {
            if (dlTotal > 0) {

                int downloadPrecentage = (dlNow / dlTotal * 100);
                precentage = downloadPrecentage;
                content_strings.overwriteLatest(outputNetworkStringA("File Download", std::to_string(downloadPrecentage) + "% (" + std::to_string(dlNow) + " / " + std::to_string(dlTotal) + ")"));

                CurlDebugStringA("Download Progress: " + downloadPrecentage);

            }
            return 0;
        });

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