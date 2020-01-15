
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <algorithm> 
#include <string.h>
#include "SenorDevices.hpp"
#include "SenorDevices.cpp"
#include <thread>
#include <string.h>
#include <sstream>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/shm.h>
#include <iostream>
#include <mutex>
using namespace std;

#define MYPORT  7000

#define BUFFER_SIZE 1024

//generate temperature random:
int t1; //temperature
int sock_cli;// socket
int timecounter;

int iSecret;
char ibuff[30];

void RandomID(){
    srand (time(NULL));
    iSecret = rand() % 10;
    sprintf(ibuff, "UID: %d----TemperSensor\n",iSecret);
    std::cout<<ibuff;
    send(sock_cli, ibuff, strlen(ibuff),0);
    sleep(1);
    
}
void GenerateTemper(){
//extern char xx[1020];
    while(true){
        
        sleep(60);
        t1 = rand() % 35 -5;
//        sprintf(xx,"%d",t1);
//        cout<<"Right now the temperature:"<<t1<<endl;
        
    }
    
}
//get data and handle it
void CheckTemper(){
    while(true){
        
        if (t1>26){
           
        sleep(60);
//            char str2[5];
            
//            for(int i=0;i<1;i++){
//                sprintf(str2,"%d",t1);
//            }
//            sprintf(str,"%d",t1);
//            strcat(xx,"°,the weather is hot");
//            char str[]="Right now the temperature is hot\nPipe heater: closed\n";
            char str[1024];
            sleep(60);
            sprintf(str,"%d",t1);  //int to char
            strcat(str,"°<--the temperature is hot\nPipe heater: closed."); // extend char str[]
            
            //open file
            strcat(str,"-");
            strcat(str, "Timestamps(received)->");
            timecounter++;
            ofstream myfile;
            myfile.open ("TempTimestamps.txt");
            myfile << timecounter;
            myfile.close();
            
            string line;
            ifstream in ("TempTimestamps.txt");
            getline(in,line);
            
            strcat(str,line.c_str());
            
            strcat(str,"\n");
            write(sock_cli, str, sizeof(str)); //send char str[]
             cout<<"Right now the temperature:"<<t1<<"   Hot"<<endl;
            
            
        }
        //judge temperature
        if((t1<10)&&(t1>=3)){
            sleep(60);
            
            
            
           
            char str[1024];
            sleep(60);
            sprintf(str,"%d",t1);
            strcat(str,"°<--the temperature is cold\nPipe heater: closed");
            //open file
            strcat(str,"----");
            strcat(str, "Timestamps(received)->");
            timecounter++;
            ofstream myfile;
            myfile.open ("TempTimestamps.txt");
            myfile << timecounter;
            myfile.close();
            
            string line;
            ifstream in ("TempTimestamps.txt");
            getline(in,line);
            
            strcat(str,line.c_str());
            strcat(str,"\n");
            write(sock_cli, str, sizeof(str));
            //             sleep(5);
            cout<<"Right now the temperature:"<<t1<<"   cold"<<endl;
            
        }
        
        
        if ((t1<=26)&&(t1>=10)){
            
           
            sleep(60);
            
            
            char str[1024];
            sleep(60);
            //            char str[1024];
//            sleep(5);
            sprintf(str,"%d",t1);
            strcat(str,"°<--the temperature is warm\nPipe heater: closed");
            
            //open file
            strcat(str,"----");
            strcat(str, "Timestamps(received)->");
            timecounter++;
            ofstream myfile;
            myfile.open ("TempTimestamps.txt");
            myfile << timecounter;
            myfile.close();
            
            string line;
            ifstream in ("TempTimestamps.txt");
            getline(in,line);
            
            strcat(str,line.c_str());
            strcat(str,"\n");
//            char str[]="Right now the temperature is cool\nPipe heater: closed\n";
            write(sock_cli, str, sizeof(str));
//             sleep(5);
                cout<<"Right now the temperature:"<<t1<<"   cool"<<endl;
                
            
//
           
        }
   
        if(t1<=2){
           sleep(60);
            
            
//            char str2[]="Hello:";
            char sendbuf[80];
            sprintf(sendbuf,"%d",t1);
            strcat(sendbuf,"°<--the weather is cold\nPipe heater : open");
            
            //open file
            strcat(sendbuf,"----");
            strcat(sendbuf, "Timestamps(received)->");
            timecounter++;
            ofstream myfile;
            myfile.open ("TempTimestamps.txt");
            myfile << timecounter;
            myfile.close();
            
            string line;
            ifstream in ("TempTimestamps.txt");
            getline(in,line);
            
            strcat(sendbuf,line.c_str());
            strcat(sendbuf,"\n");
            
            
//                            fgets(sendbuf, sizeof(sendbuf), stdin);
            send(sock_cli, sendbuf, strlen(sendbuf),0); //send
//           memset(sendbuf, 0, sizeof(sendbuf));
            
//
//
            cout<<"Right now the temperature:"<<t1<<"   cold"<<endl;
//            write(sock_cli, &t1, sizeof(t1));
            
            
        }
            
        }
    }
    
