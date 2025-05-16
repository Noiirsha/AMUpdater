#pragma once

#include <bit7z/bitfileextractor.hpp>

void ExtractorDebugStringA(std::string str) {
    std::cout << "[ INI ] " + str << std::endl;
}

int extractDlImage(std::string dlImagePath, std::string destPath) {

    try { // bit7z classes can throw BitException objects
        using namespace bit7z;

        Bit7zLibrary lib{ "7z.dll" };
        BitFileExtractor extractor{ lib, BitFormat::SevenZip };

        // Extracting a simple archive, here is dl_image
        extractor.extract(dlImagePath.c_str(), destPath.c_str());

    }
    catch (const bit7z::BitException& ex) { 
        ExtractorDebugStringA("Extract failed ");
        std::cout << ex.what() << std::endl;
        return -1;
    }

    return 0;
}