//
//  DoorSensor.cpp
//  LoTSmartHome
//
//  Created by 张环宇 on 3/10/19.
//  Copyright © 2019 Huanyu Zhang. All rights reserved.
//

#include "DoorSensor.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <algorithm> 
#include <cstdio>
#include <thread>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sqlite3.h>
#include <iostream>
#include <sstream>
#include <time.h>
using namespace std;

#define MYPORT  7000

#define BUFFER_SIZE 1024
int sock_cli;
int timecounter;
int randomval;

//leader
int iSecret;
char ibuff[30];

void RandomID(){
    
//    while(true){
    srand (time(NULL));
    iSecret = rand() % 10;
    sprintf(ibuff, "UID: %d----DoorSensor\n",iSecret);
    std::cout<<ibuff;
    send(sock_cli, ibuff, strlen(ibuff),0);
    sleep(2);
//    }
    
}






//randomly generate open the door or close door signal
void GenerateSignal(){
    
    while(true){
        sleep(40);
        randomval = rand() % 2;
    }
    
}






//process signal
//get data and handle it
void ProcessSignal(){
    while(true){
        if(randomval==1){
            sleep(40);
            cout<<"Door Open<----detected from DoorSensor"<<endl;
            //            char str[]="Motion detected in the room\n";
            char str1[80];
            //open file add timestamps
            strcpy(str1, "Door Open<----detected from DoorSensor----Timestamps(received)->");
            timecounter++;
            ofstream myfile;
            myfile.open ("LamportTimestamps.txt");
            myfile << timecounter;
            myfile.close();
            
            string line;
            ifstream in ("LamportTimestamps.txt");
            getline(in,line);
            in.close();
            
            strcat(str1,line.c_str());
            strcat(str1,"\n");
            write(sock_cli, str1, sizeof(str1));
            
    
        }
        
    }
}




//send mess when connect,excute once
void Initialsender(){
    string line;
    //    string ca="<--System Security status";
    ifstream myfile;
    myfile.open("DoorSensor.txt", ios::out | ios::in );
    //
    getline (myfile,line);
    line.append("<--DoorSensor status----Timestamps(received)->");
    
    
    
    //Timstamps
    timecounter++;
    ofstream out;
    out.open ("LamportTimestamps.txt");
    out << timecounter;
    out.close();
    
    //read
    string li;
    ifstream in ("LamportTimestamps.txt");
    getline(in,li);
    li.append("\n");
    line.append(li);
    cout<<line;
    
    //string to char
    char* chr = const_cast<char*>(line.c_str());
    //        strcat(chr, "<--System Security status");
    send(sock_cli, chr, strlen(chr),0);
//    send(sock_cli, ibuff, strlen(ibuff),0);
    
    
    
    
}

//create file to record Lamport timestamps
void TimeCounterFile(){
    ifstream in("LamportTimestamps.txt");
    if (! in.is_open())
    {
        ofstream out("LamportTimestamps.txt");
        out <<"0";
        timecounter=0;
        out.close();
    };
}

//void rec(){
//    char str[13];
//    read(sock_cli, str, sizeof(str)-1);
//
//    printf("Message form server: %s\n", str);
//}

