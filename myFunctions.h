/**
 * @file myFunctions.h
 *
 * @brief Header file containing functions for date and gd image processing.
 */
#pragma once
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <gd.h>
using namespace std;

/**
 * @brief Get the current date and time as a string.
 *
 * @return The current date.
 */
string currentDateTime();

/**
 * @brief Get the date x days before a given date.
 *
 * Given a date in the format "YYYYMMDD," it calculates and returns the date
 * that is x days earlier in the same format.
 *
 * @param inputDate The input date in "YYYYMMDD" format.
 * @return The date x days before the input date.
 */
string getDateMinus7Days(const std::string& inputDate);

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
int whichColor( int &currentDay, int &day, gdImagePtr newImage);