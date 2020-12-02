#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef uint32_t DWORD;   // DWORD = unsigned 32 bit value
typedef uint16_t WORD;    // WORD = unsigned 16 bit value
typedef uint8_t BYTE;     // BYTE = unsigned 8 bit value

typedef struct tagBITMAPFILEHEADER
{
    WORD bfType;  
    DWORD bfSize;  //specifies the size in bytes of the bitmap file
    WORD bfReserved1;  
    WORD bfReserved2;  
    DWORD bfOffBits;  //species the offset in bytes from the beginning of the file to the starting Pixel Data Byte 
} BITMAPFILEHEADER ;

typedef struct tagBITMAPINFOHEADER
{
    DWORD biSize;  
    DWORD biWidth;  //specifies width in pixels
    DWORD biHeight;  //species height in pixels
    WORD biPlanes; 
    WORD biBitCount; //specifies the number of bit per pixel
    DWORD biCompression;
    DWORD biSizeImage;  //size of image in bytes
    DWORD biXPelsPerMeter;  
    DWORD biYPelsPerMeter;  
    DWORD biClrUsed;  
    DWORD biClrImportant;  
} BITMAPINFOHEADER ;

void ReadBmpFileHeader( BITMAPFILEHEADER* FILEHEADER, FILE* fp)
{
    fread( &FILEHEADER->bfType, sizeof(FILEHEADER->bfType), 1 , fp);
    fread( &FILEHEADER->bfSize, sizeof(FILEHEADER->bfSize), 1 , fp);
    fread( &FILEHEADER->bfReserved1, sizeof(FILEHEADER->bfReserved1), 1 , fp);
    fread( &FILEHEADER->bfReserved2, sizeof(FILEHEADER->bfReserved2), 1 , fp);
    fread( &FILEHEADER->bfOffBits, sizeof(FILEHEADER->bfOffBits), 1 , fp);
}

void ReadBmpInfoHeader( BITMAPINFOHEADER* INFOHEADER, FILE* fp)
{
    fread( &INFOHEADER->biSize, sizeof(INFOHEADER->biSize), 1 , fp);
    fread( &INFOHEADER->biWidth, sizeof(INFOHEADER->biWidth), 1 , fp);
    fread( &INFOHEADER->biHeight, sizeof(INFOHEADER->biHeight), 1 , fp);
    fread( &INFOHEADER->biPlanes, sizeof(INFOHEADER->biPlanes), 1 , fp);
    fread( &INFOHEADER->biBitCount, sizeof(INFOHEADER->biBitCount), 1 , fp);
    fread( &INFOHEADER->biCompression, sizeof(INFOHEADER->biCompression), 1 , fp);
    fread( &INFOHEADER->biSizeImage, sizeof(INFOHEADER->biSizeImage), 1 , fp);
    fread( &INFOHEADER->biXPelsPerMeter, sizeof(INFOHEADER->biXPelsPerMeter), 1 , fp);
    fread( &INFOHEADER->biYPelsPerMeter, sizeof(INFOHEADER->biYPelsPerMeter), 1 , fp);
    fread( &INFOHEADER->biClrUsed, sizeof(INFOHEADER->biClrUsed), 1 , fp);
    fread( &INFOHEADER->biClrImportant, sizeof(INFOHEADER->biClrImportant), 1 , fp);
}

void WriteBmpFileHeader( BITMAPFILEHEADER* FILEHEADER, FILE* fp)
{
    fwrite( &FILEHEADER->bfType, sizeof(FILEHEADER->bfType), 1 , fp);
    fwrite( &FILEHEADER->bfSize, sizeof(FILEHEADER->bfSize), 1 , fp);
    fwrite( &FILEHEADER->bfReserved1, sizeof(FILEHEADER->bfReserved1), 1 , fp);
    fwrite( &FILEHEADER->bfReserved2, sizeof(FILEHEADER->bfReserved2), 1 , fp);
    fwrite( &FILEHEADER->bfOffBits, sizeof(FILEHEADER->bfOffBits), 1 , fp);
}

