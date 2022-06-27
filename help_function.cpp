//Reihan flhi 9732493
//ramin rowshan 9732491
#include "help_function.h"


void llvmBasciBlockPrint(BasicBlock *BB){
  llvm::outs() << getSimpleNodeLabel(BB) << "\n";
}

std::string getSimpleNodeLabel(const BasicBlock *Node) {
    if (!Node->getName().empty())
        return Node->getName().str();
    std::string Str;
    raw_string_ostream OS(Str);
    Node->printAsOperand(OS, false);
    return OS.str();
}


std::string getSimpleNodeLabelOperand(const Value *Node) {
    if (!Node->getName().empty())
        return Node->getName().str();
    std::string Str;
    raw_string_ostream OS(Str);
    Node->printAsOperand(OS, false);
    return OS.str();
}

//
std::string getLeftSideOperand(Instruction* ins){
  std::string leftSide;
  llvm::raw_string_ostream(leftSide)<<*ins;
  leftSide =  SplitString(leftSide," ")[0];
  return leftSide;
}


LimitedVariable* getValueOfRegister(std::unordered_map<std::string,LimitedVariable*> *operands,std::string registerName){
    std::string help = remove_begining_percent(registerName);


    for(auto& op : *operands) {
        if(op.first.compare(help) == 0 )
            return op.second;
        } 
    throw std::invalid_argument( "twe can not find it." );

  
}

LimitedVariable* getValueOfRegister_DSE(std::unordered_map<std::string,LimitedVariable*> *operands,std::string registerName){
    std::string help = remove_begining_percent(registerName);


    for(auto& op : *operands) {
        if(op.first.compare(help) == 0 )
            return op.second;
        } 
    return LimitedVariable::createLimitedVariableInRange();

  
}

void printRegisters(std::unordered_map<std::string,LimitedVariable*> *operands){
    llvm::outs()<<"---------------------------------------------"<<"---size---"<<operands->size()<< "\n";
    for(auto& op : *operands) 
     llvm::outs() <<op.first  <<" "<<op.second->toString()<<"\n";
    

}

bool isAOperand(std::string var){
     return (var.at(0) == 'a' && (var.size()==1||is_number(var.substr(1,var.size()))));
}
void print_a_operand(std::unordered_map<std::string,LimitedVariable*> *operands){
    for(auto& op : *operands) 
    if(isAOperand(op.first)){
     llvm::outs() <<op.first  <<" { "<< op.second->getMax() <<":"<< op.second->getMin() <<"} " <<op.second->getValueInRange()<<"\n";}
     llvm::outs()<<"\n\n\n";
}

void print_all_unique_path(std::vector<std::unordered_map<std::string,LimitedVariable*>> *paths){
    llvm::outs()<<"all unique path created by :\n\n";
    for (auto& path : *paths)
    {
        print_a_operand(&path);
    }
    llvm::outs()<<"end ,all unique path \n\n";
    
}


LimitedVariable* valueOfOperand(std::unordered_map<std::string,LimitedVariable*> *operands,std::string oper){
  if(!is_number(oper)){
    return getValueOfRegister(operands,oper);
  }else{

    int val = std::stoi(oper);
    return new LimitedVariable(val,val) ;
  }
}


LimitedVariable* valueOfOperand_DSE(std::unordered_map<std::string,LimitedVariable*> *operands,std::string oper){
  if(!is_number(oper)){
    return getValueOfRegister_DSE(operands,oper);
  }else{

    int val = std::stoi(oper);
    return new LimitedVariable(val,val) ;
  }
}


//

void insertUnorderList(std::unordered_map<std::string,LimitedVariable*> *operands,std::string leftSide,LimitedVariable* value){
    std::string help = remove_begining_percent(leftSide);

    for (auto& it: *operands) {
        if( it.first == help){
            it.second = value;
            return;
        }
    }
    operands->insert(std::pair<std::string,LimitedVariable*>(help,value));
}

void insertUnorderList_allocate(std::unordered_map<std::string,LimitedVariable*> *operands,std::string leftSide,LimitedVariable* value){
    std::string help = remove_begining_percent(leftSide);

    for (auto& it: *operands) {
        if( it.first == help){
            if(!it.second->getIsCloseToAllocate()){
                it.second = value;
                }
            return;
        }
    }
    operands->insert(std::pair<std::string,LimitedVariable*>(help,value));
}

