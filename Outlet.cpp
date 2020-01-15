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
    sprintf(ibuff, "UID: %d----Outlet\n",iSecret);
    std::cout<<ibuff;
    send(sock_cli, ibuff, strlen(ibuff),0);
    sleep(1);
    
}
//send mess when connect,excute once
void Initialsender(){
    string line;
    //    string ca="<--System Security status";
    ifstream myfile;
    myfile.open("SmartO.txt", ios::out | ios::in );
    //
    getline (myfile,line);
    line.append("<--SmartOulet status\n");
    
    //Timstamps
    timecounter++;
    ofstream out;
    out.open ("OutletTimestamps.txt");
    out << timecounter;
    out.close();
    
    //read
    string li;
    ifstream in ("OutletTimestamps.txt");
    getline(in,li);
    li.append("\n");
    line.append(li);
    cout<<line;
    
    char* chr = const_cast<char*>(line.c_str());
    //        strcat(chr, "<--System Security status");
    send(sock_cli, chr, strlen(chr),0);
//    send(sock_cli, ibuff, strlen(ibuff),0);
    
}

//time synchronization

void Time(int i){
    time_t rawtime;
    struct tm *ptminfo;
    
    time(&rawtime);
    ptminfo = localtime(&rawtime);
    if(i==1){
        char buf[100];
        
        sprintf(buf,"Current Time(Outlet) synchronized: %02d-%02d-%02d %02d:%02d:%02d",
                ptminfo->tm_year + 1900, ptminfo->tm_mon + 1, ptminfo->tm_mday,
                ptminfo->tm_hour, ptminfo->tm_min, ptminfo->tm_sec);
        strcat(buf,"-");
        strcat(buf, "Timestamps(received)->");
        //open file
        timecounter++;
        ofstream myfile;
        myfile.open ("OutletTimestamps.txt");
        myfile << timecounter;
        myfile.close();
        
        string line;
        ifstream in ("OutletTimestamps.txt");
        getline(in,line);
        
        strcat(buf,line.c_str());
        cout<<buf<<endl;
        write(sock_cli, buf, sizeof(buf));
        
    }else if (i==0){
        //send time with clock-offset variable +1 minute
        char buf[100];
        sprintf(buf,"Current Time(Outlet): %02d-%02d-%02d %02d:%02d:%02d",
                ptminfo->tm_year + 1900, ptminfo->tm_mon + 1, ptminfo->tm_mday,
                ptminfo->tm_hour, ptminfo->tm_min+1, ptminfo->tm_sec);
        strcat(buf,"-");
        strcat(buf, "Timestamps(received)->");
        //open file
        timecounter++;
        ofstream myfile;
        myfile.open ("OutletTimestamps.txt");
        myfile << timecounter;
        myfile.close();
        
        string line;
        ifstream in ("OutletTimestamps.txt");
        getline(in,line);
        
        strcat(buf,line.c_str());
        cout<< buf<<endl;
        write(sock_cli, buf, sizeof(buf));
        
        
        //        printf("Current Time: %02d-%02d-%02d %02d:%02d:%02d\n",
        //               ptminfo->tm_year + 1900, ptminfo->tm_mon + 1, ptminfo->tm_mday,
        //               ptminfo->tm_hour, ptminfo->tm_min+1, ptminfo->tm_sec);
    }
}

//create file to record Lamport timestamps
void TimeCounterFile(){
    ifstream in("OutletTimestamps.txt");
    if (! in.is_open())
    {
        ofstream out("OutletTimestamps.txt");
        out <<"0";
        timecounter=0;
        out.close();
    };
}

