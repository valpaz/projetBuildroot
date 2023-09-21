/**
 * @file myFunctions.h
 *
 * @brief Header file containing functions for date and gd image processing.
 */
#include <gd.h>
#include "myFunctions.h"
/**
 * @brief Get the current date and time as a string.
 *
 * @return The current date.
 */
string currentDateTime() {
    auto t = time(nullptr);
    auto tLocal = *localtime(&t);
    ostringstream oss;
    oss << put_time(&tLocal, "%Y%m%d%H%M%S");
    auto currentDate = oss.str();
    return currentDate;
}
/**
 * @brief Get the date x days before a given date.
 *
 * Given a date in the format "YYYYMMDD," it calculates and returns the date
 * that is x days earlier in the same format.
 *
 * @param inputDate The input date in "YYYYMMDD" format.
 * @return The date x days before the input date.
 */
string getDateMinus7Days(const std::string& inputDate) {
    std::tm timeinfo = {};
    std::istringstream ss(inputDate);
    ss >> std::get_time(&timeinfo, "%Y%m%d");
    std::time_t timestamp = std::mktime(&timeinfo) - (7 * 24 * 60 * 60);
    std::tm resultTimeinfo = *std::localtime(&timestamp);
    std::ostringstream oss;
    oss << std::put_time(&resultTimeinfo, "%Y%m%d");

    return oss.str();
}
/**
 * @brief Determine the color based on the number of days from now.
 *
 * This function determines the color code based on the difference between the current day
 * and the past days. It is used for image processing.
 *
 * @param currentDay The current day.
 * @param day The past day.
 * @param newImage A pointer to the GD image objet.
 * @return The color code.
 */
int whichColor( int &currentDay, int &day, gdImagePtr newImage){
    int diff = currentDay - day;
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