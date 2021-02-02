#include <iostream>
#include <thread>
#include "common.h"
#include "BoundedBuffer.h"
#include "Histogram.h"
#include "common.h"
#include "HistogramCollection.h"
#include "FIFOreqchannel.h"
#include <sys/epoll.h>
#include <unordered_map>
#include <fcntl.h>
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

void event_polling_thread (int w, int mb, FIFORequestChannel** wchans, BoundedBuffer* request_buffer, HistogramCollection* hc){
    char buf[1024];
    double resp = 0;

    char recvbuf [mb];

    struct epoll_event ev;
    struct epoll_event events[w];

    // create an empty epoll list
    int epollfd = epoll_create1 (0);
    if (epollfd == -1) {
        EXITONERROR ("epoll_create1");
    }

    unordered_map<int, int> fd_to_index;
    vector<vector<char>> state (w);
    // priming phase + adding each rfd to the list
    int nsent = 0, nrecv = 0;
    for (int i = 0; i < w; i++){
        int sz = request_buffer->pop(buf, 1024);
        wchans[i]->cwrite (buf, sz);
        state [i] = vector<char>(buf, buf+sz); // record the state[i]
        nsent++;
        int rfd = wchans[i]->getrfd();
        fcntl(rfd, F_SETFL, O_NONBLOCK);

        ev.events = EPOLLIN | EPOLLET;
        ev.data.fd = rfd;
        fd_to_index[rfd] = i;
        if (epoll_ctl(epollfd, EPOLL_CTL_ADD, rfd, &ev) == -1){
            EXITONERROR ("epoll_ctl: listen_sock");
        }
    }

    //nsent = w, nrecv = 0
    bool quit_recv = false;

    while(true) {
        if (quit_recv && nrecv == nsent) {
            break;
        }
        int nfds = epoll_wait(epollfd, events, w, -1);
        if (nfds == -1) {
            EXITONERROR("epoll_wait");
        }
        for (int i = 0; i < nfds; i++) {
            int rfd = events[i].data.fd;
            int index = fd_to_index [rfd];

            int resp_sz = wchans[index]->cread(recvbuf, mb);
            nrecv++;

            // process (recvbuf)
            vector<char> req = state[index];
            char* request = req.data();

            MESSAGE_TYPE* m = (MESSAGE_TYPE *) request;
            if (*m == DATA_MSG) {
                hc->update (((datamsg *)request)->person, *(double*)recvbuf);
            } else if (*m == FILE_MSG) {
                filemsg* fm = (filemsg*) request;
                string fname = (char*)(fm + 1);
                int sz = sizeof (filemsg) + fname.size() + 1;
                // chan->cwrite(buf, sz);
                // chan->cread(recvbuf, mb);

                string recvfname = "recv/" + fname;
                FILE* fp = fopen(recvfname.c_str(), "r+");
                fseek (fp, fm->offset, SEEK_SET);
                fwrite(recvbuf, 1, fm->length, fp);
                fclose(fp);
            }

            // reuse
            if (quit_recv == false) {
                int req_sz = request_buffer->pop(buf, sizeof (buf));
                if (*(MESSAGE_TYPE*) buf == QUIT_MSG) {
                    quit_recv = true;
                }
                else {
                    wchans[index]->cwrite(buf, req_sz);
                    state[index] = vector<char> (buf, buf+req_sz);
                    nsent++;
                }
            }
        }
    }
}



int main(int argc, char *argv[])
{
    // hard coded values for running just /client
    int n = 15000;    //default number of requests per "patient"
    int p = 1;     // number of patients [1,15]
    int w = 200;    //default request channels
    int b = 500; 	// default capacity of the request buffer, you should change this default
	int m = MAX_MESSAGE; 	// default capacity of the message buffer
	string fname; // can be hard coded but is left empty for data msg default
    bool isFileTransfer = false; // boolean for determining request type (data/file)
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
            case 'w': // number of request channels
                w = atoi (optarg);
                break;
            case 'f': // name of file
                isFileTransfer = true;
                fname = optarg;
                break;
        }
    }

    // fix blocked for loop in event poll thread
    if (w > n*p) {
        w = n * p;
        cout << "w adjusted to " << w << endl;
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
	FIFORequestChannel** wchans = new FIFORequestChannel*[w];
	for (int i = 0;i < w; i++) {
	    wchans[i] = create_new_channel(chan);
	}

	// start timer
    struct timeval start, end;
    gettimeofday (&start, 0);

    if (isFileTransfer) { // is a file request
        /* Start all threads here */
        // create 1 file thread
        thread filethread (file_thread_function, fname, &request_buffer, chan, m);

        // thread evp (event_polling_thread, n, p, w, m, wchans, &request_buffer, &hc);
        thread evp (event_polling_thread, w, m, wchans, &request_buffer, &hc);

        /* Join all threads here */
        // clean up file thread for file request
        filethread.join();
        
        cout << "Patient threads/file thread finished" << endl;

        // push a single quit message
        MESSAGE_TYPE q = QUIT_MSG;
        request_buffer.push((char *) &q, sizeof(q));

        // join epoll thread
        evp.join();

        cout << "Worker threads finished" << endl;

        // clean up wchans
        for (int i = 0; i < w; ++i) {
            // delete fiforequestchannel from server side
            wchans[i]->cwrite(&q, sizeof(MESSAGE_TYPE));

            // delete fiforequestchannel from client side
            delete wchans[i];
        }
        delete[] wchans;
    }
    else { // is a data request
        thread patient [p];
        // create p patient threads
        for (int i = 0;i < p; i++) {
            patient[i] = thread (patient_thread_function, n, i+1, &request_buffer);
        }

        // thread evp (event_polling_thread, n, p, w, m, wchans, &request_buffer, &hc);
        thread evp (event_polling_thread, w, m, wchans, &request_buffer, &hc);

        // /* Join all threads here */
        // clean up patient threads for data request
        for (int i = 0;i < p; i++) {
            patient[i].join();
        }

        cout << "Patient threads/file thread finished" << endl;

        // push a single quit message
        MESSAGE_TYPE q = QUIT_MSG;
        request_buffer.push((char *) &q, sizeof(q));

        // join epoll thread
        evp.join();

        cout << "Worker threads finished" << endl;

        // clean up wchans
        for (int i = 0; i < w; ++i) {
            // delete fiforequestchannel from server side
            wchans[i]->cwrite(&q, sizeof(MESSAGE_TYPE));

            // delete fiforequestchannel from client side
            delete wchans[i];
        }
        delete[] wchans;

        // print the results
	    hc.print ();
    }

    // stop timer
    gettimeofday (&end, 0);

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