//newadd1
void Time(int i){
    time_t rawtime;
    struct tm *ptminfo;
    
    time(&rawtime);
    ptminfo = localtime(&rawtime);
    if(i==1){
        char buf[100];
        
    sprintf(buf,"Current Time(DoorSensor) synchronized: %02d-%02d-%02d %02d:%02d:%02d",
           ptminfo->tm_year + 1900, ptminfo->tm_mon + 1, ptminfo->tm_mday,
           ptminfo->tm_hour, ptminfo->tm_min, ptminfo->tm_sec);
        strcat(buf,"-");
        strcat(buf, "Timestamps(received)->");
        //open file
        timecounter++;
        ofstream myfile;
        myfile.open ("LamportTimestamps.txt");
        myfile << timecounter;
        myfile.close();
        
        string line;
        ifstream in ("LamportTimestamps.txt");
        getline(in,line);
        
        strcat(buf,line.c_str());
        cout<<buf<<endl;
        write(sock_cli, buf, sizeof(buf));
        
    }else if (i==0){
        //send time with clock-offset variable +1 minute
        char buf[100];
        sprintf(buf,"Current Time(DoorSensor): %02d-%02d-%02d %02d:%02d:%02d",
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
        
        cout<< buf<<endl;
        write(sock_cli, buf, sizeof(buf));
        
        sqlite3 *db;
        char *zErrMsg = 0;
        int rc;
        char const *sql;
        const char* data = "Callback function called";
        
        
        
        
        
//        printf("Current Time: %02d-%02d-%02d %02d:%02d:%02d\n",
//               ptminfo->tm_year + 1900, ptminfo->tm_mon + 1, ptminfo->tm_mday,
//               ptminfo->tm_hour, ptminfo->tm_min+1, ptminfo->tm_sec);
    }
}
//newdone1







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
    //thread
    
    RandomID();
   Initialsender();
    
    
    sleep(1);
    Time(0);
    TimeCounterFile();
    thread g(GenerateSignal);
    g.detach();
    thread p(ProcessSignal);
    p.detach();
//newadd
    
//newdone
    
    // loop
    while(1){
        
        //Sensor device
        Bulb1 b1;
        Bulb1 * b=&b1;
        
        
        cout<<"Smart Sensor:"<<endl;
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
                            printf("%s \n", recvbuf);
//            cout<<"recvbuf:"<<recvbuf<<endl;
            
            
            //leader
            
            int iresult[5];
            const char* server[5]={"ClientController","MotionSensor","TemperSensor","Outlet","Bulb"};
            
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
                        myfile1.open ("LamportTimestamps.txt");
                        myfile1 << timecounter;
                        myfile1.close();
                    }
                    Time(1);
                    break;
                }
                
            }
            
            
            
            
            //compare new
            int compare;
            int compare2;
            int status;
            int syn;
//            string ID(recvbuf);
            char ID[]="5 off";
            char ID2[]="5 on";
            char ID3[]="5 status";
            char ID4[]="syn";
            
            syn=strncmp(recvbuf, ID4, 3);
            if(syn==0){
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
                    myfile1.open ("LamportTimestamps.txt");
                    myfile1 << timecounter;
                    myfile1.close();
                }
                Time(1);
                
            }
            
            
            
            compare=strncmp(recvbuf, ID, 5);
            if (compare==0){
                sleep(1);
                b->change_state("off");
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
                    myfile1.open ("LamportTimestamps.txt");
                    myfile1 << timecounter;
                    myfile1.close();
                    //                    cout<<"timecounter2:"<<timecounter;
                }
                
                
                
                char str1[80];
                //open file add timestamps
                strcpy(str1, "No.5 Door Sensor:off----Timestamps(received)->");
                timecounter++;
                ofstream myfile;
                myfile.open ("LamportTimestamps.txt");
                myfile << timecounter;
                myfile.close();
                
                string line;
                ifstream in ("LamportTimestamps.txt");
                getline(in,line);
                in.close();
                
                strcat(str1,line.c_str());
                
                write(sock_cli, str1, sizeof(str1));
                
            }
            status=strncmp(recvbuf, ID3, 5);
            if (status==0){
                sleep(1);
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
                    myfile1.open ("LamportTimestamps.txt");
                    myfile1 << timecounter;
                    myfile1.close();
                
                }
                Initialsender();
                
                
            }
            
            compare2=strncmp(recvbuf, ID2, 4);
            if (compare2==0){
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
                    ofstream myfile2;
                    myfile2.open ("LamportTimestamps.txt");
                    myfile2 << timecounter;
                    myfile2.close();
                }
                
                char str2[80];
//                int retVal=sprintf(str,"No.5 Door Sensor:on TimeCounter is %d",TimeCounter);
                //open file
                strcpy(str2, "No.5 Door Sensor:on----Timestamps(received)->");
                timecounter++;
                ofstream out1;
                out1.open ("LamportTimestamps.txt");
                out1 << timecounter;
                out1.close();
                
                string line;
                ifstream in ("LamportTimestamps.txt");
                getline(in,line);
                in.close();
                
                strcat(str2,line.c_str());
                cout<<str2;
                
//                char str[]="No.5 Door Sensor:on\n";
                
                write(sock_cli, str2, sizeof(str2));
                
                
                //    cout << nowtime << endl;  //输出nowtime
//
                //                time_t t =time(0);
                //                cout<<"Timestamps:"<<t<<endl;
                
                
                //add date
