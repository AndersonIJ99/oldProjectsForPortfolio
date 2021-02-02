/*
    Tanzir Ahmed
    Department of Computer Science & Engineering
    Texas A&M University
    Date  : 2/8/19
 */
#include "common.h"
#include <sys/wait.h>
#include "FIFOreqchannel.h"
#include "MQreqchannel.h"
#include "SHMreqchannel.h"

using namespace std;


int main(int argc, char *argv[]){

    int c;
    int buffercap = MAX_MESSAGE;
    int p = 0, ecg = 1;
    double t = -1.0;
    bool isnewchan = false;
    bool isfiletransfer = false;
    string filename;
    string ipcmethod = "f";
    int nchannels = 1;
    int bytesPerChannel = 0;
    int remainder = 0;

    while ((c = getopt (argc, argv, "p:t:e:m:f:c:i:")) != -1){
        switch (c){
            case 'p':
                p = atoi (optarg);
                break;
            case 't':
                t = atof (optarg);
                break;
            case 'e':
                ecg = atoi (optarg);
                break;
            case 'm':
                buffercap = atoi (optarg);
                break;
            case 'c':
                isnewchan = true;
                nchannels = atoi(optarg);
                break;
            case 'f':
                isfiletransfer = true;
                filename = optarg;
                break;
            case 'i':
                ipcmethod = optarg;
                break;
        }
    }

    // fork part
    if (fork()==0){ // child

        char* args [] = {"./server", "-m", (char *) to_string(buffercap).c_str(), "-i", (char*)ipcmethod.c_str(), NULL};
        if (execvp (args [0], args) < 0){
            perror ("exec filed");
            exit (0);
        }
    }

    RequestChannel* control_chan = NULL;
    if(ipcmethod == "f")
        control_chan = new FIFORequestChannel ("control", FIFORequestChannel::CLIENT_SIDE);
    else if(ipcmethod == "q")
        control_chan = new MQRequestChannel ("control", RequestChannel::CLIENT_SIDE, buffercap);
    else if(ipcmethod == "m")
        control_chan = new SHMRequestChannel ("control", RequestChannel::CLIENT_SIDE, buffercap);

    // comment this out if you want individual channel runtime
    // start timer
    cout << "Starting timer for process" << endl;
    struct timeval start, end;
    gettimeofday(&start, NULL);

    RequestChannel *chan = control_chan;
    for(int i = 0; i < nchannels; i ++) {
        if (isnewchan) {
            cout << "Using the new channel everything following" << endl;
            MESSAGE_TYPE m = NEWCHANNEL_MSG;
            control_chan->cwrite(&m, sizeof(m));
            char newchanname[100];
            control_chan->cread(newchanname, sizeof(newchanname));
            if (ipcmethod == "f")
                chan = new FIFORequestChannel(newchanname, FIFORequestChannel::CLIENT_SIDE);
            else if (ipcmethod == "q")
                chan = new MQRequestChannel(newchanname, RequestChannel::CLIENT_SIDE, buffercap);
            else if (ipcmethod == "m")
                chan = new SHMRequestChannel(newchanname, RequestChannel::CLIENT_SIDE, buffercap);
            cout << "New channel by the name " << newchanname << " is created" << endl;
            cout << "All further communication will happen through it instead of the main channel" << endl;
        }

        // comment this out if you want overall runtime
        // start timer
//        cout << "Starting timer for process" << endl;
//        struct timeval start, end;
//        gettimeofday(&start, NULL);

        if (!isfiletransfer) {   // requesting data msgs
            if (t >= 0) {    // 1 data point
                datamsg d(p, t, ecg);
                chan->cwrite(&d, sizeof(d));
                double ecgvalue;
                chan->cread(&ecgvalue, sizeof(double));
                cout << "Ecg " << ecg << " value for patient " << p << " at time " << t << " is: " << ecgvalue << endl;
            } else {          // bulk (i.e., 1K) data requests
                double ts = 0;
                datamsg d(p, ts, ecg);
                double ecgvalue;
                for (int i = 0; i < 1000; i++) {
                    chan->cwrite(&d, sizeof(d));
                    chan->cread(&ecgvalue, sizeof(double));
                    d.seconds += 0.004; //increment the timestamp by 4ms
                    cout << ecgvalue << endl;
                }
            }
        } else if (isfiletransfer) {
            // part 2 requesting a file
            filemsg f(0, 0);  // special first message to get file size
            int to_alloc = sizeof(filemsg) + filename.size() + 1; // extra byte for NULL
            char *buf = new char[to_alloc];
            memcpy(buf, &f, sizeof(filemsg));
            strcpy(buf + sizeof(filemsg), filename.c_str());
            chan->cwrite(buf, to_alloc);
            __int64_t filesize;
            chan->cread(&filesize, sizeof(__int64_t));

            // multi-channel file transfer
            if (nchannels > 1) {
                // calculate each channels contribution
                bytesPerChannel = filesize / nchannels;
                // if file size is not evenly divisible by the number of new channels
                // then find remainder
                if (filesize % nchannels != 0)
                    remainder = filesize % nchannels;
                // have the last channel make up for the remainder that can't be evenly divided
                if (i == nchannels - 1)
                    bytesPerChannel += remainder;
            }
            // print out file size, planned bytes per channel & open recv file
            if (i == 0) {
                cout << "File size: " << filesize << endl;
                cout << "Bytes per channel: " << bytesPerChannel << endl;
            }

            //int transfers = ceil (1.0 * filesize / MAX_MESSAGE);
            // updated PA1 solution for file transfer to multi-channel transfer
            filemsg *fm = (filemsg *) buf;
            __int64_t rem = filesize;
            string outfilepath = string("received/") + filename;
            FILE *outfile = fopen(outfilepath.c_str(), "wb");
            fm->offset = 0;

            char *recv_buffer = new char[MAX_MESSAGE];
            while (rem > 0) {
                // introduce byte counter
                int copiedBytes = 0;

                fm->length = (int) min(rem, (__int64_t) MAX_MESSAGE);
                chan->cwrite(buf, to_alloc);
                chan->cread(recv_buffer, MAX_MESSAGE);
                fwrite(recv_buffer, 1, fm->length, outfile);
                rem -= fm->length;
                fm->offset += fm->length;
                // add to byte counter
                copiedBytes += fm->length;
                // if channel has finished its portion of file copy, break
                if (copiedBytes == bytesPerChannel)
                    break;
                //cout << fm->offset << endl;
            }

            fclose(outfile);
            delete recv_buffer;
            delete buf;
            cout << "File transfer completed" << endl;
        }

        // comment this out if you want overall runtime
        // stop timer
//        cout << "Stopping timer for process" << endl;
//        gettimeofday(&end, NULL);

        // calculate and display time taken
//        double runtime;
//        runtime = (end.tv_sec - start.tv_sec) * 1e6;
//        runtime = (runtime + (end.tv_usec - start.tv_usec)) * 1e-6;
//        cout << "Runtime: " << fixed << runtime << setprecision(6) << " seconds" << endl;


        MESSAGE_TYPE q = QUIT_MSG;
        chan->cwrite(&q, sizeof(MESSAGE_TYPE));

        // delete new channels or control channel if no new channels
        delete chan;
    }

    // comment this out if you want individual channel runtime
    // stop timer
    cout << "Stopping timer for process" << endl;
    gettimeofday(&end, NULL);

    // calculate and display time taken
    double runtime;
    runtime = (end.tv_sec - start.tv_sec) * 1e6;
    runtime = (runtime + (end.tv_usec - start.tv_usec)) * 1e-6;
    cout << "Runtime: " << fixed << runtime << setprecision(6) << " seconds" << endl;

    // delete control channel if new channels were created
    MESSAGE_TYPE q = QUIT_MSG;
    if (chan != control_chan){ // this means that the user requested a new channel, so the control_channel must be destroyed as well
        control_chan->cwrite (&q, sizeof (MESSAGE_TYPE));
        delete control_chan;
    }

    // wait for the child process running server
    // this will allow the server to properly do clean up
    // if wait is not used, the server may sometimes crash
    wait (0);

}