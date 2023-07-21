#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum
{
	IMAGE_PALETTE_NONE = 0,
	IMAGE_PALETTE_16 = 1,
	IMAGE_PALETTE_256 = 2
} ImagePalette;

bool LoadPCX(u8* Data, sImage* Image, u8* PaletteType)
{
	if (!Data)
		return false;

	PCXHeader* Header = (PCXHeader*)Data;
	Data += sizeof(PCXHeader);

	Image->width = Header->xmax - Header->xmin + 1;
	Image->height = Header->ymax - Header->ymin + 1;
	Image->bpp = Header->bitsPerPixel;

	s32 Colors = (s32)(1L << (Header->bitsPerPixel * Header->colorPlanes));

	if (Colors == 16)
		*PaletteType = IMAGE_PALETTE_16;
	else if (Colors == 256)
		*PaletteType = IMAGE_PALETTE_256;
	else
		*PaletteType = IMAGE_PALETTE_NONE;
	

	size_t PixelSize = Image->width * Image->height;//((Header->bitsPerPixel * Header->colorPlanes) * Image->width * Image->height) / 8;
	printf("Pixel Size: %lu\n", PixelSize);

	u8* Pixels = (u8*)malloc(PixelSize);

	if (!Pixels)
		return false;

	Image->image.data8 = Pixels;

/*
	{
		//WIP-Decompression
		u32 runcount = 0;
		u8 runvalue;
		u32 total = 0;
		u32 index = 0;
		do
		{
			u8 byte = *(Data++);                  
			if ((byte & 0xC0) == 0xC0) {         
				runcount = byte & 0x3F;              
				runvalue = *(Data++);          
			}
			else                                   
			{
				runcount = 1;                    
				runvalue = byte;                   
			}
			
			for (total += runcount;               
				runcount && index < PixelSize;    
				runcount--, index++)                 
				Pixels[index] = runvalue;             
		} while (index < PixelSize);
	}
*/

	unsigned char byte, color, color1, color2;
	u8* BytePtr;
	int count, i;
	int index = 0;

	switch (Colors) {
    case 256: // 256-color
        while (index < PixelSize) 
		{
			byte = *(Data++);
         //   fread(&byte, sizeof(unsigned char), 1, inputFile);
            if ((byte & 0xc0) == 0xc0) 
			{
                count = byte & 0x3f;
				color = *(Data++);
             //   fread(&color, sizeof(unsigned char), 1, inputFile);
                for (i = 0; i < count; ++i) 
				{
                    Pixels[index++] = color;
                }
            } else 
			{
                Pixels[index++] = byte;
            }
        }
		printf("256: rle: %d\n", Header->encoding);
        break;
        
    case 16: // 16-color
        // Similar to 256-color decoding, but with a different run-length encoding scheme
        // Implement the decoding logic for 16-color mode here
	//	decompress(Data, Pixels, RLE);
		bool UpperTurn = true;
		count = 0;
		while (index < PixelSize/2) 
		{
			count++;
			BytePtr = (Data++);
			byte = *BytePtr;

			u8 Upper = (byte >> 4) & 0x0F;
			u8 Lower = byte & 0x0F;



			if ((Upper & 0xc0) == 0xc0) 
			{
                count = Upper & 0x3f;
			//	color = *(Data++);
             //   fread(&color, sizeof(unsigned char), 1, inputFile);
                for (i = 0; i < count; ++i) 
				{
                    if (UpperTurn)
					{
						UpperTurn = false;
						Pixels[index] = 0;
					//	Pixels[index] &= 0x0F;
					//	Pixels[index] |= ((Lower & 4));
					//	Pixels[index] = Lower >> 4;
						Pixels[index] = (Pixels[index] & 0x0F) | ((Lower << 4) & 0xF0);
					}
					else
					{
					//	Pixels[index] &= 0x0F;
					//	Pixels[index] |= ((Lower << 4) & 0xF0);
						Pixels[index] = (Pixels[index] & 0xF0) | (Lower & 0x0F);
						index++;
						UpperTurn = true;
					}
                }
            } else 
			{
                if (UpperTurn)
				{
					UpperTurn = false;
					Pixels[index] = 0;
				//	Pixels[index] &= 0x0F;
				//	Pixels[index] |= ((Lower & 4));
				//	Pixels[index] = Lower >> 4;
					Pixels[index] = (Pixels[index] & 0x0F) | ((Upper << 4) & 0xF0);
				}
				else
				{
				//	Pixels[index] &= 0x0F;
				//	Pixels[index] |= ((Lower << 4) & 0xF0);
					Pixels[index] = (Pixels[index] & 0xF0) | (Upper & 0x0F);
					index++;
					UpperTurn = true;
				}
            }

		//	if (BytePtr[0] == 0x41 &&
		//		BytePtr[1] == 0x13)
		//		break;
		}
		/*
		bool UpperTurn = true;
		count = 0;
		while (index < PixelSize/2) 
		{
			count++;
			BytePtr = (Data++);
			byte = *BytePtr;

			u8 Upper = (byte >> 4) & 0x0F;
			u8 Lower = byte & 0x0F;

			


			for (u8 i = 0; i < Upper; i++)
			{
				//Pixels[index++]  = Lower;
				
				if (UpperTurn)
				{
					UpperTurn = false;
					Pixels[index] = 0;
				//	Pixels[index] &= 0x0F;
				//	Pixels[index] |= ((Lower & 4));
				//	Pixels[index] = Lower >> 4;
					Pixels[index] = (Pixels[index] & 0x0F) | ((Lower << 4) & 0xF0);
				}
				else
				{
				//	Pixels[index] &= 0x0F;
				//	Pixels[index] |= ((Lower << 4) & 0xF0);
					Pixels[index] = (Pixels[index] & 0xF0) | (Lower & 0x0F);
					index++;
					UpperTurn = true;
				}
				
			}

		//	if (BytePtr[0] == 0x41 &&
		//		BytePtr[1] == 0x13)
		//		break;
		}
		*/
		printf("Yey: %d\n", count);
	//	swiDelay(3000);
		
	//	memcpy(Pixels, Data, PixelSize / 2);
	//	Data += PixelSize / 2;
/*
		while (count < PixelSize) {
			unsigned char byte = *Data++;
			if ((byte & 0xC0) == 0xC0) {
				int runLength = byte & 0x3F;
				byte = *Data++;

				for (int i = 0; i < runLength; i++) {
					*Pixels++ = byte;
					count++;
				}
			} else {
				*Pixels++ = byte;
				count++;
			}
		}
		*/
/*
	while (count < PixelSize) {
        unsigned char byte = *Data++;

        if ((byte & 0xC0) == 0xC0) {
            int runLength = byte & 0x3F;
            unsigned char colorIndex1 = *Data >> 4;
            unsigned char colorIndex2 = *Data & 0x0F;

            for (int i = 0; i < runLength; i++) {
              //  *Pixels++ = colorIndex1;
              //  *Pixels++ = colorIndex2;
			  *Pixels++ = (byte << 4) | (byte &0x0F);
                count += 2;
            }

            Data++;
        } else {
            unsigned char colorIndex1 = byte >> 4;
            unsigned char colorIndex2 = byte & 0x0F;
		//	*Pixels++ = byte;
			*Pixels++ = (byte << 4) | (byte &0x0F);
        //    *Pixels++ = colorIndex1;
        //    *Pixels++ = colorIndex2;
            count += 2;
        }
    }
	*/
/*
		while (index < PixelSize) 
		{
			byte = *(Data++);
			if ((byte & 0xc0) == 0xc0) {
				count = byte & 0x3f;
				color1 = *(Data++);
				color2 = *(Data++);
			//	fread(&color1, sizeof(unsigned char), 1, inputFile);
			//	fread(&color2, sizeof(unsigned char), 1, inputFile);
				
				for (i = 0; i < count; ++i) {
					if (i % 2 == 0) {
						Pixels[index++] = color1 >> 4; // Upper 4 bits
					} else {
						Pixels[index++] = color1 & 0x0f; // Lower 4 bits
					}
					
					if (index >= PixelSize) {
						break;
					}
					
					if (i % 2 == 0) {
						Pixels[index++] = color2 >> 4; // Upper 4 bits
					} else {
						Pixels[index++] = color2 & 0x0f; // Lower 4 bits
					}
				}
			} else {
				Pixels[index++] = byte;
			}
		}
*/
/*
	while (index < PixelSize) 
		{
			byte = *(Data++);
         //   fread(&byte, sizeof(unsigned char), 1, inputFile);
            if ((byte & 0xc0) == 0xc0) 
			{
                count = byte & 0x3f;
				color = *(Data++);
             //   fread(&color, sizeof(unsigned char), 1, inputFile);
                for (i = 0; i < count; ++i) 
				{
                    Pixels[index++] = color;
                }
            } else 
			{
                Pixels[index++] = byte;
            }
        }
*/
/*
		u32 ScanLineWidth = Header->bytesPerLine * Header->colorPlanes;
		u32 ScanLineHeight = Image->height;

		

		for (index = 0; index < ScanLineHeight; index++) 
		for (i = 0; i < ScanLineWidth; i++) 
		{
			byte = *(Data++);
			color = *(Data++);
			for (u8 j = 0; j < byte; j++) 
			{
				*(Pixels++) = color;
			}
		}

		printf("16: W: %d, H: %d, %d/%d\n", ScanLineWidth, ScanLineHeight, ScanLineWidth * ScanLineHeight, PixelSize);
*/
/*
		while (index < PixelSize) 
		{
			byte = *(Data++);
         //   fread(&byte, sizeof(unsigned char), 1, inputFile);
            if ((byte & 0xc0) == 0xc0) 
			{
                count = byte & 0x3f;
				color = *(Data++);
             //   fread(&color, sizeof(unsigned char), 1, inputFile);
                for (i = 0; i < count; ++i) 
				{
                    Pixels[index++] = color;
                }
            } else 
			{
                Pixels[index++] = byte;
            }
        }
*/


/*
		int scanlineSize = Image->width;
		int SizeImages = scanlineSize * Image->height;
		int decompressedIndex = 0;

	//	int decompressedIndex = 0;
		int count = 0;
		unsigned char pixel = 0;

		for (int i = 0; i < scanlineSize * (Image->height); i++) {
			unsigned char byte = Data[i];
			if (count == 0) {
				count = byte & 0x3F;
				pixel = (byte >> 6) & 0x03;
			} else {
			//	Pixels[decompressedIndex++] = (byte << 4) | (pixel & 0x0F);
				Pixels[decompressedIndex++] = pixel;
				Pixels[decompressedIndex++] = byte;
				count--;
			}

			if (decompressedIndex >= SizeImages) {
				break;
			}
		}
		*/
/*
		for (int i = 0; i < scanlineSize * (Image->height); i++) 
		{
			unsigned char byte = Data[i];
			if (byte & 0xC0) {
				int count = byte & 0x3F;
				unsigned char pixel = Data[++i];
				for (int j = 0; j < count; j++) {
					Pixels[decompressedIndex++] = pixel;
				}
			} else {
				Pixels[decompressedIndex++] = byte;
			}

			if (decompressedIndex >= SizeImages) {
				break;
			}
		}
*/
	//	index = 0;
	//	for (i = 0; i < decompressedIndex; i += 2) 
	//	{
	//		u8 Upper = Pixels[i];
	//		u8 Lower = Pixels[i + 1];
	//
	//		Pixels[index++] = (Upper << 4) | (Lower & 0x0F);
	//	}

	//	for (int i = 0; i < PixelSize; i++) 
	//	{
	//		unsigned char index = Pixels[i];
	//		unsigned char colorIndex1 = (index >> 4) & 0x0F;
	//		unsigned char colorIndex2 = index & 0x0F;
//
	//		// Do something with the color indexes...
	//		printf("Pixel %d: ColorIndex1 = %d, ColorIndex2 = %d\n", i, colorIndex1, colorIndex2);
	//	}


        break;
        
    case 16777216: // True color
		PixelSize *= 3;
		Pixels = (u8*)realloc(Pixels, PixelSize);
		memcpy(Pixels, Data, PixelSize);
		Data += PixelSize;
		printf("true\n");
        break;
}

	{
		u16* Palette;
		if (Colors == 256)
		{
			
			Palette = (u16*)malloc(256 * sizeof(u16));
			if (!Palette)
				return false;

			while(*(Data++) != 0x0C);

			for (u16 i = 0; i < 256; i++)
			{				
				u8 r = *(Data++);				
				u8 g = *(Data++);
				u8 b = *(Data++);

				Palette[i] = ARGB16(1, (r >> 3) & 31, (g >> 3) & 31, (b >> 3) & 31);
			}

			printf("256 palette\n");
		}
		else
		{
			
			printf("16 palette Start\n");
			u16 j = 0;
			Palette = (u16*)malloc(16 * sizeof(u16));
			
			if (!Palette)
			{
				
				Image->palette = NULL;
				free(Pixels);
				printf("16 palette\n");
				exit(0);//just for testing purpose
				return false;
			}
			

			for (u8 i = 0; i < 48; i += 3)
			{				
				u8 r = Header->palette16[i];				
				u8 g = Header->palette16[i + 1];
				u8 b = Header->palette16[i + 2];

				Palette[j++] = ARGB16(1, (r >> 3) & 31, (g >> 3) & 31, (b >> 3) & 31);
			}

			
		}

		Image->palette = (unsigned short*)Palette;
	}
}