void WriteBmpInfoHeader( BITMAPINFOHEADER* INFOHEADER, FILE* fp)
{
    fwrite( &INFOHEADER->biSize, sizeof(INFOHEADER->biSize), 1 , fp);
    fwrite( &INFOHEADER->biWidth, sizeof(INFOHEADER->biWidth), 1 , fp);
    fwrite( &INFOHEADER->biHeight, sizeof(INFOHEADER->biHeight), 1 , fp);
    fwrite( &INFOHEADER->biPlanes, sizeof(INFOHEADER->biPlanes), 1 , fp);
    fwrite( &INFOHEADER->biBitCount, sizeof(INFOHEADER->biBitCount), 1 , fp);
    fwrite( &INFOHEADER->biCompression, sizeof(INFOHEADER->biCompression), 1 , fp);
    fwrite( &INFOHEADER->biSizeImage, sizeof(INFOHEADER->biSizeImage), 1 , fp);
    fwrite( &INFOHEADER->biXPelsPerMeter, sizeof(INFOHEADER->biXPelsPerMeter), 1 , fp);
    fwrite( &INFOHEADER->biYPelsPerMeter, sizeof(INFOHEADER->biYPelsPerMeter), 1 , fp);
    fwrite( &INFOHEADER->biClrUsed, sizeof(INFOHEADER->biClrUsed), 1 , fp);
    fwrite( &INFOHEADER->biClrImportant, sizeof(INFOHEADER->biClrImportant), 1 , fp);
}

void SetBit( unsigned char* A ,  int k )
{
    int i = k/8;        //gives the corresponding index in the array A
    int pos = k%8;      //gives the corresponding bit position in A[i] ; K >= 1

    BYTE flag = 0x80;   // flag = 1000 0000

    flag = flag >> pos;      // flag = 00...010...0   (shifted k positions)

    A[i] = A[i] | flag;      // Set the bit at the k-th position in A[i]
}

void ClearBit( unsigned char* A ,  int k )                
{
    int i = k/8;
    int pos = k%8;

    BYTE flag = 0x80;
    flag = flag >> pos;

    A[i] = A[i] & (~flag);
}

int GetBit( unsigned char* A,  int k )
{
    int i = k/8;
    int pos = k%8;

    BYTE flag = 0x80;
    flag = flag >> pos;

    if(A[i] & flag != 0){ // returns 1 (true), if bit is set, else 0
        return 1;
    }
    else{
        return 0;
    }     
}
 
