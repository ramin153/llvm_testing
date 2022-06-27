
#include "DSE.h"

Compare::Compare(int valueI,std::string registerNameI,std::string comparetorI){
         
        registerName = registerNameI;
        value = valueI;
        comparetor = comparetorI;
         
     }
void Compare::llvm_print(){
        llvm::outs()<< "{ "<<this->registerName<<" "<<this->comparetor <<" " <<this->value << " }\n";

}

void DSE::llvm_print(){
    llvm::outs()<< "size: "<<this->compares.size()<< " \n";
    for (auto& t : this->compares)   
        t->llvm_print();
   
}
bool DSE::addNewComapers(std::string registerName,int value, std::string comparetor)
{
    compares.push_back(new Compare(value,registerName,comparetor));
    return true;
}
int DSE::maximumInput()
{

    return pow(2,compares.size());

}
std::unordered_map<std::string,LimitedVariable*>* DSE::getRegisters(int minTerm,
                        std::unordered_map<std::string,LimitedVariable*>*  theOperands)
{
    int size = compares.size();
    int minTermCheck = 1;
    for (int i = 0; i < size; i++)
    {
        Compare* cmp = compares.at(i);

        if (!this->addNewRestrict (theOperands,cmp,(minTermCheck & minTerm) != 0)){

            return nullptr;
        }
        
        minTermCheck *=2;
        
    }
    llvm::outs()<<"goal\n";
    print_a_operand(theOperands);

    return theOperands;
    
}

 bool DSE::addNewRestrict(std::unordered_map<std::string,LimitedVariable*> *oprand,
                Compare* cmp,bool isIn){

        std::string comparetor = cmp->comparetor;
        std::string reg = cmp->registerName;
        int val = cmp->value;
        LimitedVariable *  limVar ; 
        if (oprand->find(reg) == oprand->end()){
            limVar = LimitedVariable::createLimitedVariableInRange();}
        else{
            limVar = oprand->find(reg)->second;
            }

        
        if ( (comparetor == "slt" && isIn) || (comparetor == "sge" && !isIn))
        {
            if (! limVar->isItValidToresticMax(val -1))
            {

                return false;
            }
            limVar->restrictMax(val-1);
            
        }else if ( (comparetor == "sge" && isIn) || (comparetor == "slt" && !isIn))
        {
             if (! limVar->isItValidToresticMin(val ))
            {
                return false;
            }
            limVar->restrictMin(val);

        }else if ( (comparetor == "sle" && isIn) || (comparetor == "sgt" && !isIn))
        {
             if (! limVar->isItValidToresticMax(val))
            {
                return false;
            }
            limVar->restrictMax(val);
        }else
        {

             if (! limVar->isItValidToresticMin(val + 1))
            {

                return false;
            }
            limVar->restrictMin(val+ 1);
        }
        limVar->setIsCloseToAllocate(true);
        insertUnorderList(oprand,reg,limVar);
        return true;


}