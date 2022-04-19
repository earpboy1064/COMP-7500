#include "cpmfsys.h" 
#include "diskSimulator.h"

//hexdump -C image1.img  prints drive

//File length = NB * 1024 + RC *128 + BC
//assigning values to struct:
// d->BC = (e + index * EXTENT_SIZE)[13];
//static uint8_t disk[256][1024]; 

bool freelist[255];
uint8_t dBlock[BLOCK_SIZE];


DirStructType *mkDirStruct(int index, uint8_t *e)
{
 index = index * 32; 
 //uint8_t *dirBlock = e; // reassigning for clarity

 

 int ex_loop = 0;

 // workaround for simplicity, rather than modify the pointer just have the pointer
 // point to my variable then return pointer
 DirStructType *extent_ptr = malloc(sizeof(DirStructType));
 DirStructType extent;
 extent_ptr = &extent;

 for(ex_loop = 0; ex_loop < 32; ex_loop++)
    {
        // status
        if(ex_loop == 0)
        {
        extent.status = e[index];
        }

        // name
        else if(ex_loop <= 8)
        {
            if(e[index + ex_loop] == ' ')
            {
                extent.name[ex_loop-1] = '\0';
            }
            else
            {
                extent.name[ex_loop-1] = e[index + ex_loop];
            }
         // printf("ex_loop is %d", ex_loop);
        }

        else if(ex_loop <= 11)
        {
           // if(e[index + ex_loop] == ' ')
            //{
                extent.extension[ex_loop-9] = '\0';
            //}
          //printf("exloop %d ",ex_loop);
          //else{
          extent.extension[ex_loop-9] = e[index + ex_loop];
          extent.extension[3] = '\0';
         // }
          //printf("index: %d, %c\n", ex_loop-9,directory[index].extension[ex_loop-9]);

        }

        // note this is out of order the switch should go ^ but it doesnt like not having an if above
         else if (ex_loop > 15) // builds blocks
        {
            extent.blocks[ex_loop-16] = e[index + ex_loop];

        }

        switch( ex_loop )
        {
            case(12):
                extent.XL = e[index + ex_loop];
                break;
            
            case(13):
                extent.BC = e[index + ex_loop];
                break;

            case(14):
                extent.XH = e[index + ex_loop];
                break;

            case(15):
                extent.RC = e[index + ex_loop];
                break;
        }
       
    }
          
    extent.name[8] = '\0';
    extent.extension[3] = '\0';
    return extent_ptr;
}

// debugg remove before submission
void print_extent(int index)
 {
 // uint8_t dBlock[BLOCK_SIZE];
  //blockRead(dBlock,0);


  DirStructType *extent;

  extent = mkDirStruct(index,dBlock);
  printf("ex:%2d status:%2x name:%8s  ext:%3s ", index+1, extent->status, extent->name, extent->extension);
  printf("XL:%2x BC:%2x XH:%2x RC:%2x\n", extent->XL,extent->BC,extent->XH,extent->RC);
  int j = 0;
  for(j = 0; j < 15; j++)
  {
    printf("B%d:%d ",j+1,extent->blocks[j]);
  }
  printf("\n\n");

}

void makeFreeList(){

  memset(freelist, true, sizeof freelist); // sets array to all true

  //uint8_t dBlock[BLOCK_SIZE];

  blockRead(dBlock,0); // this should be the only block read
  DirStructType *extent;
  int j = 0;
  int counter = 0;

  for( counter = 0; counter < 32; counter++)
  {

    extent = mkDirStruct(counter,dBlock);

    freelist[0] = false; // block 0 is always in use.

    for(j = 0; j < 16; j++)
    {

        if (extent->blocks[j] != 0 ) // had != 0 but free list didnt match
        {
            freelist[extent->blocks[j]] = false;
            //printf("block: %d is filled\n", extent->blocks[j]);
        }
    
    }
    //printf("\n");
    
  }
} 

void printFreeList()
 {
 int j = 0;
    for(j = 0; j <= 255; j++)
    {
           if (j % 16 == 0) 
        { 
            printf("%4x: ",j); 
        }
      
        if (freelist[j] == true)
        {
            printf(" .");
        }
        else
        {
            printf(" *");
        }

        if (j % 16 == 15) { 
         printf("\n"); 
        }
    }
         printf("\n"); 
}


void cpmDir()
 {
  
  int index = 0;
  //uint8_t dBlock[BLOCK_SIZE];
  //blockRead(dBlock,0);
  DirStructType *extent;


  for( index = 0; index < 32; index++)
  {

  extent = mkDirStruct(index,dBlock);

  if( extent->status != 229){  // e5 is 299 in dec 

    int blockCount = 0;

    if(index == 0 ){blockCount++;} // adds block zero. 

    int j = 0;

       for(j = 0; j < 15; j++)
    {
       

        if (extent->blocks[j] != 0 )
        {
            blockCount++;
           // printf("block: %d is filled\n", block->blocks[j]);
        }
    
    }
  int size = (blockCount-1)*1024+extent->RC*128+extent->BC;
  printf("%8s.%3s  size:%4d\n", extent->name, extent->extension,size);
  }

  }


}


int  cpmDelete(char * name)
{


DirStructType *extent;

extent = mkDirStruct(0,dBlock);

int index = 0;
bool isComplete = false;
char name_ex[12];


while((isComplete == false) && (index < 32)){

name_ex = extent->name;
strcat(name_ex, extent->extension);


if(strcmp(name_ex, name) == 0){
isComplete = true;
dBlock[index] = 229; // sets status to e5
int ex_loop = 0;
 for(ex_loop = 1; ex_loop < 32; ex_loop++)
    {
     dBlock[index+ex_loop] = 0;
        // status
    }
    
}


else
{
     index++;
     extent = mkDirStruct(index,dBlock);
}
}
return isComplete;

}