#pragma once
#include <SimpleIni.h>
#include "message_box.hpp"

/*
	AMUpdater General Setting
*/

CSimpleIniA AMConfig;

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

	return rev + "." + ver;
}

std::string AMConfig_GetServer() {

	const char* auth_server_url = AMConfig.GetValue("MuchaCAConfig", "cacfg-auth_server_url", "http://error/");

	return auth_server_url;
}

std::string AMConfig_GetGameTitle() {

	const char* title = AMConfig.GetValue("AMUpdaterConfig", "amucfg-title", "NO GAME");
	return title;

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

	return serial;
}