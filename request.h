#pragma once
#include <iostream>
#include <curl/curl.h>
#include <string>
#include <fstream>
#include <filesystem>
#include <istream>
using namespace std;

/**
 * @file request.h
 * @brief This file contains the implementation of the moveBankRequests class constructor and methods
 */

/**
 * @class moveBankRequests
 * @brief This class is used to make requests to moveBank database (moveBank.org)
 *
 * This class is used to request different types of data to moveBank database.
 * It use the library curl and is implemented with my identifier. (identifier is needed to access moveBank) 
 * You could need to change it to your identifier. Just create an account on moveBank and change it here.
 * Methods are used to parse raw data and store them in data and indfiles folders
 */
class moveBankRequests {

	
public:
    /**
     * @brief Constructor for moveBankRequests.
     * 
	 * @param adresse This is the https adress to where the request is made
	 * @param outputFilePath This is the path to where the data from the request are stored
	 */
	moveBankRequests(const string &adresse,const string &outputFilePath);
	 /**
     * @brief Destructor for moveBankRequests.
     * 
	 */
	/**
	 * @brief cleaner for moveBankAuth as the destructor bring core dump error with curl cleanup
	 * 
	 */
	void cleanUp();
	// ~moveBankRequests();
    /**
     * @brief Parsing for moveBank individual data.
     * 
	 * @param homeDir The path representing your home directory
	 * @param outputInd This is the path to where the raw data from the request are stored
	 */
	void idParsing(const string &homeDir,const string &outputInd);

    /**
     * @brief Parsing for moveBank initial event data.
     * 
	 * @param homeDir The path representing your home directory
	 * @param outputEvent This is the path to where the raw data from the request are stored
	 */
	void eventInitialParsing(const string &homeDir,const string &outputEvent);

    /**
     * @brief Parsing for moveBank update event data.
     * 
	 * @param homeDir The path representing your home directory
	 * @param outputUpdateEvent This is the path to where the raw data from the request are stored
	 */
	void eventUpdateparsing(const string &homeDir,const string &outputUpdateEvent);
private:
    CURL* curl;
};