#include "cpmfsys.h" 
#include "diskSimulator.h"

//hexdump -C image1.img  prints drive

//File length = NB * 1024 + RC *128 + BC
//assigning values to struct:
// d->BC = (e + index * EXTENT_SIZE)[13];
//static uint8_t disk[256][1024]; 

bool freelist[255];
//uint8_t block0[BLOCK_SIZE];


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
        }

        else if(ex_loop <= 11)
        {
           
                extent.extension[ex_loop-9] = '\0';
            
          extent.extension[ex_loop-9] = e[index + ex_loop];
          extent.extension[3] = '\0';
         

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

void writeDirStruct(DirStructType *d, uint8_t index, uint8_t *e)
{

 //printf("\n\nWriteDirStruct");

 //printf("\n\nname:%s \n\n", d->name);

 index = index * 32; 

int ex_loop = 0;

     for(ex_loop = 0; ex_loop < 32; ex_loop++)
    {
        // status
        if(ex_loop == 0)
        {
        e[index] = d->status;
        }

        // name
        else if(ex_loop <= 8)
        {
            
              e[index + ex_loop] =  d->name[ex_loop-1]; 
          
        }

        else if(ex_loop <= 11)
        {
          
          e[index + ex_loop] = d->extension[ex_loop-9];          

        }

        // note this is out of order the switch should go ^ but it doesnt like not having an if above
         else if (ex_loop > 15) // builds blocks
        {
            e[index + ex_loop] = d->blocks[ex_loop-16]; 

        }

        switch( ex_loop )
        {
            case(12):
               e[index + ex_loop] = d->XL; 
                break;
            
            case(13):
                 e[index + ex_loop] = d->BC;
                break;

            case(14):
               e[index + ex_loop] = d->XH; 
                break;

            case(15):
                e[index + ex_loop] = d->RC; 
                break;
        }
       
    }
    d->name[8] = '\0';
    d->extension[3] = '\0';
          
}

void testwrite()
{
   printf("fault?");

  int index = 0;
  uint8_t block0[BLOCK_SIZE];
  blockRead(block0,0);
  DirStructType *extent = malloc(sizeof(DirStructType));


  extent = mkDirStruct(0,block0);

 printf("\n\nname:%s \n\n", extent->name);


 /*
 extent->name[0] = 't';
 extent->name[1] = 'e';
 extent->name[2] = 's';
 extent->name[3] = 't';
 extent->name[4] = '\0';
 extent->name[5] = '\0';
 extent->name[6] = '\0';
 extent->name[7] = '\0';
 extent->name[8] = '\0';
 //extent->name[9] = '\0';

*/

 writeDirStruct(extent, 0, block0);


 DirStructType *extent2 = malloc(sizeof(DirStructType));

 extent2 = mkDirStruct(0,block0);
 printf("\n\nname:%s \n\n", extent2->name);


}




// debugg remove before submission
void print_extent(int index)
 {
  uint8_t block0[BLOCK_SIZE];
  blockRead(block0,0);


  DirStructType *extent;

  extent = mkDirStruct(index,block0);
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

  uint8_t block0[BLOCK_SIZE];

  blockRead(block0,0); // this should be the only block read
  DirStructType *extent;

  int j = 0;
  int counter = 0;

  for( counter = 0; counter < 32; counter++)
  {

    extent = mkDirStruct(counter,block0);

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


int findExtentWithName(char *name, uint8_t *block0)
{

  int index = 0;
  DirStructType *extent;

  int i = 0;
  char file_name[9];
  char file_extension[4];

  for (i = 0; i < 9; i++)
  {
    if(name[i] == '.')
    {
        file_name[i] = '\0';
    }
    else if(name[i] == ' ')
    {
        file_name[i] = '\0';
    }
    else
    {
        file_name[i] = name[i];
    }
  }
  //printf("the file name is %s \n", file_name);
  for( index = 0; index < 32; index++)
  {
    extent = mkDirStruct(index,block0);

    if(strcmp(extent->name, file_name) == 0)
    {
     //printf("we have a match in our NEW FUNCION YAY the index is:%d\n", index);
     return index;
    }
  }

  return -1; // filename not found
}




void cpmDir()
 {
  
  int index = 0;
  uint8_t block0[BLOCK_SIZE];
  blockRead(block0,0);
  DirStructType *extent;


  for( index = 0; index < 32; index++)
    {

     extent = mkDirStruct(index,block0);

     if( extent->status != 229)// e5 is 299 in dec
     {   

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
      printf("%s.%s %d\n", extent->name, extent->extension,size);

     }

    }
}



int cpmRename(char *oldName, char * newName)
{
 //#  cpmRename code explaination
 //#  
 //#  The new file name must fall under the following rules
 //#  name < 9 charectors
 //#  file extension < 4 charectors 
 //#  
 //#  We first check if the file name is legal using checkLegalName function.
 //#  After we look for the extent containing the old name
 //#  if none exist we return -1 for invalid file name and do no writing operations
 //#
 //#  therefore to make it easier to copy to the block we build and fill
 //#  variables of that size with with the new names. That way the copy can be 1 to 1
 //#  To do this we loop through the new name until it reaches the '.' charector
 //#  we then fill the remaining spaces with \0 to signify unused space.
 //#
 //#  we then repeat the process for the extension except we just copy the 3 bytes that
 //#  the extension is allowed
 //#
 //#  We then simply 1 to 1 copy the bytes to the block.
 //#
 //# 
 //#

 // might be best just to copy the new name to the appropriate size char[] then just copy the whole thing over.
    // need to include bool checkLegalName(char *name); 




  // problems to be fixed instead of filling with \0 maybe just fill with  ' ' and end with \0 
  // need to check valid file name before building the file name and need to return if not valid
  char file_name[9];
  char file_extension[4];
  int i = 0;
  bool EOF_name = false;
  int ex_index = 1; // holds the index of the extentsion



 uint8_t block0[BLOCK_SIZE];
 blockRead(block0,0);

 DirStructType *extent;


//Error handling
 int index = 0;
 int error_code = 0;
 index = findExtentWithName(oldName, block0);
 
  






  for (i = 0; i < 9; i++)
  {
    if(EOF_name == true)
    {
        file_name[i] = '\0';
    }
    else if(newName[i] == '.')
    {
        file_name[i] = '\0';

        // prevents us from trying to write parts of the extension to the name if the name is short
        EOF_name = true; 
        ex_index += i; // adding i because i points to the '.' we want the space after
    }
    else if(newName[i] == ' ')
    {
        file_name[i] = '\0'; // this could maybe just be space
    }
    else
    {
        file_name[i] = newName[i];
    }
  }
 
  for ( i=0; i <= 3; i++)
  {
    if(newName[i+ex_index] == '.')
    {
        file_extension[i] = 'f';
    }
    else if(newName[i+ex_index] == ' ')
    {
        file_extension[i] = ' ';
    }
    else
    {
        file_extension[i] = newName[ex_index+i];
    }
  }
    
    printf("File extension:%s ", file_extension);
    printf(" File name:%s\n", file_name);
    

 

 if(index >= 0)
 {
   extent = mkDirStruct(index,block0);


 //  printf("we have a match the index is:%d\n", index);
   int write_block = index*32;
   int ex_loop = 0;
   bool EOF_name;


   // iterates through the 32 byte extent replacing the name
   for(ex_loop = 1; ex_loop < 9; ex_loop++)
    {
       // printf("We are replacing name %c with %c \n", block0[write_block+ex_loop],file_name[ex_loop-1]);
        block0[write_block+ex_loop] = file_name[ex_loop-1];
         
    }

    for(ex_loop = 9; ex_loop < 12; ex_loop++)
    {

       // printf("We are replacing extentsion %c with %c \n", block0[write_block+ex_loop], file_extension[ex_loop-9] );
        block0[write_block+ex_loop] = file_extension[ex_loop-9];

    }



     blockWrite(block0,0); // writes updates to disk array. not to the image.
    }

  else 
  {
  return index; // index will have a value less than 0 if there is an error and this will return that.
  }

}; 



int  cpmDelete(char * name)
{


     // need to include bool checkLegalName(char *name); 


 // read the block 
 uint8_t block0[BLOCK_SIZE];
 blockRead(block0,0);

 DirStructType *extent;

 int index = 0;
 index = findExtentWithName(name, block0);

 if(index > 0)
 {
   extent = mkDirStruct(index,block0);


  // printf("we have a match the index is:%d\n", index);
   int wipe_block = index*32;
   int ex_loop = 0;


    block0[wipe_block] = 229; // sets status to e5

   // iterates through the 32 byte extent setting values to 0 
   for(ex_loop = 0; ex_loop < 32; ex_loop++)
    {

      if(ex_loop < 16)
      {
        if (extent->blocks[ex_loop] != 0)
        {  // updates free list
            //printf("freeing block  %d\n", extent->blocks[ex_loop]);
            freelist[extent->blocks[ex_loop]] = true;
        }
      }

     if(ex_loop > 0){ block0[wipe_block+ex_loop] = 0; } // prevents setting status to 0
    
    }

  blockWrite(block0,0); // writes updates to disk array. not to the image.
  }

  else 
  {
  return index; // index will have a value less than 0 if there is an error and this will return that.
  }

}