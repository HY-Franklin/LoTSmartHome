
#include "SenorDevices.hpp"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <unistd.h>
using namespace std;
    

static char type[20]="Sensor Device";


//sensor 1
class Action1:public Action{

public:
    
    
        char name[20]="Motion Sensor";
//record state
    void query_state(){
            
        std::cout<<"     No.1 "<<name<<":";
        
//        Initial state
        
//        ofstream out("Sensor.txt");
//        if (out.is_open())
//        {
//
//            out <<"Off";
//
//            out.close();
//        }

        
        //ouput status
        char buffer[256];
        
        //create data txt
        ifstream in("Sensor.txt");
        if (! in.is_open()) //if txt not exisit, create new one
        {
            ofstream out("Sensor.txt");
                    if (out.is_open())
                    {
            
                        out <<"Off";
                        sleep(1);
                        out.close();
                    }
            
//            out.close();
            
        }
        
        
        while (!in.eof() )   //show data txt

        {
            in.getline (buffer,100);
            sleep(1);
            cout << buffer << endl;
        }
//
        
        }
    
    //change state
    
    void change_state(string button){
        
        ofstream out("Sensor.txt");
 
        if(button=="on"){

                out <<button;
                
                out.close();
                std::cout<<"Your Motion Sensor is open"<<endl;
            
            
           
            
        }else if(button=="off"){
            
            out <<button;
            
            out.close();
            
            std::cout<<"Your Motion Sensor is close"<<endl;
            
        }
//            else if(button!="on"||"off"){
//
//
//            while(button!="on"||"off"){
//                std::cout<<"Please input the correct name!"<<endl;
//
//                getline(cin,button);
//
//                if(button=="on"){
//                    out <<button;
//
//                    out.close();
//                    std::cout<<"Your Motion Sensor is open"<<endl;
//                    break;
//
//                }else if(button=="off"){
//                    out <<button;
//
//                    out.close();
//                    std::cout<<"Your Motion Sensor is close"<<endl;
//                    break;
//
//                }
//
//            }
//        }
       
    }

    
};



//sensor 2
class Temperature1:public Temperature{
    
public:
//    char type[20]="SensorDevice";
    char name[20]="Temperature Sensor";

    void registerInfo(){
        std::cout<<"     No.2 "<<name<<":";
        
//        Initial state
        
//                         ofstream out("SmartT.txt");
//                         if (out.is_open())
//                         {
//
//                             out <<"Off";
//
//                             out.close();
//                         }

        
        //output
        
        //
        char buffer[256];
        ifstream in("SmartT.txt");
        if (! in.is_open())
        {
            
            ofstream out("SmartT.txt");
            out <<"off";
            sleep(1);
//            out.close();
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
    
    void change_state(string button){
        
//        cout<<"on or off: ";
        ofstream out("SmartT.txt");
        //        cin>>button;
//        getline(cin,button);
        
        
        //check if the right name of device
        
        if(button=="on"){
            out <<button;
            
            out.close();
                        std::cout<<"Your Temperature Sensor is open"<<endl;
            
            
            }else if(button=="off"){
                out <<button;
                
                out.close();
                std::cout<<"Your Temperature Sensor is close"<<endl;
            
            }
        
//            else if(button!="open"||"close"){
//
//
//        while(button!="on"||"off"){
//            std::cout<<"Please input the correct button name!"<<endl;
//            getline(cin,button);
//
//            if(button=="on"){
//                out <<button;
//
//                out.close();
//                std::cout<<"Your Temperature Sensor is open"<<endl;
//                break;
//
//            }else if(button=="off"){
//                out <<button;
//
//                out.close();
//                std::cout<<"Your Temperature Sensor is close"<<endl;
//                break;
//
//            }
//
//        }
//                    }
        
        
        
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
    
    
    

};
        

