//Reihan flhi 9732493
//ramin rowshan 9732491
#ifndef help_function_h


#define help_function_h





#define LOAD 32
#define ICMP 53
#define BR 2
#define ADD 13
#define STORE 33
#define SUBTRACT 15
#define ALLOCATE 31
#define MULTIPLAY 17
#define SIGN_DIVISION 20
#define RETURN 1

#define ERROR_INVALID_RANGE 3
#define ACCEPTED_RANGE 2

#define RETURN_APP 4

#define BRANCH_ACCEPT 0
#define BRANCH_REGECT 1

#include <stdlib.h>
#include <stdio.h>
#include <sstream>
#include <iostream>
#include <random>
#include <set>
#include <unordered_map>
#include <list>

#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/raw_ostream.h"

#include "LimitedVariable.h"

using namespace llvm;

void llvmBasciBlockPrint(BasicBlock *Node);
void print_a_operand(std::unordered_map<std::string,LimitedVariable*> *operands);
std::string getSimpleNodeLabelOperand(const Value *Node);
void print_all_unique_path(std::vector<std::unordered_map<std::string,LimitedVariable*>> *paths);
std::string getSimpleNodeLabel(const BasicBlock *Node);

//



std::string getLeftSideOperand(Instruction* ins);

LimitedVariable* getValueOfRegister(std::unordered_map<std::string,LimitedVariable*> *operands,std::string registerName);

LimitedVariable* valueOfOperand(std::unordered_map<std::string,LimitedVariable*> *operands,std::string oper);
void printRegisters(std::unordered_map<std::string,LimitedVariable*> *operands);
//
void insertUnorderList(std::unordered_map<std::string,LimitedVariable*> *operands,std::string leftSIde,LimitedVariable* value);
void insertUnorderList(std::unordered_map<std::string,LimitedVariable*> *operands,std::string leftSide,int value);
void insertUnorderList_allocate(std::unordered_map<std::string,LimitedVariable*> *operands,std::string leftSide,LimitedVariable* value);



int basicBlockEval(BasicBlock* BB,std::unordered_map<std::string,LimitedVariable*> *operands,bool isAccepted);
void llvm_store(Instruction* ins,std::unordered_map<std::string,LimitedVariable*> *operands);
void llvm_load(Instruction* ins,std::unordered_map<std::string,LimitedVariable*> *operands);
void llvm_add(Instruction* ins,std::unordered_map<std::string,LimitedVariable*> *operands);
void llvm_sub(Instruction* ins,std::unordered_map<std::string,LimitedVariable*> *operands);
void llvm_mult(Instruction* ins,std::unordered_map<std::string,LimitedVariable*> *operands);
void llvm_div(Instruction* ins,std::unordered_map<std::string,LimitedVariable*> *operands);
int llvm_branch(Instruction* ins,std::unordered_map<std::string,LimitedVariable*> *operands);

std::string llvm_ICMP_function(Instruction* ins);
int llvm_ICMP(Instruction* ins,std::unordered_map<std::string,LimitedVariable*> *operands,bool isAccepted);
int llvm_ICMP_rightSide_check(bool isAccepted,std::unordered_map<std::string,LimitedVariable*> *operands,std::string leftSide,
                            LimitedVariable* rightside_1_Val,LimitedVariable* rightside_2_Val,std::string function_ICMP);
void llvm_ICMP_rightSide_NormalComaper(bool isAccepted,std::unordered_map<std::string,LimitedVariable*> *operands,std::string leftSide,
LimitedVariable* rightside_1_Val,LimitedVariable* rightside_2_Val,std::string function_ICMP);
int LLVM_ICMP_sle(std::unordered_map<std::string,LimitedVariable*> *operands,
                std::string leftSide,LimitedVariable* left_side_op,LimitedVariable* right_side_op,
                int accept,int regect);
int LLVM_ICMP_slt(std::unordered_map<std::string,LimitedVariable*> *operands,
                std::string leftSide,LimitedVariable* left_side_op,LimitedVariable* right_side_op,
                int accept,int regect);
int LLVM_ICMP_sge(std::unordered_map<std::string,LimitedVariable*> *operands,
                std::string leftSide,LimitedVariable* left_side_op,LimitedVariable* right_side_op,
                int accept,int regect);
int LLVM_ICMP_sgt(std::unordered_map<std::string,LimitedVariable*> *operands,
                std::string leftSide,LimitedVariable* left_side_op,LimitedVariable* right_side_op,
                int accept,int regect);

std::string llvm_ICMP_switch_mode(std::string operand);
 //
bool is_number(const std::string& s);

std::vector<std::string> SplitString(std::string str,std::string delimeter);

std::string remove_begining_percent(std::string str);


LimitedVariable* valueOfOperand_DSE(std::unordered_map<std::string,LimitedVariable*> *operands,std::string oper);
LimitedVariable* getValueOfRegister_DSE(std::unordered_map<std::string,LimitedVariable*> *operands,std::string registerName);
bool isAOperand(std::string var);
#endif