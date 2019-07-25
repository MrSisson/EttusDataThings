 
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <sstream>
#include <stdio.h>
#include <string.h>

int saveHdr(FILE* dataFile, float chirpBW, float chirpCF,
    float chirpTL, float chripAmp, float prf, float receiveTL,
    int transmitSampFreq, int receiveSampFreq, unsigned int stackSize);
int saveTrace(FILE* dataFile, float *data, std::string nmea, int traceLen);

int main(int argc,char* argv[]) {

    FILE* outFile;
    //std::cout << "HELLO" << std::endl;
    if (argc == 2) {
        outFile = fopen(argv[1], "wb");
        std::cout << "file: " << argv[1] << std::endl;
    }
    if (outFile == NULL) {
        std::cout << "Can't open that file boi" << std::endl;
        return 1; //error
    }

    float bw = 100;
    float cf = 2e6;
    float tl = 2e-6;
    float amp = .5;
    float prf = 1e3;
    int sampFreq = 100e6;
    float rtl = 20e-6;
    unsigned int stack = 100;

    saveHdr(outFile, bw, cf, tl, amp, prf, rtl, sampFreq, sampFreq,
        stack);

    float data[] = {0, .2, .4, .8, 1.6, 3.2};
    int traceLen = 6;
    std::string nmea = 
        "$GPGGA,181908.00,3404.7041778,N,07044.3966270,W"
        ",4,13,1.00,495.144,M,29.200,M,0.10,0000*40";

    nmea = "$GPGGA,134658.00,5106.9792,S,11402.3003,W,2,09,1.0,1048.47,M,-16.27,M,08,AAAA*60";
    saveTrace(outFile, data, nmea, 6);

    fclose(outFile);
    return 0;
}

//Save header information in data file
int saveHdr(FILE* dataFile, float chirpBW, float chirpCF,
    float chirpTL, float chripAmp, float prf, float receiveTL,
    int transmitSampFreq, int receiveSampFreq, unsigned int stackSize) {
    
    float firstbuf[] = { chirpBW, chirpCF, chirpTL, chripAmp, prf,
        receiveTL};
    int secbuf[] = {transmitSampFreq, receiveSampFreq, stackSize};

    fwrite(firstbuf, sizeof(float), 6, dataFile);
    fwrite(secbuf, sizeof(int), 3, dataFile);

    return 0;
}

