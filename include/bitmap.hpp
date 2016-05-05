#ifndef __BITMAP__
#define __BITMAP__
#include <cstdint>
#include <cstdio>
#include <cstring>

class Bitmap{
    protected:
        int32_t capacity;
        int32_t size;
        uint32_t* words;
        int32_t intNumber;
        int32_t maxNumber;
        void expandBuffer(){
            uint32_t* temp = new uint32_t[capacity * 2];
            std::memcpy(temp,words,sizeof(uint32_t)*size);
            delete[] words;
            words = temp;
            capacity *= 2;
        }

    public:
        Bitmap(const int& cap){
            if(cap < 20) capacity = 20;
            else capacity = cap;
            size = 0;
            words = new uint32_t[capacity];
            intNumber = 0;
            maxNumber = -1;
        }

        ~Bitmap(){
            if(words != nullptr) delete[] words;
        }

        
        virtual void readFromFile(const char* filename){
            FILE* p = fopen(filename,"rb");
            fread(&size,4,1,p);
            delete[] words;
            words = new uint32_t[size];
            fread(&intNumber,4,1,p);
            fread(&maxNumber,4,1,p);
            fread(words,4,size,p);
            capacity = size;
            fclose(p);
            return;
        }
        

        virtual void copyFrom(const Bitmap& b){
            if(capacity < b.size){
                delete[] words;
                words = new uint32_t[b.size];
                capacity = b.size;
            }
            std::memcpy(words,b.words,sizeof(uint32_t)*b.size);
            size = b.size;
            intNumber = b.intNumber;
            maxNumber = b.maxNumber;
        }
        
        virtual void appendNumber(const int& n)=0;
        virtual void clear()=0;
        virtual void clearAndShrink()=0;

        virtual void printNumbers(const int& start=0,const int& length=10) const =0;
        
        void printBuffers(const int& start=0,const int& length=10) const{
            if(start >= size) {
                printf("Start from Nothing\n");
                return;
            }
            int end = start + length;
            if(end > size) end = size;
            for(int i=start;i<end;i++){
                printf("%08X ",words[i]);
            }
            if(end == size) {
                printf("END");
            }
            printf("\n");
        }
        
        void printStatus(){
            printf("Buffer Used: %d/%d\ninteger NO.: %d\nMaximal NO.: %d\n",size,capacity,intNumber,maxNumber);
        }


        virtual int* extractNumbers() const =0;
        
        virtual void writeToFile(const char* filename){
            FILE* p = fopen(filename,"wb");
            fwrite(&size,sizeof(int32_t),1,p);
            fwrite(&intNumber,4,1,p);
            fwrite(&maxNumber,4,1,p);
            fwrite(words,4,size,p);
            fclose(p);
            return;
        }
        
        virtual void bitwiseAnd(const Bitmap& b)=0;
        virtual void bitwiseOr(const Bitmap& b)=0;
        virtual void bitwiseXor(const Bitmap& b)=0;
};


#endif
