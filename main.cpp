#include <iostream>
#include <stdio.h>
#include <string>
#include <curl/curl.h>
#include <vector>
#include <fstream>
#include <istream>
#include <cryptopp/cryptlib.h>
#include <cryptopp/hex.h>
#include <cryptopp/md5.h>
#include <cryptopp/filters.h>
#include <cryptopp/files.h>
#include <filesystem>
#include <iomanip>
#include <ctime>
#include <sstream>

// A installer sur buildroot : curl (libcurl4-openssl-dev), Crypto++ (libcrypto++-dev)
using namespace std;

string calculateMD5(const std::string& filename) {
    CryptoPP::Weak1::MD5 md5;
    string result;
    ifstream file(filename);
    CryptoPP::FileSource(file, true, new CryptoPP::HashFilter(md5, new CryptoPP::StringSink(result)));
    string md5Hex;
    CryptoPP::StringSource(result, true, new CryptoPP::HexEncoder(new CryptoPP::StringSink(md5Hex), false));
    return md5Hex;
}

std::string getDateMinus7Days(const std::string& inputDate) {
    // Convertissez la date d'entrée en une structure tm
    std::tm timeinfo = {};
    std::istringstream ss(inputDate);
    ss >> std::get_time(&timeinfo, "%Y%m%d");

    // Soustrayez 7 jours en secondes (7 jours * 24 heures * 60 minutes * 60 secondes)
    std::time_t timestamp = std::mktime(&timeinfo) - (1 * 24 * 60 * 60);

    // Convertissez le résultat en une nouvelle structure tm
    std::tm resultTimeinfo = *std::localtime(&timestamp);

    // Formatez la nouvelle date en tant que chaîne de caractères au format "YYYYMMDD"
    std::ostringstream oss;
    oss << std::put_time(&resultTimeinfo, "%Y%m%d");

    return oss.str();
}
int main() {

    cout << "LAAAAA" << endl;
    CURL* curl;

    // Initialisation de cURL
    curl = curl_easy_init();
    string md5;
    std::string filename = "cookies.txt";
    std::ifstream file(filename.c_str());
    if (file.good()) {
        cout << "Le fichier existe." << endl;
        string filename = "license_terms.txt";
        md5 = calculateMD5(filename);
    } else {
        cout << "Le fichier n'existe pas." << endl;
        const char* urlAuthentification = "https://www.movebank.org/movebank/service/direct-read?entity_type=event&study_id=1266784970";
        curl_easy_setopt(curl, CURLOPT_URL, urlAuthentification);
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
        curl_easy_setopt(curl, CURLOPT_USERPWD, "valpaz:longboard09");
        curl_easy_setopt(curl, CURLOPT_COOKIEJAR, "cookies.txt");
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fopen("license_terms.txt", "wb"));

        CURLcode resInit = curl_easy_perform(curl);
        if (resInit != CURLE_OK) {
            cerr << "Error :" << curl_easy_strerror(resInit) << endl;
        }else {
            cout << "Requette réussie : Mise en place de l'authentification" << endl;
            string filename = "license_terms.txt";
            md5 = calculateMD5(filename);
        }
    }
    if (!std::filesystem::exists("individu.txt")){
        // GET INDIVIDUAL FILE //
        string adresse = "https://www.movebank.org/movebank/service/direct-read?entity_type=individual&study_id=1266784970&license-md5="+md5; // Remplacez par l'URL de votre choix
        const char* url = adresse.c_str();
        cout<< url <<  endl;
        CURLcode res;
        // Configuration de l'URL
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_USERPWD, "valpaz:longboard09");
        curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "cookies.txt");
        FILE* outputFile;
        outputFile = fopen("output.txt", "wb");
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, outputFile);
        // Exécution de la requête
        res = curl_easy_perform(curl);
        // Vérification de la réponse
        if (res != CURLE_OK) {
            cerr << "Erreur lors de la requête cURL : \n" << endl;
        }else {
            cout << "Requette réussie : Mise en place Récupération des individues\n" << endl;
        }
        fclose(outputFile);
        //curl_easy_cleanup(curl);
        // PARSE INDIVIDUAL FILE AND CREATE OBJECT //
        string line;
        ifstream fileOutput("output.txt");
        ofstream fileIndiv("individu.txt", std::ios::out | std::ios::trunc);
        while (getline(fileOutput, line)) {
            stringstream ss(line);
            int cpt =0;
            while (ss.good()) {
                std::string substr;
                getline(ss, substr, ',');
                if (cpt == 7){
                    if (isdigit(substr[0])){
                        fileIndiv << substr << std::endl;
                    }
                    cout << substr << endl;
                }
                cpt++;
            }

        }
        fileIndiv.close();
        fileOutput.close();
        remove ("output.txt");


        // Recuperation initiale des events //
        auto t = time(nullptr);
        auto tm = *localtime(&t);
        ostringstream oss;
        oss << put_time(&tm, "%Y%m%d%H%M%S000");
        auto actualDate = oss.str();
        string past7date=getDateMinus7Days(actualDate)+actualDate.substr(8, 9);;
        cout << "ICI" << endl;
        cout << past7date << endl;
        cout << actualDate << endl;

        string adresse2 = "https://www.movebank.org/movebank/service/direct-read?entity_type=event&study_id=1266784970&timestamp_start="+past7date+"&timestamp_end="+actualDate+"&license-md5="+md5;
        const char* url2 = adresse2.c_str();
        // Configuration de l'URL
        CURLcode res2;
        curl_easy_setopt(curl, CURLOPT_URL, url2);
        curl_easy_setopt(curl, CURLOPT_USERPWD, "valpaz:longboard09");
        curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "cookies.txt");
        FILE* outputEvent;
        outputEvent = fopen("outputEvent.csv", "wb");
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, outputEvent);
        // Exécution de la requête
        res2 = curl_easy_perform(curl);
        // Vérification de la réponse
        if (res2 != CURLE_OK) {
            cerr << "Erreur lors de la requête cURL : \n" << endl;
        }else {
            cout << "Requette réussie : Récuperation initial des 7 derniers jours d'event pour chaque individue\n" << endl;
        }
        curl_easy_cleanup(curl);

        ifstream fileEvent("outputEvent.csv");
        ifstream fileInds("individu.txt");
        string ligne;
        string ligneEvent;
        string actualId;
        int cptt;
        while (std::getline(fileInds, ligne)) {
            actualId = ligne;
            ofstream fileInd(("fichierInd/"+actualId + ".csv").c_str());
            while (getline(fileEvent, ligneEvent)) {
                stringstream ss(ligneEvent);
                string valeur;
                cptt = 0;
                while (getline(ss, valeur, ',')) {
                    if (cptt==4){
                        cout << "\n" << endl;
                        cout << valeur << endl;
                        cout << actualId << endl;
                        if (valeur==actualId){
                            fileInd << ligneEvent << std::endl;


                        }
                    }
                    cptt++;
                }
            }
            fileInd.close();

        }
        fileEvent.close();
        fileInds.close();




    }


    return 0;
}