//send mess when connect, report status
void Initialsender(){
    string line;
    //    string ca="<--System Security status";
    ifstream myfile;
    myfile.open("SmartT.txt", ios::out | ios::in );
    //
    getline (myfile,line);
    line.append("<--Temperature Sensor status\n");
    //Timstamps
    timecounter++;
    ofstream out;
    out.open ("TempTimestamps.txt");
    out << timecounter;
    out.close();
    
    //read
    string li;
    ifstream in ("TempTimestamps.txt");
    getline(in,li);
    li.append("\n");
    line.append(li);
    cout<<line;
    
    
    char* chr = const_cast<char*>(line.c_str());
    //        strcat(chr, "<--System Security status");
//    send(sock_cli, ibuff, strlen(ibuff),0);
    send(sock_cli, chr, strlen(chr),0);
    
}

void switch1(){
    string line;
    
    ifstream myfile;// open file
    myfile.open("SmartT.txt", ios::out | ios::in );
    getline (myfile,line);//get data to line
    if (line=="on"){
        
        thread a(GenerateTemper);
        a.detach();
        
        thread b(CheckTemper);
        b.detach();
        
    }else if(line=="off"){
        pause();
        
        
        
    }
}

//time
void Time(int i){
    time_t rawtime;
    struct tm *ptminfo;
    
    time(&rawtime);
    ptminfo = localtime(&rawtime);
    if(i==1){
        char buf[100];
        
        sprintf(buf,"Current Time(TemperSensor) synchronized: %02d-%02d-%02d %02d:%02d:%02d",
                ptminfo->tm_year + 1900, ptminfo->tm_mon + 1, ptminfo->tm_mday,
                ptminfo->tm_hour, ptminfo->tm_min, ptminfo->tm_sec);
        strcat(buf,"-");
        strcat(buf, "Timestamps(received)->");
        //open file
        timecounter++;
        ofstream myfile;
        myfile.open ("TempTimestamps.txt");
        myfile << timecounter;
        myfile.close();
        
        string line;
        ifstream in ("TempTimestamps.txt");
        getline(in,line);
        
        strcat(buf,line.c_str());
        cout<<buf<<endl;
       
        write(sock_cli, buf, sizeof(buf));
        
    }else if (i==0){
        //send time with clock-offset variable +1 minute
        char buf[100];
        sprintf(buf,"Current Time(TemperSensor): %02d-%02d-%02d %02d:%02d:%02d",
                ptminfo->tm_year + 1900, ptminfo->tm_mon + 1, ptminfo->tm_mday,
                ptminfo->tm_hour, ptminfo->tm_min+1, ptminfo->tm_sec);
        //open file
        strcat(buf,"-");
        strcat(buf, "Timestamps(received)->");
        timecounter++;
        ofstream myfile;
        myfile.open ("TempTimestamps.txt");
        myfile << timecounter;
        myfile.close();
        
        string line;
        ifstream in ("TempTimestamps.txt");
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
    ifstream in("TempTimestamps.txt");
    if (! in.is_open())
    {
        ofstream out("TempTimestamps.txt");
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
    
    ///define sockfd
    sock_cli = socket(AF_INET,SOCK_STREAM, 0);
    ///define sockaddr_in
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(MYPORT);
    
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");  
    
   //connect
    if (connect(sock_cli, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("connect");
        exit(1);
    }
    
    
    
    
     RandomID();
    //Thread
    
    thread Ini(Initialsender);
    Ini.detach();
   Time(0);
    TimeCounterFile();
    //loop
    while(1){
        
        //Sensor device
        Temperature1 t1;
        Temperature1* t=&t1;
        
        thread s(switch1);
        s.detach();
        cout<<"Sensor Devices:"<<endl;
        t->registerInfo();
        
        
  
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
            const char* server[5]={"ClientController","MotionSensor","DoorSensor","Outlet","Bulb"};
            
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
                        myfile1.open ("TempTimestamps.txt");
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
            int syn;
            char ID[]="2 on";
            char ID2[]="2 off";
            char ID5[]="syn";
            
            on=strncmp(recvbuf, ID, 4);
            off=strncmp(recvbuf, ID2, 4);
            syn=strncmp(recvbuf, ID5, 3);
            
            if (on==0){
                
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
                    myfile2.open ("TempTimestamps.txt");
                    myfile2 << timecounter;
                    myfile2.close();
                }
                
                thread a(GenerateTemper);
                a.detach();
                
                thread b(CheckTemper);
                b.detach();
                
                t->change_state("on");
//                char str[]="No.2 Temperature Sensor:on\n";
                char str2[80];
                //                int retVal=sprintf(str,"No.5 Door Sensor:on TimeCounter is %d",TimeCounter);
                //open file
                strcpy(str2, "No.2 Temperature Sensor:on----Timestamps(received)->");
                timecounter++;
                ofstream out1;
                out1.open ("TempTimestamps.txt");
                out1 << timecounter;
                out1.close();
                
                string line;
                ifstream in ("TempTimestamps.txt");
                getline(in,line);
                in.close();
                
                strcat(str2,line.c_str());
                
                write(sock_cli, str2, sizeof(str2));
            }
            //off function
            
            else if(off==0){
//                char str[]="No.2 Temperature Sensor:Off\n";
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
                    myfile1.open ("TempTimestamps.txt");
                    myfile1 << timecounter;
                    myfile1.close();
                    //                    cout<<"timecounter2:"<<timecounter;
                }
                
                
                
                char str1[80];
                //open file
                strcpy(str1, "No.2 Temperature Sensor:Off----Timestamps(received)->");
                timecounter++;
                ofstream myfile;
                myfile.open ("TempTimestamps.txt");
                myfile << timecounter;
                myfile.close();
                
                string line;
                ifstream in ("TempTimestamps.txt");
                getline(in,line);
                in.close();
                
                strcat(str1,line.c_str());
                write(sock_cli, str1, sizeof(str1));
                t->change_state("off");
            }
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
                    myfile1.open ("TempTimestamps.txt");
                    myfile1 << timecounter;
                    myfile1.close();
                }
                Time(1);
            }
            
            
            
//            string ID(recvbuf);
//            if(ID=="2 on"){
//                thread a(GenerateTemper);
//                a.detach();
//                
//                thread b(CheckTemper);
//                b.detach();
//                
//                t->change_state("on");
//                char str[]="No.2 Temperature Sensor:on\n";
//                write(sock_cli, str, sizeof(str));
//                
//                
//            }else if(ID=="2 off"){
//                
//                
//                char str[]="No.2 Temperature Sensor:Off\n";
//                write(sock_cli, str, sizeof(str));
//                t->change_state("off");
//                
//                //yield process
//                thread s(switch1);
//
//                this_thread::yield();
//            }else if (ID=="syn"){
//                Time(1);
//            }
            
            
            
            memset(recvbuf, 0, sizeof(recvbuf));
            
        }
        
        
        
    }
    
    close(sock_cli);
    return 0;
}