int saveTrace(FILE* dataFile, float *data, std::string nmea, int traceLen) {
    //testing stuff
    clock_t start, end;
    double cpu_time_used1, cpu_time_used2;
    // end testing stuff

    /* SHORT VERSION OF CODE
     * (no way to ensure const character length for nmea)
     */
    start = clock();
    //Max length of nmea sentence is 80 characters!
    for (int i = 0; i < 1000; ++i) {
    fwrite(nmea.c_str(), sizeof(char), nmea.length(), dataFile);
    fwrite(data, sizeof(float), traceLen, dataFile);
    }
    end = clock();
    cpu_time_used1 = ((double)(end - start) / CLOCKS_PER_SEC);
    std::cout << "Short one: " << cpu_time_used1 << std::endl;

    
    std::string sgps(nmea);
    time_t now = time(0);
    tm *gmtm = gmtime(&now);
    
    //Get todays date
    // std::stringstream date;
    // if (gmtm->tm_mday < 10) {
    //     date << '0';
    // }
    // date << gmtm->tm_mday << ':';
    // if (gmtm->tm_mon + 1 < 10) {
    //     date << '0';
    // }
    // date << gmtm->tm_mon + 1 << ':'
    //      << gmtm->tm_year + 1900 << '-';
    
    //*****************************FUCKCK
    start = clock();
    cpu_time_used2 = 0;

/**/for (int i = 0; i < 1000; ++i) {
        
        
    /*
    char gtime[40];
    //std::string gtime;
    float lat;
    char latD;
    float lon;
    char longD;
    int numSats = 0;
    float altitude = 0;

    //Scan each important element of the nmea->gps data
    sscanf(nmea.c_str(), "%*[^,], %[^,], %f, %c, %f, %c, %*[^,], "
         "%d, %*[^,], %f, %*[^,], %*[^,], %*[^,], %*[^,], %*[^,]",
         gtime, &lat, &latD, &lon, &longD, &numSats, &altitude);
    */
    
    std::string temp;
    int sectStrt = 0;
    int cnt = 0;
    float lat = 0;
    float lon = 0;

    for (int j = 0; j < nmea.length(); ++j) {
        if (nmea[j] == ',') {
            if (cnt == 1) {
                temp = nmea.substr(sectStrt, j - sectStrt);
                if (temp.length() > 15) {
                    temp.erase(15, std::string::npos);
                }
                else {
                    while (temp.length() < 15) {
                        temp.append("0");
                    }
                }
                fwrite(temp.c_str(), sizeof(char), 15, dataFile);
            }
            else if (cnt == 2) {
                lat = stof(nmea.substr(sectStrt, j - sectStrt));
            }
            else if (cnt == 3) {
                if (nmea[j - 1] == 'S') {
                    lat = -lat;
                }
                fwrite(&lat, sizeof(float), 1, dataFile);
            }
            else if (cnt == 4) {
                lon = stof(nmea.substr(sectStrt, j - sectStrt));
            }
            else if (cnt == 5) {
                if (nmea[j-1] == 'W') {
                    lon = -lon;
                }
                fwrite(&lon, sizeof(float), 1, dataFile);
            }
            else if (cnt == 7) {
                int sats = stoi(nmea.substr(sectStrt, j - sectStrt));
                fwrite(&sats, sizeof(int), 1, dataFile);
            }
            else if (cnt == 9) {
                float altitude = stof(nmea.substr(sectStrt, j - sectStrt));
                fwrite(&altitude, sizeof(float), 1, dataFile);
            }
            ++cnt;
            sectStrt = j + 1;
        }
    }
    
    /*
    //create nice string to work with because char* sucks
    std::string gtimeS(gtime);
    bool odd = false;
    
    
    //Loop to insert ':' into gps time string
    for (int i = 1; i < gtimeS.length(); ++i) {
        if (gtimeS[i] == '.') 
            break;
        else if (i % 2 == 0 && !odd) { 
            //We want a colon separating groups of two digits (except
            //for the float at the end)
            gtimeS.insert(i, ":");
            odd = true;
            ++i;
            //inserting offsets the even value spacing, so 
            //we must accomodate with a bool and incrementing
        }
        else if ((i) % 2 == 1 && odd) {
            gtimeS.insert(i, ":");
            odd = false;
            ++i;
        }
    }

    //We want exactly 26 characters  or 15 without date prepended
        
    if (gtimeS.length() > 15) {
        gtimeS.erase(15, std::string::npos);
    }
    else {
        while (gtimeS.length() < 15) {
            gtimeS.append("0");
        }
    }
    // Update lon/lat positivity (positive == North or East, else negative)
    if (latD == 'S') {
        lat = -lat;
    }
    if (longD == 'W') {
        lon = -lon;
    }

    
    const char *finalGTime = gtimeS.c_str();
    
    //std::cout << gtimeS << std::endl;
    
    fwrite(finalGTime, sizeof(char), 15, dataFile);
    fwrite(&numSats, sizeof(int), 1, dataFile);
    fwrite(&lon, sizeof(float), 1, dataFile);
    fwrite(&lat, sizeof(float), 1, dataFile);
    fwrite(&altitude, sizeof(float), 1, dataFile);
    fwrite(data, sizeof(float), traceLen, dataFile);
    */

    } //END FOR LOOP
    end = clock();
    cpu_time_used2 = ((double)(end - start) / CLOCKS_PER_SEC);

    double slower = cpu_time_used2 / (cpu_time_used1 ); 

    std::cout << "Longest code time: " << cpu_time_used2 << std::endl;
    std::cout << "About " << (int)slower << " times slower" << std::endl;
    //std::cout << "Difference = " << cpu_time_used2 - cpu_time_used1
        //<< std::endl << std::endl;
    


    //std::cout << gtimeS << std::endl << lat << " " << latD
    //    << std::endl << lon << " " << longD << std::endl
    //    << numSats << std::endl << altitude << std::endl;

    return 0;
}

