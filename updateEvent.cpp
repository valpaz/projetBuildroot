/**
 * @file updateEvent.cpp
 *
 * @brief This program updates events from the MoveBank web service.
 */
#include <iostream>
#include <stdio.h>
#include <string>
#include <filesystem>
#include <iomanip>
#include "myFunctions.h"
#include "request.h"

// g++ -std=c++17 -o updateEvent updateEvent.cpp myFunctions.cpp request.cpp -lcurl -lcryptopp -lgd
// /home/david/formation/buildroot-2023.08/output/host/bin/aarch64-buildroot-linux-gnu-g++ -o updateEventQemu updateEvent.cpp myFunctions.cpp request.cpp -lcurl -lcryptopp

/**
 * @brief This is the main function of the update part of this program current.
 *
 * This main function updates events from the MoveBank database. It reads the last update date using
 * lastDateUpdate.txt date file and uses that date to get the new events. 
 * The results are saved in the outputUpdateEvent.txt file.
 * Then, the update date is updated to the current date and stored in lastDateUpdate.txt file again.
 *
 * @return 0 on success.
 */
int main() {

	// Get last date
	const char* homeDir = getenv("HOME");
    ifstream filedate(string(homeDir) + "/moveBankProjet/data/lastDateUpdate.txt");
    string laDate;
    string lastDate;
    while (getline(filedate, laDate)) {
        lastDate=laDate;}
    filedate.close();

    // Get md5 hash
    ifstream fileMd5(string(homeDir) + "/moveBankProjet/data/md5Value.txt");
    string md5;
    string md5Value;
    while (getline(fileMd5, md5)) {
        md5Value=md5;}
    filedate.close();
    string currentDate = currentDateTime()+"000";

    // Configure the URL for updating events
    string studyName = "1266784970";
    string updateEventUrl = "https://www.movebank.org/movebank/service/direct-read?entity_type=event&study_id="+studyName+"&timestamp_start="+lastDate+"&timestamp_end="+currentDate+"&license-md5="+md5Value;
    string updateEventFilePath = string(homeDir) + "/moveBankProjet/data/outputUpdateEvent.txt";
    moveBankRequests newRequestUpdateEvent(updateEventUrl,updateEventFilePath);
    newRequestUpdateEvent.eventUpdateparsing(homeDir,updateEventFilePath);

    // Last date become the current date for the next update event
    ofstream fileDate(string(homeDir) + "/moveBankProjet/data/lastDateUpdate.txt");
    fileDate << currentDate << endl;
    fileDate.close();
	return 0;
}