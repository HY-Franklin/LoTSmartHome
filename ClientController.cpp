
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sstream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sqlite3.h>
#include <sys/shm.h>
#include <fstream>
#include <string>
#include <iostream>
#include <thread>
#include <algorithm> 
#include <list>
#include <time.h>
#include <algorithm>

using namespace std;

#include "ClientController.hpp"
#define PORT 7000
#define IP "127.0.0.1"

int s;
int timecounter;
struct sockaddr_in servaddr;
socklen_t len;
std::list<int> li;
int iSecret;
char ibuff[30];
//leader
void RandomID(){
    srand (time(NULL));
    iSecret = rand() % 10;
    sprintf(ibuff, "UID: %d----ClientController\n",iSecret);
    std::cout<<ibuff;
    
    ofstream out("MaxID.txt");
    out <<iSecret;
    out.close();
   
    ofstream out2("MaxName.txt");
    out2 <<"ClientController";
    out2.close();
    sleep(1);
    
}



void TimeCounterFile(){
    ifstream in("ClientLamportTimestamps.txt");
    if (! in.is_open())
    {
        ofstream out("ClientLamportTimestamps.txt");
        out <<"0";
        out.close();
    };
}

//Initial door sigle and motion detected file
void InitialTwoFile(){
    ifstream in("ClientDoorOpen.txt");
    if (! in.is_open())
    {
        ofstream out("ClientDoorOpen.txt");
        out <<"";
        out.close();
    };
    ifstream in1("ClientMotionDetected.txt");
    if (! in1.is_open())
    {
        ofstream out("ClientMotionDetected.txt");
        out <<"";
        out.close();
    };
    
    ifstream in2("ClientKey.txt");
    if (! in2.is_open())
    {
        ofstream out("ClientKey.txt");
        out <<"";
        out.close();
    };
    //leader
    ifstream in3("MaxID.txt");
    if (! in2.is_open())
    {
        ofstream out("MaxID.txt");
        out <<"";
        out.close();
    };
    ifstream in4("MaxName.txt");
    if (! in2.is_open())
    {
        ofstream out("MaxName.txt");
        out <<"";
        out.close();
    };
    
    
}



//get data
void getConn() {
    while(1){
        int conn = accept(s, (struct sockaddr*)&servaddr, &len);
        li.push_back(conn);
        if(conn==5){
        cout<<"One server connected"<<endl;
            
            
        }else if(conn==6){
            cout<<"Two servers connected"<<endl;
            
        }else if(conn==7){
            cout<<"Three servers connected"<<endl;
        }else if(conn==8){
            cout<<"Four servers connected"<<endl;
        }else if(conn==9){
            //write maxName
            sleep(5);
            //read
            string line;
            ifstream in ("MaxName.txt");
            getline(in,line);
            in.close();
            cout<<"All servers connected and after leader election algorithm, the leader is "<<line<<endl;
            
            //compare if it's ClientCOntroller
            int iresult[5];
            char iline[15];
            const char* server[5]={"DoorSensor","MotionSensor","TemperSensor","Outlet","Bulb"};
            strcpy(iline, line.c_str());
            for(int i=0;i<=4;i++){
                iresult[i]=strncmp(iline, server[i], 5);
                if(iresult[i]==0){
                    //syn time
                    //get local time
                    time_t nowtime;
                    nowtime = time(NULL); //获取日历时间
                    //    cout << nowtime << endl;  //输出nowtime
                    struct tm *local;
                    local=localtime(&nowtime);  //获取当前系统时间
                    char buf[80];
                    strftime(buf,80,"Current Time(synchronized-ClientController)：%Y-%m-%d %H:%M:%S",local);
                    cout << buf << endl;
                    break;
                }
            }
            
//            int icompare;
//            char iline[15];
//            char local[]="ClientController";
//            strcpy(iline, line.c_str());
//            if(icompare==0){
                //syn time
                //get local time
//                time_t nowtime;
//                nowtime = time(NULL); //获取日历时间
                //    cout << nowtime << endl;  //输出nowtime
                
//                struct tm *local;
//                local=localtime(&nowtime);  //获取当前系统时间
            
//                char buf[80];
//                strftime(buf,80,"Current Time(synchronized-ClientController)：%Y-%m-%d %H:%M:%S",local);
//                cout << buf << endl;
//            }
            
            //leader
            char is[80];
            int retVal=sprintf(is,"%s",line.c_str());
            
            std::list<int>::iterator it;
            for(it=li.begin(); it!=li.end(); ++it){
                
                send(*it, is, sizeof(is), 0);
            }
            
            
//            cout<<"All servers connected, we elect ClientContronller as a Master for clock syncronization"<<endl;
            
            
            
        }
        
    }
}



