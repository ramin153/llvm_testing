//Reihan flhi 9732493
//ramin rowshan 9732491
#include <cstdio>
#include <iostream>
#include <stdio.h>
#include <set>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <stdexcept>
#include <vector> 

#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/raw_ostream.h"

#include "LimitedVariable.h"
#include "help_function.h"
#include "DSE.h"

#define MODE 1


using namespace llvm;

LLVMContext &getGlobalContext() {
  static LLVMContext context;
  return context;
}

std::set<std::pair<std::string,BasicBlock*>> allBlock;
std::set<std::pair<std::string,BasicBlock*>> visitedBlock;

std::vector<std::unordered_map<std::string,LimitedVariable*>> operrand_with_unique_path;

void printBlockResult(int numberOfSearch);
void generatePath(BasicBlock* BB,std::unordered_map<std::string,LimitedVariable*>   * operands);
void search(std::shared_ptr<Module> theModule);
void addAllBlock(std::shared_ptr<Module> theModule);
void generatePath_task1(BasicBlock* BB);
void search_task1(std::shared_ptr<Module> theModule);


void search_DSE(std::shared_ptr<Module> theModule);
DSE* getAllCompare(BasicBlock* BB ,DSE* compare,std::unordered_map<std::string,LimitedVariable*> *oprand ,
std::set<std::string> *visitedBB);
BasicBlock* firstBasicBlock(std::shared_ptr<Module> theModule);



int main(int argc, char **argv)
{
    LLVMContext &Context = getGlobalContext();
    SMDiagnostic Err;
    std::shared_ptr<Module> M = parseIRFile(argv[1], Err, Context);
    if (M == nullptr)
    {
      fprintf(stderr, "error: failed to load LLVM IR file \"%s\"", argv[1]);
      return EXIT_FAILURE;
    }

    addAllBlock(M);
    //search_task1(M);

    search_DSE(M);

    return 0;
}


void search(std::shared_ptr<Module> theModule){
  srand(time(NULL));
  llvm::outs()<<"\n\n\n----------------------------------------------------search start------------------------------------------------\n";
  int i;
  for( i = 0;i<100 && (visitedBlock.size()/allBlock.size())< 1 ; i++)
  {
    for(auto &F : *theModule)
    {
      if(strncmp(F.getName().str().c_str(),"main",4) == 0)
      {
        llvm::outs()<<"\n------------------------------------------------------\n";

        std::unordered_map<std::string,LimitedVariable*>  theOperands;
        BasicBlock* BB = dyn_cast<BasicBlock>(F.begin());
        int number_of_block_visisted_before_new_run = visitedBlock.size();
        generatePath(BB,&theOperands);
        print_a_operand(&theOperands);
        if (number_of_block_visisted_before_new_run < visitedBlock.size())
        {
          operrand_with_unique_path.push_back(theOperands);
        }
        //printRegisters(&theOperands);
      }
    }
    llvm::outs()<<"__________END_OF_PATH__________\n\n\n";
  }
  printBlockResult(i);
  llvm::outs()<<"\n----------------------------------------------------search end------------------------------------------------\n\n\n";
}


void search_task1(std::shared_ptr<Module> theModule){
  srand(time(NULL));
  llvm::outs()<<"\n\n\n----------------------------------------------------search start------------------------------------------------\n";
  int i;
  for( i = 0;i<100 && (visitedBlock.size()/allBlock.size())< 1 ; i++)
  {
    for(auto &F : *theModule)
    {
      if(strncmp(F.getName().str().c_str(),"main",4) == 0)
      {
        llvm::outs()<<"\n------------------------------------------------------\n";

        BasicBlock* BB = dyn_cast<BasicBlock>(F.begin());
        generatePath_task1(BB);
        
      }
    }
    llvm::outs()<<"__________END_OF_PATH__________\n\n\n";
  }
  printBlockResult(i);
  llvm::outs()<<"\n----------------------------------------------------search end------------------------------------------------\n\n\n";
}


