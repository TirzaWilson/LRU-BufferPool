#ifndef LRUBUFFERPOOL_H
#define	LRUBUFFERPOOL_H


#include <fstream>
#include <string>
#include "BufferPoolADT.h"
#include "BufferBlock.h"

using namespace std;

class LRUBufferPool : public BufferPoolADT {
private:
    ifstream fileStream;
    BufferBlock blockArray[POOL_SIZE];
    string file;

public:
    // Constructor initializes the buffer pool with the provided file and pool size
    LRUBufferPool(const string& filename, int poolSize = 5, int blockSize = 4096) {
        // Initialize the buffer blocks with their IDs, heads, and tails
        for (int i = 0; i < POOL_SIZE; i++) {
            blockArray[i].setID(i);
            blockArray[i].setHead(i * BLOCKSIZE);       // Set the head of each block (start position)
            blockArray[i].setTail((i + 1) * BLOCKSIZE); // Set the tail of each block (end position)
        }
        // Open the file using the filename 
        fileStream.open(filename, ios::in | ios::out | ios::binary);

        if (!fileStream.is_open()) {
            cerr << "Error: Could not open file " << filename << std::endl;
            // Handle error (throw an exception or exit)
        }
        fileStream.seekg(0, ifstream::end); // Seek to the beginning and end of the file
        fileStream.seekg(0, ifstream::beg);

        for (int i = 0; i < POOL_SIZE; i++) { // Load data into the buffer blocks
            char* temp = new char[BLOCKSIZE]; // Allocate memory for a block
            fileStream.read(temp, BLOCKSIZE); // Read data into the block
            blockArray[i].setBlock(temp);     // Store the block in the pool
        }
    }
    ~LRUBufferPool() {}

    virtual void getBytes(char* space, int sz, int pos) override {

        for (int i = 0; i < POOL_SIZE; i++) {
            if (pos >= blockArray[i].getHead() && pos < blockArray[i].getTail()) {
                // Read the data from the correct block
                blockArray[i].getData(pos, sz, space);
                LRUNodeReordered(i);  // Move this block to the front (most recently used)
                return;
            }
        }
        ifstream temp; // If the block is not in the buffer pool, read it from the file
        temp.open(file, ifstream::in | ifstream::binary);
        temp.seekg(pos);      // Seek to the correct position in the file
        temp.read(space, sz); // Read the required bytes into the provided buffer
        LRUNode(file, pos);   // Load the block into the buffer pool
    }
    // Print the current order of blocks in the buffer pool (most recently used to least recently used)
    virtual void printBufferBlockOrder() override { 
        cout << "My buffer block order from most recently used to LRU is:" << endl;
        for (int i = 0; i < POOL_SIZE; i++) {
            cout << blockArray[i].getID(); // Print the block ID
            if (i != (POOL_SIZE - 1)) {
                cout << ", ";
            } else {
                cout << "\n" << endl; // End the output line after the last block
            }
        } return;
    }
    virtual int getLRUBlockID() override { 
        return blockArray[POOL_SIZE - 1].getID(); // The LRU block is the last one in the array
    }
    void LRUNode(string file, int pos) { // Load a new block from the file into the buffer pool, pushing existing blocks down
        
        for (int i = POOL_SIZE - 1; i > 0; i--) {  // Shift blocks to the right to make space for the new block
            blockArray[i] = blockArray[i - 1];     // Shift elements to the right
        }
        BufferBlock buffBlock; // Create a temporary buffer block
        char* charArr = new char[BLOCKSIZE];
        ifstream data;
        data.open(file, ifstream::in | ifstream::binary); // Open the file to read the new block
       
        int blockID = pos / BLOCKSIZE;   // Determine which block this is based on the position
        int start = blockID * BLOCKSIZE; // Calculate the start of the block in the file
       
        data.seekg(start);             // Seek to the correct start position in the file
        data.read(charArr, BLOCKSIZE); // Read the block's data into the allocated memory
        // Set the attributes of the new block
        buffBlock.setBlock(charArr);
        buffBlock.setHead(start);             // Set the head (start position) of the block
        buffBlock.setTail(start + BLOCKSIZE); // Set the tail (end position) of the block
        buffBlock.setID(blockID);             // Set the block's ID

        blockArray[0] = buffBlock; // Place the newly loaded block at the front (most recently used)
        return;
    }
    void LRUNodeReordered(int block) {
        if (block == 0) return;  // No need to move if the block is already in the first position

        // Shift blocks to the right
        BufferBlock tempBlock = blockArray[block];  // Store the block to be moved
        for (int i = block - 1; i >= 0; i--) {
            blockArray[i + 1] = blockArray[i];  // Shift each block one position to the right
        }
        blockArray[0] = tempBlock;  // Place the original block at the front (index 0)
    }

};

#endif	/* LRUBUFFERPOOL_H */