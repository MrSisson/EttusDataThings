/*  
 *  Author: Alex Sisson
 *  Date: 7/9/19
 *
 *  This Code is just here to test piping between cpp and python
 */

#define _USE_MATH_DEFINES
#include <iostream>
#include <string.h>
#include <chrono>
#include <thread>
#include <signal.h>
#include <stdio.h>
#include <fcntl.h> //Allows non-blocking pipe IMPORTANT
#include <math.h>

using namespace std;


/* 
    SIGNAL HANDLER
 */
static bool stop_signal_called = false;
void sig_int_handler(int)
{
    stop_signal_called = true;
}


int main(int argc, char** argv) {
    /*
    if (argc <= 1) {
        cout << "There are no arguments provided" << endl;
    }
    else {
        if (argc == 2)
            cout << "There is 1 extra command line argument" << endl;
        else   {
            cout << "There are " << argc - 1 << " extra command line arguments" << endl;
        }
        for (int i = 1; i < argc; ++i) {
            int numChars = strlen(argv[i]);

            for (int j = 0; j < numChars; ++j) {
                cout << argv[i][j];
            }
            cout << "  ";
        }
        cout << endl;
    }
    */

   // 16384 * 4 is the maximum buffer size (in bytes)
   // 16384 samples * 4 bytes per sample == limit
   int numSamps = 1000;
   if (argc == 2) {
       sscanf(argv[1], "%d", &numSamps);
       //numSamps = (int)argv[1];
       if (numSamps > 16384){
           numSamps = 16384;
       }
       else if (numSamps < 0) {
           numSamps = 1000;
       }
   }

    signal(SIGINT, &sig_int_handler);
    
    float amp = 1;
    float samps[numSamps];
    
    //Sending 1000 samples for now
    for (int i = 0; i < numSamps; ++i) {
        samps[i] = (float)sin(2* M_PI * ((float)i / (float)(numSamps-1)));
    }

    float q = 0;
    int i = 0;
    while (!stop_signal_called) {
        //cout << "Program is chillin" << endl;
        if (i == numSamps) {
            i = 0;
            amp = -amp;
            for (int j = 0; j < numSamps; ++j) {
                samps[j] = -samps[j];
            }
            this_thread::sleep_for(chrono::microseconds(300));
        }
        else if (numSamps - i >= 1024 ) {
            fwrite(&samps[i], 4, 1024, stdout); //Write 1024 floats to pipe
            i += 1024;
        }
        else { //Write (numSamps - i) floats to pipe
            fwrite(&samps[i], 4, numSamps - i, stdout); 
            i += (numSamps - i);
        }
        /*
        q = samps[i];
        fwrite(&q, 1, sizeof(float), stdout);
        fflush(stdout);
        ++i;
        */
    }
    cout << endl << "Program getting killed" << endl;

    cout << endl << endl;
    return 0;
}