void cropImage_1bit( BITMAPFILEHEADER* bitmapFileHeader, BITMAPINFOHEADER* bitmapInfoHeader, FILE* filePtr, int x, int y, int width, int height)
{
    // store 4-bit color table information - 2 colors * 4 bytes
    unsigned char* colorTable;
    colorTable = (unsigned char*)malloc(8);

    fread(colorTable, 8, 1 , filePtr);

    int rowJunkByte; // stores the no. of junk bytes added per row, to make the pixel data row a multiple of 4 bytes - in input image

    // extract "width" and "height" information of image :- "in terms of no. of pixels"
    int bm_width = bitmapInfoHeader->biWidth;
    int bm_height = bitmapInfoHeader->biHeight;

    printf("width of image in pixel : %d  \n", bm_width);
    printf("height of image in pixel : %d  \n", bm_height);
    printf("Bits per pixel of image : %d  \n", bitmapInfoHeader->biBitCount);

    // calculate junkByte added per row 
    int width_bytes = (bm_width % 8 == 0) ? (bm_width/8) : (bm_width/8)+1 ; // for handling odd no. of pixels in width

    if((bitmapInfoHeader->biSizeImage - (bm_height * width_bytes)) == 0){
        rowJunkByte = 0;
    }
    else{
        rowJunkByte = ((bitmapInfoHeader->biSizeImage - (bm_height * width_bytes)) / bm_height);
    }
    printf("No. of junk-Bytes in a row: %d \n", rowJunkByte);

    //move "file pointer" to the beginning of Bitmap Pixel Data Byte
    fseek(filePtr, bitmapFileHeader->bfOffBits, SEEK_SET);  

    //a pointer of type unsigned char* - to store "input image" pixel data bytes: 1 byte = 2 pixels
    unsigned char* bitmapImage;  
    //allocate enough memory for the bitmap image data
    bitmapImage = (unsigned char*)malloc(bitmapInfoHeader->biSizeImage);

    //check if memory allocation failed
    if (bitmapImage == NULL)
    {
        printf("bit-map-image MEMORY ALLOCATION FAILED");
        //free(bitmapImage);
        fclose(filePtr);
        return;
    }

    //read the bitmap image pixel data - skipping the "padding bytes"
    if(rowJunkByte == 0){
        fread(bitmapImage, bitmapInfoHeader->biSizeImage, 1 , filePtr);
    }
    else{
        int i=0;

        for(int height=1; height <= bm_height; height++){

            // read, image pixel data bytes from each row 
            fread(bitmapImage + i, width_bytes , 1 , filePtr);

            // skip the junk-bytes
            fseek(filePtr, rowJunkByte , SEEK_CUR); 

            i = i + width_bytes;
        }
    }
    fclose(filePtr);

    unsigned char* croppedImage;  
    //allocate enough memory for the bitmap image data
    int cropWidthBytes =(width % 8 == 0) ? (width/8) : (width/8)+1 ;

    croppedImage = (unsigned char*)malloc(cropWidthBytes*height);

    int indexCounter = 0;

    // skipping rows that are above(in height) the required pixel data
    for(int i=1; i<= (bm_height-height-y) +1 ; i++)
    {
        indexCounter += width_bytes; // for skipping a row of pixel data from "bitmapImage"
    }

    int idx = 0;

    for(int h=1 ; h <= height ; h++ )
    {
        int k=1;

        for(int bit=x ; bit <= (x+width-1) ; bit++)
        {
            int ans = GetBit(bitmapImage+indexCounter , bit-1);

            if(ans==1){
                SetBit(croppedImage+idx, k-1);
            }
            else{
                ClearBit(croppedImage+idx, k-1);
            }
            k++;
        }
        idx += cropWidthBytes;
        indexCounter += width_bytes;
    }

    // creating a new .bmp file for cropped image
    int junkBytes = (cropWidthBytes % 4) ? 4 - (cropWidthBytes % 4) : 0 ;
    BYTE junk = 0x00;

    bitmapInfoHeader->biWidth = width;
    bitmapInfoHeader->biHeight = height;
    bitmapInfoHeader->biSizeImage = (height*cropWidthBytes) + (junkBytes*height);
    
    FILE* croppedImage_ptr;
    croppedImage_ptr = fopen("C:\\Users\\ASUS\\Desktop\\cropped_image\\cropped_1bit.bmp","wb");

    WriteBmpFileHeader( bitmapFileHeader, croppedImage_ptr);
    
    WriteBmpInfoHeader( bitmapInfoHeader, croppedImage_ptr);

    fwrite( colorTable, 8, 1 , croppedImage_ptr);

    fseek(croppedImage_ptr, bitmapFileHeader->bfOffBits, SEEK_SET);

    int index=0;
    for(int h=1; h<=height ; h++)
    {
        fwrite(croppedImage + index, cropWidthBytes, 1, croppedImage_ptr);

        for(int i=1; i<= junkBytes ; i++)
            fwrite(&junk, 1, 1, croppedImage_ptr);

        index += cropWidthBytes;

    }
    fclose(croppedImage_ptr);
    
}