int main()
{
//    int sock_cli;
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
    servaddr.sin_port = htons(MYPORT);
    
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    //connect,sucess return 0，failed return -1
    
    if (connect(sock_cli, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("connect");
        exit(1);
    }
    //thread
     RandomID();
    thread s(Initialsender);
    s.detach();
    Time(0);
    TimeCounterFile();
    //loop
    while(1){
        
        //Sensor device
        Outlet1 o1;
        Outlet1 *o=&o1;
        
        
        cout<<"Smart Devices:"<<endl;
        o->registerInfo();
        
        
       
        FD_ZERO(&rfds);
  
        FD_SET(0, &rfds);
        maxfd = 0;
   
        FD_SET(sock_cli, &rfds);
       
        if(maxfd < sock_cli)
            maxfd = sock_cli;
        
    
        if(FD_ISSET(sock_cli,&rfds)){
            char recvbuf[BUFFER_SIZE];
            int len;
            
            
            
            len = recv(sock_cli, recvbuf, sizeof(recvbuf),0);
                            printf("%s", recvbuf);
            //leader
            
            int iresult[5];
            const char* server[5]={"ClientController","MotionSensor","TemperSensor","DoorSensor","Bulb"};
            
            for(int i=0;i<=4;i++){
                
                iresult[i]=strncmp(recvbuf, server[i], 5);
                if(iresult[i]==0){
//                    cout<<"Leader:"<<server[i];
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
                        myfile1.open ("OutletTimestamps.txt");
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
            char ID[]="4 on";
            char ID2[]="4 off";
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
                o->change_state("on");
//                char str[]="No.4 Smart Outlet:on\n";
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
                    ofstream myfile2;
                    myfile2.open ("OutletTimestamps.txt");
                    myfile2 << timecounter;
                    myfile2.close();
                }
                
                char str2[80];
                //                int retVal=sprintf(str,"No.5 Door Sensor:on TimeCounter is %d",TimeCounter);
                //open file
                strcpy(str2, "No.4 Smart Outlet:on----Timestamps(received)->");
                timecounter++;
                ofstream out1;
                out1.open ("OutletTimestamps.txt");
                out1 << timecounter;
                out1.close();
                
                string line;
                ifstream in ("OutletTimestamps.txt");
                getline(in,line);
                in.close();
                
                strcat(str2,line.c_str());
//                cout<<"str:"<<str2;
                write(sock_cli, str2, sizeof(str2));
            }
            //off function
            
            else if(off==0){
                sleep(1);
                o->change_state("off");
//                char str[]="No.4 Smart Outlet:off\n";
                
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
                    myfile1.open ("OutletTimestamps.txt");
                    myfile1 << timecounter;
                    myfile1.close();
                    //                    cout<<"timecounter2:"<<timecounter;
                }
                
                
                
                char str1[80];
                //open file
                strcpy(str1, "No.4 Smart Outlet:off----Timestamps(received)->");
                timecounter++;
                ofstream myfile;
                myfile.open ("OutletTimestamps.txt");
                myfile << timecounter;
                myfile.close();
                
                string line;
                ifstream in ("OutletTimestamps.txt");
                getline(in,line);
                in.close();
                
                strcat(str1,line.c_str());
                write(sock_cli, str1, sizeof(str1));
                sleep(1);
                
                
            }
            //away mod
            //            away=strncmp(recvbuf, ID3, 4);
            else if(away==0){
                sleep(2);
                o->change_state("off");
//                char str[]="No.4 Smart Outlet:off\n";
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
                    myfile1.open ("OutletTimestamps.txt");
                    myfile1 << timecounter;
                    myfile1.close();
                    //                    cout<<"timecounter2:"<<timecounter;
                }
                
                
                
                char str1[80];
                //open file
                strcpy(str1, "No.4 Smart Outlet:off----Timestamps(received)->");
                timecounter++;
                ofstream myfile;
                myfile.open ("OutletTimestamps.txt");
                myfile << timecounter;
                myfile.close();
                
                string line;
                ifstream in ("OutletTimestamps.txt");
                getline(in,line);
                in.close();
                
                strcat(str1,line.c_str());
                
                write(sock_cli, str1, sizeof(str1));
            }
            
            //home mode
            //            home==strncmp(recvbuf, ID6, 5);
            //            cout<<"home="<<home;
            else if (home==0){
                sleep(2);
                o->change_state("on");
//                char str[]="No.4 Smart Outlet:on\n";
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
                    myfile1.open ("OutletTimestamps.txt");
                    myfile1 << timecounter;
                    myfile1.close();
                    //                    cout<<"timecounter2:"<<timecounter;
                }
                
                
                
                char str1[80];
                //open file
                strcpy(str1, "No.4 Smart Outlet:on----Timestamps(received)->");
                timecounter++;
                ofstream myfile;
                myfile.open ("OutletTimestamps.txt");
                myfile << timecounter;
                myfile.close();
                
                string line;
                ifstream in ("OutletTimestamps.txt");
                getline(in,line);
                in.close();
                
                strcat(str1,line.c_str());
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
                    myfile1.open ("OutletTimestamps.txt");
                    myfile1 << timecounter;
                    myfile1.close();
                }
                Time(1);
            }
            
            
            
//            string ID(recvbuf);
//            if(ID=="4 on"){
//                  sleep(1);
//                o->change_state("on");
//                char str[]="No.4 Smart Outlet:on\n";
//                write(sock_cli, str, sizeof(str));
//                //                send(conn, buffer, len, 0);
//                
//            }else if(ID=="4 off"){
//                  sleep(1);
//                o->change_state("off");
//                char str[]="No.4 Smart Outlet:off\n";
//                write(sock_cli, str, sizeof(str));
//                sleep(1);
//                sched_yield();
//                
//                
//            }else if (ID=="Away"){
//                sleep(2);
//                o->change_state("off");
//                char str[]="No.4 Smart Outlet:off\n";
//                write(sock_cli, str, sizeof(str));
//                
//            }else if(ID=="Home"){
//                sleep(2);
//                o->change_state("on");
//                char str[]="No.4 Smart Outlet:on\n";
//                write(sock_cli, str, sizeof(str));
//            }else if (ID=="syn"){
//                Time(1);
//            }
            
            memset(recvbuf, 0, sizeof(recvbuf));
        
        }
    }
    
    close(sock_cli);
    return 0;

}
