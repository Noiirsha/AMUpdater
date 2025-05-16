#pragma once

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <wininet.h>

#include <httplib.h>
#include <nlohmann/json.hpp>

#include "message_queue.hpp"
#include "utils.hpp"

#pragma comment(lib, "wininet.lib")

class UpdaterNetwork {

private:
	std::string server_url;
	std::string net_id;
	std::string serial;
	int countdown;

	/*
		Some utils
	*/

	void NetworkDebugStringA(std::string str) {
		std::cout << "[ NET ] " + str << std::endl;
	}

	std::string outputNetworkStringA(std::string content, std::string status) {

		std::ostringstream oss;

		oss << "  " << std::left << std::setw(40) << content;
		std::string result = oss.str();

		return result + " :  " + status;
	}

	/*
		Implements
	*/

	void terminateSuccessSession(std::string message) {
		content_strings.push(message);
		content_strings.push("Completed");

		int time = countdown;
		content_strings.push("Application will automatically close in " + std::to_string(time) + " seconds");

		while (time > 0) {
			content_strings.overwriteLatest("Application will automatically close in " + std::to_string(time) + " seconds");
			time--;
			Sleep(1000);
		}

		exit(0);
	}

	void terminateErrorSession(std::string message) {
		content_strings.push("Error (" + message + ")");

		int time = countdown;
		content_strings.push("Application will automatically close in " + std::to_string(time) + " seconds");

		while (time > 0) {
			content_strings.overwriteLatest("Application will automatically close in " + std::to_string(time) + " seconds");
			time--;
			Sleep(1000);
		}

		exit(-1);
	}

	void getHops() {
		content_strings.push(outputNetworkStringA("Get Hops", "In Progress"));
		bool serverNotAvail = server_url.find_last_of("error") == std::string::npos;
		if (serverNotAvail) {
			content_strings.overwriteLatest(outputNetworkStringA("Get Hops", "Failed"));
			terminateErrorSession("Get Hops Failed");
		}
		else content_strings.overwriteLatest(outputNetworkStringA("Get Hops", "Completed"));
	}

	void routerResolution() {
		content_strings.push(outputNetworkStringA("Router name resolution", "In Progress"));

		DWORD dwFlags = 0;
		bool isNetworkAvail = InternetGetConnectedState(&dwFlags, 0);

		if (!isNetworkAvail) {
			content_strings.overwriteLatest(outputNetworkStringA("Router name resolution", "Failed"));
			terminateErrorSession("Cannot resolve router name");
		}
		else {
			content_strings.overwriteLatest(outputNetworkStringA("Router name resolution", "Completed"));
		}
	}

	void serverResolution() {
		content_strings.push(outputNetworkStringA("Server name resolution", "In Progress"));

		std::wstring serverUrlWStr(server_url.begin(), server_url.end());
		bool isNetworkAvail = isUrlReachable(serverUrlWStr);

		if (!isNetworkAvail) {
			content_strings.overwriteLatest(outputNetworkStringA("Server name resolution", "Failed"));
			terminateErrorSession("Server not reachable");
		}
		else {
			content_strings.overwriteLatest(outputNetworkStringA("Server name resolution", "Completed"));
		}
	}

	void checkSystemSetup_Stage1() {
		content_strings.push(outputNetworkStringA("System Setup", "Completed"));
		content_strings.push(outputNetworkStringA("ALL.Net Authentication", "In Progress"));

		bool isAMAuthdRunning = IsProcessRunning(_T("amauthd.exe"));
		bool isMuchaCDRunning = IsProcessRunning(_T("muchacd.exe"));

		if (!(isAMAuthdRunning && isMuchaCDRunning)) {
			content_strings.overwriteLatest(outputNetworkStringA("ALL.Net Authentication", "FAILURE"));
			terminateErrorSession("Authentication communication Error");
		}
		else {
			content_strings.overwriteLatest(outputNetworkStringA("ALL.Net Authentication", "SUCCESS"));
		}
	}


public:

	void init(std::string a1, std::string a2, std::string a3, int countdown) {
		this->server_url = a1;
		this->net_id = a2;
		this->serial = a3;
		this->countdown = countdown;
		content_strings.push("Starting Initialization .....");
	}

	void startNetworkUpdater() {
		getHops();
		Sleep(200);

		routerResolution();
		Sleep(200);

		serverResolution();
		Sleep(200);

		checkSystemSetup_Stage1();
		Sleep(200);
	}

};

UpdaterNetwork updateNetwork;