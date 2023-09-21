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
// g++ -std=c++17 -o testGraph graph.cpp -lgd
// g++ -std=c++17 -o testP main.cpp -lcurl -lcryptopp
// /home/david/formation/buildroot-2023.08/output/host/bin/aarch64-buildroot-linux-gnu-g++ -o progOiseauxGraph graph.cpp -lgd
// /home/david/formation/buildroot-2023.08/output/host/bin/aarch64-buildroot-linux-gnu-g++ -o progOiseaux main.cpp -lcryptopp -lcurl

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

string getDateMinus7Days(const std::string& inputDate) {
    // Convertissez la date d'entrée en une structure tm
    std::tm timeinfo = {};
    std::istringstream ss(inputDate);
    ss >> std::get_time(&timeinfo, "%Y%m%d");

    // Soustrayez 7 jours en secondes (7 jours * 24 heures * 60 minutes * 60 secondes)
    std::time_t timestamp = std::mktime(&timeinfo) - (7 * 24 * 60 * 60);

    // Convertissez le résultat en une nouvelle structure tm
    std::tm resultTimeinfo = *std::localtime(&timestamp);

    // Formatez la nouvelle date en tant que chaîne de caractères au format "YYYYMMDD"
    std::ostringstream oss;
    oss << std::put_time(&resultTimeinfo, "%Y%m%d");

    return oss.str();
}
int main() {

    // Creation de l'espace de stockage //
    string folderPath = "/home/david/progMoveBankDir/indFile/"; // Remplacez par le chemin du dossier que vous souhaitez vider
    for (const auto& entry : filesystem::directory_iterator(folderPath)) {
        filesystem::remove_all(entry.path());}
    string Path = "/home/david/progMoveBankDir/";
    string indPath = "/home/david/progMoveBankDir/indFile/";
    string indGraph = "/home/david/progMoveBankDir/indGraph/";
    // Vérifiez si le dossier existe déjà
    if (!std::filesystem::exists(Path)) {
        filesystem::create_directory(Path);}
    if (!std::filesystem::exists(indPath)) {
        filesystem::create_directory(indPath);}
    if (!std::filesystem::exists(indGraph)) {
        filesystem::create_directory(indGraph);}
   


    // Initialisation : Authentification avec moveBank //
    CURL* curl;
    curl = curl_easy_init();
    string md5;

    if (filesystem::exists("/home/david/progMoveBankDir/cookies.txt")){
        cout << "Le fichier d'authentification existe." << endl;
        string filename = "/home/david/progMoveBankDir/license_terms.txt";
        md5 = calculateMD5(filename);
    } else {
        cout << "Le fichier n'existe pas." << endl;
        const char *urlAuthentification = "https://www.movebank.org/movebank/service/direct-read?entity_type=event&study_id=1266784970";
        curl_easy_setopt(curl, CURLOPT_URL, urlAuthentification);
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
        curl_easy_setopt(curl, CURLOPT_USERPWD, "valpaz:valpaz21");
        curl_easy_setopt(curl, CURLOPT_COOKIEJAR, "/home/david/progMoveBankDir/cookies.txt");
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fopen("/home/david/progMoveBankDir/license_terms.txt", "wb"));

        CURLcode resInit = curl_easy_perform(curl);
        if (resInit != CURLE_OK) {
            cerr << "Error :" << curl_easy_strerror(resInit) << endl;
        }else {
            cout << "Requette réussie : Mise en place de l'authentification" << endl;
            string filename = "/home/david/progMoveBankDir/license_terms.txt";
            md5 = calculateMD5(filename);
        }
    }
    if (!std::filesystem::exists("/home/david/progMoveBankDir/indFile/individu.txt")){
        cout << "Le fichier d'individus n'existe pas" << endl;

        // Récupération des données sur les tag de l'étude //
        string adresse = "https://www.movebank.org/movebank/service/direct-read?entity_type=tag&study_id=1266784970&license-md5="+md5; // Remplacez par l'URL de votre choix
        const char *url = adresse.c_str();
        CURLcode res;
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_USERPWD, "valpaz:valpaz21");
        curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "/home/david/progMoveBankDir/cookies.txt");
        FILE* outputFile;
        outputFile = fopen("/home/david/progMoveBankDir/output.txt", "wb");
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, outputFile);
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            cerr << "Erreur lors de la requête cURL : \n" << endl;
        }else {
            cout << "Requette réussie : Mise en place Récupération des individues\n" << endl;
        }
        fclose(outputFile);

        // Création du fichier individu contenant les id (Tag) de chaque individu de l'étude //
        string line;
        ifstream fileOutput("/home/david/progMoveBankDir/output.txt");
        ofstream fileIndiv("/home/david/progMoveBankDir/individu.txt", std::ios::out | std::ios::trunc);
        while (getline(fileOutput, line)) {
            stringstream ss(line);
            int cpt =0;
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
        //remove ("/home/david/progMoveBankDir/output.txt");
    }
    if (!std::filesystem::exists("/home/david/progMoveBankDir/indFile/individu.txt")){
        cout << "Recuperation initiale des events" << endl;
        // Recuperation initiale des events //
        auto t = time(nullptr);
        auto tm = *localtime(&t);
        ostringstream oss;
        oss << put_time(&tm, "%Y%m%d%H%M%S000");
        auto actualDate = oss.str();

        // Configuration de la période de récupération des events initial ( 1 semaine )
        ofstream fileDate("/home/david/progMoveBankDir/lastDateUpdate.txt");
        fileDate << actualDate << endl;
        fileDate.close();
        string past7date=getDateMinus7Days(actualDate)+actualDate.substr(8, 9);

        string adresse2 = "https://www.movebank.org/movebank/service/direct-read?entity_type=event&study_id=1266784970&timestamp_start="+past7date+"&timestamp_end="+actualDate+"&license-md5="+md5;
        const char *url2 = adresse2.c_str();
        CURLcode res2;
        curl_easy_setopt(curl, CURLOPT_URL, url2);
        curl_easy_setopt(curl, CURLOPT_USERPWD, "valpaz:valpaz21");
        curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "/home/david/progMoveBankDir/cookies.txt");
        FILE* outputEvent;
        outputEvent = fopen("/home/david/progMoveBankDir/outputEvent.txt", "wb");
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, outputEvent);
        // Exécution de la requête
        res2 = curl_easy_perform(curl);
        // Vérification de la réponse
        if (res2 != CURLE_OK) {
            cerr << "Erreur lors de la requête cURL : \n" << endl;
        }else {
            cout << "Requette réussie : Récuperation initial des 7 derniers jours d'event pour chaque individue\n" << endl;
        }
        fclose(outputEvent);

        
        ifstream fileInds("/home/david/progMoveBankDir/individu.txt");
        string ligne;
        string ligneEvent;
        string actualId;
        int cptt;
        while (getline(fileInds, ligne)) {
            actualId = ligne;

            ofstream fileInd(("/home/david/progMoveBankDir/indFile/"+actualId + ".csv").c_str());
            ifstream fileEvent("/home/david/progMoveBankDir/outputEvent.txt");
            getline(fileEvent, ligneEvent);
            while (getline(fileEvent, ligneEvent)) {
                stringstream ss(ligneEvent);
                string valeur;
                cptt = 0;
                while (getline(ss, valeur, ',')) {
                    if (cptt==4){
                        valeur.erase(valeur.find_last_not_of("\n\r") + 1); // Enleve le \n a la fin de la ligne           
                        if (valeur==actualId){
                            fileInd << ligneEvent << endl;

                        }
                    }
                    cptt++;
                }
            }
            fileInd.close();
            fileEvent.close();
            //remove ("/home/david/progMoveBankDir/outputEvent.txt");

        }
        fileInds.close();
    }


    // Get update event (every hours) //
    cout << "Recuperation UPDATE des events" << endl;
    string laDate;
    string lastDate;
    ifstream filedate("/home/david/progMoveBankDir/lastDateUpdate.txt");
    while (getline(filedate, laDate)) {
        lastDate=laDate;
    }
    filedate.close();
    auto t2 = time(nullptr);
    auto tm2 = *localtime(&t2);
    ostringstream oss2;
    oss2 << put_time(&tm2, "%Y%m%d%H%M%S000");
    auto actualDate2 = oss2.str();
    string adresse3 = "https://www.movebank.org/movebank/service/direct-read?entity_type=event&study_id=1266784970&timestamp_start="+lastDate+"&timestamp_end="+actualDate2+"&license-md5="+md5;
    
    // The actual date is the new last date
    ofstream fileDate("/home/david/progMoveBankDir/lastDateUpdate.txt");
    fileDate << actualDate2 << endl;
    fileDate.close();

    // Recuperation des events récents
    const char* url3 = adresse3.c_str();
    CURLcode res3;
    curl_easy_setopt(curl, CURLOPT_URL, url3);
    curl_easy_setopt(curl, CURLOPT_USERPWD, "valpaz:valpaz21");
    curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "/home/david/progMoveBankDir/cookies.txt");
    FILE* outputUpdate;
    outputUpdate = fopen("/home/david/progMoveBankDir/outputUpdateEvent.csv", "wb");
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, outputUpdate);
    // Exécution de la requête
    res3 = curl_easy_perform(curl);
    // Vérification de la réponse
    if (res3 != CURLE_OK) {
        cerr << "Erreur lors de la requête cURL : \n" << endl;
    }else {
        cout << "Requette réussie : Récuperation des event de la derniere heure pour chaque individue\n" << endl;
    }
    fclose(outputUpdate);


    // Ajout des nouveaux events pour chaque fichier d'individu
    ifstream fileUpdateInd("/home/david/progMoveBankDir/outputUpdate.csv");
    ifstream fileIndis("/home/david/progMoveBankDir/individu.txt");
    string ligne2;
    string ligneEvent2;
    string actualId2;
    int cptt2;
    while (getline(fileIndis, ligne2)) {
        actualId2 = ligne2;

        ofstream fileUpdateInd(("/home/david/progMoveBankDir/indFile/"+actualId2 + ".csv").c_str(),std::ios::app);
        ifstream fileUpdateEvent("/home/david/progMoveBankDir/outputUpdateEvent.csv");
        while (getline(fileUpdateEvent, ligneEvent2)) {
            stringstream ss(ligneEvent2);
            string valeur;
            cptt2 = 0;
            while (getline(ss, valeur, ',')) {
                if (cptt2==4){
                    if (valeur==actualId2){
                        fileUpdateInd << ligneEvent2 << endl;


                    }
                }
                cptt2++;
            }
        }
        fileUpdateInd.close();
        fileUpdateEvent.close();
        //remove ("/home/david/progMoveBankDir/outputUpdate.csv");

    }

    fileIndis.close();

    curl_easy_cleanup(curl);
    return 0;
}



