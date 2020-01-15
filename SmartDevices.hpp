

#ifndef SmartDevices_hpp
#define SmartDevices_hpp

#include <stdio.h>
class Bulb{
public:
    
    
    
    virtual void registerInfo()=0;

};


class Outlet{
public:
    virtual void registerInfo()=0;

};
#endif /* SmartDevices_hpp */
