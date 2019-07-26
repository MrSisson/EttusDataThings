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
        #print('You created an object! FileName given: ' + self.fileName)
        try: self.dataFile = open(self.fileName, 'rb')
        except FileNotFoundError:
            pass    
        if (self.dataFile):
            print('Opened Successfully!!!')
            self.parseHeader()
            self.parseData()
            self.dataFile.close()
        else:
            print('Could not open file: ' + self.fileName)

    def parseHeader(self):
        print('****************************************')
        date = self.dataFile.read(10)
        print('Date:', end =" ")
        print(date.decode("utf-8"))
        print('****************************************')
        print(' ')
        # Parse floats
        print('****************************************')
        print('**************** HEADER ****************')
        print('****************************************')
        chirpBw = self.dataFile.read(4) #reads float
        chirpBw = struct.unpack('f', chirpBw)[0]
        print('Bandwidth: ' + str(chirpBw) + '%')
        chirpCf = self.dataFile.read(4)
        chirpCf = struct.unpack('f', chirpCf)[0]
        print('Center Frequency: ' + str(chirpCf / 1e6) + ' MHz')
        chirpTL = self.dataFile.read(4)
        chirpTL = struct.unpack('f', chirpTL)[0]
        print('Chirp Time Length: ' + str(round(chirpTL * 1e6, 1)) + ' us')
        chirpAmp = self.dataFile.read(4)
        chirpAmp = struct.unpack('f', chirpAmp)[0]
        print('Amplitude: ' + str(round(chirpAmp, 3)))
        prf = self.dataFile.read(4)
        prf = struct.unpack('f', prf)[0]
        print('Pulse Repetition Frequency: ' + str(prf / 1e3) + ' KHz')
        receiveTL = self.dataFile.read(4)
        receiveTL = struct.unpack('f', receiveTL)[0]
        print('Trace Time Length: ' + str(round(receiveTL * 1e6, 1)) + ' us')

        # Parse ints
        txSampFreq = self.dataFile.read(4)
        txSampFreq = struct.unpack('i', txSampFreq)[0]
        print('Transmit Sampling Frequency: ' + str(txSampFreq / 1e6) + ' MHz')
        rxSampFreq = self.dataFile.read(4)
        rxSampFreq = struct.unpack('i', rxSampFreq)[0]
        print('Receiver Sampling Frequency: ' + str(rxSampFreq / 1e6) + ' MHz')
        stackSize = self.dataFile.read(4)
        stackSize = struct.unpack('i', stackSize)[0]
        print('Stack Size: ' + str(stackSize))


    def parseData(self):
        print('\n')
        print('****************************************')
        print('****************  DATA  ****************')
        print('****************************************')
        gpsDate = self.dataFile.read(13)
        print('GPS Time:', end =" ")
        print(gpsDate.decode("utf-8"))
        lat = self.dataFile.read(4)
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