void insertUnorderList(std::unordered_map<std::string,LimitedVariable*> *operands,std::string leftSide,int value){
    std::string help = remove_begining_percent(leftSide);

    for (auto& it: *operands) {
        if( it.first == help){
            it.second = new LimitedVariable(value,value);
            return;
        }
    }
    operands->insert(std::pair<std::string,LimitedVariable*>(help, new LimitedVariable(value,value)));

}

void llvm_add(Instruction* ins,std::unordered_map<std::string,LimitedVariable*> *operands){

  std::string leftSide = getLeftSideOperand(ins);
  std::string rightSide1 = getSimpleNodeLabelOperand(ins->getOperand(0)) ;
  std::string rightSide2 = getSimpleNodeLabelOperand(ins->getOperand(1)) ;
  insertUnorderList(operands,leftSide,valueOfOperand(operands,rightSide1)->addAndNew(valueOfOperand(operands,rightSide2)) );
}


void llvm_sub(Instruction* ins,std::unordered_map<std::string,LimitedVariable*> *operands){

   std::string leftSide = getLeftSideOperand(ins);
   std::string rightSide1 = getSimpleNodeLabelOperand(ins->getOperand(0)) ;
   std::string rightSide2 = getSimpleNodeLabelOperand(ins->getOperand(1)) ;

  insertUnorderList(operands,leftSide,valueOfOperand(operands,rightSide1)->subAndNew(valueOfOperand(operands,rightSide2)));
}


void llvm_mult(Instruction* ins,std::unordered_map<std::string,LimitedVariable*> *operands){

    std::string leftSide = getLeftSideOperand(ins);
    std::string rightSide1 = getSimpleNodeLabelOperand(ins->getOperand(0)) ;
    std::string rightSide2 = getSimpleNodeLabelOperand(ins->getOperand(1)) ;

  insertUnorderList(operands,leftSide,valueOfOperand(operands,rightSide1)->multAndNew(valueOfOperand(operands,rightSide2)) );
}

void llvm_div(Instruction* ins,std::unordered_map<std::string,LimitedVariable*> *operands){

    std::string leftSide = getLeftSideOperand(ins);
    std::string rightSide1 = getSimpleNodeLabelOperand(ins->getOperand(0)) ;
    std::string rightSide2 = getSimpleNodeLabelOperand(ins->getOperand(1)) ;

  insertUnorderList(operands,leftSide,valueOfOperand(operands,rightSide1)->divAndNew(valueOfOperand(operands,rightSide2)) );
}


int llvm_branch(Instruction* ins,std::unordered_map<std::string,LimitedVariable*> *operands){

    if(ins->getNumOperands() == 1){
        
        return 0;
    }
    std::string rightSide = getSimpleNodeLabelOperand(ins->getOperand(0)) ;
    
    return valueOfOperand(operands,rightSide)->getMin();
}

void llvm_alocate(Instruction* ins,std::unordered_map<std::string,LimitedVariable*> *operands){
    std::string leftSide = getLeftSideOperand(ins);
    std::string rightSide = getSimpleNodeLabelOperand(ins->getOperand(0));

    insertUnorderList_allocate(operands,leftSide,LimitedVariable::createLimitedVariableInRange());
}

void llvm_load(Instruction* ins,std::unordered_map<std::string,LimitedVariable*> *operands){
    std::string leftSide = getLeftSideOperand(ins);
    std::string rightSide = getSimpleNodeLabelOperand(ins->getOperand(0)) ;
    insertUnorderList(operands,leftSide,valueOfOperand(operands,rightSide)->copyVariable());
    
  
}

void llvm_store(Instruction* ins,std::unordered_map<std::string,LimitedVariable*> *operands){
  
    std::string leftSide = getSimpleNodeLabelOperand(ins->getOperand(1));
    std::string rightSide = getSimpleNodeLabelOperand(ins->getOperand(0));
    
    insertUnorderList(operands,leftSide,valueOfOperand(operands,rightSide)->copyVariable());
  
}

int LLVM_ICMP_sle(std::unordered_map<std::string,LimitedVariable*> *operands,
                std::string leftSide,LimitedVariable* left_side_op,LimitedVariable* right_side_op,
                int accept,int regect){
    if(left_side_op->isItValidToresticMax(right_side_op->getMax())){
        left_side_op->restrictMax(right_side_op->getMax());
        insertUnorderList(operands,leftSide,accept);

    }else if(left_side_op->isItValidToresticMin(right_side_op->getMin()+1)) {
        left_side_op->restrictMin(right_side_op->getMin()+1);
        insertUnorderList(operands,leftSide,regect);

    }else{
        return ERROR_INVALID_RANGE;
    }
    return ACCEPTED_RANGE;
}

