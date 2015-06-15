/**
 * resize.c
 *
 * Computer Science 50
 * Problem Set 4
 *
 * Copies a BMP piece by piece, just because.
 */
       
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char* argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        printf("Usage: ./copy [n] infile outfile\n");
        return 1;
    }

    // store resize variable as factor n
    int n = atoi(argv[1]);
    
    // ensure the resize factor n is non-negative between 1 and 100
    if (n < 1 || n > 100)
    {
        printf("first argument must be a positive integer between 1 to 100\n");
        return 1;
    
    }
    
    // remember filenames
    char* infile = argv[2];
    char* outfile = argv[3];

    // open input file 
    FILE* inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE* outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);
  

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);
      

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }
    
    // store value of original bmp height
    int original_height = bi.biHeight;
    
    
    // store value of originAL bmp width
    int original_width = bi.biWidth;
    
    // calculate new width in pixels, does not include padding
    bi.biWidth = bi.biWidth * n;
    
    // calculate new height in pixels
    bi.biHeight = bi.biHeight * n;

    // determine padding for scanlines
    int padding =  (4 - (original_width * sizeof(RGBTRIPLE)) % 4) % 4;
    
    // determine padding for outfile
    int paddingNew = (padding * n) % 4;
    
    // calculate new image size biSizeImage pixels and padding
    // bi.biSizeIMage
    bi.biSizeImage = (bi.biWidth * sizeof(RGBTRIPLE) + paddingNew)  * abs(bi.biHeight);
    
    // calculate new file size
    //bf.bfSize
    bf.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + bi.biSizeImage;
    
   

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);
    
    // Read each scanline pixel by pixel
    // iterate over infile's scanlines
    for (int i = 0; i < abs(original_height); i++)
    {
    
        // repeate for the number of rows
        for (int r = 0; r < n; r++)
        {
        
                // iterate over pixels in scanline
                for (int j = 0; j < original_width; j++)
                {
                    // temporary storage
                    RGBTRIPLE triple;

                    // read RGB triple from infile
                    fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
                    

                    // write RGB triple to outfile n number of times
                   for (int k = 0; k < n; k++)
                    {
                        fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                    }
                }
             
                
            

        // skip over padding, if any
        fseek(inptr, padding, SEEK_CUR);

        // then add it back (to demonstrate how)
        for (int k = 0; k < paddingNew; k++)
        {
            fputc(0x00, outptr);
        }
        
        // seek back to front of scanline to repeat n times
                if (r != (n-1))
                {
                    fseek(inptr, -original_width * sizeof(RGBTRIPLE) - padding, SEEK_CUR);
                
                }   
            }
        
        
        
        
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // that's all folks
    return 0;
}
