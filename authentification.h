#pragma once
#include <string>
#include <iostream>
#include <curl/curl.h>
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include <cryptopp/cryptlib.h>
#include <cryptopp/hex.h>
#include <cryptopp/md5.h>
#include <cryptopp/filters.h>
#include <cryptopp/files.h>
#include <fstream>
#include <filesystem>
using namespace std;

/**
 * @file authentification.h
 * @brief This file contains the declaration of the moveBankAuth class
 */

/**
 * @class moveBankAuth
 * @brief This class is used to authentificate to the moveBank database.
 *
 * This class is used to create an anthentification between moveBank.org and your moveBank identifier (here my are used by default) so that you can retrieve the data.
 * You could need to change it to your identifier. Just create an account on moveBank and change it here. (id and password)
 * Methods are used to create the certificate, the md5 hash and store these data.
 */
class moveBankAuth {

	
public:
    /**
     * @brief Constructor for moveBankAuth.
     * 
	 * @param adresse This is the https moveBank study adress with which the certificate is made. Each study need a specific certificate
	 * @param outputFilePath This is the path to where the data from the request are stored
	 */
	moveBankAuth(const string &homeDir,const string &adresse);
	 /**
     * @brief Destructor for moveBankAuth.
     * 
	 */
	~moveBankAuth();
    /**
     * @brief Calculating the md5 using the license_terms.txt file
     * 
	 * @param fileName The path to the license_terms.txt file
	 */
	string calculateMD5 (string &fileName);
    /**
     * @brief Get the md5 key
	 */	
	string getMD5();
private:
    CURL* curl;
    string md5;
};