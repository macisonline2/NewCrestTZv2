//csvFile class -- csvFile.h

#pragma once
using namespace std;
#include "cresHeader.h"
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <cstdio>
#include <sstream>

class csvRec
    //csv record from csv CRM file 
{
    string  lineRec;             //csv line record
    string  siteAbbrv;           //site abbreviation
    string  siteTimeZoneName;    //site time zone description
    string  siteTimeZoneOffset;  //site time zone offset

public:
    void parseRec(string);
    void shwRec(void);
    string shareAbbrv(void);
    ~csvRec() {};
};

void csvRec::parseRec(string line)
{
    string word, w[3];          //record and field of csv file
    stringstream s(line);       // parsing line into words

    // read every column data of the line and store it in 'word'
    for (int j = 0; getline(s, word, ','); j++) {
        w[j] = word;
        char ch = w[j][0];
    }
    if (w[0].length() < 4) w[0] = w[0] + "__";
    else if (w[0].length() < 5) w[0] = w[0] + "_";

    siteAbbrv = "\"" + w[0] + "\"";
    siteTimeZoneName = w[1];
    siteTimeZoneOffset = w[2];

}

void csvRec::shwRec(void)
//print one record in csv format
{
    cout << siteAbbrv << ',' << siteTimeZoneName << ',' << siteTimeZoneOffset << endl;
}

string csvRec::shareAbbrv(void)
//extract and share the site abbrv
{
    return siteAbbrv;
}
