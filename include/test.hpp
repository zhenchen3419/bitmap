#ifndef __TEST__
#define __TEST__
#include "wah.hpp"
#include <cstdlib>

void print(int* buf){
    for(int i=0;i<10;i++){
        printf("%d ",buf[i]);
    }
    printf("\n");
}

void wahTest(){
    // 测试生成
    WAH wah(20);
    wah.appendNumber(0);
    wah.appendNumber(12);
    wah.appendNumber(120);
    wah.appendNumber(30);
    wah.printBuffers();
    wah.printStatus();
    wah.clear();


    for(int i=200;i<600;i++){
        wah.appendNumber(i);
    }
    wah.printBuffers();
    wah.printStatus();
    wah.clear();

    int* arr = new int[300];
    arr[0] = 100;
    for(int i=0;i<100;i++){
        arr[i] = arr[i-1] + rand()%64 + 1;
    }    
    
    for(int i=100;i<200;i++){
        arr[i] = arr[i-1]+1;
    }

    for(int i=200;i<300;i++){
        arr[i] = arr[i-1] + rand()%50+1;
    }

    for(int i=0;i<300;i++){
        wah.appendNumber(arr[i]);
    }

    int* ret = wah.extractNumbers();
    print(arr);
    print(ret);
    for(int i=0;i<300;i++){
        if(arr[i] != ret[i]){
            printf("%d != %d, %dth\n",arr[i],ret[i],i);
            break;
        }
    }
    
    delete[] arr;
    delete[] ret;
    wah.printBuffers();
    wah.printStatus();

    wah.writeToFile("out.wah"); 
    wah.clearAndShrink();
    wah.readFromFile("out.wah");
    wah.printBuffers();
    wah.printStatus();
}


#endif
