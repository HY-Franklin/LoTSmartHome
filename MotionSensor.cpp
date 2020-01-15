

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <algorithm> 
#include <string.h>
#include <thread>
#include <fstream>
#include "SenorDevices.hpp"
#include "SenorDevices.cpp"


#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
using namespace std;

#define MYPORT  7000

#define BUFFER_SIZE 1024

int m1;//prameter of motion detection
int sock_cli;  //socket
int randomval; //equal to m1
string mm;
int timecounter;
int iSecret;
char ibuff[30];

void RandomID(){
    srand (time(NULL));
    iSecret = rand() % 10;
    sprintf(ibuff, "UID: %d----MotionSensor\n",iSecret);
    std::cout<<ibuff;
    send(sock_cli, ibuff, strlen(ibuff),0);
    sleep(1);
    
}
//check if motion in room
//thread
void GenerateMotion(){
    
    while(true){
        sleep(40);
        randomval = rand() % 2;
    }
   
}
//get data and handle it
void CheckMotion(){
    while(true){
        if(randomval==1){
            sleep(40);
            cout<<"Motion<-detected in the room"<<endl;
//            char str[]="Motion detected in the room\n";
            char str1[80];
            //open file add timestamps
            strcpy(str1, "Motion<-detected in the room----Timestamps(received)->");
            timecounter++;
            ofstream myfile;
            myfile.open ("MotionTimestamps.txt");
            myfile << timecounter;
            myfile.close();
            
            string line;
            ifstream in ("MotionTimestamps.txt");
            getline(in,line);
            in.close();
            
            strcat(str1,line.c_str());
            strcat(str1,"\n");
            write(sock_cli, str1, sizeof(str1));
            
            ofstream out("SecurityState.txt");
            
            if(mm=="Away"){
                sleep(1);
//                cout<<"Alarm! Motion detected under \"Away\" Security Status\n"<<endl;
//                char str[]="Alarm! Motion detected under \"Away\" Security Status\n";
                char str1[80];
                //open file add timestamps
                strcpy(str1, "Alarm! Motion<-detected under \"Away\" Security Status----Timestamps(received)->");
                timecounter++;
                ofstream myfile;
                myfile.open ("MotionTimestamps.txt");
                myfile << timecounter;
                myfile.close();
                
                string line;
                ifstream in ("MotionTimestamps.txt");
                getline(in,line);
                in.close();
                
                strcat(str1,line.c_str());
                strcat(str1,"\n");
                
                write(sock_cli, str1, sizeof(str1));
                cout<<"Alarm! Motion<-detected under \"Away\" Security Status\n"<<endl;
            }
            
                
                
        }else{
            sleep(30);
            cout<<"No Motion detected in the room"<<endl;
//            char str[]="No Motion detected in the room\n";
            char str1[80];
            //open file add timestamps
            strcpy(str1, "No Motion detected in the room----Timestamps(received)->");
            timecounter++;
            ofstream myfile;
            myfile.open ("MotionTimestamps.txt");
            myfile << timecounter;
            myfile.close();
            
            string line;
            ifstream in ("MotionTimestamps.txt");
            getline(in,line);
            in.close();
            
            strcat(str1,line.c_str());
//            strcat(str1,"\n");
            
            write(sock_cli, str1, sizeof(str1));
        }
        
    }
}


//change mode
void change_mode(string cm){
    mm=cm;
//    while(true){
    ofstream out("SecurityState.txt");
    if(cm=="Home"){
//        sleep(60);
//        char ca[]="Home Security: shutdown";
            out <<cm;
        sleep(1);
//            out.close();
        std::cout<<"Security Model: Home"<<endl;
        
        cout<<"Home security: shutdown\n"<<endl;
//        char strs[]="Home security: shutdown\n";
        
        char str1[80];
        //open file add timestamps
        strcpy(str1, "Home security: shutdown----Timestamps(received)->");
        timecounter++;
        ofstream myfile;
        myfile.open ("MotionTimestamps.txt");
        myfile << timecounter;
        myfile.close();
        
        string line;
        ifstream in ("MotionTimestamps.txt");
        getline(in,line);
        in.close();
        
        strcat(str1,line.c_str());
        strcat(str1,"\n");
        
        write(sock_cli, str1, sizeof(str1));
        
        
        write(sock_cli, str1, sizeof(str1));
    }else if(cm=="Away"){
//        sleep(60);
        
        out <<cm;
        sleep(1);
//        out.close();
        std::cout<<"Security Model: Away"<<endl;
        
        cout<<"Home security: Activated\n"<<endl;
//        char strs[]="Home security: Activated\n";
        char str1[80];
        //open file add timestamps
        strcpy(str1, "Home security: Activated----Timestamps(received)->");
        timecounter++;
        ofstream myfile;
        myfile.open ("MotionTimestamps.txt");
        myfile << timecounter;
        myfile.close();
        
        string line;
        ifstream in ("MotionTimestamps.txt");
        getline(in,line);
        in.close();
        
        strcat(str1,line.c_str());
        strcat(str1,"\n");
        
        write(sock_cli, str1, sizeof(str1));
        
        
        write(sock_cli, str1, sizeof(str1));
//    }
    }
}

