#pragma once

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>

#include <httplib.h>
#include <nlohmann/json.hpp>

#include "message_queue.hpp"

class UpdaterNetwork {

private:
	std::string server_url;
	std::string net_id;
	std::string serial;
	int countdown;

public:

	std::string outputNetworkStringA(std::string content, std::string status) {
		
		std::ostringstream oss;

		oss << "  " << std::left << std::setw(40) << content;
		std::string result = oss.str();

		return result + " :  " + status;

	}

	void init(std::string a1, std::string a2, std::string a3, int countdown) {
		this->server_url = a1;
		this->net_id = a2;
		this->serial = a3;
		this->countdown = countdown;
		content_strings.push("Starting Initialization .....");
	}

	void startNetworkUpdater() {
		getHops();
	}

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

};

UpdaterNetwork updateNetwork;