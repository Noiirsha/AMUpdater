#pragma once

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <wininet.h>
#include <regex>

#include <nlohmann/json.hpp>

#include "message_queue.hpp"
#include "utils.hpp"
#include "curl_helper.hpp"

#pragma comment(lib, "wininet.lib")

using json = nlohmann::json;

#define PROTOCOL 1
#define HOST 2
#define PORT 3
#define PATH 4

void NetworkDebugStringA(std::string str) {
#ifdef _DEBUG
	std::cout << "[ NET ] " + str << std::endl;
#endif // _DEBUG
}

class TargetServerInformation {
private:
	std::string protocol = "";  // "https"
	std::string host = "";      // "v388-front.mucha-prd.nbgi-amnet.jp"
	std::string port = "";      // "10082"
	std::string path = "";      // "/mucha_front/"

public:
	int build(std::string url) {

		std::regex url_regex(R"(^(https?)://([^:/]+)(?::(\d+))?(/.*)?$)");
		std::smatch match;

		if (std::regex_match(url, match, url_regex)) {
			this->protocol = match[PROTOCOL];
			this->host = match[HOST];
			this->port = match[PORT];
			this->path = match[PATH];

			if (port.empty()) {
				port = (protocol == "https") ? "443" : "80";
			}

			if (path.empty()) {
				path = "/";
			}

#ifdef _DEBUG
			NetworkDebugStringA(this->protocol);
			NetworkDebugStringA(this->host);
			NetworkDebugStringA(this->port);
			NetworkDebugStringA(this->path);
#endif // _DEBUG


			return 0;
		}
		else {
			return -1;
		}
	}

	std::string getProtocol() {
		return this->protocol;
	}

	std::string getHost() {
		return this->host;
	}

	int getPort() {
		return std::stoi(this->port);
	}

	std::string getPath() {
		return this->path;
	}

};
TargetServerInformation serverNode;

class UpdaterNetwork {

private:
	std::string server_url;
	std::string revision;
	std::string net_id;
	std::string serial;
	std::string game_cd;
	int countdown;

	/*
		Some utils
	*/

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
			terminateErrorSession("ALL.NET AIP Name resolution failed");
		}
		else content_strings.overwriteLatest(outputNetworkStringA("Get Hops", "Completed"));
	}

	void routerResolution() {
		content_strings.push(outputNetworkStringA("Router name resolution", "In Progress"));

		DWORD dwFlags = 0;
		bool isNetworkAvail = InternetGetConnectedState(&dwFlags, 0);

		if (!isNetworkAvail) {
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
			terminateErrorSession("Server not reachable");
		}
		else {
			content_strings.overwriteLatest(outputNetworkStringA("Server name resolution", "Completed"));
		}
	}

	void checkSystemSetup_Stage1() {
		content_strings.push(outputNetworkStringA("System Setup", "In Progress"));

		if (serverNode.build(server_url) == -1) {
			NetworkDebugStringA("Parse url to node failed");
			terminateErrorSession("System Error");
		}
		else {
			content_strings.overwriteLatest(outputNetworkStringA("System Setup", "Completed"));
		}

		content_strings.push(outputNetworkStringA("ALL.Net Authentication", "In Progress"));

		bool isAMAuthdRunning = IsProcessRunning(_T("amauthd.exe"));
		bool isMuchaCDRunning = IsProcessRunning(_T("muchacd.exe"));

		if (!(isAMAuthdRunning && isMuchaCDRunning)) {
			content_strings.overwriteLatest(outputNetworkStringA("ALL.Net Authentication", "FAILURE"));
			terminateErrorSession("AMAuthd disconnected");
		}
		else {
			content_strings.overwriteLatest(outputNetworkStringA("ALL.Net Authentication", "SUCCESS"));
		}
	}

	void checkSystemSetup_Stage2 (){

		content_strings.push(outputNetworkStringA("Updater Authentication", "In Progress"));

		// Im decide to use curl here
		
		// Send request should contains mucha_front
		std::string requestPath = "updater_poweron";
		
		std::string responseData = Curl_Get(server_url + requestPath);

		if (!responseData.empty()) {

			try {

				json j = json::parse(responseData);
				NetworkDebugStringA(j.dump(4));

				if (j.contains("server") && j["server"] == "alive") {
					content_strings.overwriteLatest(outputNetworkStringA("Updater Authentication", "SUCCESS"));
					return;
				}

			}
			catch (const json::parse_error& e) {
				NetworkDebugStringA("JSON parse error");
				content_strings.overwriteLatest(outputNetworkStringA("Updater Authentication", "FAILURE"));
				terminateErrorSession("System parser Error");
			}

		}
		else {
			content_strings.overwriteLatest(outputNetworkStringA("Updater Authentication", "FAILURE"));
			terminateErrorSession("Updater Authentication communication Error");
		}
	}

	void checkIsUpdateAvail() {

		content_strings.push("Checking Update .....");

		// Send request should contains mucha_front
		std::string requestPath = "updater_getrevision";
		std::string responseData = Curl_Get(server_url + requestPath);

		if (!responseData.empty()) {

			try {

				json j = json::parse(responseData);
				NetworkDebugStringA(j.dump(4));

				std::string revisionFromServer;
				if (j.contains("revision")) {
					revisionFromServer = j["revision"];
				}

				if (revisionFromServer == revision) {
					content_strings.overwriteLatest("Update Not Avaliable");
				}
				else {
					revision_string = "REV " + revision + " => " + "REV " + revisionFromServer;
					content_strings.overwriteLatest("Starting Update .....");
				}

			}
			catch (const json::parse_error& e) {
				NetworkDebugStringA("JSON parse error");
				content_strings.overwriteLatest("Update Not Avaliable");
				terminateErrorSession("System parser Error");
			}

		}
		else {
			content_strings.overwriteLatest(outputNetworkStringA("Updater Authentication", "FAILURE"));
			terminateErrorSession("Updater Authentication communication Error");
		}


	}

public:

	void init(std::string a1, std::string a2, std::string a3, std::string a4, std::string a5, int countdown) {
		this->server_url = a1;
		this->net_id = a2;
		this->serial = a3;
		this->game_cd = a4;
		this->revision = a5;
		this->countdown = countdown;
		content_strings.push("Starting Initialization .....");
	}

	void startNetworkUpdater() {
		getHops();
		Sleep(100);

		routerResolution();
		Sleep(100);

		serverResolution();
		Sleep(100);

		checkSystemSetup_Stage1();
		Sleep(100);

		checkSystemSetup_Stage2();
		Sleep(100);

		checkIsUpdateAvail();
		Sleep(100);
	}

};

UpdaterNetwork updateNetwork;