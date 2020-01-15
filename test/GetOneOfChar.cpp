//
//  GetOneOfChar.cpp
//  LoTSmartHome
//
//  Created by 张环宇 on 3/14/19.
//  Copyright © 2019 Huanyu Zhang. All rights reserved.
//

#include "GetOneOfChar.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
//#include
using namespace std;

int main()
{
    char buf[80];
    
    
    strcpy(buf, "12345-24");
    
    string s(buf);
    
    char c;
    char b;
    
//    if (isdigit(buf[5]))
//    {
//        num = atoi (buf);
//        printf ("The number that followed %d was %d.\n",num,num+1);
//    }
    
    c = s.at(1);
    b = s.at(2);
    int num=c- '0';
    int num2=b- '0';
    if (num>num2){
        cout<<num;
    }else{
        cout<<num2;
    }
    
    return 0;
}