void getData() {
    struct timeval tv;
    tv.tv_sec = 2;
    tv.tv_usec = 0;
    
    //databse operation
    
    //get local time+1
    time_t rawtime;
    struct tm *ptminfo;
    
    time(&rawtime);
    ptminfo = localtime(&rawtime);
    printf("Current Time(ClientController): %02d-%02d-%02d %02d:%02d:%02d\n",
           ptminfo->tm_year + 1900, ptminfo->tm_mon + 1, ptminfo->tm_mday,
           ptminfo->tm_hour, ptminfo->tm_min+1, ptminfo->tm_sec);
    
    
    
    //database
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
    char const *sql;
    const char* data = "Callback function called";
    
    /* Open database */
    rc = sqlite3_open("/Users/zhanghuanyu/huanyu.db", &db);
    if( rc ){
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        exit(0);
    }else{
        fprintf(stderr, "Opened database successfully\n");
    }
    
    
    //insert
    
    //                    sql = "INSERT INTO ClockEvent (NAME) VALUES ('Fraknlin'); ";
    //
    //                    //    sql="SELECT strftime('%Y-%m-%d %H:%M:%S', 'now');";
    //
    //
    //                    /* Execute SQL statement */
    //                    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
    //                    if( rc != SQLITE_OK ){
    //                        fprintf(stderr, "SQL error: %s\n", zErrMsg);
    //                        sqlite3_free(zErrMsg);
    //                    }else{
    //                        fprintf(stdout, "Records created successfully\n");
    //                    }
    
    
    
    
    
    /* Create SQL statement */
//    sql ="select * from ClockEvent;";
//
//    /* Execute SQL statement */
//    rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
//    if( rc != SQLITE_OK ){
//        fprintf(stderr, "SQL error: %s\n", zErrMsg);
//        sqlite3_free(zErrMsg);
//    }else{
//        fprintf(stdout, "Operation done successfully\n");
//    }
    
    
    
    
    while(1) {
        
       
        //connect four servers
        std::list<int>::iterator it;
        for(it=li.begin(); it!=li.end(); ++it){
            fd_set rfds;
            FD_ZERO(&rfds);
            int maxfd = 0;
            int retval = 0;
            FD_SET(*it, &rfds);
            if(maxfd < *it){
                maxfd = *it;
                
            }
            retval = select(maxfd+1, &rfds, NULL, NULL, &tv);
            
            
            
            
            
            if(retval == -1){
                printf("select error\n");
            }else if(retval == 0) {
                //printf("not message\n");
            }else{
   
                char buf[2048];
                memset(buf, 0 ,sizeof(buf));
                int len = recv(*it, buf, sizeof(buf), 0);
//                cout<<"Your input:";
//                printf("%s \n", buf);
                
                
                
                string find(buf);
                
                //get if receive open sigle from door sensor
                try{
                    size_t pos = find.find("Door Open");
                    string str3 = find.substr (pos);
                    //found, save in local
                    ofstream myfile;
                    myfile.open ("ClientDoorOpen.txt");
                    myfile << "Open";
                    myfile.close();
                    //read file
                    cout<<"found message from Door sensor"<<endl;
                
                        //Motion<-detected
                        //check ClientMotionDetected file
                        string line;
                        ifstream in ("ClientMotionDetected.txt");
                        getline(in,line);
                        char char_array[80];
                        strcpy(char_array, line.c_str());
//                        cout<<"char_array:"<<char_array;
                        char example2[]="Motion";
                        int compare;
                        compare=strncmp(char_array, example2, 6);
                        if (compare==0){
                            cout<<"Someone has entered the house.\n";
                            char entered[80];
                            char bufplus[1024];
                            timecounter++;
                            strcpy(entered, "Someone has entered the house");
                            strcat(entered, "----Timestamps:");
                            string ss=to_string(timecounter);
                            strcat(entered,ss.c_str());
                            //record home mode information in local
                            ofstream myfile;
                            myfile.open ("ClientSecuritystatus.txt");
                            myfile << "home";
                            myfile.close();
                            
                            //record into database
                            
                            sprintf(bufplus, "insert into TotalOrderEvent (EVENT) values('%s'); ",  entered);
                            sql=bufplus;
                            
                            /* Execute SQL statement */
                            rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
                            if( rc != SQLITE_OK ){
                                fprintf(stderr, "SQL error: %s\n", zErrMsg);
                                sqlite3_free(zErrMsg);
                            }else{
                                fprintf(stdout, "Records created successfully\n");
                            }
                            
                            
                            
                            //delete local data
                            ofstream out;
                            out.open ("ClientDoorOpen.txt");
                            out << "";
                            ofstream out2;
                            out2.open ("ClientMotionDetected.txt");
                            out2 << "";
                        }else{
                            
                        }
                   
                }catch(...){
//                    cout<<"Not found message from Door sensor"<<endl;
                };
                
                
                //get if received UID
                try{
                    int toint;
                    int txtint;
                    int maxOne = 0;
                    
                    size_t pos = find.find("UID:");
                    string posp = find.substr (pos);
                    
//                    cout<<find<<endl;
                    string number=find.substr(5,1);
                    string name=find.substr(10);
                    cout<<"received IDname:"<<name;
                    stringstream geek(number);
                    
                    
                    geek>>toint;
                    
                    //compare MaxID
                    string line;
                    ifstream in ("MaxID.txt");
                    getline(in,line);
                    in.close();
                    cout<<"Last MaxID:"<<line<<endl<<"receive ID:"<<toint<<endl;
                    
                    stringstream StrtoInt(line);
                    StrtoInt>>txtint;
                    
//                    maxOne=max(toint,txtint);
                    
                    if (toint!=txtint){
                        maxOne=max(toint,txtint);
                        if (maxOne==toint){
                            //write maxName
//                            ofstream out2("MaxName.txt");
//                            out2 <<name;
//                            out2.close();
                            
                            
                            ofstream myfile;
                            myfile.open ("MaxName.txt");
                            myfile << name;
                            myfile.close();
                            
                            ofstream ifile;
                            ifile.open ("MaxID.txt");
                            ifile << maxOne;
                            ifile.close();
                        }
                        
                    }else {
                        toint++;
                        cout<<"UID:"<<toint<<endl;
                        ofstream myfile;
                        myfile.open ("MaxID.txt");
                        myfile << toint;
                        myfile.close();
                        
                    }
                    
                    
                    //input maxID
//                    ofstream myfile;
                    string iline;
//                    myfile.open ("MaxID.txt");
                    ifstream inn ("MaxName.txt");
                    getline(inn,iline);
//                    myfile << maxOne;
                    cout<<"maxUID:"<<maxOne<<"----"<<iline<<endl;
//                    myfile.close();
                    inn.close();
                    
                    
                    
                }catch(...){
                    //                    cout<<"Not found message from Motion sensor"<<endl;
                };
                
                
                
//                get if receive motion detected message
                try{
                    size_t pos = find.find("Motion<-detected");
                    string str3 = find.substr (pos);


                    //found, save in local
                    ofstream myfile;
                    myfile.open ("ClientMotionDetected.txt");
                    myfile << "Motion";
                    myfile.close();
                    //read file
                    cout<<"found message from Motion sensor"<<endl;

                    //Motion<-detected
                    //check ClientDoorOpen.txt file
                    string line;
                    ifstream in ("ClientDoorOpen.txt");
                    getline(in,line);
                    char char_array[80];
                    strcpy(char_array, line.c_str());
                    //                        cout<<"char_array:"<<char_array;
                    char example2[]="Open";
                    int compare;
                    compare=strncmp(char_array, example2, 4);
                    if (compare==0){
                        cout<<"Someone has exited the house.\n";
                        
                        //record away mode information in local
                        ofstream myfile;
                        myfile.open ("ClientSecuritystatus.txt");
                        myfile << "away";
                        myfile.close();
                        
                        
                        
                        //get timestamps
                        char exited[80];
                        char bufplus[1024];
                        strcpy(exited, "Someone has exited the house");
                        timecounter++;
                        strcat(exited, "----Timestamps:");
                        string ss=to_string(timecounter);
                        strcat(exited,ss.c_str());
                        //record into database
                        
                        sprintf(bufplus, "insert into TotalOrderEvent (EVENT) values('%s'); ",  exited);
                        sql=bufplus;
                        
                        
                        
                        /* Execute SQL statement */
                        rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
                        if( rc != SQLITE_OK ){
                            fprintf(stderr, "SQL error: %s\n", zErrMsg);
                            sqlite3_free(zErrMsg);
                        }else{
                            fprintf(stdout, "Records created successfully\n");
                        }
                        
                        
                        //delete data
                        ofstream out;
                        out.open ("ClientDoorOpen.txt");
                        out << "";
                        ofstream out2;
                        out2.open ("ClientMotionDetected.txt");
                        out2 << "";

                    }else{

                    }


                }catch(...){
//                    cout<<"Not found message from Motion sensor"<<endl;
                };
                
                
                
                //get received timestamps
                int leng;
                leng=strlen(buf);
                if(leng>40){
                    size_t pos = find.find("Timestamps");
                    string str3 = find.substr (pos);
                    string str4 = str3.substr (22);
                    //get Max timestaps
                    string::size_type sz;
                    int ReceiveT=stoi (str4,&sz);
//                    cout<<"timecounter:"<<timecounter;
                    timecounter=max(ReceiveT,timecounter);
                    timecounter++;
                    strcat(buf, "---After synchronized, the Timestamps:");
                    string ss=to_string(timecounter);
                    strcat(buf,ss.c_str());
                    
                    //input to file
                    ofstream myfile1;
                    myfile1.open ("ClientLamportTimestamps.txt");
                    myfile1 << timecounter;
                    myfile1.close();
                    printf("%s \n", buf);
                    
                    //recode into data table3
                    
                    
                    /* Create SQL statement */
                    char bufplus[1024];
                    sprintf(bufplus, "insert into TotalOrderEvent (EVENT) values('%s'); ",  buf);
                    sql=bufplus;
                    
                    /* Execute SQL statement */
                    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
                    if( rc != SQLITE_OK ){
                        fprintf(stderr, "SQL error: %s\n", zErrMsg);
                        sqlite3_free(zErrMsg);
                    }else{
                        fprintf(stdout, "Records created successfully\n");
                    }
                    
    
                    
                    
                    
                    
//                    cout<<"timecounter2:"<<timecounter;
                }
                
                
//                size_t pos = findx.find("Timestamps");
//                string str3 = findx.substr (pos);
//                cout<<"str3"<<str3;
//                string str4 = str3.substr (2);
//                cout<<"str4"<<str4;
                
                
                
                
                
                
                
                //record time in databse
          
                
                int result;
                
                
                
                
                //Door Sensor device
                
                char compare[21];
                strcpy(compare, "No.5 Door Sensor:on\n");
                result = strncmp(buf, compare, 20);
                if (result == 0) {
                    printf("Correct\n");
                    
                    sql = "INSERT INTO ClockEvent (EVENTNAME) VALUES ('accept 5on'); ";
                    
                    //    sql="SELECT strftime('%Y-%m-%d %H:%M:%S', 'now');";
                    
                    
                    /* Execute SQL statement */
                    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
                    if( rc != SQLITE_OK ){
                        fprintf(stderr, "SQL error: %s\n", zErrMsg);
                        sqlite3_free(zErrMsg);
                    }else{
                        fprintf(stdout, "Records created successfully\n");
//                        cout<<"Please input:"<<endl;
                    }
                }
            
            int result2;
            char compare2[22];
            strcpy(compare2, "No.5 Door Sensor:off\n");
            result2 = strncmp(buf, compare2, 22);
            if (result2 == 0) {
                printf("Correct\n");

                sql = "INSERT INTO ClockEvent (EVENTNAME) VALUES ('accept 5off'); ";

                //    sql="SELECT strftime('%Y-%m-%d %H:%M:%S', 'now');";


                /* Execute SQL statement */
                rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
                if( rc != SQLITE_OK ){
                    fprintf(stderr, "SQL error: %s\n", zErrMsg);
                    sqlite3_free(zErrMsg);
                }else{
                    fprintf(stdout, "Records created successfully\n");
//                    cout<<"Please input:"<<endl;
                }
            }
                
    //Time Synchronization record to database
                int result3;
                char compare3[8];
                char bufff[120];
                strcpy(compare3, "Current");
                sprintf(bufff, "insert into ClockSyn (EVENTNAME) values('%s');",  buf);
                
                result3 = strncmp(buf, compare3, 7);
                
                
                if (result3 == 0) {
                    printf("\nCorrect\n");
                    
                    sql =bufff;
                    
                    //    sql="SELECT strftime('%Y-%m-%d %H:%M:%S', 'now');";
                    
                    
                    /* Execute SQL statement */
                    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
                    if( rc != SQLITE_OK ){
                        fprintf(stderr, "SQL error: %s\n", zErrMsg);
                        sqlite3_free(zErrMsg);
                    }else{
                        fprintf(stdout, "Records created successfully\n");
//                        cout<<"Please input:"<<endl;
                    }
                }
           
                
// total order table
                
                
            
                
                
                cout<<"Please input:"<<endl;
//                send(*it, buf, len, 0);//send back
                
                
               
            
                
            }
        }
        sleep(1);
        
    }
}

