#include "authentification.h"

/**
 * @file authentification.cpp
 * @brief This file contains the implementation of the moveBankAuth class constructor and methods 
 */

/**
 * @class moveBankAuth
 * @brief This class is used to authentificate to the moveBank database.
 *
 * This class is used to create an anthentification between moveBank.org and your moveBank identifier (here my are used by default) so that you can retrieve the data.
 * You could need to change it to your identifier. Just create an account on moveBank and change it here. (id and password)
 * Methods are used to create the certificate, the md5 encoded bound and store these data.
 * This constructor use curl library to communicate with moveBank database
 */
moveBankAuth::moveBankAuth(const string &homeDir,const string &adresse){
	curl = curl_easy_init();
    string filename = string(homeDir) + "/moveBankProjet/license_terms.txt";
    if (!filesystem::exists(filename)){
	    curl_easy_setopt(curl, CURLOPT_URL, adresse.c_str());
	    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
	    curl_easy_setopt(curl, CURLOPT_USERPWD, "valpaz:valpaz21");
	    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fopen((string(homeDir) + "/moveBankProjet/license_terms.txt").c_str(), "wb"));
	    CURLcode resInit = curl_easy_perform(curl);
	    if (resInit != CURLE_OK) {
	        cerr << "Request error :" << curl_easy_strerror(resInit) << endl;
	    }else {
	        cout << "Successful request : authentification files set up " << endl;
	        md5 = calculateMD5(filename);
    	}
    }else{
    	md5 = calculateMD5(filename);
    }
    curl_easy_cleanup(curl);
}
/**
 * @brief cleaner for moveBankAuth as the destructor bring core dump error with curl cleanup
 * 
 */
void moveBankAuth::cleanUp() {
    if (curl) {
        curl_easy_cleanup(curl);
    }
}

// moveBankAuth::~moveBankAuth() {
//     if (curl) {
//         curl_easy_cleanup(curl);
//     }
// }
/**
 * @brief Constructor for moveBankRequests.
 * 
 * This method takes a file as input, calculates its MD5 hash and returns the result as a hexadecimal string
 * 
 * @param adresse This is the https adress to where the request is made
 * @param outputFilePath This is the path to where the data from the request are stored
 */
string moveBankAuth::calculateMD5(string &fileName){
    CryptoPP::Weak::MD5 md5;
    string result;
    ifstream file(fileName);
    CryptoPP::FileSource(file, true, new CryptoPP::HashFilter(md5, new CryptoPP::StringSink(result)));
    string md5Hex;
    CryptoPP::StringSource(result, true, new CryptoPP::HexEncoder(new CryptoPP::StringSink(md5Hex), false));
    return md5Hex;
}

/**
 * @brief Get the md5 key
 * 
 * Return the md5 generated hash
 * 
 */	
string moveBankAuth::getMD5() {
    return md5;
}