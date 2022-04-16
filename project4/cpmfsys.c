#include "cpmfsys.h" 
#include "diskSimulator.h"

//hexdump -C image1.img  prints drive

//File length = NB * 1024 + RC *128 + BC
//assigning values to struct:
// d->BC = (e + index * EXTENT_SIZE)[13];
//static uint8_t disk[256][1024]; 

bool freelist[300];
uint8_t dirBlock[BLOCK_SIZE];


DirStructType directory[32];


DirStructType *mkDirStruct(/*int index,uint8_t *e*/)
{

 
    
//uint8_t dirBlock[BLOCK_SIZE];
blockRead(dirBlock,0);
int index = 0;
int extent = 0;
// 32 extents of size 32
for(index = 0; index < 32; index++)
{

/* fills status */


/* Fills name */
    int ex_loop = 0;
    for(ex_loop = 0; ex_loop < 32; ex_loop++)
    {
        // status
        if(ex_loop == 0)
        {
        directory[index].status = dirBlock[extent];
        }

        // name
        else if(ex_loop <= 8)
        {
          directory[index].name[ex_loop-1] = dirBlock[extent + ex_loop];
         // printf("ex_loop is %d", ex_loop);
        }

        else if(ex_loop <= 11)
        {
          //printf("exloop %d ",ex_loop);
          directory[index].extension[ex_loop-9] = dirBlock[extent + ex_loop];
          //printf("index: %d, %c\n", ex_loop-9,directory[index].extension[ex_loop-9]);

        }


        // note this is out of order the switch should go ^ but it doesnt like not having an if above
         else if (ex_loop > 15)
        {
            directory[index].blocks[ex_loop-16] = dirBlock[extent + ex_loop];

        }


        switch( ex_loop )
        {
            case(12):
                directory[index].XL = dirBlock[extent + ex_loop];
                break;
            
            case(13):
                directory[index].BC = dirBlock[extent + ex_loop];
                break;

            case(14):
                directory[index].XH = dirBlock[extent + ex_loop];
                break;

            case(15):
                directory[index].RC = dirBlock[extent + ex_loop];
                break;
        }
       
    }
          

    //directory.extension 

    directory[index].name[8]='\0'; // adds null terminator may need to add /0 for missing char
    directory[index].extension[3]='\0'; // adds null terminator may need to add /0 for missing char
    // NOTE YOU WILL OVERWRITE THE NEXT FILE LOCATION IF YOU GO PAST THE CORRECT INDEX
 
    printf("ex:%2d status:%2x name:%8s ext:%3s ",index+1, directory[index].status, directory[index].name, directory[index].extension);
    printf("XL:%2x BC:%2x XH:%2x RC:%2x\n", directory[index].XL,directory[index].BC,directory[index].XH,directory[index].RC);
    int j = 0;
    for(j = 0; j < 15; j++)
    {

        printf("B%d:%x ",j+1,directory[index].blocks[j]);

    }
    printf("\n\n");

    extent +=32; // moves to the next extent
}
}
/*





void makeFreeList()
{
    /*
printBlock(0);
int count = 0;
blockRead(dirBlock,(uint8_t) 0); // this loads all of block zero like we need

// we are printing 16 2Byte digits to produce the 32 byte extent.
int i;
  for (i = 0; i < BLOCK_SIZE; i++) { 
   // if (i % 16 == 0) { 
        if (dirBlock[i] == 229)
        {
            freelist[count] = true;
            count++;
        }
        else
        {
            freelist[count] = false;
            count++;
        }
        
   // }
  }
     // printf("\n\n**count: %d",count);

    




} 

void printFreeList()
{
int j = 0;
    for(j = 0; j< 64; j++)
    {
        if (j % 16 == 0) 
        { 
            printf("%4x: ",j); 
        }
        if (j % 16 == 15) { 
         printf("\n"); 
        }

        if (freelist[j] == true)
        {
            printf(" .");
        }
        else
        {
            printf(" *");
        }

       // printf("%4x: %2x %2d\n",j,freelist[j], freelist[j]);

    }


}

*/