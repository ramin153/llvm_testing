//Reihan flhi 9732493
//ramin rowshan 9732491
#ifndef LimitedVariable_h

#include <stdlib.h>
#include <stdio.h>
#include <sstream>
#include <iostream>
#include <random>

#include "llvm/Support/raw_ostream.h"

#define LimitedVariable_h

#define MAX_SIZE 100
#define MIN_SIZE -100
#define OUT_OF_RANGE 2147483647

class LimitedVariable{
    static int count;
    int id = 0;
    int max = 0;
    int min = 0;
    bool isCloseToAllocate = false;
    LimitedVariable * parent;
    public:

        LimitedVariable(int maxSize,int minSize);
        LimitedVariable(int maxSize,int minSize,LimitedVariable * theParent);

        bool getIsCloseToAllocate();
        void setIsCloseToAllocate(bool isClose);

        void setNewLimit(int maxSize,int minSize);
        void restrictMin(int newMin);
        void restrictMax(int newMax);
        
        bool isItValid();
        bool isItValidToresticMin(int newMin);
        bool isItValidToresticMax(int newMax);
        bool isItAbsoulte();
        int getValueInRange();
        int getMin();
        int getMax();
        
        void add(int forMin,int forMax);
        void div(int forMin,int forMax);
        void mult(int forMin,int forMax);
        void sub(int forMin,int forMax);

        LimitedVariable* addAndNew(LimitedVariable* lv);
        LimitedVariable* divAndNew(LimitedVariable* lv);
        LimitedVariable* multAndNew(LimitedVariable* lv);
        LimitedVariable* subAndNew(LimitedVariable* lv);

        LimitedVariable* copyVariable();

        std::string toString(){
            std::stringstream ss;
            std::string p = "{}";
            if(parent != nullptr)
                p = parent->toString();
            ss << "{ id: "<<id <<"\tmin: " << min << "\t max: "<< max << "\tparent:"<<p << " }";
            return ss.str();
        }


        static LimitedVariable* createLimitedVariableInRange(){    
             return new LimitedVariable(MAX_SIZE,MIN_SIZE);
        }

    
};

#endif
