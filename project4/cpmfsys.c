#include "cpmfsys.h" 
#include "diskSimulator.h"

//hexdump -C image1.img  prints drive

//File length = NB * 1024 + RC *128 + BC
//assigning values to struct:
// d->BC = (e + index * EXTENT_SIZE)[13];

DirStructType *mkDirStruct(/*int index,uint8_t *e*/)
{
uint8_t dirBlock[BLOCK_SIZE];
int i =0;


//for (i =0; i < 15; i++){
blockRead(dirBlock,(uint8_t) 0); // this loads all of block zero like we need
//printBlock(i); // this prints all of a block

// this prints the first row of block 0 aka the metadata that we need
for (i = 0; i < 32; i++){
printf("%2x",dirBlock[i]);
}
// this is the data that we need to build the struct. 






}