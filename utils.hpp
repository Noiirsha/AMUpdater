#pragma once
#include <windows.h>
#include <winhttp.h>
#include <string>
#include <tlhelp32.h>
#include <tchar.h>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <openssl/md5.h>

#pragma warning(disable : 4996)
#pragma comment(lib, "winhttp.lib")

LPCWSTR string2LPCWSTR(std::string str)
{
	size_t size = str.length();
	int wLen = ::MultiByteToWideChar(CP_UTF8,
		0,
		str.c_str(),
		-1,
		NULL,
		0);
	wchar_t* buffer = new wchar_t[wLen + 1];
	memset(buffer, 0, (wLen + 1) * sizeof(wchar_t));
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), size, (LPWSTR)buffer, wLen);
	return buffer;
}

bool isUrlReachable(const std::wstring& url) {
    HINTERNET hSession = WinHttpOpen(L"URL Reachability Check",
        WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
        WINHTTP_NO_PROXY_NAME,
        WINHTTP_NO_PROXY_BYPASS, 0);
    if (!hSession) return false;

    URL_COMPONENTS urlComp;
    ZeroMemory(&urlComp, sizeof(urlComp));
    urlComp.dwStructSize = sizeof(urlComp);

    urlComp.dwSchemeLength = (DWORD)-1;
    urlComp.dwHostNameLength = (DWORD)-1;
    urlComp.dwUrlPathLength = (DWORD)-1;

    if (!WinHttpCrackUrl(url.c_str(), (DWORD)url.length(), 0, &urlComp)) {
        WinHttpCloseHandle(hSession);
        return false;
    }

    std::wstring hostName(urlComp.lpszHostName, urlComp.dwHostNameLength);
    std::wstring urlPath(urlComp.lpszUrlPath, urlComp.dwUrlPathLength);

    HINTERNET hConnect = WinHttpConnect(hSession, hostName.c_str(), urlComp.nPort, 0);
    if (!hConnect) {
        WinHttpCloseHandle(hSession);
        return false;
    }

    DWORD flags = WINHTTP_FLAG_REFRESH;
    if (urlComp.nScheme == INTERNET_SCHEME_HTTPS) {
        flags |= WINHTTP_FLAG_SECURE;
    }

    HINTERNET hRequest = WinHttpOpenRequest(hConnect, L"HEAD", urlPath.c_str(),
        NULL, WINHTTP_NO_REFERER,
        WINHTTP_DEFAULT_ACCEPT_TYPES,
        flags);
    if (!hRequest) {
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        return false;
    }

    // NO CA Check
    DWORD securityFlags = SECURITY_FLAG_IGNORE_UNKNOWN_CA |
        SECURITY_FLAG_IGNORE_CERT_DATE_INVALID |
        SECURITY_FLAG_IGNORE_CERT_CN_INVALID |
        SECURITY_FLAG_IGNORE_CERT_WRONG_USAGE;

    WinHttpSetOption(hRequest, WINHTTP_OPTION_SECURITY_FLAGS, &securityFlags, sizeof(securityFlags));

    bool result = WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0,
        WINHTTP_NO_REQUEST_DATA, 0, 0, 0);
    if (result) {

        if (!WinHttpReceiveResponse(hRequest, NULL)) {
            DWORD error = GetLastError();
            std::wcerr << L"[ NET ]WinHttpReceiveResponse failed, error: " << error << std::endl;
            if (error == 12152) return true;
            return false;
        }

    }

    DWORD statusCode = 0;
    DWORD dwSize = sizeof(statusCode);
    if (result) {
        WinHttpQueryHeaders(hRequest, WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER,
            WINHTTP_HEADER_NAME_BY_INDEX, &statusCode, &dwSize, WINHTTP_NO_HEADER_INDEX);
    }

    WinHttpCloseHandle(hRequest);
    WinHttpCloseHandle(hConnect);
    WinHttpCloseHandle(hSession);

    return result;
}

bool IsProcessRunning(const TCHAR* processName) {
    bool exists = false;
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    if (Process32First(snapshot, &entry)) {
        while (Process32Next(snapshot, &entry)) {
            if (_tcsicmp(entry.szExeFile, processName) == 0) {
                exists = true;
                break;
            }
        }
    }

    CloseHandle(snapshot);
    return exists;
}

std::string GetFileMD5(std::string filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Cannot open file: " << filename << std::endl;
        return "FileError";
    }

    MD5_CTX md5Context;
    MD5_Init(&md5Context);

    char buffer[1024];
    while (file.read(buffer, sizeof(buffer)) || file.gcount()) {
        MD5_Update(&md5Context, buffer, file.gcount());
    }

    unsigned char result[MD5_DIGEST_LENGTH];
    MD5_Final(result, &md5Context);

    std::stringstream md5string;
    md5string << std::hex << std::setfill('0');
    for (const auto& byte : result) {
        md5string << std::setw(2) << (int)byte;
    }

    return md5string.str();
}

/*
    OutputLog Utils
*/

std::string outputNetworkStringA(std::string content, std::string status) {

    std::ostringstream oss;

    oss << "  " << std::left << std::setw(40) << content;
    std::string result = oss.str();

    return result + " :  " + status;
}