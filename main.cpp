
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
    fwrite(data, sizeof(float), traceLen, dataFile);

    return 0;
}
