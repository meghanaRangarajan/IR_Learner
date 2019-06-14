#include <pigpio.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <chrono>
#include <thread>
#include <unistd.h>
#include <cmath>
#include <string>
#include <iterator>
#include <vector> 
#include <list> 
#include <utility> 
#include "cpp_code.h"


int main()
    {
    uint32_t result = code();
    create_insert(10,result);
    std::cout<<"___________RESULT = "<<std::hex<<result<<"___________"<<std::dec<<std::endl;

        
        return 0;
    }