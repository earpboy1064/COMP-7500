// cpmfsys.c 
// By Wyatt LeMaster
// For COMP 7500

#include "cpmfsys.h" 
#include "diskSimulator.h"
#include "string.h" // for strlen
#include "ctype.h" // for isalpha and isdigit


bool freelist[255];


DirStructType *mkDirStruct(int index, uint8_t *e)
{

  //# COMMENT BLOCK
  //#  this function allocates and fills the memory for the DirStructType structure
  //#  this structure holds the 32 byte extent from disk 0
  //#  It takes an index which shows the location in block0 for the extent
  //#  it also receieves a pointer e which points to the block0 that is being 
  //#  translated
  //#
  //#  the function iterates through the 32 bytes and assigns the values
  //#  to the correct locations in the stucture
  //#  it then returns a pointer which points to the allocated 
  //#  memory where the structure is held.



// calculating the index location
 index = index * 32; 

 //ex loop stands for extent loop. Its used to interate through the extent.
 int ex_loop = 0;

 // done simplicity, rather than modify the pointer just have the pointer
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
          

    // adds null terminators for name and extension
    // this is adding the terminator to the location reserved for it
    // therefore we shouldnt be worried about overwriting data.
    extent.name[8] = '\0';
    extent.extension[3] = '\0';

    // returns pointer to the location
    return extent_ptr;
}

void writeDirStruct(DirStructType *d, uint8_t index, uint8_t *e)
{
  //# COMMENT BLOCK
  //# This function is used to write the contents of a modified
  //# extent to the block where it originated from.

  //# d is a pointer to the modifed extent,
  //# index is the location in block0 where the extent is stored
  //# e is a pointer to the block to be altered

  //# The function operates the same as mkDirStruct just reversed



 // debug print statements 
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


 /*  THIS PROB SHOULD BE FIXED BEFORE SUBMISSION
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
        }*/
       
    }
    d->name[8] = '\0';
    d->extension[3] = '\0';
          
}

// debugg remove before submission
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

//#  COMMENT BLOCK
//#  This function creates a list of block usage in the memory
//#
//#  A used block is denoted by *
//#
//#  A free block is denoted by a .
//#
//#  In order to achieve this we iterate through every extent 
//#  and look at every block. if the block is greater then 0 we mark it as in use


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

        if (extent->blocks[j] != 0 ) // checks if the block is empty
        {
            freelist[extent->blocks[j]] = false;
        }
    
    }
    
  }
} 

void printFreeList(){

//# This function serves only to print the free list in rows of 16
//# It simply iterates through all 255 values in the free list and prints

//# . for empty 

//# * for full 

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

//#  This function is used to find and return the index 
//#  of the extent that contains the filename provided.
//#
//#  This is done by iterating through every extent and compairing 
//#  The name.
//#
//#  if no match is found -1 is returned



  int index = 0;
  DirStructType *extent;

  int i = 0;
  char file_name[9];
  char file_extension[4];


  // we first start by building the name in the same format as the 
  // name stored in the extent to make for easy compairison
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

  // we then search for an extent that matches
  // if we find a match it returns the index
  for( index = 0; index < 32; index++)
  {
    extent = mkDirStruct(index,block0);

    if(strcmp(extent->name, file_name) == 0)
    {
     return index;
    }
  }

  // returned if no filename is found
  return -1; // filename not found
}



bool checkLegalName(char *name)
{
//# Code block 
//#  We need to check to ensure that the new name is not longer then 8 char
//#  and that the extension is not longer then 3
//#  if they are accepted it needs to return 1 else return -1

 int i = 0;
 bool valid_file = true;
 int name_size = 0;

 if (strlen(name) > 12){ valid_file = false;}

 //printf("name is %s\n", name);
 //printf("size of name %d\n",strlen(name) );

 
 if(valid_file == 1)
 {

   if((isalpha(name[0]) == 0) && (isdigit(name[0]) == 0))
   {
   valid_file = false;
   }


   for(i = 0; i < 9; i++)
   {

   //printf("checking char %c\n", name[i]);

   if ((name[i] == '.') && (valid_file == true))
   {
    //valid_file = true;
    name_size = i+1;
    //printf("index where . is %d\n", i);

   }  
 }

 if((strlen(name) - name_size) > 3)
 {
   valid_file = false; 
 }

 if((isalpha(name[strlen(name) - name_size]) == 0) && (isdigit(name[strlen(name) - name_size]) == 0))
  {
  valid_file = false;
  }

  //printf("file is %d", valid_file);
 }

 return valid_file;
}


void cpmDir()
 {
 //# COMMENT BLOCK  
 //#  This function prints the contents of the directory block 0
 //#
 //# it iterates through all 32 extents and prints all information
 //#  this is made possible by calling mkDirStruct for every of the 32 extents
 //#  then printing the information contained in the structure.
 //#
 //#  It also calculates the size of each file.
 //#  This is done by the following equation:
 //# (blockCount-1)*1024+extent->RC*128+extent->BC;
 



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
 bool legal_name = true;
 index = findExtentWithName(oldName, block0);
 legal_name = checkLegalName(newName);
 
 if(legal_name == false)
 {
   return -2;
 }

 if(index < 0)
 {
    return -1;
 }


 if(index >= 0)
 {


   extent = mkDirStruct(index,block0);


    //printf("File extension before:%s ", extent->extension);
    //printf(" File name before:%s\n", extent->name);

  for (i = 0; i < 9; i++)
  {
    if(EOF_name == true)
    {
        extent->name[i] = '\0';
    }
    else if(newName[i] == '.')
    {
        extent->name[i] = '\0';

        // prevents us from trying to write parts of the extension to the name if the name is short
        EOF_name = true; 
        ex_index += i; // adding i because i points to the '.' we want the space after
    }
    else if(newName[i] == ' ')
    {
        extent->name[i] = '\0'; // this could maybe just be space
    }
    else
    {
        extent->name[i] = newName[i];
    }
  }
 
  for ( i=0; i <= 3; i++)
  {
    if(newName[i+ex_index] == '.')
    {
        extent->extension[i] = '\0';
    }
    else if(newName[i+ex_index] == ' ')
    {
         extent->extension[i] = '\0';
    }
    else
    {
         extent->extension[i] = newName[ex_index+i];
    }
  }
    
    //printf("File extension after:%s ", extent->extension);
    //printf(" File name after:%s\n", extent->name);
    
   writeDirStruct(extent, index, block0);
 
   blockWrite(block0,0); // writes updates to disk array. not to the image.
 
  }
}



int  cpmDelete(char * name)
{


     // need to include bool checkLegalName(char *name); 

 // read the block 
 uint8_t block0[BLOCK_SIZE];
 blockRead(block0,0);

 DirStructType *extent;

 int index = 0;
 index = findExtentWithName(name, block0);


 legal_name = checkLegalName(newName);
 
 if(legal_name == false)
 {
   return -2;
 }

 if(index < 0)
 {
    return -1;
 }


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