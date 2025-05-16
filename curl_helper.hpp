#pragma once

#include <curl/curl.h>
#include <iostream>

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

