#ifndef BUFFERBLOCK_H
#define	BUFFERBLOCK_H

#include "BufferBlockADT.h" 
#include <cstring>

class BufferBlock: public BufferblockADT {
private:
	int blockID, blockSize, head, tail;
	char* blockPtr;

public:
	BufferBlock() {
		// Initialize variables to default values
		blockID = -1;
		blockSize = 0;
		head = -1;
		tail = -1;
		blockPtr = nullptr;  // No memory allocated yet
	}
	BufferBlock(char* blk, int size = 4096) : blockPtr(blk), blockSize(size) {}
	// Destructor
	~BufferBlock() {
		// Set the start & end points to -1 (until they're reassigned)
		head = -1;
		tail = -1;
	}
	// Method to retrieve data from the block given a starting position (pos) and size (sz)
	virtual void getData(int pos, int sz, char* data) {
		int j = 0; // This method copies data from blockPtr into the provided data buffer
		pos = pos % BLOCKSIZE; // Ensure position is within valid bounds (wrap around if necessary)
		for (int i = pos; i < (pos + sz); i++) { // Iterate over the block's data
			data[j] = blockPtr[i]; // Copy each byte from blockPtr to the data buffer
			j++;
		} return; // Return after copying the data
	}
	virtual int getID() const{ 
		return blockID;
	}
	virtual void setID(int id){
		blockID = id;
	}
	virtual void setBlock(char* blk) {
		blockPtr = blk;
		//blockSize = strlen(blockPtr);
	}
	virtual char* getBlock() const {
		return blockPtr;
	}
	virtual int getBlocksize() const{
		return blockSize;
	}
	void setHead(int start){ 
		head = start;
	}
	int getHead() {
		return head;
	}
	void setTail(int end){ 
		tail = end;
	}
	int getTail() {
		return tail;
	}
};

#endif	/* BUFFERBLOCK_H */