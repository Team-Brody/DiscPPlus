#include "client.h"
#include <iostream>
#include <chrono>
#include <windows.h>
#include <psapi.h>
#include <stdio.h>


using DiscPPlus::Commands; // basic commands listed in libs
using DiscPPlus::Client; // main functions

auto start = std::chrono::steady_clock::now();;

int main() {


	std::string const token = ""; // our token
	start = std::chrono::steady_clock::now();
	Client c;
	c.establishConnection(token); // establish connection using token

	std::cout << std::string("Connection Lost Restart to continue...");
	return 0; // exits the program
}

std::string ftime(int sec) {
	int min = 00;
	int hour = 00;

	if (sec > 60) {
		min = sec / 60;
		sec -= min * 60;
	}
	if (min > 60) {
		hour = min / 60;
		min -= hour * 60;
	}
	std::string secs = (std::stringstream() << std::setw(2) << std::setfill('0') << sec).str();
	std::string mins = (std::stringstream() << std::setw(2) << std::setfill('0') << min).str();
	std::string hours = (std::stringstream() << std::setw(2) << std::setfill('0') << hour).str();


	return hours + ":" + mins + ":" + secs;


}

void DiscPPlus::Commands::OnMsg(DiscPPlus::Message msg, DiscPPlus::Bot bot) { // on message event teliing us that a message has been sent


	// all this stuff here should be pretty obvious
	if (msg.content == "$hi") {
		msg.channel.Send("hello! "+ msg.author.mention + "! \n https://rb.gy/nnqou3", bot); 
	}
	else if (msg.content == "$help") { 
		msg.channel.Send("http://sparktonian.net/Secure/Filesharing/uploads/PrivateStorage/Capture.png", bot);
	}
	else if (msg.content == "$runtime") {
		auto end = std::chrono::steady_clock::now();
		
		msg.channel.Send("Elapsed Time in HH:MM:SS is: " + ftime(std::chrono::duration_cast<std::chrono::seconds>(end - start).count()), bot);
	}
	else if (msg.author.id == "844006016172687410") {
		msg.channel.Send("Wow what a loon", bot);
	}
	else if (msg.content == "$embed") {
		DiscPPlus::Embed embedmsg;
		embedmsg.SetAuthor("SparksCool", "", "https://cdn.discordapp.com/attachments/740770560149291089/845730338239938590/detective.png");
		embedmsg.SetTitle("Whats This?");
		embedmsg.SetColor(16645526);
		embedmsg.SetDesc("A Test Description???");
		embedmsg.AddField("A Field?", "and a value too!");
		embedmsg.AddField("A Second Field?", "and a second value too!");
		embedmsg.AddField("A Third Field?", "and a third value too!");
		embedmsg.SetFooter("Guess what we have a footer aswell!");
		msg.channel.SendEmbed(embedmsg, bot);
	}
	else if (msg.content=="$ram") {
		PROCESS_MEMORY_COUNTERS_EX pmc;
		GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
		float virtualMemUsedByMe = pmc.PrivateUsage;
		msg.channel.Send("Current Memory Used is : " + std::to_string((virtualMemUsedByMe / (1024 * 1000))) + "MB", bot);
	}
}