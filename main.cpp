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

// A installer sur buildroot : curl (libcurl4-openssl-dev), Crypto++ (libcrypto++-dev)
using namespace std;

// Fonction de rappel pour traiter les données reçues ligne par ligne
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::vector<std::string>* lines) {
    size_t total_size = size * nmemb;
    string line(static_cast<char*>(contents), total_size);

    // Stocker la ligne dans le conteneur (par exemple, un vecteur)
    lines->push_back(line);

    return total_size;
}
string calculateMD5(const std::string& filename) {
    CryptoPP::Weak1::MD5 md5;
    string result;
    ifstream file(filename);
    CryptoPP::FileSource(file, true, new CryptoPP::HashFilter(md5, new CryptoPP::StringSink(result)));
    string md5Hex;
    CryptoPP::StringSource(result, true, new CryptoPP::HexEncoder(new CryptoPP::StringSink(md5Hex), false));
    return md5Hex;
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
            cout << "Requette d'authentification réussie " << endl;
            string filename = "license_terms.txt";
            md5 = calculateMD5(filename);
        }
    }
    if (!std::filesystem::exists("output.txt")){
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
            cout << "Requette réussie : \n" << endl;
        }
        fclose(outputFile);
        curl_easy_cleanup(curl);
    }

    // PARSE INDIVIDUAL FILE AND CREATE OBJECT //
    string line;
    ifstream fileIndiv("output.txt");
    while (getline(fileIndiv, line)) {
        stringstream ss(line);
        int cpt =0;
        while (ss.good()) {
            std::string substr;
            getline(ss, substr, ',');
            if (cpt == 6){
                cout << substr << endl;
            }
            if (cpt == 8){
                cout << substr << endl;
            }
            if (cpt == 23){
                cout << substr << endl;
            }
            cpt++;
        }

    }
    return 0;
}