void sendMess() {
    while(1) {
        
        char buf[1024];
//        char str[] = "Hello World!";
//        cout<<"Please input:"<<endl;
    
        fgets(buf, sizeof(buf), stdin);
//        fgets(str, sizeof(str), stdin);
        
        printf("Sending %s", buf);
        
        //key chain
        int keyC;
        int keyO;
        char keyo[]="open";
        char keyc[]="close";
        keyC=strncmp(buf, keyo, 4);
        keyO=strncmp(buf, keyc, 5);
        if (keyC==0){
            ofstream myfile;
            myfile.open ("ClientKey.txt");
            myfile << "open";
        }else if(keyO==0){
            ofstream myfile;
            myfile.open ("ClientKey.txt");
            myfile << "close";
        }
        
        
        
        //Time counter
        timecounter++;
        ofstream out;
        out.open ("ClientLamportTimestamps.txt");
        out << timecounter;
        out.close();
        //output time counter data
//        string line;
//        ifstream in("ClientLamportTimestamps.txt");
//        getline(in,line);
//        cout<<"Time Counter:"<<line<<endl;
        
        
        
//database operation
        sqlite3 *db;
        char *zErrMsg = 0;
        int rc;
        char const *sql;
        const char* data = "Callback function called";
        
        /* Open database */
        rc = sqlite3_open("/Users/zhanghuanyu/huanyu.db", &db);
        
        
        //select table2 from database
        int OutData;
        char OutDataC[4];
        strcpy(OutDataC, "t2\n");
        OutData = strncmp(buf, OutDataC, 4);
        
        if (OutData == 0) {
            printf("Correct\n");
            sql="select * from ClockEvent;";
            
        /* Execute SQL statement */
            rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
            if( rc != SQLITE_OK ){
                fprintf(stderr, "SQL error: %s\n", zErrMsg);
                sqlite3_free(zErrMsg);
            }else{
                fprintf(stdout, "Output ClockEvent successfully\n");
            }
        }
        
        //table1
        int OutData2;
        char OutDataC2[4];
        strcpy(OutDataC2, "t1\n");
        OutData2 = strncmp(buf, OutDataC2, 4);
        
        if (OutData2 == 0) {
            printf("Correct\n");
            sql="select * from ClockSyn;";
            
            /* Execute SQL statement */
            rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
            if( rc != SQLITE_OK ){
                fprintf(stderr, "SQL error: %s\n", zErrMsg);
                sqlite3_free(zErrMsg);
            }else{
                fprintf(stdout, "Output ClockSyn successfully\n");
            }
        }
        
        
        // TotalOrderEvent table operation
        
        int TotalOrder;
        char TotalOrderEvent[4];
        strcpy(TotalOrderEvent, "t3\n");
        TotalOrder = strncmp(buf, TotalOrderEvent, 4);
        
        if (TotalOrder == 0) {
            printf("Correct\n");
            sql="select * from TotalOrderEvent;";
            
            /* Execute SQL statement */
            rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
            if( rc != SQLITE_OK ){
                fprintf(stderr, "SQL error: %s\n", zErrMsg);
                sqlite3_free(zErrMsg);
            }else{
                fprintf(stdout, "Output TotalOrderEvent successfully\n");
            }
        }
        
        
        
        
        
        //next operation
        int result;
        
        char compare[21];
        strcpy(compare, "5 on\n");
        result = strncmp(buf, compare, 6);
        if (result == 0) {
            printf("Correct\n");
            
            sql = "INSERT INTO ClockEvent (EVENTNAME) VALUES ('Send 5on command'); ";
            
            //    sql="SELECT strftime('%Y-%m-%d %H:%M:%S', 'now');";
            
            
            /* Execute SQL statement */
            rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
            if( rc != SQLITE_OK ){
                fprintf(stderr, "SQL error: %s\n", zErrMsg);
                sqlite3_free(zErrMsg);
            }else{
                fprintf(stdout, "Records 5 on successfully\n");
            }
        }
        
        

        
        
        //delete all data in table1
        int deletea;
        
        
        char compare3[7];
        strcpy(compare3, "dt1\n");
        deletea = strncmp(buf, compare3, 7);
        if (deletea == 0) {
            printf("Correct\n");
            
            sql = "delete from ClockSyn; DELETE FROM sqlite_sequence;";
            
            //    sql="SELECT strftime('%Y-%m-%d %H:%M:%S', 'now');";
              sleep(1);
            
            /* Execute SQL statement */
            rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
            if( rc != SQLITE_OK ){
                fprintf(stderr, "SQL error: %s\n", zErrMsg);
                sqlite3_free(zErrMsg);
            }else{
                fprintf(stdout, "Delete data from ClockSyn successfully\n");
            }
        }
        
        //delete all data in table2
        int deleteb;
        
        char compare4[8];
        strcpy(compare4, "dt2\n");
        deleteb = strncmp(buf, compare4, 8);
        if (deleteb == 0) {
            printf("Correct\n");
            
            sql = "delete from ClockEvent; DELETE FROM sqlite_sequence;";
            
            //    sql="SELECT strftime('%Y-%m-%d %H:%M:%S', 'now');";
              sleep(1);
            
            /* Execute SQL statement */
            rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
            if( rc != SQLITE_OK ){
                fprintf(stderr, "SQL error: %s\n", zErrMsg);
                sqlite3_free(zErrMsg);
            }else{
                fprintf(stdout, "Delete data from ClockEvent successfully\n");
            }
        }
        
        
    //delete all data in table3
        int deleted3;
        
        char compared3[8];
        strcpy(compared3, "dt3\n");
        deleted3 = strncmp(buf, compared3, 8);
        if (deleted3 == 0) {
            printf("Correct\n");
            
            sql = "delete from TotalOrderEvent; DELETE FROM sqlite_sequence;";
            
            //    sql="SELECT strftime('%Y-%m-%d %H:%M:%S', 'now');";
            sleep(1);
            
            /* Execute SQL statement */
            rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
            if( rc != SQLITE_OK ){
                fprintf(stderr, "SQL error: %s\n", zErrMsg);
                sqlite3_free(zErrMsg);
            }else{
                fprintf(stdout, "Delete data from TotalOrderEvent successfully\n");
            }
        }
        
        
        
        
//        printf("%s", str);
        std::list<int>::iterator it;
        for(it=li.begin(); it!=li.end(); ++it){
            //add Time counter to buff
            string mess;
//            char messPlus[60]="Time Counter from ClientController is ";
            ifstream in ("ClientLamportTimestamps.txt");
            getline(in,mess);
            
            
            mess.insert(0,"Timestamps(send):");
            
            
            //delete "\n" of the tail
            string ToString(buf);
            string chars = "\n";
            for (char c: chars) {
                ToString.erase(std::remove(ToString.begin(), ToString.end(), c), ToString.end());
            }
            strcpy(buf, ToString.c_str());
            
//            strcpy(messPlus,mess.c_str());
//            sprintf(messPlus,"Time Counter from ClientController is %s ",mess.c_str());
//            cout<<"messPlus"<<messPlus<<endl;
            strcat(buf,"->");
            strcat(buf,mess.c_str());
            
            
            
            
            
            
            
            send(*it, buf, sizeof(buf), 0);
//            send(*it, str, sizeof(str), 0);
        }
        cout<<buf<<endl;
        
        //recode into data table3
    
        
        /* Create SQL statement */
        char bufplus[1024];
        sprintf(bufplus, "insert into TotalOrderEvent (EVENT) values('%s'); ",  buf);
        sql=bufplus;
        
        /* Execute SQL statement */
        rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
        if( rc != SQLITE_OK ){
            fprintf(stderr, "SQL error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
        }else{
            fprintf(stdout, "Records created successfully\n");
        }
        
        
        
    }
}

// Check if user is at home, send message to Motion sensor
void AutosendMess() {
    while(1) {
        sleep(1);
        char buf[1024];
//        fgets(buf, sizeof(buf), stdin);
//        strcat(buf, "Huanyu Zhangaxaxasafafsas");
//        printf("you are send %s", buf);
        
        //check local security file
        //read
        string home="home";
        string away="away";
        string line;
        ifstream into ("ClientSecuritystatus.txt");
        getline(into,line);
        if (line.compare(home)==0){
            string ss(buf);
            ss.insert(0, home);
            strcpy(buf, ss.c_str());
            
            std::list<int>::iterator it;
            for(it=li.begin(); it!=li.end(); ++it){
                send(*it, buf, sizeof(buf), 0);
            }
            
            //read
            string open="open";
            string line;
            ifstream in ("ClientKey.txt");
            getline(in,line);
//            cout<<"line:"<<line<<endl<<"open:"<<open;
            
            if(open.compare(line)==0){
                cout<<"Someone entered the home. Detected user's keychain,it's User"<<endl;
            }else {
                cout<<"Someone came home."<<endl;
            }
            
            
            //empty txt
            ofstream key;
            key.open ("ClientKey.txt");
            key << "";
            key.close();
            
//            cout<<"User came home."<<endl;
            //empty txt
            ofstream myfile;
            myfile.open ("ClientSecuritystatus.txt");
            myfile << "";
        }else if (line.compare(away)==0){
            string ss(buf);
            ss.insert(0, away);
            strcpy(buf, ss.c_str());
            
            std::list<int>::iterator it;
            for(it=li.begin(); it!=li.end(); ++it){
                send(*it, buf, sizeof(buf), 0);
            }
            //read
            string close="close";
            string line;
            ifstream in ("ClientKey.txt");
            getline(in,line);
//            cout<<"line:"<<line<<endl<<"close:"<<close;
            if(close.compare(line)==0){
                cout<<"Someone entered the home. Not detected user's keychain, it's Intruder"<<endl;
            }else{
                cout<<"Someone left home."<<endl;
            }
            //empty txt
            ofstream key;
            key.open ("ClientKey.txt");
            key << "";
            
            
//            cout<<"User left home."<<endl;
            //empty txt
            ofstream myfile;
            myfile.open ("ClientSecuritystatus.txt");
            myfile << "";
        }
        
        
        
        
    }
}




//database operation unused

void db(){
                    //get local time
    
                    time_t nowtime;
                    nowtime = time(NULL); //获取日历时间
                    //    cout << nowtime << endl;  //输出nowtime
    
                    struct tm *local;
                    local=localtime(&nowtime);  //获取当前系统时间
    
                    char buf[80];
                    strftime(buf,80,"Time(Now)：%Y-%m-%d %H:%M:%S",local);
                    cout << buf << endl;
    
    
                    //database
                    sqlite3 *db;
                    char *zErrMsg = 0;
                    int rc;
                    char const *sql;
                    const char* data = "Callback function called";
    
                    /* Open database */
                    rc = sqlite3_open("/Users/zhanghuanyu/huanyu.db", &db);
                    if( rc ){
                        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
                        exit(0);
                    }else{
                        fprintf(stderr, "Connect database successfully\n");
                    }
    
                    //insert
    
//                    sql = "INSERT INTO ClockEvent (NAME) VALUES ('Fraknlin'); ";
//
//                    //    sql="SELECT strftime('%Y-%m-%d %H:%M:%S', 'now');";
//
//
//                    /* Execute SQL statement */
//                    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
//                    if( rc != SQLITE_OK ){
//                        fprintf(stderr, "SQL error: %s\n", zErrMsg);
//                        sqlite3_free(zErrMsg);
//                    }else{
//                        fprintf(stdout, "Records created successfully\n");
//                    }
    
    
    
    
    
        /* Create SQL statement */
//                    sql ="select * from ClockEvent;";
//
//                    /* Execute SQL statement */
//                    rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
//                    if( rc != SQLITE_OK ){
//                        fprintf(stderr, "SQL error: %s\n", zErrMsg);
//                        sqlite3_free(zErrMsg);
//                    }else{
//                        fprintf(stdout, "Operation done successfully!\n");
//                    }
    
    
//                    sqlite3_close(db);
}


//void isend(){
//    while(1){
//        sleep(1);
//    char iibuf[]="Hello world~~~~~~~~~~~~~~~~~~~~";
//    std::list<int>::iterator it;
//    for(it=li.begin(); it!=li.end(); ++it){
//
//        send(*it, iibuf, sizeof(iibuf), 0);
//    }
//
//    }
//}



int main() {
    //new socket
    s = socket(AF_INET, SOCK_STREAM, 0);
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr(IP);
   
    if(bind(s, (struct sockaddr* ) &servaddr, sizeof(servaddr))==-1) {
        perror("bind");
        exit(1);
    }
    if(listen(s, 20) == -1) {
        perror("listen");
        exit(1);
    }
    len = sizeof(servaddr);
    
    //
    
    //introduction
    cout<<endl<<endl<<"Welcome to the controller center!"<<endl<<"There are some instructions to manage your LoT devices:"<<endl<<"1.Motion Sensor  2.Temperature Sensor  3.SmartBulb  4.SmartOutlet 5.DoorSensor"<<endl<<endl<<"Instructions:"<<endl<<"\"1 on\"or\"1 off\" to turn Motion Sensor."<<endl<<"\"2 on\"or\"2 off\" to turn Temperature Sensor on or off."<<endl<<"\"3 on\"or\"3 off\" to turn SmartBulb on or off."<<endl<<"\"4 on\"or\"4 off\" to turn SmartOutlet on or off."<<endl<<"\"home\" or \"away\" to shutdown to activated Security System."<<endl<<"\"5 on\"or\"5 off\" to turn Door Sensor on or off"<<endl<<"\"5 status\"to Check status of Door Sensor."<<endl<<"\"syn\" to Synchronise Time"<<endl<<"\"open\" , \"close\" to set if user's key chain can be detected or not by ClientController. "<<endl<<endl<<"Database operation:"<<endl<<"\"t1\" - \"t2\" - \"t3\"to check table [ClockSyn] or [ClockEvent] [TotalOrderEvent]from database."<<endl<<"\"dt1\" , \"dt2\" , \"dt3\" to Delete all data from table [ClockSyn] or [ClockEvent] or [TotalOrderEvent]. !^_^!"<<endl<<endl;

    
//    thread df(sendx);
//    df.join();
    //thread : while ==>> accpet
    
     RandomID();
    
//    cout<<"Please input:"<<endl;
    timecounter=0;
    TimeCounterFile();//initialize time counter
    InitialTwoFile();//initialize
    std::thread t(getConn);
    t.detach();
    std::thread t1(sendMess);
    t1.detach();
    //thread : recv ==>> show
    std::thread t2(getData);
    t2.detach();
//    cout<<"Please input:"<<endl;
    
    thread tt(AutosendMess);
    tt.detach();
    
//    thread is(isend);
//    is.detach();
    

    
    
    while(1){
        
    }
    return 0;
}
