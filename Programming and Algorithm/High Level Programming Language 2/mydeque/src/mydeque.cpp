#include "mydeque.hpp"

int mydeque::size(){ return this->totalCount; }
mydeque::mydeque(){
    this->headOffset = this->tailOffset = this->totalCount = 0;

    this->deque = new int* [this->totalBlock = 2];
    this->deque[this->headPosition = 0] = new int[blockLength];
    this->deque[this->tailPosition = 1] = new int[blockLength];
}
mydeque::mydeque(const mydeque& other){ (*this) = other; }
mydeque::~mydeque(){
    for(int i = 0;i < this->totalBlock;i++)
        if(this->deque[i] != NULL) delete [] this->deque[i];
    delete [] this->deque;
}
void mydeque::operator = (const mydeque& other){
    for(int i = 0;i < this->totalBlock;i++)
        if(this->deque[i] != NULL) delete [] this->deque[i];
    delete [] this->deque;

    this->totalBlock = other.totalBlock;
    this->totalCount = other.totalCount;
    this->headOffset = other.headOffset;
    this->tailOffset = other.tailOffset;
    this->headPosition = other.headPosition;
    this->tailPosition = other.tailPosition;
    
    this->deque = new int* [this->totalBlock];
    for(int i = 0;i < this->totalBlock;i++)
        this->deque[i] = NULL;

    for(int i = 0;i < this->totalBlock;i++)
        if(other.deque[i] != NULL){
            this->deque[i] = new int[blockLength];
            for(int j = 0;j < blockLength;j++)
                this->deque[i][j] = other.deque[i][j];
        }
}
myIterator mydeque::begin(){
    if(!this->headOffset) 
        return myIterator(this->deque,0,this->headPosition + 1,0); 
    else 
        return myIterator(this->deque,0,this->headPosition,blockLength - this->headOffset);
}
myIterator mydeque::rbegin(){
    if(!this->tailOffset) 
        return myIterator(this->deque,1,this->tailPosition - 1,blockLength - 1);
    else
        return myIterator(this->deque,1,this->tailPosition,this->tailOffset - 1);
}
myIterator mydeque::end(){
    if(this->tailOffset == blockLength)
        return myIterator(this->deque,0,this->tailPosition + 1,0);
    else
        return myIterator(this->deque,0,this->tailPosition,this->tailOffset);
}
myIterator mydeque::rend(){
    if(this->headOffset == blockLength)
        return myIterator(this->deque,1,this->headPosition - 1,blockLength - 1);
    else 
        return myIterator(this->deque,1,this->headPosition,blockLength - this->headOffset - 1);
}
int mydeque::front(){ return *(this->begin()); }
int mydeque::back(){ return *(this->rbegin()); }
int result = 0;
int& mydeque::operator [](int pos){
    if(pos < this->headOffset){
        return this->deque[headPosition][blockLength - (this->headOffset - pos)];
    }else{
        return this->deque[headPosition + (pos - this->headOffset) / blockLength + 1][(pos - this->headOffset ) % blockLength];
    }
//    return result;
}
void mydeque::expandAndRearrange(){
    int oldTotalBlock = this->totalBlock;
    int** newDeque = new int* [this->totalBlock *= expandFactor];
    for(int i = 0;i < this->totalBlock;i++) newDeque[i] = NULL;

    int NewMidPos = (this->totalBlock - 1)/2;
    int midPos = (this->headPosition + this->tailPosition)/2;
    for(int i = 0;midPos + i <= this->tailPosition || midPos - i >= this->headPosition;i++){
        if(midPos + i <= this->tailPosition) newDeque[NewMidPos + i] = this->deque[midPos + i];
        if(midPos - i >= this->headPosition) newDeque[NewMidPos - i] = this->deque[midPos - i];
    }

    delete [] this->deque;

    this->deque = newDeque;
    this->tailPosition += NewMidPos-midPos;
    this->headPosition += NewMidPos-midPos;
}
void mydeque::push_front(int num){
    if(this->headOffset == blockLength){
        if(!this->headPosition) this->expandAndRearrange();
        this->deque[--this->headPosition] = new int[blockLength];
        this->headOffset = 1;
    }else this->headOffset++;
    this->deque[this->headPosition][blockLength - this->headOffset] = num;
    this->totalCount++;
}
void mydeque::push_back(int num){
    if(this->tailOffset == blockLength){
        if(this->tailPosition == this->totalBlock - 1) this->expandAndRearrange();
        this->deque[++this->tailPosition] = new int[blockLength];
        this->tailOffset = 1;
    }else this->tailOffset++;
    this->deque[this->tailPosition][this->tailOffset - 1] = num;
    this->totalCount++;
}
void mydeque::insert(myIterator pos,int num){
    this->totalCount++;
    int midPos = (this->headPosition + this->tailPosition) / 2;
    if(pos.showDequePos() <= midPos){
        int currentDequePos = this->headOffset == blockLength ? this->headPosition - 1 : this->headPosition;
        int currentBlockPos = this->headOffset == blockLength ? blockLength - 1 : blockLength - this->headOffset - 1;
        while(currentDequePos < pos.showDequePos() || 
             (currentDequePos == pos.showDequePos() && currentBlockPos < pos.showBlockPos())){
            
            int nextDequePos = currentBlockPos == blockLength - 1 ? currentDequePos + 1 : currentDequePos;
            int nextBlockPos = currentBlockPos == blockLength - 1 ? 0 : currentBlockPos + 1;
            if(this->deque[currentDequePos] == NULL) 
                this->deque[currentDequePos] = new int[blockLength];

            this->deque[currentDequePos][currentBlockPos] = 
            this->deque[nextDequePos][nextBlockPos];
            currentDequePos = nextDequePos;
            currentBlockPos = nextBlockPos;
        }
        this->deque[currentDequePos][currentBlockPos] = num;
        this->headPosition = this->headOffset == blockLength ? this->headPosition - 1 : this->headPosition;
        this->headOffset = this->headOffset == blockLength ? 1 : this->headOffset + 1;

        if(this->headPosition == 0 && this->headOffset == blockLength)
            this->expandAndRearrange();
    }else{
        int currentDequePos = this->tailOffset == blockLength ? this->tailPosition + 1 : this->tailPosition;
        int currentBlockPos = this->tailOffset == blockLength ? 0 : this->tailOffset;
        while(currentDequePos > pos.showDequePos() || 
             (currentDequePos == pos.showDequePos() && currentBlockPos > pos.showBlockPos())){
            
            int nextDequePos = currentBlockPos == 0 ? currentDequePos - 1 : currentDequePos;
            int nextBlockPos = currentBlockPos == 0 ? blockLength - 1 : currentBlockPos - 1;
            if(this->deque[currentDequePos] == NULL)
                this->deque[currentDequePos] = new int[blockLength];

            this->deque[currentDequePos][currentBlockPos] = 
            this->deque[nextDequePos][nextBlockPos];
            currentDequePos = nextDequePos;
            currentBlockPos = nextBlockPos; 
        }
        this->deque[currentDequePos][currentBlockPos] = num;
        this->tailPosition = this->tailOffset == blockLength ? this->tailPosition + 1 : this->tailPosition;
        this->tailOffset = this->tailOffset == blockLength ? 1 : this->tailOffset + 1;

        if(this->tailPosition == this->totalBlock - 1 && this->tailOffset == blockLength)
            this->expandAndRearrange(); 
    }
}
void mydeque::erase(myIterator pos){
    this->totalCount--;
    int midPos = (this->headPosition + this->tailPosition) / 2;
    if(pos.showDequePos() <= midPos){
        int currentDequePos = pos.showDequePos();
        int currentBlockPos = pos.showBlockPos();
        while(currentDequePos > this->headPosition ||
             (currentDequePos == this->headPosition && currentBlockPos > (blockLength - this->headOffset))){
            
            int nextDequePos = currentBlockPos == 0 ? currentDequePos - 1 : currentDequePos;
            int nextBlockPos = currentBlockPos == 0 ? blockLength - 1 : currentBlockPos - 1;
            this->deque[currentDequePos][currentBlockPos] = this->deque[nextDequePos][nextBlockPos];
            currentDequePos = nextDequePos;
            currentBlockPos = nextBlockPos;
        }
        if(currentBlockPos == blockLength - 1){
            this->headPosition++;
            this->headOffset = blockLength;
        }
    }else{
        int currentDequePos = pos.showDequePos();
        int currentBlockPos = pos.showBlockPos();
        while(currentDequePos < this->tailPosition ||
             (currentDequePos == this->tailPosition && currentBlockPos < this->tailOffset)){
            
            int nextDequePos = currentBlockPos == blockLength ? currentDequePos + 1 : currentDequePos;
            int nextBlockPos = currentBlockPos == blockLength ? 0 : currentBlockPos + 1;
            this->deque[currentDequePos][currentBlockPos] = 
            this->deque[nextDequePos][nextBlockPos];
            currentDequePos = nextDequePos;
            currentBlockPos = nextBlockPos;
        }
        if(currentBlockPos == 0){
            this->tailPosition--;
            this->tailOffset = blockLength;
        }
    }
}
void mydeque::clear(){
    while(this->size()){
        myIterator temp(this->deque,0,this->headPosition,blockLength - this->headOffset);
        this->erase(temp);
    }
}