int LLVM_ICMP_slt(std::unordered_map<std::string,LimitedVariable*> *operands,
                std::string leftSide,LimitedVariable* left_side_op,LimitedVariable* right_side_op,
                int accept,int regect){
    if(left_side_op->isItValidToresticMax(right_side_op->getMax()-1)){
        left_side_op->restrictMax(right_side_op->getMax()-1);
        insertUnorderList(operands,leftSide,accept);
    }else if(left_side_op->isItValidToresticMin(right_side_op->getMin())) {
        left_side_op->restrictMin(right_side_op->getMin());
        insertUnorderList(operands,leftSide,regect);
    }else{
        return ERROR_INVALID_RANGE;
    }
    return ACCEPTED_RANGE;
}

int LLVM_ICMP_sgt(std::unordered_map<std::string,LimitedVariable*> *operands,
                std::string leftSide,LimitedVariable* left_side_op,LimitedVariable* right_side_op,
                int accept,int regect){
    if(left_side_op->isItValidToresticMin(right_side_op->getMin()+1)){
        left_side_op->restrictMin(right_side_op->getMin()+1);
        insertUnorderList(operands,leftSide,accept);
    }else if(left_side_op->isItValidToresticMax(right_side_op->getMax())) {
        left_side_op->restrictMax(right_side_op->getMax());
        insertUnorderList(operands,leftSide,regect);
    }else{
        return ERROR_INVALID_RANGE;
    }
    return ACCEPTED_RANGE;
}


int LLVM_ICMP_sge(std::unordered_map<std::string,LimitedVariable*> *operands,
                std::string leftSide,LimitedVariable* left_side_op,LimitedVariable* right_side_op,
                int accept,int regect){
     if( left_side_op->isItValidToresticMin(right_side_op->getMin() )){
        left_side_op->restrictMin(right_side_op->getMin());
        insertUnorderList(operands,leftSide,accept);
    }else if(left_side_op->isItValidToresticMax(right_side_op->getMax()-1)) {
        left_side_op->restrictMax(right_side_op->getMax()-1);
        insertUnorderList(operands,leftSide,regect);

    }else{
        return ERROR_INVALID_RANGE;
    }
    return ACCEPTED_RANGE;
}