void cropImage_4bit( BITMAPFILEHEADER* bitmapFileHeader, BITMAPINFOHEADER* bitmapInfoHeader, FILE* filePtr, int x, int y, int width, int height)
{
    // store 4-bit color table information - 16 colors * 4 bytes
    unsigned char* colorTable;
    colorTable = (unsigned char*)malloc(64);

    fread(colorTable, 64, 1 , filePtr);

    int rowJunkByte; // stores the no. of junk bytes added per row, to make the pixel data row a multiple of 4 bytes - in input image

    // extract "width" and "height" information of image :- "in terms of no. of pixels"
    int bm_width = bitmapInfoHeader->biWidth;
    int bm_height = bitmapInfoHeader->biHeight;

    printf("width of image in pixel : %d  \n", bm_width);
    printf("height of image in pixel : %d  \n", bm_height);
    printf("Bits per pixel of image : %d  \n", bitmapInfoHeader->biBitCount);

    // calculate junkByte added per row 
    int width_bytes = (bm_width % 2 == 0) ? (bm_width/2) : (bm_width/2)+1 ; // for handling odd no. of pixels in width

    if((bitmapInfoHeader->biSizeImage - (bm_height * width_bytes)) == 0){
        rowJunkByte = 0;
    }
    else{
        rowJunkByte = ((bitmapInfoHeader->biSizeImage - (bm_height * width_bytes)) / bm_height);
    }
    printf("No. of junk-Bytes in a row: %d \n", rowJunkByte);

    //move "file pointer" to the beginning of Bitmap Pixel Data Byte
    fseek(filePtr, bitmapFileHeader->bfOffBits, SEEK_SET);  

    //a pointer of type unsigned char* - to store "input image" pixel data bytes: 1 byte = 2 pixels
    unsigned char* bitmapImage;  
    //allocate enough memory for the bitmap image data
    bitmapImage = (unsigned char*)malloc(bitmapInfoHeader->biSizeImage);

    //check if memory allocation failed
    if (bitmapImage == NULL)
    {
        printf("bit-map-image MEMORY ALLOCATION FAILED");
        //free(bitmapImage);
        fclose(filePtr);
        return;
    }

    //read the bitmap image pixel data - skipping the "padding bytes"
    if(rowJunkByte == 0){
        fread(bitmapImage, bitmapInfoHeader->biSizeImage, 1 , filePtr);
    }
    else{
        int i=0;

        for(int height=1; height <= bm_height; height++){

            // read, image pixel data bytes from each row 
            fread(bitmapImage + i, width_bytes , 1 , filePtr);

            // skip the junk-bytes
            fseek(filePtr, rowJunkByte , SEEK_CUR); 

            i = i + width_bytes;
        }
    }
    fclose(filePtr);

    unsigned char* croppedImage;  
    //allocate enough memory for the bitmap image data
    int cropWidthBytes =(width % 2 == 0) ? (width/2) : (width/2)+1 ;

    croppedImage = (unsigned char*)malloc(cropWidthBytes*height);

    int indexCounter = 0;

    // skipping rows that are above(in height) the required pixel data
    for(int i=1; i<= (bm_height-height-y) +1 ; i++)
    {
        indexCounter += width_bytes; // for skipping a row of pixel data from "bitmapImage"
    }

    // iterating over the height (row by row), of the required cropped image
    BYTE low_byte = 0;

    int k=0;
    BYTE low=0, high=0;

    for(int h=1; h <= height; h++)
    {        
        int nextRowIndex = indexCounter + width_bytes;

        if(x%2 == 0) // if 'x' is even
        {
            int widthPixelCount = width;

            indexCounter += ((x/2)-1); // skip (x/2)-1 bytes, x=1 case NOT handled

            // get the lower 4-bits
            BYTE temp = bitmapImage[indexCounter];
            // printf("%x - ",temp);
            low_byte = temp & 0x0F ;
             // printf("%x - ",low_byte);

            croppedImage[k] = (low_byte << 4 ); // shift the pixel to upper 4-bits position
            widthPixelCount--;

            indexCounter++;

            //copying the row pixel data 
            while(widthPixelCount > 0)
            {
                temp = bitmapImage[indexCounter];

                low =  temp & 0x0F ;
                high = temp >> 4;

                croppedImage[k] = (croppedImage[k] | high);
                widthPixelCount--;
                k++;

                if(widthPixelCount != 0) // for handling case : x=even & width=odd
                {
                    croppedImage[k] = (low << 4) ;
                    widthPixelCount--;

                    if(widthPixelCount == 0){
                        k++;
                    }
                }
                indexCounter++;
            }

            //skipping the pixels after the width of the required crop image
            indexCounter = nextRowIndex;
        }
        else // for handling case , when x=odd
        {
            indexCounter += (x/2)-1; // skip (x/2) bytes 

            int widthPixelCount = width;
            BYTE temp;

            //copying the row pixel data 
            while(widthPixelCount > 0)
            {
                temp = bitmapImage[indexCounter];

                low =  temp & 0x0F;
                high = temp & 0xF0;

                croppedImage[k] = (croppedImage[k] | high);
                widthPixelCount--;

                if(widthPixelCount != 0) // for handling case, where, x=odd & width=odd
                {
                    croppedImage[k] = (croppedImage[k] | low);
                    widthPixelCount--;
                }

                indexCounter++;
                k++;
            }

            //skipping the pixels after the width of the required crop image
            indexCounter = nextRowIndex;
        }
    }

    // creating a new .bmp file for cropped image
    int junkBytes = (cropWidthBytes % 4) ? 4 - (cropWidthBytes % 4) : 0 ;
    BYTE junk = 0x00;

    bitmapInfoHeader->biWidth = width;
    bitmapInfoHeader->biHeight = height;
    bitmapInfoHeader->biSizeImage = (height*cropWidthBytes) + (junkBytes*height);
    
    FILE* croppedImage_ptr;
    croppedImage_ptr = fopen("C:\\Users\\ASUS\\Desktop\\cropped_image\\cropped_4bit.bmp","wb");

    WriteBmpFileHeader( bitmapFileHeader, croppedImage_ptr);
    
    WriteBmpInfoHeader( bitmapInfoHeader, croppedImage_ptr);

    fwrite( colorTable, 64, 1 , croppedImage_ptr);

    fseek(croppedImage_ptr, bitmapFileHeader->bfOffBits, SEEK_SET);

    int index=0;
    for(int h=1; h<=height ; h++)
    {
        fwrite(croppedImage + index, cropWidthBytes, 1, croppedImage_ptr);

        for(int i=1; i<= junkBytes ; i++)
            fwrite(&junk, 1, 1, croppedImage_ptr);

        index += cropWidthBytes;

    }
    fclose(croppedImage_ptr);
}

