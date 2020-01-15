

#ifndef SenorDevices_hpp
#define SenorDevices_hpp

#include <stdio.h>
int test(int x,int y);


class Action{
//    virtual void change_state()=0;
    virtual void query_state()=0;
//    virtual bool getMove() = 0;
};




class Temperature{
public:
//    virtual void change_state()=0;
    virtual void registerInfo()=0;
//    virtual int getT() = 0;
};
#endif /* SenorDevices_hpp */