//Security state
void SecurityState(){
    
    char buffer[256];
    ifstream in("SecurityState.txt");
    if (! in.is_open())
    {
//        ofstream out("SecurityState.txt");
//        out<<"Home\n";
//        out.close();
        ofstream out("SecurityState.txt");
                if (out.is_open())
                {
                    sleep(1);
                    out <<"Home";
                    
                    out.close();
                }
        in.getline (buffer,100);
        sleep(1);
        cout << buffer << endl;
        
        
        
    }
    while (!in.eof() )
    {
        ofstream out("SecurityState.txt");
        if (out.is_open())
        {
            
            out <<"Home";
            sleep(1);
            out.close();
        }
        in.getline (buffer,100);
        sleep(1);
        cout << buffer << endl;
    }
    
}
//send status when connect
void Initialsender(){
    string line;
//    string ca="<--System Security status";
    ifstream myfile;
    myfile.open("SecurityState.txt", ios::out | ios::in );
//
        getline (myfile,line);
    line.append("<--Home Security status\n");
    //Timstamps
    timecounter++;
    ofstream out;
    out.open ("MotionTimestamps.txt");
    out << timecounter;
    out.close();
    
    //read
    string li;
    ifstream in ("MotionTimestamps.txt");
    getline(in,li);
    li.append("\n");
    line.append(li);
    cout<<line;
    
    
        char* chr = const_cast<char*>(line.c_str());
//        strcat(chr, "<--System Security status");
//    send(sock_cli, ibuff, strlen(ibuff),0);
        send(sock_cli, chr, strlen(chr),0);

}