//                time_t nowtime;
//                nowtime = time(NULL);
//                struct tm *local;
//                local=localtime(&nowtime);
//                char buf[80];
//                strftime(buf,80,"Event Time：%Y-%m-%d %H:%M:%S \n",local);
//                cout << buf << endl;
//                write(sock_cli, buf, sizeof(buf));
                
            }
            
            
            
            
//            if(ID=="5 on\n"){
//                sleep(1);
//                b->change_state("on");
//
//                //newadd
//
//                std::ostringstream out;
//                ofstream myfile;
//                myfile.open ("example.txt");
//                myfile << TimeCounter;
//                char str[80];
//                int retVal=sprintf(str,"No.5 Door Sensor:on TimeCounter is %d",TimeCounter);
//
////                char str[]="No.5 Door Sensor:on\n";
//
//                write(sock_cli, str, sizeof(str));
//
//                time_t nowtime;
//                nowtime = time(NULL); //获取日历时间
//                //    cout << nowtime << endl;  //输出nowtime
//
//               struct tm *local;
//               local=localtime(&nowtime);
////                time_t t =time(0);
////                cout<<"Timestamps:"<<t<<endl;
//
//
//                //add date to buf ,then send
//                char buf[80];
//                strftime(buf,80,"Event Time：%Y-%m-%d %H:%M:%S \n",local);
//                cout << buf << endl;
//                write(sock_cli, buf, sizeof(buf));
            
                
                //                send(conn, buffer, len, 0);
                
//database newadd
//                //get local time
//                
//                time_t nowtime;
//                nowtime = time(NULL); //获取日历时间
//                //    cout << nowtime << endl;  //输出nowtime
//                
//                struct tm *local;
//                local=localtime(&nowtime);  //获取当前系统时间
//                
//                char buf[80];
//                strftime(buf,80,"Event Time：%Y-%m-%d %H:%M:%S",local);
//                cout << buf << endl;
//                
//                
//                //database
//                sqlite3 *db;
//                char *zErrMsg = 0;
//                int rc;
//                char const *sql;
//                const char* data = "Callback function called";
//                
//                /* Open database */
//                rc = sqlite3_open("/Users/zhanghuanyu/huanyu.db", &db);
//                if( rc ){
//                    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
//                    exit(0);
//                }else{
//                    fprintf(stderr, "Opened database successfully\n");
//                }
//                
//                //insert
//                
//                sql = "INSERT INTO ClockEvent (NAME) VALUES ('Fraknlin'); ";
//                
//                //    sql="SELECT strftime('%Y-%m-%d %H:%M:%S', 'now');";
//                
//                
//                /* Execute SQL statement */
//                rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
//                if( rc != SQLITE_OK ){
//                    fprintf(stderr, "SQL error: %s\n", zErrMsg);
//                    sqlite3_free(zErrMsg);
//                }else{
//                    fprintf(stdout, "Records created successfully\n");
//                }
//                
//                
//                
//                
//                
//                /* Create SQL statement */
//                sql ="select * from ClockEvent;";
//                
//                /* Execute SQL statement */
//                rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
//                if( rc != SQLITE_OK ){
//                    fprintf(stderr, "SQL error: %s\n", zErrMsg);
//                    sqlite3_free(zErrMsg);
//                }else{
//                    fprintf(stdout, "Operation done successfully\n");
//                }
//                
//                
//                sqlite3_close(db);
// newdone
                
                
                
//            }else if(ID=="5 off\n"){
//                sleep(1);
//                b->change_state("off");
//                char str[]="No.5 Door Sensor:off\n";
//                write(sock_cli, str, sizeof(str));
//                sched_yield();
//            }else if(ID=="5 status\n"){
//                sleep(1);
//                Initialsender();
//
//            }else if (ID=="syn\n"){
//                Time(1);
//            }
            
//            else if (ID=="Away"){
//                sleep(2);
//                b->change_state("off");
//                char str[]="No.3 Smart Bulb:off\n";
//                write(sock_cli, str, sizeof(str));
//            }else if(ID=="Home\n"){
//                sleep(2);
//                b->change_state("on");
//                char str[]="No.3 Smart Bulb:on\n";
//                write(sock_cli, str, sizeof(str));
//                //                send(conn, buffer, len, 0);
//                
//            }
            
            memset(recvbuf, 0, sizeof(recvbuf));
        }
        
    }
    
    close(sock_cli);
    return 0;
}
