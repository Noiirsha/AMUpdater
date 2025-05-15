#pragma once
#include <windows.h>
#include <iostream>

void ErrorMessageBoxAndExit(std::string message) {
	MessageBoxA(NULL, message.c_str(), "AMUpdater", MB_ICONERROR | MB_OK);
	exit(-1);
}

void ErrorMessageBox(std::string message) {
	MessageBoxA(NULL, message.c_str(), "AMUpdater", MB_ICONERROR | MB_OK);
}

void MessageBox(std::string message, UINT uType) {
	MessageBoxA(NULL, message.c_str(), "AMUpdater", uType);
}