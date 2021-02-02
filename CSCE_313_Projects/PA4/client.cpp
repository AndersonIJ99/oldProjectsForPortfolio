#include <thread>
#include "common.h"
#include "BoundedBuffer.h"
#include "Histogram.h"
#include "common.h"
#include "HistogramCollection.h"
#include "FIFOreqchannel.h"
using namespace std;

// function for creating new FIFO channels
FIFORequestChannel* create_new_channel (FIFORequestChannel* mainchan) {
    char name [1024];
    MESSAGE_TYPE m = NEWCHANNEL_MSG;
    mainchan->cwrite(&m, sizeof (m));
    mainchan->cread(name, 1024);
    FIFORequestChannel* newchan = new FIFORequestChannel (name, FIFORequestChannel::CLIENT_SIDE);
    return newchan;
}

// function for creating patient threads for data requests
void patient_thread_function (int n, int pno, BoundedBuffer* request_buffer){
    datamsg d (pno, 0.0, 1);
    double resp = 0;
    for (int i = 0; i < n; i++) {
        request_buffer->push((char *) &d, sizeof(datamsg));
        d.seconds += 0.004;
    }
}

// function for creating the file thread for file requests
void file_thread_function (string fname, BoundedBuffer* request_buffer, FIFORequestChannel* chan, int mb) {
    //1. create the file
    string recvfname = "recv/" + fname;
    // make it as long as the original length
    char buf [1024];
    filemsg f (0,0);
    memcpy(buf, &f, sizeof(f));
    strcpy(buf + sizeof(f), fname.c_str());
    chan->cwrite(buf, sizeof(f) + fname.size() + 1);
    __int64_t filelength;
    chan->cread(&filelength, sizeof(filelength));
    FILE* fp = fopen(recvfname.c_str(), "w");
    fseek(fp, filelength, SEEK_SET);
    fclose(fp);

    //2. generate all the file messages
    filemsg* fm = (filemsg *) buf;
    __int64_t remlen = filelength;

    while (remlen > 0) {
        fm->length = min (remlen, (__int64_t) mb);
        request_buffer->push (buf, sizeof (filemsg) + fname.size() + 1);
        fm->offset += fm->length;
        remlen -= fm->length;
    }
}

// function for creating worker threads for both data and file requests
void worker_thread_function (FIFORequestChannel* chan, BoundedBuffer* request_buffer, HistogramCollection* hc, int mb){
    char buf [1024];
    double resp = 0;

    char recvbuf [mb];
    while (true) {
        request_buffer->pop(buf, 1024);
        MESSAGE_TYPE* m = (MESSAGE_TYPE *) buf;

        // handle data messages
        if (*m == DATA_MSG) {
            chan->cwrite(buf, sizeof(datamsg));
            chan->cread(&resp, sizeof(double));
            hc->update(((datamsg *) buf)->person, resp);
        }
        // quit channel and break out
        else if (*m == QUIT_MSG) {
            chan->cwrite(m, sizeof(MESSAGE_TYPE));
            delete chan;
            break;
        }
        // handle file messages
        else if (*m == FILE_MSG) {
            filemsg* fm = (filemsg*) buf;
            string fname = (char *)(fm + 1);
            int sz = sizeof(filemsg) + fname.size() + 1;
            chan->cwrite(buf, sz);
            chan->cread(recvbuf, mb);

            // recv/ directory must be created beforehand
            string recvfname = "recv/" + fname;

            FILE* fp = fopen(recvfname.c_str(), "r+");
            fseek(fp, fm->offset, SEEK_SET);
            fwrite(recvbuf, 1, fm->length, fp);
            fclose(fp);
        }
    }
}



int main(int argc, char *argv[])
{
    // hard coded values for running just /client
    int n = 15000;    //default number of requests per "patient"
    int p = 1;     // number of patients [1,15]
    int w = 200;    //default number of worker threads
    int b = 500; 	// default capacity of the request buffer, you should change this default
	int m = MAX_MESSAGE; 	// default capacity of the message buffer
	string fname; // can be hard coded but is left empty for data msg default
    srand(time_t(NULL));

    int opt = -1;
    while ((opt = getopt(argc, argv, "m:n:b:w:p:f:")) != -1) {
        switch (opt) {
            case 'm': // capacity of file buffer
                m = atoi (optarg);
                break;
            case 'n': // numbers of requests per patient
                n = atoi (optarg);
                break;
            case 'p': // number of patients
                p = atoi (optarg);
                break;
            case 'b': // capacity of request buffer
                b = atoi (optarg);
                break;
            case 'w': // number of worker threads
                w = atoi (optarg);
                break;
            case 'f': // name of file
                fname = optarg;
                break;
        }
    }
    
    int pid = fork();
    if (pid == 0){
		// modify this to pass along m
        execl ("server", "server", "-m", (char *) to_string(m).c_str(), (char *)NULL);
    }
    
    // create control channel, request buffer, and histogram collection
	FIFORequestChannel* chan = new FIFORequestChannel("control", FIFORequestChannel::CLIENT_SIDE);
    BoundedBuffer request_buffer(b);
	HistogramCollection hc;

	// making histograms and adding to the histogram collection hc
	for (int i = 0; i < p; i++) {
	    Histogram* h = new Histogram (10, -2.0, 2.0);
	    hc.add(h);
	}

	// make w worker channels
	FIFORequestChannel* wchans [w];
	for (int i = 0;i < w; i++) {
	    wchans[i] = create_new_channel(chan);
	}

	// start timer
    struct timeval start, end;
    gettimeofday (&start, 0);

    /* Start all threads here */
    // initialize threads so that they are in scope
    thread patient [p];
    thread filethread;

    // Not a file request
    if (fname.empty()) {
        // create p patient threads
        for (int i = 0;i < p; i++) {
            patient[i] = thread (patient_thread_function, n, i+1, &request_buffer);
        }
    }
    // Is a file request
    else {
        // create 1 file thread
        filethread = thread (file_thread_function, fname, &request_buffer, chan, m);
    }

    // create worker threads
    thread workers [w];
    for (int i = 0; i < w; i ++) {
        workers[i] = thread (worker_thread_function, wchans[i], &request_buffer, &hc, m);
    }

	/* Join all threads here */
    // clean up patient threads for data request
    if (fname.empty()) {
        for (int i = 0;i < p; i++) {
            patient[i].join();
        }
    }
    // clean up file thread for file request
    else {
        filethread.join();
    }
    cout << "Patient threads/file thread finished" << endl;

    // send quit message to worker channels
    for (int i = 0;i < w; i++) {
        MESSAGE_TYPE q = QUIT_MSG;
        request_buffer.push((char *) &q, sizeof(q));
    }

    // clean up worker threads
    for (int i = 0;i < w; i++) {
        workers[i].join();
    }
    cout << "Worker threads finished" << endl;

    // stop timer
    gettimeofday (&end, 0);
    // print the results
	hc.print ();

    // display runtime
    int secs = (end.tv_sec * 1e6 + end.tv_usec - start.tv_sec * 1e6 - start.tv_usec)/(int) 1e6;
    int usecs = (int)(end.tv_sec * 1e6 + end.tv_usec - start.tv_sec * 1e6 - start.tv_usec)%((int) 1e6);
    cout << "Took " << secs << " seconds and " << usecs << " micro seconds" << endl;

    // cleaning the main channel
    MESSAGE_TYPE q = QUIT_MSG;
    chan->cwrite ((char *) &q, sizeof (MESSAGE_TYPE));
    cout << "All Done!!!" << endl;
    delete chan;
    
}
