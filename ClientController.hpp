//
//  ClientController.hpp
//  LoTSmartHome
//
//  Created by 张环宇 on 3/13/19.
//  Copyright © 2019 Huanyu Zhang. All rights reserved.
//

#ifndef ClientController_h
#define ClientController_h

static int callback(void *data, int argc, char **argv, char **azColName){
    int i;
    fprintf(stderr, "%s: ", (const char*)data);
    for(i=0; i<argc; i++){
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}


#endif /* ClientController_h */
