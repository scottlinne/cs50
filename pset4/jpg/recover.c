/**
 * Scott Linne 2015 CS50
 * recover.c
 *
 * Computer Science 50
 * Problem Set 4
 *
 * Recovers JPEGs from a forensic image.
 */
 
#include <cs50.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

char title[8];
FILE* outptr;

int main(int argc, char* argv[])
{
    // Open memory card file
    FILE* inptr = fopen("card.raw", "r");
    
        if(inptr == NULL)
        {
            printf("can't open %s\n", "card.raw");
            return 1;
        }
        
    // read 512 bytes into buffer
    typedef uint8_t  BYTE;
    BYTE buffer[512];

     
    // initialize counter variable
    int k = 0;
    int open = 0;
       // keep reading until end of file
       while(fread(&buffer, 512, 1, inptr) > 0)
       {
          
           // initialize file open variable check
           
            // check if at start of a new jpg
            if((buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff) && 
            (buffer[3] == 0xe0 || buffer[3] ==0xe1))
            {
                if(outptr != NULL)
                {
                    fclose(outptr);
                    open = 0;
                }
               // create outfile
                sprintf(title, "%03i.jpg", k);
                   char* outfile = title;
                   
                   // open jpg file
                    outptr = fopen(outfile, "w");
                   // write 512 bytes into outfile until next jpg found
                    fwrite(&buffer, sizeof(buffer), 1, outptr);
                    
                 // increment counter variable for building file name  
                 k++;
                 
                 // update open indicator to show file is now open
                open = 1;
                
            }
            // if file is already open, write bytes to outfile
            // if not at new jpg continue writing bytes into outfile
            else if(open == 1)
            {
                fwrite(&buffer, sizeof(buffer), 1, outptr);
            }
                
        }
    // close all open files
    fclose(inptr);
    fclose(outptr);
    
}