int llvm_ICMP(Instruction* ins,std::unordered_map<std::string,LimitedVariable*> *operands,bool isAccepted){
    std::string leftSide = getLeftSideOperand(ins);

    std::string rightSide1 = getSimpleNodeLabelOperand(ins->getOperand(0)) ;
    LimitedVariable* rightside_1_Val = valueOfOperand(operands,rightSide1);

    std::string rightSide2 = getSimpleNodeLabelOperand(ins->getOperand(1)) ;
    LimitedVariable* rightside_2_Val = valueOfOperand(operands,rightSide2);

    std::string function_ICMP = llvm_ICMP_function(ins);
    if(rightside_1_Val->isItAbsoulte() && rightside_2_Val->isItAbsoulte()){
        llvm_ICMP_rightSide_NormalComaper(isAccepted,operands,leftSide,rightside_1_Val,rightside_2_Val,function_ICMP);
        return ACCEPTED_RANGE;
    }else if(rightside_2_Val->isItAbsoulte()){

        return llvm_ICMP_rightSide_check(isAccepted,operands,leftSide,rightside_1_Val,rightside_2_Val,function_ICMP);
    }else{

        return llvm_ICMP_rightSide_check(isAccepted,operands,leftSide,rightside_2_Val,rightside_1_Val,llvm_ICMP_switch_mode(function_ICMP));
    }

    

}
std::string llvm_ICMP_switch_mode(std::string operand){
    if(operand ==  "slt")
        return"sge";
    else if(operand ==  "sge")
        return"slt";
    else if(operand ==  "sle")
        return"sgt";
    else 
        return"sle";
}
void llvm_ICMP_rightSide_NormalComaper(bool isAccepted,std::unordered_map<std::string,LimitedVariable*> *operands,std::string leftSide,
                            LimitedVariable* rightside_1_Val,LimitedVariable* rightside_2_Val,std::string function_ICMP){
    
    
    int rightOp = rightside_1_Val->getMin();
    int leftOp = rightside_2_Val->getMin();

    if(function_ICMP == "slt"){
        if(isAccepted){
            //llvm::outs()<<"slt ac "<< ((rightOp < leftOp)?BRANCH_ACCEPT:BRANCH_REGECT) <<" \n";

            insertUnorderList(operands,leftSide,(rightOp < leftOp)?BRANCH_ACCEPT:BRANCH_REGECT);
        }
        else{
            //llvm::outs()<<"slt not ac "<< ((rightOp >= leftOp)?BRANCH_ACCEPT:BRANCH_REGECT) <<" \n";

            insertUnorderList(operands,leftSide,(rightOp >= leftOp)?BRANCH_REGECT:BRANCH_ACCEPT);
            }
    }else if (function_ICMP == "sle"){
        if(isAccepted){
            //llvm::outs()<<"sle ac "<< ((rightOp <= leftOp)?BRANCH_ACCEPT:BRANCH_REGECT) <<" \n";

            insertUnorderList(operands,leftSide,(rightOp <= leftOp)?BRANCH_ACCEPT:BRANCH_REGECT);}
        else{
            //llvm::outs()<<"sle not ac "<< ((rightOp > leftOp)?BRANCH_ACCEPT:BRANCH_REGECT) <<" \n";

            insertUnorderList(operands,leftSide,(rightOp > leftOp)?BRANCH_REGECT:BRANCH_ACCEPT);}
    }else if (function_ICMP == "sge"){
         if(isAccepted)
            {
            //llvm::outs()<<"sge  ac "<< ((rightOp >= leftOp)?BRANCH_ACCEPT:BRANCH_REGECT) <<" \n";

            insertUnorderList(operands,leftSide,(rightOp >= leftOp)?BRANCH_ACCEPT:BRANCH_REGECT);}
        else
            {
            //llvm::outs()<<"sge not  ac "<< ((rightOp < leftOp)?BRANCH_ACCEPT:BRANCH_REGECT) <<" \n";

            insertUnorderList(operands,leftSide,(rightOp < leftOp)?BRANCH_REGECT:BRANCH_ACCEPT);}
    }else if (function_ICMP == "sgt"){
       if(isAccepted)
            {
            //llvm::outs()<<"sgt   ac "<< ((rightOp > leftOp)?BRANCH_ACCEPT:BRANCH_REGECT) <<" \n";
            insertUnorderList(operands,leftSide,(rightOp > leftOp)?BRANCH_ACCEPT:BRANCH_REGECT);}
        else
            {
            //llvm::outs()<<"sgt not  ac "<<rightOp<<"<= "<<leftOp<<" " <<((rightOp <= leftOp)?BRANCH_REGECT:BRANCH_ACCEPT) <<" \n";
            insertUnorderList(operands,leftSide,(rightOp <= leftOp)?BRANCH_REGECT:BRANCH_ACCEPT);}
    }

}
int llvm_ICMP_rightSide_check(bool isAccepted,std::unordered_map<std::string,LimitedVariable*> *operands,std::string leftSide,
                            LimitedVariable* rightside_1_Val,LimitedVariable* rightside_2_Val,std::string function_ICMP){
    int resultCode = ERROR_INVALID_RANGE ;
    if(function_ICMP == "slt"){
        if(isAccepted)
            resultCode = LLVM_ICMP_slt(operands,leftSide,rightside_1_Val,rightside_2_Val,BRANCH_ACCEPT,BRANCH_REGECT);
        else
            resultCode = LLVM_ICMP_sge(operands,leftSide,rightside_1_Val,rightside_2_Val,BRANCH_REGECT,BRANCH_ACCEPT);
    }else if (function_ICMP == "sle"){
         if(isAccepted){
            resultCode = LLVM_ICMP_sle(operands,leftSide,rightside_1_Val,rightside_2_Val,BRANCH_ACCEPT,BRANCH_REGECT);
            }
        else
            {
                resultCode = LLVM_ICMP_sgt(operands,leftSide,rightside_1_Val,rightside_2_Val,BRANCH_REGECT,BRANCH_ACCEPT);
                }
    }else if (function_ICMP == "sge"){
         if(isAccepted)
            resultCode = LLVM_ICMP_sge(operands,leftSide,rightside_1_Val,rightside_2_Val,BRANCH_ACCEPT,BRANCH_REGECT);
        else
            resultCode = LLVM_ICMP_slt(operands,leftSide,rightside_1_Val,rightside_2_Val,BRANCH_REGECT,BRANCH_ACCEPT);
    }else if (function_ICMP == "sgt"){
       if(isAccepted)
            resultCode = LLVM_ICMP_sgt(operands,leftSide,rightside_1_Val,rightside_2_Val,BRANCH_ACCEPT,BRANCH_REGECT);
        else
            resultCode = LLVM_ICMP_sle(operands,leftSide,rightside_1_Val,rightside_2_Val,BRANCH_REGECT,BRANCH_ACCEPT);
    }

    return resultCode;
}

