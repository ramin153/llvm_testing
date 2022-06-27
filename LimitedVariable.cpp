//Reihan flhi 9732493
//ramin rowshan 9732491
#include "LimitedVariable.h"

int LimitedVariable::count = 0;
std::ostream& operator << (std::ostream &os, LimitedVariable&  s) {
    return (os << "{min: " << s.getMin() << "\t max: " << s.getMax() <<  "}");
}



LimitedVariable::LimitedVariable(int maxSize,int minSize){
         max = maxSize;
         min = minSize;
         parent = nullptr;
         id = count;
         count++;
     }

LimitedVariable::LimitedVariable(int maxSize,int minSize,LimitedVariable* theparent){
         max = maxSize;
         min = minSize;
         parent = theparent;
         id = count;
         count++;
     }






bool LimitedVariable::getIsCloseToAllocate()
{
    return isCloseToAllocate;
}
void LimitedVariable::setIsCloseToAllocate(bool isClose)
{
    isCloseToAllocate = isClose;
}
void LimitedVariable::restrictMin(int newMin){
         if(newMin > min){
             min = newMin;

             if(parent != nullptr)
                if(!parent->isItAbsoulte()){

                    parent->restrictMin(newMin);
                    }
         }
     }
void LimitedVariable::restrictMax(int newMax){
         if(newMax < max){
             max = newMax;
             if(parent != nullptr){
                if(!parent->isItAbsoulte())
                    parent->restrictMax(newMax);
             }
         }
     }
bool LimitedVariable::isItValid(){
    return max >= min;
}

bool LimitedVariable::isItValidToresticMin(int newMin){
    return max >= newMin;
}
bool LimitedVariable::isItValidToresticMax(int newMax){
    return newMax >= min;
}
bool LimitedVariable::isItAbsoulte(){
    return max == min;
}
int LimitedVariable::getValueInRange(){
    if(!LimitedVariable::isItValid())
        return OUT_OF_RANGE;
    int result = max;
    if(max > min)
        result = (std::rand() % (max - min +1)) + (min);
    return result;
}

void LimitedVariable:: setNewLimit(int maxSize,int minSize){
    max = maxSize;
    min = minSize;
}

int LimitedVariable:: getMin(){
    return min;
}
int LimitedVariable:: getMax(){
    return max;
}

void LimitedVariable::add(int forMin,int forMax){
    min += forMin;
    max += forMax;
}
void LimitedVariable::div(int forMin,int forMax){
    min /= forMin;
    max /= forMax;
}
void LimitedVariable::mult(int forMin,int forMax){
    min *= forMin;
    max *= forMax;
}
void LimitedVariable::sub(int forMin,int forMax){
    min -= forMin;
    max -= forMax;
}

LimitedVariable* LimitedVariable::copyVariable(){
    return new LimitedVariable(max,min,this);
}

LimitedVariable* LimitedVariable::addAndNew(LimitedVariable* lv)
{
    return new LimitedVariable(max + lv->getMax(),min + lv->getMin());
}

LimitedVariable* LimitedVariable::divAndNew(LimitedVariable* lv)
{
    return new LimitedVariable(max / lv->getMax(),min / lv->getMin());
}

LimitedVariable* LimitedVariable::multAndNew(LimitedVariable* lv)
{
    return new LimitedVariable(max * lv->getMax(),min * lv->getMin());
}

LimitedVariable* LimitedVariable::subAndNew(LimitedVariable* lv)
{
    return new LimitedVariable(max - lv->getMax(),min - lv->getMin());
}
