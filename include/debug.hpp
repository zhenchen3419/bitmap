#ifndef __DEBUG__
#define __DEBUG__
#include <cstdio>

#ifdef DEBUG

        #define DBGINFO(format,...) \
            printf(format"   ## %s:%d\n",__VA_ARGS__,__FILE__,__LINE__)
        #define DBGFUN(function,...) \
           function (__VA_ARGS__);

#else
    #define DBGINFO(format,...)
    #define DBGFUN(func,...)
#endif
#endif
