/*
    Tanzir Ahmed
    Department of Computer Science & Engineering
    Texas A&M University
    Date  : 2/8/20
 */
#include "common.h"
#include "FIFOreqchannel.h"

using namespace std;


int main(int argc, char *argv[]) {
    // Code for running server and client from one terminal
    int pid = fork();
    if(pid == 0) {
        //char* args[] = *argv;
        execvp("./server", argv);
    }
    else {
        FIFORequestChannel chan("control", FIFORequestChannel::CLIENT_SIDE);

        // Variable setup
        int opt;
        int patient;
        bool newchannel = false;
        string filename;
        double time;
        bool hasTime = false; // used for single or multiple data point request
        int ecgno;
        int buffercapacity = MAX_MESSAGE;

        // Handle command line args
        while ((opt = getopt(argc, argv, "p:cf:t:e:m:")) != -1) { // -p 10, -c, -f "10.csv"
            switch (opt) {
                case 'p':
                    patient = atoi(optarg);
                    break;
                case 'c':
                    newchannel = true;
                    break;
                case 'f':
                    filename = optarg;
                    break;
                case 't':
                    time = atof(optarg);
                    hasTime = true;
                    break;
                case 'e':
                    ecgno = atoi(optarg);
                    break;
                case 'm':
                    buffercapacity = atoi(optarg);
                    break;
                case '?':
                    printf("ERROR: unknown flag %c\n",optopt);
                    break;
            }
        }

        // Requesting Data Points
        if (patient > 0 and patient < 16) {
            // single data point request
            if (hasTime) {
                datamsg *d = new datamsg(patient, time, ecgno);
                chan.cwrite(d, sizeof(datamsg));
                // receiving the response for the data msg
                double *result = new double;
                chan.cread(result, sizeof(double));
                cout << "Result is: " << *result << endl;

                // deallocation
                delete d;
                delete result;
            }
            // first 1K data points with specified ecg no or alternate implementations
            else {
                // start timer for data request
                struct timeval start, end;
                gettimeofday(&start, NULL);

                // place requested data points into received directory
                ofstream myfile;
                myfile.open("received/x1.csv");
                // initialize time, counter and boundary vars
                double t = 0;
                int n = 0;
                int ENTRYCNT = 1000; // can be set to collect any number of data points

                cout << "Since no time specified, first 1K data points will be requested" << endl;
                while (n < ENTRYCNT) {
                    // write time to file
                    myfile << t << ",";
                    datamsg d(patient, t, ecgno);
                    chan.cwrite(&d, sizeof(datamsg));
                    double result;
                    chan.cread(&result, sizeof(double));
                    // write result
                    myfile << result << "\n";
                    // increment counter and time vars
                    n += 1;
                    t += 0.004;
                }

                // alternate for requesting all data points
                /*while(t < 59.996) {
                    // write time to file
                    myfile << t << ",";
                    datamsg d(patient, t, ecgno);
                    chan.cwrite(&d, sizeof(datamsg));
                    double result;
                    chan.cread(&result, sizeof(double));
                    // write result
                    myfile << result << "\n";
                    // increment time var
                    t += 0.004;
                }*/

                // alternate for requesting both ecg columns and all data points
                /*while(t < 59.996) {
                    // write time to file
                    myfile << t << ",";
                    // initialized datamsg for both ecg columns
                    datamsg d1(patient, t, 1);
                    datamsg d2(patient, t, 2);

                    // get data points for ecg 1
                    chan.cwrite(&d1, sizeof(datamsg));
                    double result1;
                    chan.cread(&result1, sizeof(double));
                    // write result
                    myfile << result1 << ",";

                    // get data points for ecg 2
                    chan.cwrite(&d2, sizeof(datamsg));
                    double result2;
                    chan.cread(&result2, sizeof(double));
                    // write result
                    myfile << result2 << "\n";
                    // increment time var
                    t += 0.004;
                }*/

                // close file and report to user
                myfile.close();
                cout << "Successful transfer of data points" << endl;

                // stop timer for data request
                gettimeofday(&end, NULL);

                // calculate and display time taken
                long seconds = (end.tv_sec - start.tv_sec);
                long microsec = ((seconds * 1000000) + end.tv_usec) - (start.tv_usec);
                cout << "Runtime: " << seconds << " seconds and " << microsec << " microseconds" << endl;
            }
        }
        // Requesting Files
        if (!filename.empty()) {
            // Get length of file
            filemsg f(0, 0);
            char buf[sizeof(filemsg) + filename.size() + 1];
            memcpy(buf, &f, sizeof(filemsg));
            strcpy(buf + sizeof(filemsg), filename.c_str());
            chan.cwrite(buf, sizeof(buf));

            __int64_t filelen;
            chan.cread(&filelen, sizeof(__int64_t));
            cout << "File length: " << filelen << endl;

            // Calculate amount of requests based on max buffer capacity
            //int requestno = ceil(filelen / buffercapacity);

            // start timer for file request
            struct timeval start, end;
            gettimeofday(&start, NULL);

            // open new file in received directory to write contents of user specified file to
            ofstream myfile;
            myfile.open("received/" + filename);

            // Process requests to complete filemsg process
            int offset = 0;
            int length = buffercapacity;
            while (offset < filelen) {
                // catch lengths longer than remainder of file
                if (filelen - offset < buffercapacity) {
                    length = filelen - offset;
                    //cout << length << endl;
                }
                filemsg f(offset, length);
                char buf[sizeof(filemsg) + filename.size() + 1];
                memcpy(buf, &f, sizeof(filemsg));
                strcpy(buf + sizeof(filemsg), filename.c_str());
                chan.cwrite(buf, sizeof(buf));

                char* recvbuf = new char[length];
                chan.cread(recvbuf, length);
                myfile.write(recvbuf, length);

                // increment offset
                offset += buffercapacity;

                delete recvbuf;
            }

            // close file and report to user
            myfile.close();
            cout << "Successful filemsg process" << endl;

            // stop timer for data request
            gettimeofday(&end, NULL);

            // calculate and display time taken
            long seconds = (end.tv_sec - start.tv_sec);
            long microsec = ((seconds * 1000000) + end.tv_usec) - (start.tv_usec);
            cout << "Runtime: " << seconds << " seconds and " << microsec << " microseconds" << endl;
        }
        // Requesting a New Channel
        if (newchannel) {
            MESSAGE_TYPE n = NEWCHANNEL_MSG;
            chan.cwrite(&n, sizeof(MESSAGE_TYPE));
            char chanName[100];
            chan.cread(chanName, sizeof(chanName));
            FIFORequestChannel chan2(chanName, FIFORequestChannel::CLIENT_SIDE);

            // Test new channel
            cout << "Testing Data Point Request on New Channel" << endl;
            cout << "Patient 15 at 0.008 sec for ecg 1" << endl;
            datamsg *d = new datamsg(15, 0.008, 1);
            chan2.cwrite(d, sizeof(datamsg));
            // receiving the response for the data msg
            double *result = new double;
            chan2.cread(result, sizeof(double));
            cout << "Result is: " << *result << endl;

            cout << "Testing Data Point Request on New Channel" << endl;
            cout << "Patient 10 at 0.004 sec for ecg 2" << endl;
            datamsg *d2 = new datamsg(10, 0.004, 2);
            chan2.cwrite(d, sizeof(datamsg));
            // receiving the response for the data msg
            double *result2 = new double;
            chan2.cread(result2, sizeof(double));
            cout << "Result is: " << *result2 << endl;

            // deallocation
            delete d;
            delete result;
            delete d2;
            delete result2;

            // Closing new channel
            cout << "Sending quit msg to new channel" << endl;
            MESSAGE_TYPE m = QUIT_MSG;
            chan2.cwrite(&m, sizeof(MESSAGE_TYPE));
        }
        // Closing Channels
        MESSAGE_TYPE m = QUIT_MSG;
        chan.cwrite(&m, sizeof(MESSAGE_TYPE));

        sleep(3);
    }
}
