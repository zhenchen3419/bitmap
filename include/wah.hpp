#ifndef __WAH__
#define __WAH__

#include "debug.hpp"
#include "bitmap.hpp"

#include <cstdio>
#include <cstdint>
#include <cstring>
#include <x86intrin.h>

#define ZERO_FILL_MAX 0x3FFFFFFF
#define ZERO_FILL_MASK 0xC0000000
#define FILL_MASK 0x80000000
#define SGN_MASK 0x40000000
#define COUNTER_MASK 0x3FFFFFFF
#define LITERAL_SIZE 31
#define LITERAL_BASE 0x00000001

class WAH: public Bitmap{
    protected:
        void appendFill(){
            if(Bitmap::capacity == Bitmap::size){
                expandBuffer();
            }

            Bitmap::words[Bitmap::size] = ZERO_FILL_MAX;
            Bitmap::size ++;
        }


    public:
        WAH(const int& cap = 20):Bitmap(cap){
            appendFill();
        }

        ~WAH(){
        }

        virtual void appendNumber(const int& number){
            if(number > Bitmap::maxNumber) {
                int threshold = (Bitmap::maxNumber/LITERAL_SIZE) + 1;
                if(maxNumber < 0){
                    uint32_t blockPos = number/31;
                    int pos = number%31;
                    if(blockPos == 0){
                        Bitmap::words[0] = FILL_MASK | (LITERAL_BASE << pos);
                        Bitmap::words[1] = ZERO_FILL_MAX;
                        Bitmap::size = 2;
                    }
                    else{
                        Bitmap::words[0] = blockPos;
                        Bitmap::words[1] =  FILL_MASK | (LITERAL_BASE << pos);
                        Bitmap::words[2] = ZERO_FILL_MAX;
                        Bitmap::size = 3;
                    }
                }
                else if( number < threshold*LITERAL_SIZE){
                    uint32_t& w = Bitmap::words[Bitmap::size - 2];
                    w |= (LITERAL_BASE << (number%LITERAL_SIZE));
                    if(w == 0xFFFFFFFF){
                        if((Bitmap::words[Bitmap::size-3] & ZERO_FILL_MASK) == SGN_MASK ){
                            words[size - 3] ++;
                            words[size - 2] = ZERO_FILL_MAX;
                            size --;
                        }
                        else{
                            w = SGN_MASK + 1;
                        }
                    }
                }
                else{
                    if(Bitmap::size + 2 > Bitmap::capacity){
                        expandBuffer();
                    }
                    uint32_t& w = Bitmap::words[Bitmap::size - 1];

                    uint32_t blockSize = number/LITERAL_SIZE;
                    int pos = number%LITERAL_SIZE;
                    blockSize -= threshold;
                    if(blockSize == 0){
                        w = FILL_MASK | (LITERAL_BASE << pos);
                        Bitmap::words[Bitmap::size] = ZERO_FILL_MAX;
                        Bitmap::size ++;
                    }
                    else{
                        w = blockSize;
                        Bitmap::words[Bitmap::size] = FILL_MASK | (LITERAL_BASE << pos);
                        Bitmap::words[Bitmap::size + 1] = ZERO_FILL_MAX;
                        Bitmap::size += 2;
                    }
                }
                Bitmap::maxNumber = number;
                Bitmap::intNumber++;
            }
            else{
                printf("Invalid Number Appended\n");
                return;
            }
        }


        virtual void clear(){
            Bitmap::size = 1;
            Bitmap::intNumber = 0;
            Bitmap::maxNumber = -1;
            Bitmap::words[0] = ZERO_FILL_MAX;
        }
        
        virtual void clearAndShrink(){
            Bitmap::capacity = 20;
            if(Bitmap::words != nullptr) delete[] Bitmap::words;
            Bitmap::words = new uint32_t[Bitmap::capacity];
            clear();
        }

        virtual void printNumbers(const int& start=0,const int& length=10) const{
            int* ret = new int[Bitmap::intNumber];
            int prog = 0;
            int cnt = 0;
            for(int i=0;i<Bitmap::size;i++){
                uint32_t w = Bitmap::words[i];
                if((w & FILL_MASK) != 0){
                    int pop = __builtin_popcount(w) - 1;
                    for(int i=0;i<pop;i++){
                        ret[cnt] = prog + __builtin_ctz(w);
                        cnt ++;
                        w = w&(w-1);
                    }
                    prog += 31;
                }
                else if((w & SGN_MASK) != 0){
                    int end = prog + (w&COUNTER_MASK)*31;
                    for(;prog < end;prog++){
                        ret[cnt] = prog;
                        cnt++;
                    }
                }
                else{
                    prog += (w*31);
                }
                if(cnt >= start + length) break;
            }

            for(int i=start;i<Bitmap::intNumber;i++){
                printf("%d ",ret[i]);
            }
            printf("\n");
            delete[] ret;
            return;
        }

        virtual int* extractNumbers() const{
            int* ret = new int[Bitmap::intNumber];
            int prog = 0;
            int cnt = 0;
            for(int i=0;i<Bitmap::size;i++){
                uint32_t w = Bitmap::words[i];
                if((w & FILL_MASK) != 0){
                    int pop = __builtin_popcount(w) - 1;
                    for(int i=0;i<pop;i++){
                        ret[cnt] = prog + __builtin_ctz(w);
                        cnt ++;
                        w = w&(w-1);
                    }
                    prog += 31;
                }
                else if((w & SGN_MASK) != 0){
                    int end = prog + (w&COUNTER_MASK)*31;
                    for(;prog < end;prog++){
                        ret[cnt] = prog;
                        cnt++;
                    }
                }
                else{
                    prog += (w*31);
                }
            }
            return ret;
        }
        
        

        virtual void bitwiseAnd(const Bitmap& b){
            
            return;
        }

        virtual void bitwiseOr(const Bitmap& b){
            return;
        }

        virtual void bitwiseXor(const Bitmap& b){
            return;
        }

};

#undef ZERO_FILL_MAX
#undef ZERO_FILL_MASK
#undef FILL_MASK
#undef SGN_MASK
#undef COUNTER_MASK
#undef LITERAL_SIZE
#undef LITERAL_BASE

#endif