//time
void Time(int i){
    time_t rawtime;
    struct tm *ptminfo;
    
    time(&rawtime);
    ptminfo = localtime(&rawtime);
    if(i==1){
        char buf[100];
        
        sprintf(buf,"Current Time(MotionSensor) synchronized: %02d-%02d-%02d %02d:%02d:%02d",
                ptminfo->tm_year + 1900, ptminfo->tm_mon + 1, ptminfo->tm_mday,
                ptminfo->tm_hour, ptminfo->tm_min, ptminfo->tm_sec);
        strcat(buf,"-");
        strcat(buf, "Timestamps(received)->");
        //open file
        timecounter++;
        ofstream myfile;
        myfile.open ("MotionTimestamps.txt");
        myfile << timecounter;
        myfile.close();
        
        string line;
        ifstream in ("MotionTimestamps.txt");
        getline(in,line);
        
        strcat(buf,line.c_str());
        cout<<buf<<endl;
        
        write(sock_cli, buf, sizeof(buf));
        
    }else if (i==0){
        //send time with clock-offset variable +1 minute
        char buf[100];
        sprintf(buf,"Current Time(MotionSensor): %02d-%02d-%02d %02d:%02d:%02d",
                ptminfo->tm_year + 1900, ptminfo->tm_mon + 1, ptminfo->tm_mday,
                ptminfo->tm_hour, ptminfo->tm_min+1, ptminfo->tm_sec);
        //open file
        strcat(buf,"-");
        strcat(buf, "Timestamps(received)->");
        timecounter++;
        ofstream myfile;
        myfile.open ("MotionTimestamps.txt");
        myfile << timecounter;
        myfile.close();
        
        string line;
        ifstream in ("MotionTimestamps.txt");
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
    ifstream in("MotionTimestamps.txt");
    if (! in.is_open())
    {
        ofstream out("MotionTimestamps.txt");
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
    
    ///定义sockfd
    sock_cli = socket(AF_INET,SOCK_STREAM, 0);
    ///定义sockaddr_in
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(MYPORT); 
    
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    
    
  
    if (connect(sock_cli, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("connect");
        exit(1);
    }
    
    RandomID();
    Time(0);
    TimeCounterFile();
    thread s1(SecurityState);
    s1.join();
    thread s2(Initialsender);
    s2.detach();
    //thead
    thread m1(GenerateMotion);
    m1.detach();
    thread m2(CheckMotion);
    m2.detach();
    
    
    
    
    
    while(1){
        
        //Sensor device
        Action1 a1;
        Action1* a=&a1;
        
        
        cout<<"Sensor Devices:"<<endl;
        a->query_state();
       
        
        
        
        
   
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
            const char* server[5]={"ClientController","DoorSensor","TemperSensor","Outlet","Bulb"};
            
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
                        myfile1.open ("MotionTimestamps.txt");
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
            int open;
            int close;
            
            
            //            string ID(recvbuf);
            char ID[]="1 on";
            char ID2[]="1 off";
            char ID3[]="away";
            char ID6[]="home";
            char ID5[]="syn";
            char ID7[]="open";
            char ID8[]="close";
            //on function
            on=strncmp(recvbuf, ID, 4);
            off=strncmp(recvbuf, ID2, 5);
            away=strncmp(recvbuf, ID3, 4);
            home=strncmp(recvbuf, ID6, 4);
            syn=strncmp(recvbuf, ID5, 3);
            
            if (on==0){
                a->change_state("on");
                //                char str[]="No.1 Motion Sensor:on\n";
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
                    myfile2.open ("MotionTimestamps.txt");
                    myfile2 << timecounter;
                    myfile2.close();
                }
                
                char str2[80];
                //                int retVal=sprintf(str,"No.5 Door Sensor:on TimeCounter is %d",TimeCounter);
                //open file
                strcpy(str2, "No.1 Motion Sensor:on----Timestamps(received)->");
                timecounter++;
                ofstream out1;
                out1.open ("MotionTimestamps.txt");
                out1 << timecounter;
                out1.close();
                
                string line;
                ifstream in ("MotionTimestamps.txt");
                getline(in,line);
                in.close();
                
                strcat(str2,line.c_str());
                
                
                write(sock_cli, str2, sizeof(str2));
            }
            //off function
            
            else if(off==0){
                a->change_state("off");
//                char str[]="No.1 Motion Sensor:off\n";
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
                    myfile1.open ("MotionTimestamps.txt");
                    myfile1 << timecounter;
                    myfile1.close();
                    //                    cout<<"timecounter2:"<<timecounter;
                }
                
                
                
                char str1[80];
                //open file
                strcpy(str1, "No.1 Motion Sensor:off----Timestamps(received)->");
                timecounter++;
                ofstream myfile;
                myfile.open ("MotionTimestamps.txt");
                myfile << timecounter;
                myfile.close();
                
                string line;
                ifstream in ("MotionTimestamps.txt");
                getline(in,line);
                in.close();
                
                strcat(str1,line.c_str());
                write(sock_cli, str1, sizeof(str1));
                sleep(1);
                thread m2(CheckMotion);
                sched_yield();
                
            }
            //away mod
            //            away=strncmp(recvbuf, ID3, 4);
            else if(away==0){
                sleep(1);
                
                change_mode("Away");
            }
            
            //home mode
            //            home==strncmp(recvbuf, ID6, 5);
            //            cout<<"home="<<home;
            else if (home==0){
                sleep(1);
                change_mode("Home");
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
                    myfile1.open ("MotionTimestamps.txt");
                    myfile1 << timecounter;
                    myfile1.close();
                }
                Time(1);
            }else if (open==0){
                
            }else if (close==0){
                
            }
            
            
            //old codes
            
//            string ID(recvbuf);
//            if(ID=="1 on"){
//                a->change_state("on");
//                char str[]="No.1 Motion Sensor:on\n";
//                write(sock_cli, str, sizeof(str));
//
//                
//            }else if(ID=="1 off"){
//                //
//                a->change_state("off");
//                char str[]="No.1 Motion Sensor:off\n";
//                write(sock_cli, str, sizeof(str));
//                sleep(1);
//                thread m2(CheckMotion);
//                sched_yield();
//                
//               
//            }else if(ID=="Home"){
//                sleep(1);
//                change_mode("Home");
//                
//            }else if(ID=="Away"){
//                sleep(1);
//                change_mode("Away");
//            }else if (ID=="syn"){
//                Time(1);
//            }
            
//            else if (ID=="Away"){
//                void change_mode{
//
//                }
//
//            }
            
            
            memset(recvbuf, 0, sizeof(recvbuf));
        }
        
    }
    
    close(sock_cli);
    return 0;
}
