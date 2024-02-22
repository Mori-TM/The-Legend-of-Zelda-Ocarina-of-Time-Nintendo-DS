#include <errno.h>

 typedef struct
 {
	char game_title[12];
	char game_code[4];
	char maker_code[2];
	u8 unit_code;
	u8 enc_seed_select;
	u8 device_capacity;
	u8 reserved_1[7];
	u16 game_revision;
	u8 version;
	u8 flags;
	u32 arm9_rom_offset;
	u32 arm9_entry_address;
	u32 arm9_ram_address;
	u32 arm9_size;
	u32 arm7_rom_offset;
	u32 arm7_entry_address;
	u32 arm7_ram_address;
	u32 arm7_size;
	u32 fnt_offset;
	u32 fnt_size;
	u32 fat_offset;
	u32 fat_size;
	u32 arm9_ovl_offset;
	u32 arm9_ovl_size;
	u32 arm7_ovl_offset;
	u32 arm7_ovl_size;
	u32 normal_card_control_register_settings;
	u32 secure_card_control_register_settings;
	u32 banner_offset;
	u16 secure_area_crc;
	u16 secure_transfer_timeout;
	u32 arm9_autoload;
	u32 arm7_autoload;
	u64 secure_disable;
	u32 ntr_size;
	u32 header_size;
	u8 reserved_2[56];
	u8 nintendo_logo[156];
	u16 nintendo_logo_crc;
	u16 header_crc;
	u8 reserved_debugger[32];
} RomHeader;

typedef struct 
{
	u8 version;
	u8 reserved_1;
	u16 crc16_v1;
	u8 reserved_2[28];
	u8 icon_chr[512];
	u8 icon_plt[32];
	s16 game_titles[6][128];
} RomBanner;

u32 GetFileSize(FILE *file) {
    fseek(file, 0, SEEK_END);
    return ftell(file);
}

char* basePath;
#define ROOT "nitrofiles"
bool saveAvailable;
char* basePath;
int lastSize;
u8 fsMode;

#define FATONLY

bool DS_InitFS(int argc, char **argv)
{
	#ifndef FATONLY
		if(nitroFSInit(NULL))
		{
			printf("init : done");
			chdir("nitro:/");
			chdir(ROOT);
			if (!argv)
			{
				saveAvailable=false;fsMode=1;
			} // no fat but nitro (gba slot)
			else
			{
				fsMode=2; // nitro and fat
				saveAvailable=true;
				chdir(basePath);
				int r=mkdir("dscraft", S_IRWXU|S_IRGRP|S_IXGRP);//!ROOT!
				if(r!=0 && errno!=EEXIST)saveAvailable=false;
				r=mkdir("dscraft/packs", S_IRWXU|S_IRGRP|S_IXGRP);
				if(r!=0 && errno!=EEXIST)saveAvailable=false;
				r=mkdir("dscraft/worlds", S_IRWXU|S_IRGRP|S_IXGRP);
				if(r!=0 && errno!=EEXIST)saveAvailable=false;
				r=mkdir("dscraft/screens", S_IRWXU|S_IRGRP|S_IXGRP);
				if(r!=0 && errno!=EEXIST)saveAvailable=false;
				
				chdir("nitro:/");
				chdir(ROOT);
			}
			printf("Some Success\n");
			while (1);
			return true;
		}
	#endif
	fsMode=3; // fat only ?
	saveAvailable=false;
	if(!fatInitDefault())return false;
	saveAvailable=true;
	// int r=mkdir("fat:/dscraft", S_IRWXU|S_IRGRP|S_IXGRP);//!ROOT!
	// if(r!=0 && errno!=EEXIST)saveAvailable=false;
	// r=mkdir("fat:/dscraft/packs", S_IRWXU|S_IRGRP|S_IXGRP);
	// if(r!=0 && errno!=EEXIST)saveAvailable=false;
	// r=mkdir("fat:/dscraft/worlds", S_IRWXU|S_IRGRP|S_IXGRP);
	// if(r!=0 && errno!=EEXIST)saveAvailable=false;
	// r=mkdir("fat:/dscraft/screens", S_IRWXU|S_IRGRP|S_IXGRP);
	// if(r!=0 && errno!=EEXIST)saveAvailable=false;
	// chdir("fat:/");
	// chdir(ROOT);
//	int r=mkdir("dscraft", S_IRWXU|S_IRGRP|S_IXGRP);//!ROOT!
//	if(r!=0 && errno!=EEXIST)saveAvailable=false;
//	r=mkdir("dscraft/packs", S_IRWXU|S_IRGRP|S_IXGRP);
//	if(r!=0 && errno!=EEXIST)saveAvailable=false;
//	r=mkdir("dscraft/worlds", S_IRWXU|S_IRGRP|S_IXGRP);
//	if(r!=0 && errno!=EEXIST)saveAvailable=false;
//	r=mkdir("dscraft/screens", S_IRWXU|S_IRGRP|S_IXGRP);
//	if(r!=0 && errno!=EEXIST)saveAvailable=false;
	// chdir("sd:/");
	chdir(ROOT);
	return true;
}