std::string llvm_ICMP_function(Instruction* ins){
  std::string function_ICMP;
  llvm::raw_string_ostream(function_ICMP)<<*ins;
  function_ICMP =  SplitString(function_ICMP," ")[3];
  return function_ICMP;
}

int basicBlockEval(BasicBlock* BB,std::unordered_map<std::string,LimitedVariable*> *operands,bool isAccepted){
  for(auto &ins:*BB){
    std::string opCode = ins.getOpcodeName();

    switch (ins.getOpcode())
    {
    case ADD:
      //llvm::outs()<<ins <<" " << opCode<< " "<< ins.getOpcode () << "|| " << ins.getNumOperands() <<" is add \n";
      llvm_add(&ins,operands);
      break;

    case SUBTRACT:
      //llvm::outs()<<ins <<" " << opCode<< " "<< ins.getOpcode () << "|| " << ins.getNumOperands() <<" is SUBTRACT \n";
      llvm_sub(&ins,operands);
      break;

    case LOAD:
      //llvm::outs()<<ins <<" " << opCode<< " "<< ins.getOpcode () <<" ||" << ins.getNumOperands() <<" is LOAD \n";
      llvm_load(&ins,operands);
      break;

    case STORE:
      //llvm::outs()<<ins <<" " << opCode<< " "<< ins.getOpcode () <<" ||" << ins.getNumOperands() <<" is STORE \n";
      llvm_store(&ins,operands);
      break;

    case ICMP:
      //llvm::outs()<<ins <<" " << opCode<< " "<< ins.getOpcode () <<" ||" << ins.getNumOperands() <<" is ICMP \n";
      if(llvm_ICMP(&ins,operands,isAccepted) == ERROR_INVALID_RANGE){
          return ERROR_INVALID_RANGE;
      }
      break;

    case MULTIPLAY:
      //llvm::outs()<<ins <<" " << opCode<< " "<< ins.getOpcode () <<" ||" << ins.getNumOperands() <<" is MULTIPLAY \n";
      llvm_mult(&ins,operands);
      break;

    case SIGN_DIVISION:
      //llvm::outs()<<ins <<" " << opCode<< " "<< ins.getOpcode () <<"|| " << ins.getNumOperands() <<" is SIGN_DIVISION \n";
      llvm_div(&ins,operands);
      break;

    case ALLOCATE:
      //llvm::outs()<<ins <<" " << opCode<< " "<< ins.getOpcode () <<" ||" << ins.getNumOperands() <<" is ALLOCATE \n";
      llvm_alocate(&ins,operands);
      break;

    case BR:
      //llvm::outs()<<ins <<" " << opCode<< " "<< ins.getOpcode () <<"|| " << ins.getNumOperands() <<" is BR \n";
      return llvm_branch(&ins,operands);
      break;

    case RETURN:
      //llvm::outs()<<ins <<" " << opCode<< " "<< ins.getOpcode () <<"|| " << ins.getNumOperands() <<" is RETURN \n";
      return RETURN_APP;
      break;
    
    default:
    //llvm::outs()<<ins <<" " << opCode<< " "<< ins.getOpcode () <<"  there is a problem , we didnt add such opcode  \n";
      break;
    }

    //printRegisters(operands);
    

  }
  return 0;
}



//
std::vector<std::string> SplitString(std::string str,std::string delimeter)
{
    std::vector<std::string> splittedStrings = {};
    size_t pos = 0;

    while ((pos = str.find(delimeter)) != std::string::npos)
    {
        std::string token = str.substr(0, pos);
        if (token.length() > 0)
            splittedStrings.push_back(token);
        str.erase(0, pos + delimeter.length());
    }

    if (str.length() > 0)
        splittedStrings.push_back(str);
    return splittedStrings;
}

bool is_number(const std::string& s)
{
    std::string::const_iterator it = s.begin();
    if(*it =='-' || *it == '+') ++it;
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

std::string remove_begining_percent(std::string str)
{
  if(str.at(0) == '%')
    return str.substr(1,str.size());
  return str;
}