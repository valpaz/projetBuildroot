#include <iostream>
#include <stdio.h>
#include <string>
#include <filesystem>
#include <fstream>
#include <gd.h>
#include <iostream>
#include <ctime>
#include <sstream>

using namespace std;

int whichColor( int &actualDay, int &day, gdImagePtr newImage){
	int diff = actualDay - day;
	if (diff == 0){
		int color = gdImageColorAllocate(newImage, 0, 0, 255);
		return color;
	}else if(diff == 1){
		int color = gdImageColorAllocate(newImage, 0, 0, 0);
		return color;		
	}else if(diff == 2){
		int color = gdImageColorAllocate(newImage, 255, 40, 40);
		return color;	
	}else if(diff == 3){
		int color = gdImageColorAllocate(newImage, 255, 80, 80);
		return color;	
	}else if(diff == 4){
		int color = gdImageColorAllocate(newImage, 255, 120, 120);
		return color;	
	}else if(diff == 5){
		int color = gdImageColorAllocate(newImage, 255, 160, 160);
		return color;	
	}else if(diff == 6){
		int color = gdImageColorAllocate(newImage, 255, 200, 200);
		return color;	
	}else if(diff == 7){
		int color = gdImageColorAllocate(newImage, 255, 240, 240);
		return color;	
	}else{
		int color = gdImageColorAllocate(newImage, 255, 240, 240);
		return color;			
	}
}
int main() {

	// General map of all individual
	FILE *carte = fopen("/home/david/progMoveBankDir/carte.png", "rb");
	
	gdImagePtr background = gdImageCreateFromPng(carte);
	int width = gdImageSX(background);
	int height = gdImageSY(background);
	gdImagePtr newImage = gdImageCreateTrueColor(width, height);
	gdImageCopy(newImage, background, 0, 0, 0, 0, width, height);
	int black = gdImageColorAllocate(newImage, 0, 0, 0);
	int blue = gdImageColorAllocate(newImage, 0, 0, 255);

	// Initialization
	string folderPath = "/home/david/progMoveBankDir/indFile/";
	double latitude;
	double longitude;
	double lastLatitude;
	double lastLongitude;
	double latitudeGD;
	double longitudeGD;
	double x;
	double y;
	double lastX;
	double lastY;
	string daySubstr;
	int couleur;
	int couleurInd;
	time_t t = time(nullptr);
	tm* currentTime = std::localtime(&t);
	int actualDay = currentTime->tm_mday;
    auto tm = *localtime(&t);
    ostringstream oss;
    oss << put_time(&tm, "%Y%m%d%H%M");
    auto actualDate = oss.str();
    string newFolder = "/home/david/progMoveBankDir/indGraph-"+actualDate+"/";
    filesystem::create_directory(newFolder.c_str());     
	// For each file in individual folder
	for (const auto &entry : filesystem::directory_iterator(folderPath)){

		string line;
		ifstream fileOutput(entry.path().string());
		string fileName = entry.path().filename().string();
		string mapName = fileName.substr(0, fileName.find_last_of('.'));
		cout << entry.path().string() << endl;
		//FILE *carteInd = fopen("/home/david/progMoveBankDir/indGraph/carte.png", "rb");
		// Check if file not empty
		fileOutput.seekg(0,ios::end);
		if (!fileOutput.tellg() == 0){
			fileOutput.seekg(0, std::ios::beg);

			//Check if file start with a number
			if (isdigit(fileName[0])){
				FILE *carteBG = fopen("/home/david/progMoveBankDir/carte.png", "rb");
				// Each individual get an associated map
				
				gdImagePtr backgroundInd = gdImageCreateFromPng(carteBG);
				gdImagePtr newImageInd = gdImageCreateTrueColor(1722, 738);
				gdImageCopy(newImageInd, backgroundInd, 0, 0, 0, 0, 1722, 738);
				int black = gdImageColorAllocate(backgroundInd, 0, 0, 0);
				int blue = gdImageColorAllocate(backgroundInd, 0, 0, 255);

				int cptFirst = 0;
				while (getline(fileOutput, line)) {

					stringstream ss1(line);
					int cpt1 = 0;
					string substr1;
					while (ss1.good()) {
						getline(ss1, substr1, ',');
						if (cpt1 == 0){
							daySubstr = substr1.substr(8, 2);
							int daySub = stod(daySubstr);
							couleur = whichColor(actualDay, daySub, newImage);
							couleurInd = whichColor(actualDay, daySub, backgroundInd);
						}
						if (cpt1 == 1){
							latitude = stod(substr1);}
						if (cpt1 == 2){
							longitude = stod(substr1);}
						cpt1++;

					}
					
					x = static_cast<double>((longitude - 2.036005930852261) * (width / (2.6273598267740628 - 2.036005930852261)));
					y = height - static_cast<double>((latitude - 48.77337476303239) * (height / (48.94012387335636 - 48.77337476303239))); // pas sur car dans libGD le 0 est en haut a gauche et sur google map en bas a gauche
					gdImageSetPixel(newImage, x, y, black);
					gdImageSetPixel(backgroundInd, x, y, black);
					if (cptFirst > 0 ){
						gdImageLine(newImage, x, y, lastX, lastY, couleur);
						gdImageLine(backgroundInd, x, y, lastX, lastY, couleur);
					}

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
					lastX = static_cast<double>((lastLongitude - 2.036005930852261) * (width / (2.6273598267740628 - 2.036005930852261)));
					lastY = height - static_cast<double>((lastLatitude - 48.77337476303239) * (height / (48.94012387335636 - 48.77337476303239))); // pas sur car dans libGD le 0 est en haut a gauche et sur google map en bas a gauche
					cptFirst++;
				}
				//Create a map for each individual
				// avoid bird from other location than France (Sicilia and Israel)
				if (48 < latitude && latitude < 49 && 2 < longitude && longitude < 3) {
					string fileIndPath = newFolder+mapName+".png";
					FILE *carteInd = fopen(fileIndPath.c_str(), "wb");
					gdImagePng(backgroundInd, carteInd);	
					fclose(carteInd);}
				fclose(carteBG);
				gdImageDestroy(backgroundInd);
				gdImageDestroy(newImageInd);
				

			}

		}
		fileOutput.close();

	}
	FILE *outputFile = fopen("/home/david/progMoveBankDir/mapAll.png", "wb");
	gdImagePng(newImage, outputFile);
	fclose(outputFile);
    gdImageDestroy(background);
    gdImageDestroy(newImage);
}

// 48.938985224316646, 2.036224305182475 NW
// 48.94012387335636, 2.6273598267740628 NE
// 48.773928364313456, 2.627065674714942 SE
// 48.773554769497984, 2.036252897418881 SW
