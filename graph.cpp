/**
 * @file graph.cpp
 *
 * @brief Generates movement maps for each individuals and also for all of them combines together on a single map.
 */

#include <iostream>
#include <stdio.h>
#include <string>
#include <filesystem>
#include <fstream>
#include <gd.h>
#include <iostream>
#include <ctime>
#include <sstream>
#include "myFunctions.h"

using namespace std;

// g++ -std=c++17 -o graph graph.cpp myFunctions.cpp -lgd
// /home/david/formation/buildroot-2023.08/output/host/bin/aarch64-buildroot-linux-gnu-g++ -o graphQemu graph.cpp myFunctions.cpp -lgd



/**
 * @brief This main function generate movement maps of the individual present in the study
 *
 * It generates individual movement maps for each GPS tracked individual and in the same time combines
 * them into a single map. 
 * It uses the GD library
 *
 */
int main() {

	const char* homeDir = getenv("HOME");

	// Generate map of all individual together
	FILE *map = fopen("./../image_repository/map.png", "rb");
	gdImagePtr background = gdImageCreateFromPng(map);
	int width = gdImageSX(background);
	int height = gdImageSY(background);
	gdImagePtr newImage = gdImageCreateTrueColor(width, height);
	gdImageCopy(newImage, background, 0, 0, 0, 0, width, height);
	int black = gdImageColorAllocate(newImage, 0, 0, 0);
	int blue = gdImageColorAllocate(newImage, 0, 0, 255);

	// Initialization
	string folderPath = string(homeDir) + "/moveBankProjet/indFiles/";
	double latitude, longitude, lastLatitude, lastLongitude, latitudeGD, longitudeGD, x, y, lastX, lastY;
	string daySubstr;
	int couleur, couleurInd;


	time_t t = time(nullptr);
	tm* currentTime = std::localtime(&t);
	int currentDay = currentTime->tm_mday;
    auto tm = *localtime(&t);
    ostringstream oss;
    oss << put_time(&tm, "%Y%m%d%H%M");
    auto currentDate = oss.str();


    string newFolder = string(homeDir) + "/moveBankProjet/indGraph-"+currentDate+"/";
    filesystem::create_directory(newFolder.c_str());   

	// For each file in individual folder (indFile)
	for (const auto &entry : filesystem::directory_iterator(folderPath)){

		string line;
		ifstream fileOutput(entry.path().string());
		string fileName = entry.path().filename().string();
		string mapName = fileName.substr(0, fileName.find_last_of('.'));

		// Check if file not empty
		fileOutput.seekg(0,ios::end);
		if (!fileOutput.tellg() == 0){
			fileOutput.seekg(0, std::ios::beg);

			// Check if file start with a number
			if (isdigit(fileName[0])){
				FILE *mapBG = fopen("./../image_repository/map.png", "rb");

				// Each individual get an associated movement map
				gdImagePtr backgroundInd = gdImageCreateFromPng(mapBG);
				gdImagePtr newImageInd = gdImageCreateTrueColor(1722, 738);
				gdImageCopy(newImageInd, backgroundInd, 0, 0, 0, 0, 1722, 738);
				int black = gdImageColorAllocate(backgroundInd, 0, 0, 0);
				int blue = gdImageColorAllocate(backgroundInd, 0, 0, 255);

				int cptFirst = 0;

				while (getline(fileOutput, line)) {

					stringstream ss1(line);
					int cpt1 = 0;
					string substr1;

					// Current latitude and longitude
					while (ss1.good()) {
						getline(ss1, substr1, ',');
						if (cpt1 == 0){
							daySubstr = substr1.substr(8, 2);
							int daySub = stod(daySubstr);
							couleur = whichColor(currentDay, daySub, newImage);
							couleurInd = whichColor(currentDay, daySub, backgroundInd);
						}
						if (cpt1 == 1){
							latitude = stod(substr1);}
						if (cpt1 == 2){
							longitude = stod(substr1);}
						cpt1++;

					}

					// Convertion from google map to libGD output
					x = static_cast<double>((longitude - 2.29537618870689) * (width / (2.467592180568534 - 2.29537618870689)));
					y = height - static_cast<double>((latitude - 48.79861078994466) * (height / (48.89937259067075 - 48.79861078994466)));
					// x = static_cast<double>((longitude - 2.036005930852261) * (width / (2.6273598267740628 - 2.036005930852261)));
					// y = height - static_cast<double>((latitude - 48.77337476303239) * (height / (48.94012387335636 - 48.77337476303239)));					
					// set lines and points
					gdImageFilledEllipse(newImage, x, y, 2 * 3, 2 * 3, blue);
					gdImageFilledEllipse(backgroundInd, x, y, 2 * 3, 2 * 3, blue);
					// gdImageSetPixel(newImage, x, y, black);
					// gdImageSetPixel(backgroundInd, x, y, black);
					if (cptFirst > 0 ){

						gdImageLine(newImage, x, y-1, lastX, lastY-1, couleur);
						gdImageLine(newImage, x, y+1, lastX, lastY+1, couleur);
						gdImageLine(newImage, x, y, lastX, lastY, couleur);
						gdImageLine(backgroundInd, x, y-1, lastX, lastY-1, couleur);
						gdImageLine(backgroundInd, x, y+1, lastX, lastY+1, couleur);
						gdImageLine(backgroundInd, x, y, lastX, lastY, couleur);
					}

					// Previous latitude and longitude
					stringstream ss2(line);
					int cpt2 = 0;
					string substr2;
					while (ss2.good()) {
						getline(ss2, substr2, ',');
						if (cpt2 == 1){
							lastLatitude = stod(substr2);}
						if (cpt2 == 2){
							lastLongitude = stod(substr2);}
						cpt2++;
					}

					// Convertion from google map to libGD
					lastX = static_cast<double>((lastLongitude - 2.29537618870689) * (width / (2.467592180568534 - 2.29537618870689)));
					lastY = height - static_cast<double>((lastLatitude - 48.79861078994466) * (height / (48.89937259067075 - 48.79861078994466)));
					cptFirst++;
				}		
				//Create a map for each individual
				// avoid bird from other location than France (Sicilia and Israel)
				if (48 < latitude && latitude < 49 && 2 < longitude && longitude < 3) {
					string fileIndPath = newFolder+mapName+".png";
					cout << fileIndPath << endl;
					FILE *mapInd = fopen(fileIndPath.c_str(), "wb");
					gdImagePng(backgroundInd, mapInd);	
					fclose(mapInd);}
				fclose(mapBG);
				gdImageDestroy(backgroundInd);
				gdImageDestroy(newImageInd);
				

			}

		}
		fileOutput.close();

	}
	FILE *outputFile = fopen((string(homeDir) + "/moveBankProjet/mapAll.png").c_str(), "wb");
	gdImagePng(newImage, outputFile);
	fclose(outputFile);
    gdImageDestroy(background);
    gdImageDestroy(newImage);
    cout << "Done." << endl;
    return 0;
}

// NW 48.89937259067075, 2.29537618870689
// NE 48.89929532761945, 2.467486943304439
// SE 48.79861078994466, 2.467592180568534
// SW 48.79836557102456, 2.295839965625057

// xmin = 2.29537618870689
// xman = 2.467592180568534
// ymax = 48.89937259067075
// ymin = 48.79861078994466

// 48.938985224316646, 2.036224305182475 NW
// 48.94012387335636, 2.6273598267740628 NE
// 48.773928364313456, 2.627065674714942 SE
// 48.773554769497984, 2.036252897418881 SW
// xmin = 2.036005930852261
// xmax = 2.6273598267740628
// ymax = 48.94012387335636
// ymin = 48.77337476303239
//
// 48.77337476303239, 2.036005930852261

// 1722x738

//longetitude c'est X
// latittude c'est Y

// G846  tag : 2612465057 
// id : 2612460752