#ifndef DSE_h

#define DSE_h

#include "help_function.h"
#include <vector> 
#include <unordered_map>
#include <math.h>
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/raw_ostream.h"

#define LimitedVariable_h

class Compare  
{    public:
          std::string registerName;  
          std::string comparetor;  
          int value;
     public :
          Compare(int valueI,std::string registerNameI,std::string comparetorI);

          void llvm_print();
} ;

class DSE {
     std::vector<Compare*>  compares;
     bool addNewRestrict(std::unordered_map<std::string,LimitedVariable*> *oprand,
                Compare* cmp,bool isIn);
     public:
        bool addNewComapers(std::string registerName,int value, std::string comparetor);
        int maximumInput();
        std::unordered_map<std::string,LimitedVariable*>* getRegisters(int minTerm,std::unordered_map<std::string,LimitedVariable*>*  theOperands);
        void llvm_print();

};

#endif