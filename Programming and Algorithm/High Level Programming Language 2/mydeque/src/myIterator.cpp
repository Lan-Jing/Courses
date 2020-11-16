#include "myIterator.hpp"


myIterator& myIterator::operator = (const myIterator &iter){
    this->setPtr(iter.showPtr());
    this->setDequePos(iter.showDequePos());
    this->setBlockPos(iter.showBlockPos());
    this->setMode(iter.showMode());
    return (*this);
}
bool myIterator::operator == (const myIterator &iter){
    return this->ptr == iter.ptr && 
           this->showDequePos() == iter.showDequePos() &&
           this->showBlockPos() == iter.showBlockPos() &&
           this->showMode() == iter.showMode();
}
bool myIterator::operator != (const myIterator &iter){
    return !((*this) == iter);
}
myIterator& myIterator::operator ++ (){
    if(this->showMode()){
        if(!this->showBlockPos()){
            this->setBlockPos(blockLength - 1);
            this->setDequePos(this->showDequePos() - 1);
        }else{
            this->setBlockPos(this->showBlockPos() - 1);
        }
    }else{
        if(this->showBlockPos() == blockLength - 1){
            this->setBlockPos(0); 
            this->setDequePos(this->showDequePos() + 1);
        }else{
            this->setBlockPos(this->showBlockPos() + 1);
        }
    }
    return (*this);
}
myIterator& myIterator::operator ++ (int){
//    myIterator& result = (*this);
    myIterator result(NULL,0,0,0);
    if(this->showMode()){
        if(!this->blockPos){
            this->blockPos = (blockLength - 1);
            this->dequePos = (this->dequePos - 1);
        }else{
            this->blockPos = (this->blockPos - 1);
        }
    }else{
        if(this->blockPos == blockLength - 1){
            this->blockPos = 0; 
            this->dequePos = this->dequePos + 1;
        }else{
            this->blockPos = this->blockPos + 1;
        }
    }
    return result;
}
myIterator myIterator::operator + (int num){
    myIterator result = *this;
    for(int i = 0;i < num;i++) result.operator++(1);
    return result;
}
int& myIterator::operator * (){
    return this->ptr[this->showDequePos()][this->showBlockPos()];
}