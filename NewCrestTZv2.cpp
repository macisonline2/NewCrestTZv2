// NewCrestTZv2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

using namespace std;
#pragma once
#include "cresHeader.h"
#include <cstdio>

//pathfile for work laptop
string PATH_IN = "C:\\Users\\aragonma\\OneDrive - Church of Jesus Christ\\4.EMP\\AV Engineering\\CresTimeZones\\data\\";

//pathfile for personal laptop
//string PATH_IN = "C:\\Users\\macis\\Documents\\Cpp\\NCTZ\\data\\";

string CURJSN_IN = "SiteTimeZones.json";        //Original json file
string NEWJSN_OUT = "newSiteTimeZones.json";	//New json file
string CRMCSV_IN = "crmActiveTemples.csv";      //csv file from crm

//***************************************************************
// Main Program 
//***************************************************************
int main(int argc, char* argv[])
{
    //Objects
    // moving the data to the heap
    csvRec* crmRec = new csvRec[RECS];          //crmFile[0..RECS]
    jsonRec* oldJson = new jsonRec[RECS];       //oldJson[0..RECS]
    jsonRec* newJson = new jsonRec[RECS];       //newJson[0..RECS]

    vector<jsonRec> jsonOrd;                    //vector jsonOrd[] to order jsonRec records

    //full pathfile of data to process
    CRMCSV_IN = PATH_IN + CRMCSV_IN;
    CURJSN_IN = PATH_IN + CURJSN_IN;
    NEWJSN_OUT = PATH_IN + NEWJSN_OUT;

    //User interface Main Menu 
    int opt = 0;
    int ctems = 0, jtems = 0, ntems = 0;        //object counters
    int nextopt = 1;
    int jcount;                                 //counter for new records

    //json file labels
    string labels[] = { "SiteName","TimeZoneDisplayName10","CrestronTimeZoneId","IgnoreDaylightSavingRules" };
    const int jlabels = sizeof(labels) / sizeof(labels[0]);       //number of fields in json record

    do {
        system("cls");
        cout << "  Site Time Zones Update Program" << endl;
        cout << "  ====================================" << endl;
        cout << "  1.  Process files SiteTimeZones.json and crmActiveTemples.csv" << endl;
        cout << "  2.  Add Time Zone information to new sites" << endl;
        cout << "  3.  Export new SiteTimeZones json file" << endl;
        cout << "  4.  Exit" << endl;
        cout << "  ====================================" << endl;
        cout << "  Enter your selection: ";

        //validate opt is an integer and not empty
        while (!(cin >> opt)) {
            cout << "Invalid value, try again.\n\nEnter your selection: ";
            cin.clear();
            cin.ignore(numeric_limits<int>::is_integer, '\n');
        }
        
        cout << endl;
    

        switch (opt) {

        case 1: {
            //Read and load crm.csv and timezone.json files
            cout << "Make sure that files:" << endl;
            cout << "\tSiteTimeZones.json and \n\tcrmActiveTemples.csv \nare in the folder [data].\n" << endl;
            system("pause");

            //*****Parse old site json file and Save it into a json_object_1
            ifstream f_in_j;
            string linejson, jrec;              //record and field of csv file
            f_in_j.open(CURJSN_IN, ios::in);
            int i = 0;
            while (f_in_j) {
                getline(f_in_j, linejson);
                if (linejson == "[" || linejson == "]") continue;   //skip lines begining w/ [ or ] 
                if (linejson == "  {") {            //json record begins
                    //extract every line until end of rec
                    for (int k = 0; (linejson != "  }," && linejson != "  }"); k++) {
                        getline(f_in_j, linejson);
                        oldJson[i].parseRec(k, linejson);
                    };
                    i++;
                }
            }
            for (int j = 0; j < i; j++) oldJson[j].shwRec(labels);
            jtems = i;
            cout << jtems << " File SiteTimeZones.json records have been processed.\n";
            system("pause");

            //*****Parse the new sites from CRM csv file and Save it into a crm_object
            ifstream f_in_c;
            string linecsv;                         //record and field of csv file
            f_in_c.open(CRMCSV_IN, ios::in);
            int ix = 0;
            while (f_in_c) {
                getline(f_in_c, linecsv);
                if (linecsv[0] < 65 || linecsv[0] > 95) continue;
                crmRec[ix].parseRec(linecsv);
                cout << ix << " ";
                crmRec[ix].shwRec();
                ix++;
            }
            ctems = ix;
            cout << " File crmActiveTemples.csv records have been processed.\n";
            system("pause");

        }
              nextopt = 2;
              break;

        case 2: {
            // Copy all old sites from json file to oldJson class
            // Read crm sites from crmRec to find new sites not present oldJson class
            // Add new crm_sites(sitename,UTC) to newJson class
            // Prompt user to complete the data in newJson (TZDisplayName10,CrestronTZId,DLSrules) 

            string newSite = "", jsonSite = "", abbrv = "";
            string timezoneDesc;
            int crestID;
            string ignoreTZ;

            if (nextopt != 2) {
                cout << "ERROR - You have to run the option 1 first\n";
                system("pause");
                break;
            };

            cout << "Have ready timezone data to fill the requested information\n" << endl;
            system("pause");
            jcount = jtems;

            //copy oldjson array to a newjson array
            for (int j = 0; j < jtems; j++) {
                newJson[j] = oldJson[j];
            }

            //select each CRM object to find
            for (int c = 0; c < ctems; c++) {
                bool found = false;
                newSite = crmRec[c].shareAbbrv();           //add quotes

                //compare with every json object
                for (int j = 0; j < jtems; j++) {
                    jsonSite = oldJson[j].shareAbbv();

                    if (newSite == jsonSite) {
                        j = jtems;
                        found = true;                       //crmRec found
                    }
                }


                if (found == false) {
                    cout << "Entering new crm record (Abbv,Desc,UTC): ";
                    crmRec[c].shwRec();

                    //enter new record crmRec not found in old jsonRec
                    newJson[jcount].createRec(newSite);
                    cout << "\nUpdating new record: " << " - Site: " << newSite << "\n\n";

                    cout << "Enter the corresponding time zone description: ";
                    cin.ignore();
                    getline(cin, timezoneDesc, '\n');

                    cout << "\nEnter the corresponding Crestron time zone ID: ";
                    cin >> crestID;

                    cout << "\nEnter the corresponding IgnoreTimeZoneRule 'T' or 'F': ";
                    cin.ignore();
                    getline(cin, ignoreTZ, '\n');

                    //Prompt to enter missing data for crmRec not found in old jsonRec
                    newJson[jcount].updateRec(timezoneDesc, crestID, ignoreTZ);

                    jcount++;       //jcount = total of newJson records
                }

            }

            //enter data into vector
            for (int i = 0; i < jcount; i++) {
                jsonOrd.push_back(newJson[i]);
            }

            //sort vector
            sort(jsonOrd.begin(), jsonOrd.end(), sortjsonRecByAbbvrAscending);

        }
              nextopt = 3;
              break;

        case 3: {
            // Export newJson class to a json file
            if (nextopt != 3) {
                cout << "ERROR - You have to run the options 1 and 2 first\n";
                system("pause");
                break;
            };

            int k1;
            string tab = "  ";                  // define tab as two spaces
            ofstream f_out_j(NEWJSN_OUT, ios::out);
            string field[jlabels];              //jlabels is number of fields in json record

            cout << "Exporting data to SiteTimeZones.json" << endl;
            system("pause");

            //write output file
            f_out_j << "[" << endl;
            for (int k = 0; k < jcount; k++) {
                for (int i = 0; i < jlabels; i++) {
                    field[i] = jsonOrd.at(k).wrtRec(i);
                }
                f_out_j << tab << '{' << endl;
                f_out_j << tab << tab << '"' << labels[0] << "\": " << field[0] << ',' << endl;
                f_out_j << tab << tab << '"' << labels[1] << "\": " << field[1] << ',' << endl;
                f_out_j << tab << tab << '"' << labels[2] << "\": " << field[2] << ',' << endl;
                f_out_j << tab << tab << '"' << labels[3] << "\": " << field[3] << endl;

                if (k < (k1 = jcount - 1))
                    f_out_j << tab << "}," << endl;
                else
                    f_out_j << tab << "}" << endl;

            }

            f_out_j << "]" << endl;

            f_out_j.close();
            cout << "File "<< NEWJSN_OUT <<" is ready.\n" << endl; 
            system("pause");
        }
              break;

        case 4: {
            // Exit
            cout << "**** GOOD BYE!!!!!! ****" << endl;
            //continue;
        }
              break;
    
        default:
            system("cls");
        }
    } while (opt != 4);

    return 0;
}
