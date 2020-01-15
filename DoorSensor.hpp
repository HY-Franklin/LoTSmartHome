//
//  DoorSensor.hpp
//  LoTSmartHome
// 73.139.215.52
//  Created by 张环宇 on 3/10/19.
//  Copyright © 2019 Huanyu Zhang. All rights reserved.
//

#ifndef DoorSensor_hpp
#define DoorSensor_hpp

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <unistd.h>
using namespace std;
#include <stdio.h>
int TimeCounter;
class Bulb{
public:
    
    
    
    virtual void registerInfo()=0;
    
};

//database function newadd
//static int callback(void *data, int argc, char **argv, char **azColName){
//    int i;
//    fprintf(stderr, "%s: ", (const char*)data);
//    for(i=0; i<argc; i++){
//        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
//    }
//    printf("\n");
//    return 0;
//}
// newdone



extern int device_id;
class Bulb1:public Bulb{
public:
    
    char type[20]="Smart Device";
    char name[20]="Door Sensor";
    
    bool state=true;
    //record status
    void registerInfo(){
        std::cout<<"     No.5 "<<name<<":";
        //         Initial state
        
        //                 ofstream out("SmartBulb.txt");
        //                 if (out.is_open())
        //                 {
        //
        //                     out <<"Off";
        //
        //                     out.close();
        //                 }
        //
        
        
        //read status from txt
        char buffer[256];
        ifstream in("DoorSensor.txt");
        if (! in.is_open())
        {
            ofstream out("DoorSensor.txt");
            out <<"Off";
            sleep(1);
            out.close();
            //             if (out.is_open())
            //             {
            
            //                 out <<"Off";
            
            
            //             }
            
            
            //             cout << "Error opening file"; exit (1);
            
        }
        while (!in.eof() )
        {
            
            in.getline (buffer,100);
            sleep(1);
            cout << buffer << endl;
        }
        
        
        
        
        
    }
    
    //change state
    void change_state(string button){
        
        //        cout<<"on or off: ";
        //        cin>>button;
        ofstream out("DoorSensor.txt");
        //        getline(cin,button);
        
        //open file
        ofstream myfile;
        myfile.open ("LamportTimestamps.txt");
        myfile << TimeCounter;
        //check if the right name of device
        
        if(button=="on"){
            out <<button;
            
            out.close();
            std::cout<<"Your Door Sensor is open"<<endl;
            
            TimeCounter++;
            cout<<"TimeCounter:"<<TimeCounter<<endl;
            
            
        }else if(button=="off"){
            out <<button;
            
            out.close();
            std::cout<<"Your Door Sensor is close"<<endl;
            TimeCounter++;
            cout<<"TimeCounter:"<<TimeCounter<<endl;
        }
        //        else if(button!="on"||"off"){
        //
        //
        //            while(button!="on"||"off"){
        //                std::cout<<"Please input the correct name!"<<endl;
        //                getline(cin,button);
        //
        //                if(button=="on"){
        //                    out <<button;
        //
        //                    out.close();
        //                    std::cout<<"Your Smart Bulb is open"<<endl;
        //                    break;
        //
        //                }else if(button=="off"){
        //                    out <<button;
        //
        //                    out.close();
        //                    std::cout<<"Your Smart Bulb is close"<<endl;
        //                    break;
        //
        //                }
        //
        //            }
        //        }
        
        
        
        //        if (button=="open"){
        //            std::cout<<"Your Temperature Sensor is open"<<endl;
        //        }else if(button=="close"){
        //            std::cout<<"Your Temperature Sensor is close"<<endl;
        //        }else{
        //            while(button!="open"||button!="close"){
        //            std::cout<<"Please input the correct name!"<<endl;
        //            getline(cin,button);
        //
        //            }
        //        }
        
        
        
    }
    
    
    //     bool getStatusBulb(){
    //        //
    //         std::cout<<name;
    //        return true;
    //    }
    //     bool query_state(){
    //        //
    //         std::cout<<name;
    //        return true;
    //    }
    //     void report_state(){
    //         std::cout<<name;
    //        //
    //    }
    
};




#endif /* DoorSensor_hpp */
