# This file will parse the .dat files from the
# traces collected

#import numpy as np
import argparse
import struct

class ReadAlloftheDataAndStuffALSOProbablyDoSomethingWithIt:
    def __init__(self, fileName):
        self.fileName = fileName
        self.dataFile = None

    def readData(self):
        print('You created an object! FileName given: ' + self.fileName)
        self.dataFile = open(self.fileName, 'rb')
        if (self.dataFile):
            print('Opened Successfully!!!')
            self.parseData()
            self.dataFile.close()

    def parseHeader(self):
        pass #TODO parse header 

    def parseData(self):
        gpsDate = self.dataFile.read(15)
        print('GPS Time:'),
        print(gpsDate.decode("utf-8"))
        lat = self.dataFile.read(4) #NEED TO UNPACK
        lat = struct.unpack('f', lat)[0]
        print('Latitude:'),
        print(lat)
        lon = self.dataFile.read(4)
        lon = struct.unpack('f', lon)[0]
        print('Longitude:'),
        print(lon)
        sats = self.dataFile.read(4)
        sats = struct.unpack('i', sats)[0]
        print('Number of Satalites:'),
        print(sats)
        altitude = self.dataFile.read(4)
        altitude = struct.unpack('f', altitude)[0]
        print('Altitude:'),
        print(altitude)
        
        #TODO now get data (need header info for numBytes to read)


#Get arguments
def main():
    parser = argparse.ArgumentParser(description='Process some stuff.')
    parser.add_argument('--file', '-f', help='Give file name')
    parser.add_argument('-a', help='additional arg')
    args =vars( parser.parse_args())

    # We need a file to read for this script to have any point
    # FILEPATH IS PROBABLY GONNA BE "../cpp_format_writer/usrp_samples.dat"
    if args['file']:
        reader = ReadAlloftheDataAndStuffALSOProbablyDoSomethingWithIt(args['file'])
        reader.readData()

        del reader
    else:
        print('Please enter a file name after a -f.')
        print('For example: -f data.dat')
    

    
    


#GET DATA FILE

#READ DATA FILE BASED OF FORMAT FILE

if __name__ == "__main__":
    main()
