#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <algorithm> 

#include "SmartDevices.hpp"
#include "SmartDevices.cpp"
#include <thread>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>

#define MYPORT  7000

#define BUFFER_SIZE 1024
int sock_cli;
int timecounter;
int iSecret;
char ibuff[30];

void RandomID(){
    srand (time(NULL));
    iSecret = rand() % 10;
    sprintf(ibuff, "UID: %d----Bulb\n",iSecret);
    std::cout<<ibuff;
    send(sock_cli, ibuff, strlen(ibuff),0);
    sleep(1);
    
}
//send mess when connect,excute once
void Initialsender(){
    string line;
    //    string ca="<--System Security status";
    ifstream myfile;
    myfile.open("SmartBulb.txt", ios::out | ios::in );
    //
    getline (myfile,line);
    line.append("<--SmartBulb status\n");
    
    //Timstamps
    timecounter++;
    ofstream out;
    out.open ("BulbTimestamps.txt");
    out << timecounter;
    out.close();
    //read
    string li;
    ifstream in ("BulbTimestamps.txt");
    getline(in,li);
    li.append("\n");
    line.append(li);
    cout<<line;
    
    
    char* chr = const_cast<char*>(line.c_str());
    //        strcat(chr, "<--System Security status");
    send(sock_cli, chr, strlen(chr),0);
    
    
}


void Time(int i){
    time_t rawtime;
    struct tm *ptminfo;
    
    time(&rawtime);
    ptminfo = localtime(&rawtime);
    if(i==1){
        char buf[100];
        
        sprintf(buf,"Current Time(Bulb) synchronized: %02d-%02d-%02d %02d:%02d:%02d",
                ptminfo->tm_year + 1900, ptminfo->tm_mon + 1, ptminfo->tm_mday,
                ptminfo->tm_hour, ptminfo->tm_min, ptminfo->tm_sec);
        
        //new2
        strcat(buf,"-");
        strcat(buf, "Timestamps(received)->");
        //open file
        timecounter++;
        ofstream myfile;
        myfile.open ("BulbTimestamps.txt");
        myfile << timecounter;
        myfile.close();
        
        string line;
        ifstream in ("BulbTimestamps.txt");
        getline(in,line);
        
        strcat(buf,line.c_str());
        cout<<"Send:"<<buf<<endl;
        write(sock_cli, buf, sizeof(buf));
        
    }else if (i==0){
        //send time with clock-offset variable +1 minute
        char buf[100];
        sprintf(buf,"Current Time(Bulb): %02d-%02d-%02d %02d:%02d:%02d",
                ptminfo->tm_year + 1900, ptminfo->tm_mon + 1, ptminfo->tm_mday,
                ptminfo->tm_hour, ptminfo->tm_min+1, ptminfo->tm_sec);
        //open file
        strcat(buf,"-");
        strcat(buf, "Timestamps(received)->");
        timecounter++;
        ofstream myfile;
        myfile.open ("LamportTimestamps.txt");
        myfile << timecounter;
        myfile.close();
        
        string line;
        ifstream in ("LamportTimestamps.txt");
        getline(in,line);
        
        strcat(buf,line.c_str());
        
        cout<<"Send:"<<buf<<endl;
        
        write(sock_cli, buf, sizeof(buf));
        
        
        //        printf("Current Time: %02d-%02d-%02d %02d:%02d:%02d\n",
        //               ptminfo->tm_year + 1900, ptminfo->tm_mon + 1, ptminfo->tm_mday,
        //               ptminfo->tm_hour, ptminfo->tm_min+1, ptminfo->tm_sec);
    }
}

void TimeCounterFile(){
    ifstream in("BulbTimestamps.txt");
    if (! in.is_open())
    {
        ofstream out("BulbTimestamps.txt");
        out <<"0";
        timecounter=0;
        out.close();
    };
}