void cropImage_8bit( BITMAPFILEHEADER* bitmapFileHeader, BITMAPINFOHEADER* bitmapInfoHeader, FILE* filePtr, int x, int y, int width, int height)
{
    // store 8-bit color table information: 256 colors* 4 bytes
    unsigned char* colorTable;
    colorTable = (unsigned char*)malloc(1024);

    fread(colorTable, 1024, 1 , filePtr);

    int rowJunkByte; // stores the no. of junk bytes added per row, to make the pixel data row a multiple of 4 bytes - in input image

    // extract "width" and "height" information of image :- in terms of no. of pixels
    int bm_width = bitmapInfoHeader->biWidth;
    int bm_height = bitmapInfoHeader->biHeight;

    printf("width of image in pixel : %d  \n", bm_width);
    printf("height of image in pixel : %d  \n", bm_height);
    printf("Bits per pixel of image : %d  \n", bitmapInfoHeader->biBitCount);

    // calculate junkByte added per row 
    if(bitmapInfoHeader->biSizeImage - (bm_height * bm_width) == 0){
        rowJunkByte = 0;
    }
    else{
        rowJunkByte = ((bitmapInfoHeader->biSizeImage - (bm_height * bm_width)) / bm_height);
    }
    printf("No. of junk-Bytes in a row: %d \n", rowJunkByte);

    //move "file pointer" to the beginning of Bitmap Pixel Data Byte
    fseek(filePtr, bitmapFileHeader->bfOffBits, SEEK_SET);  

    //a pointer of type unsigned char* - to store "input image" pixel data bytes
    unsigned char* bitmapImage;  
    //allocate enough memory for the bitmap image data
    bitmapImage = (unsigned char*)malloc(bitmapInfoHeader->biSizeImage);

    //check if memory allocation failed
    if (bitmapImage == NULL)
    {
        printf("bit-map-image MEMORY ALLOCATION FAILED");
        //free(bitmapImage);
        fclose(filePtr);
        return;
    }

    //read the bitmap image pixel data - skipping the "padding bytes"
    if(rowJunkByte == 0){
        fread(bitmapImage, bitmapInfoHeader->biSizeImage, 1 , filePtr);
    }
    else{
        int i=0;

        for(int height=1; height <= bm_height; height++){

            // read, image pixel data bytes from each row 
            fread(bitmapImage + i, bm_width , 1 , filePtr);

            // skip the junk-bytes
            fseek(filePtr, rowJunkByte , SEEK_CUR); 

            i = i + (bm_width);
        }
    }
    fclose(filePtr);

    unsigned char* croppedImage;  
    //allocate enough memory for the bitmap image data
    croppedImage = (unsigned char*)malloc(width*height);

    int indexCounter = 0;

    // skipping rows that are above(in height) the required pixel data
    for(int i=1; i<= (bm_height-height-y+1); i++)
    {
        indexCounter += (bm_width);
    }

    // iterating over the height, of the required cropped image
    int k=0;
    for(int h=1; h <= height; h++)
    {
        // skipping starting 'x' pixels
        indexCounter += x-1;

        //copying the row pixel data 
        for(int w=1; w<=width ; w++)
        {
            croppedImage[k] = bitmapImage[indexCounter];

            indexCounter++;
            k++;
        }

        //skipping the pixels after the width of the required crop image
        indexCounter += (bm_width-x-width+1);
    }

    // creating a new .bmp file for cropped image
    int junkBytes = ((width)%4) ? 4 - ((width) % 4) : 0 ;
    BYTE junk = 0x00;

    bitmapInfoHeader->biWidth = width;
    bitmapInfoHeader->biHeight = height;
    bitmapInfoHeader->biSizeImage = (height*width) + (junkBytes*height);
    
    FILE* croppedImage_ptr;
    croppedImage_ptr = fopen("C:\\Users\\ASUS\\Desktop\\cropped_image\\cropped_8bit.bmp","wb");

    WriteBmpFileHeader( bitmapFileHeader, croppedImage_ptr);
    
    WriteBmpInfoHeader( bitmapInfoHeader, croppedImage_ptr);

    fwrite( colorTable, 1024, 1 , croppedImage_ptr);

    fseek(croppedImage_ptr, bitmapFileHeader->bfOffBits, SEEK_SET);
    int index=0;
    for(int h=1; h<=height ; h++)
    {
        fwrite(croppedImage + index, width, 1, croppedImage_ptr);

        for(int i=1; i<= junkBytes ; i++)
            fwrite(&junk, 1, 1, croppedImage_ptr);

        index += width;

    }
    fclose(croppedImage_ptr);
}


