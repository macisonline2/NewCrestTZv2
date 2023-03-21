//jsonRec class -- jsonRec.h
using namespace std;
#pragma once
#include "cresHeader.h"
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <cstdio>
#include <sstream>

class jsonRec
{
	//json file containing site time zone data
	//
	string	siteAbbrv;			//site abbreviation
	string	siteTZ;				//site time zone name 
	int		crestronTZ;			//site time zone index for crestron devices
	string	siteIgnDLSrule;		//site flag indicating whether ignoring time zone rules

	friend bool sortjsonRecByAbbvrAscending(const jsonRec& lhs, const jsonRec& rhs);
	friend bool sortjsonRecByAbbvrDescending(const jsonRec& lhs, const jsonRec& rhs);

public:
	void createRec(string, string, int, string);
	void createRec(string);
	void updateRec(string, int, string);
	void shwRec(string*);
	string wrtRec(int);
	void parseRec(int, string);
	string shareAbbv(void);

	~jsonRec() {};
};

void jsonRec::createRec(string Abbv, string namTZ, int creTZ, string ignRule)
{
	if (Abbv.length() < 4)
		siteAbbrv = Abbv + "__";
	else if (Abbv.length() < 5)
		siteAbbrv = Abbv + "_";

	siteTZ = namTZ;
	crestronTZ = creTZ;
	siteIgnDLSrule = ignRule;
}

void jsonRec::createRec(string Abbv)
{
	siteAbbrv = Abbv;
}

void jsonRec::updateRec(string namTZ, int creTZ, string ignRule)
{
	siteTZ = "\"" + namTZ + "\"";
	crestronTZ = creTZ;
	if (ignRule == "T" || ignRule == "t" || ignRule == "1")
		siteIgnDLSrule = "true";
	else
		siteIgnDLSrule = "false";
}

void jsonRec::shwRec(string* lbl)
//show json records
{
	string tab = "  "; // define tab as two spaces

	cout << tab << '{' << endl;
	cout << tab << tab << '"' << lbl[0] << "\":" << siteAbbrv << ',' << endl;
	cout << tab << tab << '"' << lbl[1] << "\":" << siteTZ << ',' << endl;
	cout << tab << tab << '"' << lbl[2] << "\":" << crestronTZ << ',' << endl;
	cout << tab << tab << '"' << lbl[3] << "\":" << siteIgnDLSrule << endl;
	cout << tab << "}," << endl;
}

string jsonRec::wrtRec(int i)
//export out json record fields
{
	switch (i)
	{
	case 0:
		return siteAbbrv;
		break;
	case 1:
		return siteTZ;
		break;
	case 2:
		return to_string(crestronTZ);
		break;
	case 3:
		return siteIgnDLSrule;
		break;
	default:
		break;
	}
}

string ltrim(string& s)
{
	const std::string WHITESPACE = " ";

	size_t start = s.find_first_not_of(WHITESPACE);
	return (start == std::string::npos) ? "" : s.substr(start);
}

void jsonRec::parseRec(int idx, string bufline)
{
	string val, v[4];          //record and field of csv file

	stringstream s(bufline);       // parsing line into words

	// read value data of the line and store it in 'val'
	getline(s, val, ':');
	getline(s, val, ',');

	val = ltrim(val);

	switch (idx)
	{
	case 0:
		siteAbbrv = val;
		break;
	case 1:
		siteTZ = val;
		break;
	case 2:
		crestronTZ = stoi(val);
		break;
	case 3:
		siteIgnDLSrule = val;
		break;
	default:
		break;
	}
}

string jsonRec::shareAbbv(void)
//extract and share the site abbrv
{
	return siteAbbrv;
}

bool sortjsonRecByAbbvrAscending(const jsonRec& lhs, const jsonRec& rhs)
{
	return lhs.siteAbbrv < rhs.siteAbbrv;
}

bool sortjsonRecByAbbvrDescending(const jsonRec& lhs, const jsonRec& rhs)
{
	return lhs.siteAbbrv > rhs.siteAbbrv;
}

