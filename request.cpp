#include "request.h"
/**
 * @file request.cpp
 * @brief This file contains the declaration of the moveBankRequests class
 */

/**
 * @brief Constructor for moveBankRequests.
 * 
 * Make the call to the database, retrieve the data and store them
 * 
 * @param adresse This is the https adress to where the request is made
 * @param outputFilePath This is the path to where the data from the request are stored
 */
moveBankRequests::moveBankRequests(const string &adresse, const string &outputFilePath){
	curl = curl_easy_init();
    CURLcode res;
    curl_easy_setopt(curl, CURLOPT_URL, adresse.c_str());
    curl_easy_setopt(curl, CURLOPT_USERPWD, "valpaz:valpaz21");
    FILE* outputFile;
    outputFile = fopen(outputFilePath.c_str(), "wb");
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, outputFile);
    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        cerr << "Request error " << endl;
    }else {
        cout << "Successful request " << endl;
    }
    fclose(outputFile);
}

/**
 * @brief Parsing for moveBank individual data.
 * 
 * Parse the raw data containing ids, informations, etc and create a new file containing only the tag Id (skipping useless data)
 * 
 * @param homeDir The path representing your home directory
 * @param outputInd This is the path to where the raw data from the request are stored
 */
void moveBankRequests::idParsing(const string &homeDir,const string &outputInd){
        string line;
        ifstream fileOutput(outputInd.c_str());
        ofstream fileIndiv(string(homeDir) + "/moveBankProjet/data/individu.txt", std::ios::out | std::ios::trunc);
        while (getline(fileOutput, line)) {
            stringstream ss(line);
            int cpt = 0;
            while (ss.good()) {
                std::string substr;
                getline(ss, substr, ',');
                if (cpt == 2){
                    if (isdigit(substr[0])){
                        fileIndiv << substr << std::endl;
                    }
                }
                cpt++;
            }

        }
        fileIndiv.close();
        fileOutput.close();
}

/**
 * @brief Parsing for moveBank initial event data.
 * 
 * Parse the raw event data (containing mixed data of all individual) and create new files for each tag_id.
 * Each of this new files are filled with their corresponding event which contain their location and time of this location.
 * Thus are the initial data to populate the future graph. You can choose how much day of data you want to initialy retrieve by changing the number of day in myFunctions.cpp
 * 
 * @param homeDir The path representing your home directory
 * @param outputEvent This is the path to where the raw data from the request are stored
 */
void moveBankRequests::eventInitialParsing(const string &homeDir,const string &outputEvent){
	ifstream fileInds(string(homeDir) + "/moveBankProjet/data/individu.txt");
    string ligne;
    string ligneEvent;
    string currentId;
    int cptt;
    while (getline(fileInds, ligne)) {
        currentId = ligne;

        // Creating a file for each individual
        ofstream fileInd((string(homeDir) + "/moveBankProjet/indFiles/"+currentId + ".csv").c_str());
        ifstream fileEvent(outputEvent.c_str());
        getline(fileEvent, ligneEvent);
        while (getline(fileEvent, ligneEvent)) {
            stringstream ss(ligneEvent);
            string valeur;
            cptt = 0;
            while (getline(ss, valeur, ',')) {
                if (cptt==4){
                    valeur.erase(valeur.find_last_not_of("\n\r") + 1);         
                    if (valeur==currentId){
                        fileInd << ligneEvent << endl;

                    }
                }
                cptt++;
            }
        }
        fileInd.close();
        fileEvent.close();
    }
    fileInds.close();
}

/**
 * @brief Parsing for moveBank update event data.
 * 
 * Parse the raw event data (containing mixed data of all individual) and update already existing files for each tag_id.
 * Each of this files are filled with their new corresponding event which contain their location and time of this location.
 * 
 * @param homeDir The path representing your home directory
 * @param outputUpdateEvent This is the path to where the raw data from the request are stored
 */
void moveBankRequests::eventUpdateparsing(const string &homeDir,const string &outputUpdateEvent){
    ifstream fileIndis(string(homeDir) + "/moveBankProjet/data/individu.txt");
    string line;
    string ligneEvent;
    string currentId;
    int cptt;
    while (getline(fileIndis, line)) {
        currentId = line;

        ofstream fileUpdateInd((string(homeDir) + "/moveBankProjet/indFiles/"+currentId + ".csv").c_str(),std::ios::app);
        ifstream fileUpdateEvent(outputUpdateEvent.c_str());
        while (getline(fileUpdateEvent, ligneEvent)) {
            stringstream ss(ligneEvent);
            string valeur;
            cptt = 0;
            while (getline(ss, valeur, ',')) {
                if (cptt==4){
                	valeur.erase(valeur.find_last_not_of("\n\r") + 1);   
                    if (valeur==currentId){
                        fileUpdateInd << ligneEvent << endl;


                    }
                }
                cptt++;
            }
        }
        fileUpdateInd.close();
        fileUpdateEvent.close();

    }
}