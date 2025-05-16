#pragma once
#include <SimpleIni.h>
#include "message_box.hpp"

/*
	AMUpdater General Setting
*/

CSimpleIniA AMConfig;

void AMConfigDebugStringA(std::string str) {
	std::cout << "[ INI ] " + str << std::endl;
}

void AMConfig_Init() {
	SI_Error rc = AMConfig.LoadFile("./AMConfig.ini");
	if (rc < 0) { 
		ErrorMessageBoxAndExit("Load AMConfig Failed");
	};
}

std::string AMConfig_GetRevision() {

	const char* game_rev = AMConfig.GetValue("AMUpdaterConfig", "amucfg-game_rev", "0");
	const char* game_ver = AMConfig.GetValue("MuchaCAConfig", "cacfg-game_ver", "00.00");

	std::string rev = game_rev;
	std::string ver = game_ver;

#ifdef _DEBUG
	AMConfigDebugStringA(rev + "." + ver);
#endif // _DEBUG


	return rev + "." + ver;
}

std::string AMConfig_GetServer() {

	const char* auth_server_url = AMConfig.GetValue("MuchaCAConfig", "cacfg-auth_server_url", "error");

#ifdef _DEBUG
	AMConfigDebugStringA(auth_server_url);
#endif // _DEBUG

	return auth_server_url;
}

int AMConfig_GetCountdown() {

	const char* countdown_time = AMConfig.GetValue("AMUpdaterConfig", "amucfg-countdown", "5");

#ifdef _DEBUG
	AMConfigDebugStringA(countdown_time);
#endif // _DEBUG

	return std::stoi(countdown_time);
}

std::string AMConfig_GetGameTitle() {

	const char* title = AMConfig.GetValue("AMUpdaterConfig", "amucfg-title", "NO GAME");

#ifdef _DEBUG
	AMConfigDebugStringA(title);
#endif // _DEBUG

	return title;
}

std::string AMConfig_GetGameCD() {

	const char* game_cd = AMConfig.GetValue("MuchaCAConfig", "cacfg-game_cd", "");

#ifdef _DEBUG
	AMConfigDebugStringA(game_cd);
#endif // _DEBUG

	return game_cd;
}

std::string AMConfig_GetDlImagePath() {

	const char* downloadPath = AMConfig.GetValue("MuchaDtConfig", "dtcfg-dl_image_path", "./");

	std::string result = downloadPath;

	for (auto& c : result) {
		if (c == '\\') c = '/';
	}

#ifdef _DEBUG
	AMConfigDebugStringA(result);
#endif // _DEBUG

	return result;
}

std::string AMConfig_GetNetId() {

	CSimpleIniA WritableConfig;
	const char* writableConfig = AMConfig.GetValue("AMAuthdConfig", "amdcfg-writableConfig", "error");

	std::string wconfigPath = writableConfig;
	SI_Error rc = WritableConfig.LoadFile(wconfigPath.c_str());
	if (rc < 0) {
		ErrorMessageBoxAndExit("Load WritableConfig Failed");
	}

	const char* netId = WritableConfig.GetValue("RuntimeConfig", "netID", "");

#ifdef _DEBUG
	AMConfigDebugStringA(netId);
#endif // _DEBUG

	return netId;
}

// im not sure this stuff will working or not
std::string AMConfig_GetSerial() {
	CSimpleIniA WritableConfig;
	const char* writableConfig = AMConfig.GetValue("AMAuthdConfig", "amdcfg-writableConfig", "error");

	std::string wconfigPath = writableConfig;
	SI_Error rc = WritableConfig.LoadFile(wconfigPath.c_str());
	if (rc < 0) {
		ErrorMessageBoxAndExit("Load WritableConfig Failed");
	}

	const char* serial = WritableConfig.GetValue("RuntimeConfig", "serialID", "");

#ifdef _DEBUG
	AMConfigDebugStringA(serial);
#endif // _DEBUG

	return serial;
}