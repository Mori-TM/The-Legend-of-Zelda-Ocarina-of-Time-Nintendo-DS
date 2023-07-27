#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum
{
	IMAGE_PALETTE_NONE = 0,
	IMAGE_PALETTE_16 = 1,
	IMAGE_PALETTE_256 = 2
} ImagePalette;

//Implementation by GIMP: https://github.com/MichaelMure/gimp-plugins/blob/master/common/file-pcx.c#L558
//And slightly modified by me
u8* ReadLine(u8* Data, u8* Buffer, s32 Bytes)
{
	static u8 Count = 0;
	static u8 Value = 0;

	while (Bytes--)
	{
		if (Count == 0)
		{
			Value = *(Data++);
			if (Value < 0xc0)
			{
				Count = 1;
			}
			else
			{
				Count = Value - 0xc0;
				Value = *(Data++);
			}
		}
		Count--;
		*(Buffer++) = Value;
	}

	return Data;
}

void Load4BitPCX(u8* Data, s32 Width, s32 Height, u8 *Buffer, u32 Bytes)
{
	s32 x;
	s32 y;
	s32 c;
	
	u8* Line = malloc(Bytes);
	if (!Line)
		return;

	for (y = 0; y < Height; Buffer += Width, ++y)
	{
		for (x = 0; x < Width; ++x)
			Buffer[x] = 0;
			
		for (c = 0; c < 4; ++c)
		{
			Data = ReadLine(Data, Line, Bytes);
			for (x = 0; x < Width; ++x)
			{
				if (Line[x / 8] & (128 >> mod32(x, 8)))
				Buffer[x] += (1 << c);
			}
		}
	}

	free(Line);
}

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
	
	
	size_t PixelSize = Image->width * Image->height;

	u8* Pixels = (u8*)malloc(PixelSize);

	if (!Pixels)
		return false;

	Image->image.data8 = Pixels;

	u8 Byte;
	u8 Color;
	s32 Count;
	s32 i;
	s32 Index = 0;

	switch (Colors) 
	{
    case 256: // 256-color
        while (Index < PixelSize) 
		{
			Byte = *(Data++);
            if ((Byte & 0xc0) == 0xc0) 
			{
                Count = Byte & 0x3f;
				Color = *(Data++);
                for (i = 0; i < Count; ++i) 
				{
                    Pixels[Index++] = Color;
                }
            } else 
			{
                Pixels[Index++] = Byte;
            }
        }
        break;
        
    case 16: // 16-color
		Load4BitPCX(Data, Image->width, Image->height, Pixels, Header->bytesPerLine);

		Index = 0;
		i = 0;
		while (Index < PixelSize)
		{
			u8 Upper = Pixels[Index];
			u8 Lower = Pixels[Index + 1];
			
			Pixels[i++] = (Lower << 4) | (Upper & 0x0F);
			Index += 2;
		}
		
		Pixels = (u8*)realloc(Pixels, i);
        break;
        
    case 16777216: // True color
		PixelSize *= 3;
		Pixels = (u8*)realloc(Pixels, PixelSize);
		memcpy(Pixels, Data, PixelSize);
		Data += PixelSize;
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
		}
		else
		{			
			u16 j = 0;
			Palette = (u16*)malloc(16 * sizeof(u16));
			
			if (!Palette)
			{
				Image->palette = NULL;
				free(Pixels);
				exit(0);//if this line is removed game doesn't work, tested on melon ds 0.9.4
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