void cropImage_24bit( BITMAPFILEHEADER* bitmapFileHeader, BITMAPINFOHEADER* bitmapInfoHeader, FILE* filePtr, int x, int y, int width, int height)
{
    int rowJunkByte; // stores the no. of junk bytes added per row, to make the pixel data row a multiple of 4 bytes - in input image

    // extract "width" and "height" information of image :- in terms of no. of pixels
    int bm_width = bitmapInfoHeader->biWidth;
    int bm_height = bitmapInfoHeader->biHeight;

    printf("width of image in pixel : %d  \n", bm_width);
    printf("height of image in pixel : %d  \n", bm_height);
    printf("Bits per pixel of image : %d  \n", bitmapInfoHeader->biBitCount);

    // calculate junkByte added per row 
    if(bitmapInfoHeader->biSizeImage - (bm_height * bm_width * 3) == 0){
        rowJunkByte = 0;
    }
    else{
        rowJunkByte = ((bitmapInfoHeader->biSizeImage - (bm_height * bm_width * 3)) / bm_height);
    }
    printf("No. of junk-Bytes in a row: %d \n", rowJunkByte);

    //move "file pointer" to the beginning of Bitmap Pixel Data Byte
    fseek(filePtr, bitmapFileHeader->bfOffBits, SEEK_SET);  

    //a pointer of type unsigned char* - to store image pixel data byte
    unsigned char* bitmapImage;  
    //allocate enough memory for the bitmap image data
    bitmapImage = (unsigned char*)malloc(bitmapInfoHeader->biSizeImage);

    //check if memory allocation failed
    if (bitmapImage == NULL)
    {
        printf("bit-map-image MEMORY ALLOCATION FAILED");
        //free(bitmapImage);
        fclose(filePtr);
        return;
    }

    //read the bitmap image pixel data - skipping the "padding bytes"
    if(rowJunkByte == 0){
        fread(bitmapImage, bitmapInfoHeader->biSizeImage, 1 , filePtr);
    }
    else{
        int i=0;

        for(int height=1; height <= bm_height; height++){

            // read, image pixel data bytes from each row 
            fread(bitmapImage + i, bm_width*3 , 1 , filePtr);

            // skip the junk-bytes
            fseek(filePtr, rowJunkByte , SEEK_CUR); 

            i = i + (bm_width*3);
        }
    }

    fclose(filePtr);

    unsigned char* croppedImage;  
    //allocate enough memory for the bitmap image data
    croppedImage = (unsigned char*)malloc(width*height*3);

    int indexCounter = 0;

    // skipping rows that are above(in height) the required pixel data
    for(int i=1; i<= (bm_height-height-y); i++)
    {
        indexCounter += (bm_width*3);
    }

    // iterating over the height, of the required cropped image
    int k=0;
    for(int h=1; h <= height; h++)
    {
        // skipping starting 'x' pixels
        indexCounter += (x*3);

        //copying the row pixel data 
        for(int w=1; w<=width ; w++)
        {
            croppedImage[k] = bitmapImage[indexCounter];
            croppedImage[k+1] = bitmapImage[indexCounter+1];
            croppedImage[k+2] = bitmapImage[indexCounter+2];

            indexCounter += 3;
            k += 3;
        }

        //skipping the pixels after the width of the required crop image
        indexCounter += ((bm_width-x-width)*3);
    }

    // creating a new .bmp file for cropped image
    int junkBytes = ((width*3)%4)? 4 - ((width*3) % 4) : 0 ;
    BYTE junk = 0x00;

    bitmapInfoHeader->biWidth = width;
    bitmapInfoHeader->biHeight = height;
    bitmapInfoHeader->biSizeImage = (height*width*3) + (junkBytes*height);
    
    FILE* croppedImage_ptr;
    croppedImage_ptr = fopen("C:\\Users\\ASUS\\Desktop\\cropped_image\\cropped_24bit.bmp","wb");

    WriteBmpFileHeader( bitmapFileHeader, croppedImage_ptr);
    
    WriteBmpInfoHeader( bitmapInfoHeader, croppedImage_ptr);

    fseek(croppedImage_ptr, bitmapFileHeader->bfOffBits, SEEK_SET);
    int index=0;
    for(int h=1; h<=height ; h++)
    {
        fwrite(croppedImage + index, width*3, 1, croppedImage_ptr);

        for(int i=1; i<= junkBytes ; i++)
            fwrite(&junk, 1, 1, croppedImage_ptr);

        index += width*3;

    }
    fclose(croppedImage_ptr);

}