void search_DSE(std::shared_ptr<Module> theModule){
srand(time(NULL));
  llvm::outs()<<"\n\n\n----------------------------------------------------search start------------------------------------------------\n";
  
  std::unordered_map<std::string,LimitedVariable*>  help;
  DSE* dse = new DSE();
  BasicBlock* fbb = firstBasicBlock(theModule);
  std::set<std::string> visitedBB;
  getAllCompare(fbb,dse,&help,&visitedBB);
  dse->llvm_print();
  for(int i = 0;i< dse->maximumInput() ; i++)
  {
    for(auto &F : *theModule)
    {
      if(strncmp(F.getName().str().c_str(),"main",4) == 0)
      {
        llvm::outs()<<"\n----------------------begin--------------------------\n";

        std::unordered_map<std::string,LimitedVariable*>  theOperands;
        BasicBlock* BB = dyn_cast<BasicBlock>(F.begin());
        llvm::outs()<< " minterm :"<< i<<"\n";

        if(nullptr==dse->getRegisters(i,&theOperands)){
          llvm::outs()<< " we can not create this "<<"\n";
          continue;
        }
        generatePath(BB,&theOperands);
        print_a_operand(&theOperands);
        
        //printRegisters(&theOperands);
      }
    }
    llvm::outs()<<"__________END_OF_PATH__________\n\n\n";
  }
  printBlockResult(0);
  llvm::outs()<<"\n----------------------------------------------------search end------------------------------------------------\n\n\n";

}

BasicBlock* firstBasicBlock(std::shared_ptr<Module> theModule){
   for(auto &F : *theModule)
    {
      if(strncmp(F.getName().str().c_str(),"main",4) == 0)
      {
        
        BasicBlock* BB = dyn_cast<BasicBlock>(F.begin());
        return BB;
      }
    }
    return NULL;
}


DSE* getAllCompare(BasicBlock* BB ,DSE* compare,std::unordered_map<std::string,LimitedVariable*> *oprand ,
std::set<std::string> *visitedBB){
  

  visitedBB->insert(BB->getName());
  
  const Instruction *TInst = BB->getTerminator();
  unsigned NSucc = TInst->getNumSuccessors();
  if (NSucc == 1){

      BasicBlock *Succ = TInst->getSuccessor(0);
      if(visitedBB->find( Succ->getName()) == visitedBB->end()){

        getAllCompare(Succ,compare,oprand,visitedBB);
      }
      
      return compare ;
  }else if (NSucc == 2){
    std::string leftSide;
    std::string rightSide = "not";
    for(auto &ins:*BB){
    std::string opCode = ins.getOpcodeName();
    if(ins.getOpcode() == LOAD){

      if(!isAOperand(getSimpleNodeLabelOperand(ins.getOperand(0))))
        continue;

      leftSide = getLeftSideOperand(&ins);
      rightSide = getSimpleNodeLabelOperand(ins.getOperand(0)) ;
      

    }

    else if (ins.getOpcode() == ICMP)
      {
        if ( rightSide == "not"  )
        {
          break;
        }
        if (leftSide == getSimpleNodeLabelOperand(ins.getOperand(0)))
        {
          LimitedVariable* rightside_1_Val = valueOfOperand_DSE(oprand,rightSide);
          std::string rightSide2 = getSimpleNodeLabelOperand(ins.getOperand(1)) ;
          LimitedVariable* rightside_2_Val = valueOfOperand_DSE(oprand,rightSide2);
          std::string function_ICMP = llvm_ICMP_function(&ins);
          compare->addNewComapers(rightSide,rightside_2_Val->getValueInRange(),function_ICMP);
        }else if(leftSide == getSimpleNodeLabelOperand(ins.getOperand(1)))
        {
          std::string rightSide1 = getSimpleNodeLabelOperand(ins.getOperand(0)) ;
          LimitedVariable* rightside_1_Val = valueOfOperand_DSE(oprand,rightSide1);
          LimitedVariable* rightside_2_Val = valueOfOperand_DSE(oprand,rightSide);
          std::string function_ICMP = llvm_ICMP_function(&ins);
          compare->addNewComapers(rightSide,rightside_1_Val->getValueInRange(),llvm_ICMP_switch_mode(function_ICMP));

        }
        
        
       

        break;
      }else{
        rightSide = "not";
      }
    }

    BasicBlock *Succ = TInst->getSuccessor(0);
    if(visitedBB->find( Succ->getName()) == visitedBB->end()){
      getAllCompare(Succ,compare,oprand,visitedBB);
    }
    
    Succ = TInst->getSuccessor(1);
    if(visitedBB->find( Succ->getName()) == visitedBB->end()){
      getAllCompare(Succ,compare,oprand,visitedBB);
    }
    
    return compare;
    
  }else{

    return compare;
  }
  
}


