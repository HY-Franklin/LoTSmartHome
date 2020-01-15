

#include "SmartDevices.hpp"

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include<fstream>
#include <unistd.h>
using namespace std;

extern int device_id;
class Bulb1:public Bulb{
public:
    
    char type[20]="Smart Device";
    char name[20]="Smart Bulb";
    
    bool state=true;
    //record status
     void registerInfo(){
         std::cout<<"     No.3 "<<name<<":";
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
         ifstream in("SmartBulb.txt");
         if (! in.is_open())
         {
             ofstream out("SmartBulb.txt");
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
        ofstream out("SmartBulb.txt");
//        getline(cin,button);
        
        
        //check if the right name of device
        
        if(button=="on"){
            out <<button;
            
            out.close();
            std::cout<<"Your Smart Bulb is open"<<endl;
            
            
        }else if(button=="off"){
            out <<button;
            
            out.close();
            std::cout<<"Your Smart Bulb is close"<<endl;
            
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
//inherit
class Outlet1:public Outlet{
    char type[20]="Smart Device";
    char name[20]="Smart Outlet";
public:
    void registerInfo(){
        
        std::cout<<"     No.4 "<<name<<":";
        //Initial state
        
//                         ofstream out("SmartO.txt");
//                         if (out.is_open())
//                         {
//
//                             out <<"Off";
//
//                             out.close();
//                         }
        
        
        //output
        char buffer[256];
        ifstream in("SmartO.txt");
        if (! in.is_open())
        {
            ofstream out("SmartO.txt");
            out <<"Off";
            sleep(1);
            out.close();
//            if (out.is_open())
//            {
            
//                out <<"Off";
            
            
//            }
            
            
            
//            cout << "Error opening file"; exit (1);
            
            
            
        }
        while (!in.eof() )
        {
            in.getline (buffer,100);
            sleep(1);
            cout << buffer << endl;
        }
        
        
    }
public:
    void change_state(string button){
        
//        cout<<"on or off: ";
        //        cin>>button;
        ofstream out("SmartO.txt");
//        getline(cin,button);
        
        
        //check if the right name of device
        
        if(button=="on"){
            out <<button;
            
            out.close();
            std::cout<<"Your Smart Device is open"<<endl;
            
            
        }else if(button=="off"){
            out <<button;
            
            out.close();
            std::cout<<"Your Smart Device is close"<<endl;
            
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
//                    std::cout<<"Your Smart Device is open"<<endl;
//                    break;
//                    
//                }else if(button=="off"){
//                    out <<button;
//                    
//                    out.close();
//                    std::cout<<"Your Smart Device is close"<<endl;
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
    
    
//     bool getStatusOutlet(){
//        //
//
//        return true;
    
    
};