bool InitFileSystem(int Argc, char** Argv)
{
	if (!fatInitDefault())
	{
		if(!nitroFSInit(NULL))
		{
			return false;
		}		
	}

	chdir(ROOT);
	return true;
}

void* DS_OpenFile (char* filename, char* dir, bool bufferize, bool binary)
{
	char path[255];getcwd(path,255);
	chdir(dir);
	FILE* file;

	if(binary==false)file = fopen(filename, "r+");
	else file = fopen(filename, "rb+");
	//DS_Debug("done.");
	
		if(file == NULL){return NULL; printf("Some Error\n");}
	
	if(bufferize == true)
	{
		u8* buffer;
		long lsize;
		fseek (file, 0 , SEEK_END);
		lsize = ftell (file);
		lastSize=lsize;
		rewind (file);
		buffer = (u8*) malloc (lsize);
		
			if(buffer==NULL)printf("Some Error buffer\n");
			
		fread (buffer, 1, lsize, file);
		fclose (file);
		chdir(path);
		return buffer;
	}
	
	chdir(path);
	return file;
}

void* DS_OpenFile2 (char* filename, char* dir, bool binary)
{
	char path[255];getcwd(path,255);
	int r=chdir(dir);
	if(strlen(dir)<=0)r=-1;
	FILE* file;
	
	
		if(binary==false)file = fopen(filename, "r+");
		else file = fopen(filename, "rb+");
	
	
	
		if(file == NULL)printf("Some Error 5 file\n");
		
		u8* buffer;
		long lsize;
		fseek (file, 0 , SEEK_END);
		lsize = ftell (file);
		rewind (file);
		buffer = (u8*) malloc (lsize);
		
			if(buffer==NULL)printf("Some Error buffer 2\n");
			
		fread (buffer, 1, lsize, file);
		fclose (file);
		chdir(path);
		return buffer;
}


char* ArgDef;

char* LoadFile(const char* FileName, int* Length)
{
//	return DS_OpenFile(FileName, "nitrofiles", true, true);
	
//	chdir(ROOT);
	
//	char NewFile[256];
//	sprintf(NewFile, "%s", FileName);
	
	FILE* File = fopen(FileName, "rb");
	if (File)
	{
		fseek(File, 0, SEEK_END);
		int Size = ftell(File);
		fseek(File, 0, SEEK_SET);

		char* Buffer = (char*)malloc(Size + 1);
		if (!Buffer)
		{
			printf("Failed allocating: %s\n", FileName);
			fclose(File);
			return NULL;
		}	
			
		if (fread(Buffer, 1, Size, File) != Size)
		{
			printf("Failed reading: %s\n", FileName);
			fclose(File);
			return NULL;
		}			
			
		Buffer[Size] = 0;
	//	printf("Length: %d\n", Size);
		
	//	*OutBuffer = Buffer;
		if (Length)
			*Length = Size;
		fclose(File);
		
	//	printf("Success to Load: %s\n", FileName);
		return Buffer;
	}
	
	printf("Failed to Load: %s\n", FileName);
	while (1);
	return NULL;
}