void printBlockResult(int numberOfSearch)
{

  if(numberOfSearch != 0){
    errs()<<"totla number of search : "<< numberOfSearch<<"\n";}
  errs()<<"totla number of blocks : "<< allBlock.size()<<"\n";
  errs()<<"totla number of visited blocks : "<< visitedBlock.size()<<"\n";
  errs()<<"coverage : "<< static_cast<int>( ((float) visitedBlock.size() * 100 ) / ((float)allBlock.size()))<<"\n";
  if(MODE == 0){
  print_all_unique_path(&operrand_with_unique_path);}

}
void addAllBlock(std::shared_ptr<Module> theModule){
   for(auto &F : *theModule)
    {
      for (auto &BB:F)
      {
        allBlock.insert(std::pair<std::string,BasicBlock*>(F.getName(),&BB));
        
      }
      
    }
}









void generatePath(BasicBlock* BB,std::unordered_map<std::string,LimitedVariable*>  * operands)
{
  visitedBlock.insert(std::pair<std::string,BasicBlock*>(BB->getParent()->getName(),BB));

  const Instruction* lastInst =  BB->getTerminator();

  unsigned numberOfSuccesor = lastInst->getNumSuccessors();
  BasicBlock *Successor;

  llvmBasciBlockPrint(BB);

  if(numberOfSuccesor == 1){
    basicBlockEval(BB,operands,true);

    Successor = lastInst->getSuccessor(0);

    generatePath(Successor,operands);
    
  }else if(numberOfSuccesor == 2){

    unsigned ran = std::rand();
    unsigned select_succ = ran / (RAND_MAX/numberOfSuccesor);
    Successor = lastInst->getSuccessor(select_succ);

    
    std::string functionName = lastInst->getSuccessor(0)->getParent()->getName();
    if(visitedBlock.find(std::pair<std::string,BasicBlock*>(functionName,Successor)) != visitedBlock.end()){
      select_succ = numberOfSuccesor - select_succ -1;
    }

    
    select_succ = basicBlockEval(BB,operands,(select_succ == 1)?false:true);
    Successor = lastInst->getSuccessor(select_succ);

    
    generatePath(Successor,operands);
  }else{
    basicBlockEval(BB,operands,true);
  }
  
  
}


void generatePath_task1(BasicBlock* BB)
{
  visitedBlock.insert(std::pair<std::string,BasicBlock*>(BB->getParent()->getName(),BB));
  llvmBasciBlockPrint(BB);
  const Instruction *TInst = BB->getTerminator();
  unsigned NSucc = TInst->getNumSuccessors();
  if (NSucc == 1){
      BasicBlock *Succ = TInst->getSuccessor(0);
      generatePath_task1(Succ);
  }else if (NSucc>1){
      srand(time(NULL));
      unsigned rnd = std::rand() / (RAND_MAX/NSucc); 
      BasicBlock *Succ = TInst->getSuccessor(rnd);
      generatePath_task1(Succ);
  }
}

