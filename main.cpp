/**
 * @file main.cpp
 *
 * @brief This is the  main program for retrieving moveBank data and processing them.
 */

#include <iostream>
#include <stdio.h>
#include <string>
#include <filesystem>
#include <iomanip>
#include "authentification.h"
#include "myFunctions.h"
#include "request.h"

using namespace std;

// ==2368367==    definitely lost: 10,832 bytes in 2 blocks
// ==2368367==    indirectly lost: 1,426,995 bytes in 22,385 blocks
// ==2368367==      possibly lost: 1,858 bytes in 20 blocks
// ==2368367==    still reachable: 38 bytes in 2 blocks
// ==2368367==         suppressed: 0 bytes in 0 blocks


// g++ -std=c++17 -o launcher main.cpp authentification.cpp myFunctions.cpp request.cpp -lcurl -lcryptopp -lgd
// /home/david/formation/buildroot-2023.08/output/host/bin/aarch64-buildroot-linux-gnu-g++ -o launcherQemu newMain.cpp authentification.cpp myFunctions.cpp request.cpp -lcurl -lcryptopp -lgd


/**
 * @brief The main function of the lancheur part of this program (meant to be used on a embedded machine with cron).
 *
 * This main function retrieve and process MoveBank data. It performs the following tasks:
 * - Creates necessary program folders
 * - Authenticates with the MoveBank service
 * - Retrieves individual data and stores it in files
 * - Retrieves and processes event data
 * - Updates MD5 value and last update date
 *
 */
int main() {

    // program folder creation
    const char* homeDir = getenv("HOME");
    string Path = string(homeDir) + "/moveBankProjet/";
    string indPath = string(homeDir) + "/moveBankProjet/indFiles/";
    string dataPath = string(homeDir) + "/moveBankProjet/data/";

    // check of folders exist
    if (!std::filesystem::exists(Path)) {
        filesystem::create_directory(Path);
    }

    if (!std::filesystem::exists(indPath)) {
        filesystem::create_directory(indPath);
    }else{
        // Clear folder
        for (const auto& entry : filesystem::directory_iterator(indPath)) {
            filesystem::remove_all(entry.path());}           
    }
    if (!std::filesystem::exists(dataPath)) {
        filesystem::create_directory(dataPath);
    }else{
        // Clear folder
        for (const auto& entry : filesystem::directory_iterator(dataPath)) {
            filesystem::remove_all(entry.path());}           
    }

    // Initialize authentication
    string studyName = "1266784970";
    string studyUrlAuthentification = ("https://www.movebank.org/movebank/service/direct-read?entity_type=event&study_id="+studyName);

	moveBankAuth newAuthentification(homeDir,studyUrlAuthentification);

    // Retrieve individual data and store it in a file
    string md5Value = newAuthentification.getMD5();
    string individualUrl = "https://www.movebank.org/movebank/service/direct-read?entity_type=tag&study_id="+studyName+"&license-md5="+md5Value;
    string individualFilePath = string(homeDir) + "/moveBankProjet/data/outputInd.txt";
    moveBankRequests newRequestId(individualUrl,individualFilePath);
    newRequestId.idParsing(homeDir,individualFilePath);

    // Store MD5 hash
    ofstream fileMd5(string(homeDir) + "/moveBankProjet/data/md5Value.txt");
    fileMd5 << md5Value << endl;
    fileMd5.close();    

    // Get current date
    string currentDate = currentDateTime();
    ofstream fileDate(string(homeDir) + "/moveBankProjet/data/lastDateUpdate.txt");
    fileDate << currentDate+"000" << endl;
    fileDate.close();


    // Retrieve and process event data
    string past7Date = getDateMinus7Days(currentDate);
    string initialEventUrl = "https://www.movebank.org/movebank/service/direct-read?entity_type=event&study_id="+studyName+"&timestamp_start="+past7Date+"&timestamp_end="+currentDate+"&license-md5="+md5Value;
    string initialEventFilePath = string(homeDir) + "/moveBankProjet/data/outputEvent.txt";
    moveBankRequests newRequest7daysEvent(initialEventUrl,initialEventFilePath);
    newRequest7daysEvent.eventInitialParsing(homeDir,initialEventFilePath);

    return 0;
}