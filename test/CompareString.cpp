//
//  CompareString.cpp
//  LoTSmartHome
//
//  Created by 张环宇 on 3/13/19.
//  Copyright © 2019 Huanyu Zhang. All rights reserved.
//

#include "CompareString.hpp"
#include <stdio.h>
#include <string.h>

int main(int argc, const char * argv[])
{
    /* Create a place to store our results */
    int result;
    
    /* Create two arrays to hold our data */
    char example1[50];
    char example2[50];
    
    /* Copy two strings into our data arrays */
    strcpy(example1, "No.5 Door Sensor:on\na");
    strcpy(example2, "No.5 Door Sensor:on\nn");
    
    /* Compare the two strings provided up to 11 characters */
    result = strncmp(example1, example2, 25);
    
    /* If the two strings are the same say so */
    if (result == 0) printf("Strings are the same\n");
    
    /* If the first string is less than the second say so
     (This is because the 'a' in the word 'at' is less than
     the 'i' in the word 'is' */
    if (result < 0) printf("Second string is less than the first\n");
    
    return 0;
}