void cropImageHelper(const char* inputFile, int x, int y, int width, int height)
{
    FILE *filePtr; //file pointer for pointing to .bitmap file - input

    BITMAPFILEHEADER bitmapFileHeader; //struct to store "Header File"
    BITMAPINFOHEADER bitmapInfoHeader; //struct to store "Header Info"

    filePtr = fopen(inputFile,"rb");  // open .bitmap file in "read binary" mode

    if (filePtr == NULL) // file doesnt exist
    {
        printf("CANNOT OPEN FILE, FILE DOESN'T EXIST \n");
        return ;
    }

    ReadBmpFileHeader( &bitmapFileHeader, filePtr);  //read the bitmap File header

    //verify that this is a .bmp file by checking "bfType" property
    if (bitmapFileHeader.bfType != 0x4D42) // in memory the value in 2 bytes is stored as "0x42 0x4D" - values are written in little-endian order (least-significant byte first)
    {
        fclose(filePtr);
        printf("NOT A .BMP FILE");
        return ;
    }

    ReadBmpInfoHeader( &bitmapInfoHeader, filePtr);  //read the bitmap Info header

    if(bitmapInfoHeader.biBitCount == 24){
        cropImage_24bit(&bitmapFileHeader, &bitmapInfoHeader, filePtr, x, y, width, height );
    }
    else if(bitmapInfoHeader.biBitCount == 8){
        cropImage_8bit(&bitmapFileHeader, &bitmapInfoHeader, filePtr, x, y, width, height );
    }
    else if(bitmapInfoHeader.biBitCount == 4){
        cropImage_4bit(&bitmapFileHeader, &bitmapInfoHeader, filePtr, x, y, width, height );
    }
    else if(bitmapInfoHeader.biBitCount == 1){
        cropImage_1bit(&bitmapFileHeader, &bitmapInfoHeader, filePtr, x, y, width, height );
    }
}

void main()
{
    int x=3, y=4, width=1500, height=1500;

    // cropImageHelper("C:\\Users\\ASUS\\Desktop\\IP_task\\24bit.bmp", x, y, width, height);

    // printf("------------------------------------------ \n");

    cropImageHelper("C:\\Users\\ASUS\\Desktop\\IP_task\\lic.bmp", x, y, width, height);

    // printf("------------------------------------------ \n");

    // cropImageHelper("C:\\Users\\ASUS\\Desktop\\IP_task\\4bit.bmp", x, y, width, height);

    // printf("------------------------------------------ \n");

    // cropImageHelper("C:\\Users\\ASUS\\Desktop\\IP_task\\1bit.bmp", x, y, width, height);


    return;
}