int main()
{
    
    fd_set rfds;
    struct timeval tv;
    int retval, maxfd;
    //connect to ClientController
    ///define sockfd
    sock_cli = socket(AF_INET,SOCK_STREAM, 0);
    ///define sockaddr_in
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(MYPORT);  ///server port
    
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");  ///server ip
    
    //connect,sucess return 0，failed return -1
    if (connect(sock_cli, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("connect");
        exit(1);
    }
    
     RandomID();
    //thread
    thread s(Initialsender);
    s.detach();
    TimeCounterFile();
    
    Time(0);

    // loop
    while(1){
        
        //Sensor device
        Bulb1 b1;
        Bulb1 * b=&b1;
        
        
      cout<<"Smart Devices:"<<endl;
        b->registerInfo();
        
        
      
        FD_ZERO(&rfds);
        
        FD_SET(0, &rfds);
        maxfd = 0;
        
        FD_SET(sock_cli, &rfds);
        
        if(maxfd < sock_cli)
            maxfd = sock_cli;
        
        //receive mess
        
        if(FD_ISSET(sock_cli,&rfds)){
            char recvbuf[BUFFER_SIZE];
            int len;
            
            
            
            len = recv(sock_cli, recvbuf, sizeof(recvbuf),0);
                            printf("%s", recvbuf);
            //leader
            
            int iresult[5];
            const char* server[5]={"ClientController","MotionSensor","TemperSensor","Outlet","DoorSensor"};
            
            for(int i=0;i<=4;i++){
                
                iresult[i]=strncmp(recvbuf, server[i], 5);
                if(iresult[i]==0){
                    cout<<"Leader:"<<server[i];
                    //get max timestaps
                    string find(recvbuf);
                    int leng;
                    leng=strlen(recvbuf);
                    if(leng>22){
                        size_t pos = find.find("Timestamps");
                        string str3 = find.substr (pos);
                        string str4 = str3.substr (17);
                        //                    cout<<"str4:"<<str4;
                        //get Max timestaps
                        string::size_type sz;
                        int ReceiveT=stoi (str4,&sz);
                        //               cout<<"timecounter:"<<timecounter;
                        timecounter=max(ReceiveT,timecounter);
                        //                    //input to file
                        ofstream myfile1;
                        myfile1.open ("BulbTimestamps.txt");
                        myfile1 << timecounter;
                        myfile1.close();
                    }
                    Time(1);
                    break;
                }
                
            }

            
            
            //new get message and recognize
            int on;
            int off;
            int away;
            int home;
            int syn;
            //            string ID(recvbuf);
            char ID[]="3 on";
            char ID2[]="3 off";
            char ID3[]="away";
            char ID6[]="home";
            char ID5[]="syn";
            //on function
            on=strncmp(recvbuf, ID, 4);
            off=strncmp(recvbuf, ID2, 4);
            away=strncmp(recvbuf, ID3, 4);
            home=strncmp(recvbuf, ID6, 4);
            syn=strncmp(recvbuf, ID5, 3);
            if (on==0){
                sleep(1);
                b->change_state("on");
//                char str[]="No.3 Smart Bulb:on\n";
                //get max timestaps
                string find(recvbuf);
                int leng;
                leng=strlen(recvbuf);
                if(leng>22){
                    size_t pos = find.find("Timestamps");
                    string str3 = find.substr (pos);
                    string str4 = str3.substr (17);
                    //                    cout<<"str4:"<<str4;
                    //get Max timestaps
                    string::size_type sz;
                    int ReceiveT=stoi (str4,&sz);
                    //               cout<<"timecounter:"<<timecounter;
                    timecounter=max(ReceiveT,timecounter);
                    //                    //input to file
                    ofstream myfile1;
                    myfile1.open ("BulbTimestamps.txt");
                    myfile1 << timecounter;
                    myfile1.close();
                    //                    cout<<"timecounter2:"<<timecounter;
                }
                
                
                
                char str1[80];
                //open file
                strcpy(str1, "No.3 Smart Bulb:on----Timestamps(received)->");
                timecounter++;
                ofstream myfile;
                myfile.open ("BulbTimestamps.txt");
                myfile << timecounter;
                myfile.close();
                
                string line;
                ifstream in ("BulbTimestamps.txt");
                getline(in,line);
                in.close();
                
                strcat(str1,line.c_str());
                
                write(sock_cli, str1, sizeof(str1));
            }
            //off function
            
             else if(off==0){
                sleep(1);
                b->change_state("off");
//                char str[]="No.3 Smart Bulb:off\n";
                 //get max timestaps
                 string find(recvbuf);
                 int leng;
                 leng=strlen(recvbuf);
                 if(leng>22){
                     size_t pos = find.find("Timestamps");
                     string str3 = find.substr (pos);
                     string str4 = str3.substr (17);
                     //                    cout<<"str4:"<<str4;
                     //get Max timestaps
                     string::size_type sz;
                     int ReceiveT=stoi (str4,&sz);
                     //               cout<<"timecounter:"<<timecounter;
                     timecounter=max(ReceiveT,timecounter);
                     //                    //input to file
                     ofstream myfile1;
                     myfile1.open ("BulbTimestamps.txt");
                     myfile1 << timecounter;
                     myfile1.close();
                     //                    cout<<"timecounter2:"<<timecounter;
                 }
                 
                 
                 
                 char str1[80];
                 //open file
                 strcpy(str1, "No.3 Smart Bulb:off----Timestamps(received)->");
                 timecounter++;
                 ofstream myfile;
                 myfile.open ("BulbTimestamps.txt");
                 myfile << timecounter;
                 myfile.close();
                 
                 string line;
                 ifstream in ("BulbTimestamps.txt");
                 getline(in,line);
                 in.close();
                 
                 strcat(str1,line.c_str());
                 
                 
                write(sock_cli, str1, sizeof(str1));
                
            }
            //away mod
//            away=strncmp(recvbuf, ID3, 4);
            else if(away==0){
                sleep(1);
                b->change_state("off");
//                char str[]="No.3 Smart Bulb:off\n";
                //get max timestaps
                string find(recvbuf);
                int leng;
                leng=strlen(recvbuf);
                if(leng>22){
                    size_t pos = find.find("Timestamps");
                    string str3 = find.substr (pos);
                    string str4 = str3.substr (17);
                    //                    cout<<"str4:"<<str4;
                    //get Max timestaps
                    string::size_type sz;
                    int ReceiveT=stoi (str4,&sz);
                    //               cout<<"timecounter:"<<timecounter;
                    timecounter=max(ReceiveT,timecounter);
                    //                    //input to file
                    ofstream myfile1;
                    myfile1.open ("BulbTimestamps.txt");
                    myfile1 << timecounter;
                    myfile1.close();
                    //                    cout<<"timecounter2:"<<timecounter;
                }
                
                
                
                char str1[80];
                //open file
                strcpy(str1, "No.3 Smart Bulb:off----Timestamps(received)->");
                timecounter++;
                ofstream myfile;
                myfile.open ("BulbTimestamps.txt");
                myfile << timecounter;
                myfile.close();
                
                string line;
                ifstream in ("BulbTimestamps.txt");
                getline(in,line);
                in.close();
                
                strcat(str1,line.c_str());
                
                
                write(sock_cli, str1, sizeof(str1));
            }
            
            //home mode
//            home==strncmp(recvbuf, ID6, 5);
//            cout<<"home="<<home;
            else if (home==0){
                sleep(1);
                b->change_state("on");
                //get max timestaps
                string find(recvbuf);
                int leng;
                leng=strlen(recvbuf);
                if(leng>22){
                    size_t pos = find.find("Timestamps");
                    string str3 = find.substr (pos);
                    string str4 = str3.substr (17);
                    //                    cout<<"str4:"<<str4;
                    //get Max timestaps
                    string::size_type sz;
                    int ReceiveT=stoi (str4,&sz);
                    //               cout<<"timecounter:"<<timecounter;
                    timecounter=max(ReceiveT,timecounter);
                    //                    //input to file
                    ofstream myfile1;
                    myfile1.open ("BulbTimestamps.txt");
                    myfile1 << timecounter;
                    myfile1.close();
                    //                    cout<<"timecounter2:"<<timecounter;
                }
                
                
                
                char str1[80];
                //open file
                strcpy(str1, "No.3 Smart Bulb:on----Timestamps(received)->");
                timecounter++;
                ofstream myfile;
                myfile.open ("BulbTimestamps.txt");
                myfile << timecounter;
                myfile.close();
                
                string line;
                ifstream in ("BulbTimestamps.txt");
                getline(in,line);
                in.close();
                
                strcat(str1,line.c_str());
//                cout<<"ID6:"<<ID6<<"recvbuf:"<<ecvbuf;
//                char str[]="No.3 Smart Bulb:on\n";
                write(sock_cli, str1, sizeof(str1));
            }
            //Synchornize time
//            syn==strncmp(recvbuf, ID5, 3);
            else if (syn==0){
                //get max timestaps
                string find(recvbuf);
                int leng;
                leng=strlen(recvbuf);
                if(leng>22){
                    size_t pos = find.find("Timestamps");
                    string str3 = find.substr (pos);
                    string str4 = str3.substr (17);
                    //                    cout<<"str4:"<<str4;
                    //get Max timestaps
                    string::size_type sz;
                    int ReceiveT=stoi (str4,&sz);
                    //               cout<<"timecounter:"<<timecounter;
                    timecounter=max(ReceiveT,timecounter);
                    //                    //input to file
                    ofstream myfile1;
                    myfile1.open ("BulbTimestamps.txt");
                    myfile1 << timecounter;
                    myfile1.close();
                    //                    cout<<"timecounter2:"<<timecounter;
                }
                
                Time(1);
            }
            
            
//            if(ID=="3 on"){
//                sleep(1);
//                b->change_state("on");
//                char str[]="No.3 Smart Bulb:on\n";
//                write(sock_cli, str, sizeof(str));
//                //                send(conn, buffer, len, 0);
//
//            }else if(ID=="3 off"){
//                  sleep(1);
//                b->change_state("off");
//                char str[]="No.3 Smart Bulb:off\n";
//                write(sock_cli, str, sizeof(str));
//                sched_yield();
//            }
//
//            else if (ID=="Away"){
//                sleep(2);
//                b->change_state("off");
//                char str[]="No.3 Smart Bulb:off\n";
//                write(sock_cli, str, sizeof(str));
//            }else if(ID=="Home"){
//                sleep(2);
//                b->change_state("on");
//                char str[]="No.3 Smart Bulb:on\n";
//                write(sock_cli, str, sizeof(str));
//                //                send(conn, buffer, len, 0);
//
//            }else if (ID=="syn"){
//                Time(1);
//            }
            
            memset(recvbuf, 0, sizeof(recvbuf));
        }
        
    }
    
    close(sock_cli);
    return 0;
}
