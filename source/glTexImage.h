/*
void
vramBlock_init(s_vramBlock* mb) {
	// Construct a new block that will be set as the first block, as well as the first empty block
	struct s_SingleBlock* newBlock = (struct s_SingleBlock*)malloc(sizeof(struct s_SingleBlock));
	memset((void*)newBlock, 0, sizeof(s_SingleBlock));
	newBlock->AddrSet = mb->startAddr;
	newBlock->blockSize = (uint32)mb->endAddr - (uint32)mb->startAddr;

	mb->firstBlock = mb->firstEmpty = newBlock;

	// Default settings and initializations for up to 16 blocks ( will increase	as limit is reached )
	mb->blockCount = 1;
	mb->deallocCount = 0;

	mb->lastExamined = NULL;
	mb->lastExaminedAddr = NULL;
	mb->lastExaminedSize = 0;

	DynamicArrayInit(&mb->blockPtrs, 16);
	DynamicArrayInit(&mb->deallocBlocks, 16);
	int i;
	for (i = 0; i < 16; i++) {
		DynamicArraySet(&mb->blockPtrs, i, (void*)0);
		DynamicArraySet(&mb->deallocBlocks, i, (void*)0);
	}
}


s_vramBlock* vramBlock_Construct(uint8* start, uint8* end) {
	// Block Container is constructed, with a starting and ending address. Then initialization of the first block is made
	struct s_vramBlock* mb = (struct s_vramBlock*)malloc(sizeof(s_vramBlock));
	if (start > end) {
		mb->startAddr = end; mb->endAddr = start;
	}
	else {
		mb->startAddr = start; mb->endAddr = end;
	}
	vramBlock_init(mb);
	return mb;
}


void
vramBlock_terminate(s_vramBlock* mb) {
	// Starts at the container's first block, and goes through each sequential block, deleting them
	struct s_SingleBlock* curBlock = mb->firstBlock;

	while (curBlock != NULL) {
		struct s_SingleBlock* nextBlock = curBlock->node[1];
		free(curBlock);
		curBlock = nextBlock;
	}

	DynamicArrayDelete(&mb->deallocBlocks);
	DynamicArrayDelete(&mb->blockPtrs);
}


void vramBlock_Deconstruct(s_vramBlock* mb) {
	// Container must exist for deconstructing
	if (mb) {
		vramBlock_terminate(mb);
		free(mb);
	}
}

uint8*
vramBlock__allocateBlock(s_vramBlock* mb, struct s_SingleBlock* block, uint8* addr, uint32 size) {
	// Initial tests to ensure allocation is valid
	if (!size || !addr || !block || block->indexOut || addr < block->AddrSet || (addr + size) >(block->AddrSet + block->blockSize)) return 0;

	// Get pointers to the various blocks, as those may change from allocation
	struct s_SingleBlock** first = &mb->firstBlock, ** alloc = &mb->firstAlloc, ** empty = &mb->firstEmpty;
	// The nodes in the test block array will change as examinations of pre/post blocks are done
	struct s_SingleBlock* testBlock[4] = { block->node[0], block->node[1], block->node[2], block->node[3] };

	// Boolean comparisons ( for determining if an empty block set for allocation should be split once, twice, or not at all )
	uint32 valComp[2] = { addr != block->AddrSet, addr + size < block->AddrSet + block->blockSize };

	int i = 0;
	for (; i < 2; i++) {
		// Generate a new block if condition requires it, based on earlier comparison
		if (valComp[i]) {
			// If comparison check is true, then empty block is split in two empty blocks. Addresses set, block sizes corrected, and
			//			nodes linked between the two. This can be done up to two times, resulting in 3 empty blocks sequentially.
			//			The middle in this case will be the true allocated block. If split once in total, either block will be
			//			the true block. Also done is examination of the first block and first empty block, which will be set as well
			struct s_SingleBlock* newBlock = (struct s_SingleBlock*)malloc(sizeof(struct s_SingleBlock));
			newBlock->indexOut = 0;
			newBlock->AddrSet = block->AddrSet + (i * size);

			if (i) {
				newBlock->blockSize = block->blockSize - size;
				block->blockSize = size;
			}
			else {
				newBlock->blockSize = (uint32)addr - (uint32)block->AddrSet;
				block->AddrSet = addr;
				block->blockSize -= newBlock->blockSize;
				if (block == *first)
					*first = newBlock;
			}

			// To get an idea of what the nodes represent, the first two in the array refer to the immediate pre/post blocks, which
			//		can be either empty or allocated blocks. The last two in the array refer to the prior/next blocks of the same type
			//		(empty or allocated), which can skip over blocks of a differing type. This allows for quick examination of similar
			//		blocks while keeping the organization of all the blocks in an ordered fashion. Think of it like a doubly-doubly
			//		linked list
			newBlock->node[1 - i] = block;
			newBlock->node[i] = testBlock[i];
			newBlock->node[i + 2] = testBlock[i + 2];

			block->node[i] = newBlock;
			if (testBlock[i])
				testBlock[i]->node[1 - i] = newBlock;
			if (testBlock[i + 2])
				testBlock[i + 2]->node[3 - i] = newBlock;

			testBlock[i + 2] = newBlock;

			if (block == *empty)
				*empty = newBlock;
		}
	}

	// Finish off node linking (in this case, NULL is possible, which refers to the end of the block sequences)
	if (testBlock[2])
		testBlock[2]->node[3] = testBlock[3];
	if (testBlock[3])
		testBlock[3]->node[2] = testBlock[2];

	// The condition of examining the first empty block is needed, in case the comparison check early is false for both
	if (block == *empty)
		*empty = block->node[3];

	block->node[2] = testBlock[0];
	block->node[3] = testBlock[1];
	if (testBlock[0])
		testBlock[0]->node[3] = block;
	else
		*alloc = block;
	if (testBlock[1])
		testBlock[1]->node[2] = block;

	return (uint8*)block;
}

uint32
vramBlock__deallocateBlock(s_vramBlock* mb, struct s_SingleBlock* block) {
	// Test to see if this is an allocated block
	if (!block->indexOut) return 0;
	struct s_SingleBlock** first = &mb->firstBlock, ** alloc = &mb->firstAlloc, ** empty = &mb->firstEmpty;

	// Unlike empty blocks, allocated blocks can be next to each other to help retain the actual spaces being allocated.
	//		This is why when contructing the Test Block for the deallocation process, it fills it with the prior/next links for
	//		both sets instead of including the immediate pre/post blocks for figuring out the prior/next closest empty block
	struct s_SingleBlock* testBlock[4] = { block->node[2], block->node[3], block->node[2], block->node[3] };

	int i = 0;
	for (; i < 2; i++) {
		// If the immediate prior/next test link is not the block's immediate prior/next link (meaning an empty block
		//		separates them), then set the prior/next link to that empty block
		if (testBlock[i] != block->node[i])
			testBlock[i + 2] = block->node[i];
		else {
			// If not, then scan through the prior/next links until either an empty block is found, or no blocks (NULL) are found
			while (testBlock[i + 2] && testBlock[i + 2]->indexOut)
				testBlock[i + 2] = testBlock[i + 2]->node[i];
		}
	}

	// Begin initial "rewiring" stage for when the block transitions from allocated to empty
	if (testBlock[0])	testBlock[0]->node[3] = testBlock[1];
	if (testBlock[1])	testBlock[1]->node[2] = testBlock[0];
	if (testBlock[2])	testBlock[2]->node[3] = block;
	if (testBlock[3])	testBlock[3]->node[2] = block;

	block->node[2] = testBlock[2];
	block->node[3] = testBlock[3];
	block->indexOut = 0;

	// If this block was the first allocated block of the group, then pass allocation lead to next one, even if that is NULL
	if (block == *alloc)
		*alloc = testBlock[1];

	for (i = 0; i < 2; i++) {
		if (testBlock[i + 2]) {
			// If true, then we must do more rewiring, as well as merging blocks together. This also includes reassigning
			//		the first block and first empty block if necessary
			if (testBlock[i + 2] == block->node[i]) {
				block->node[i] = testBlock[i + 2]->node[i];
				if (block->node[i])		block->node[i]->node[1 - i] = block;
				block->node[i + 2] = testBlock[i + 2]->node[i + 2];
				if (block->node[i + 2])		block->node[i + 2]->node[3 - i] = block;
				block->blockSize += testBlock[i + 2]->blockSize;
				if (!i) {
					block->AddrSet = testBlock[2]->AddrSet;
					if (testBlock[2] == *first)
						*first = block;
				}
				if (testBlock[i + 2] == *empty)
					*empty = block;
				free(testBlock[i + 2]);
				// Even if the above did not happen, there is still a chance the new deallocated block may now be the first
				//	empty block, so assign it if that is the case
			}
			else if (i && testBlock[i + 2] == *empty)
				*empty = block;
		}
	}

	return 1;
}

uint8*
vramBlock_examineSpecial(s_vramBlock* mb, uint8* addr, uint32 size, uint8 align) {
	// Simple validity tests
	if (!addr || !mb->firstEmpty || !size || align >= 8)
		return NULL;
	// Start with the first empty block
	struct s_SingleBlock* block = mb->firstEmpty;

	// Set these value to 0/NULL (should only be filled in with valid data in case of error), and copy the address to start checking
	mb->lastExamined = NULL;
	mb->lastExaminedAddr = NULL;
	mb->lastExaminedSize = 0;
	uint8* checkAddr = addr;

	// If the address is within a valid block, examine if it will initially fit in it
	while (block && checkAddr >= block->AddrSet + block->blockSize)
		block = block->node[3];
	if (!block) return NULL;

	// Move the address up if before the first valid block
	if (checkAddr < block->AddrSet)
		checkAddr = block->AddrSet;

	uint8* bankLock[5] = { 0x0 };
	uint32 bankSize[5] = { 0x0 };
	uint32 curBank = 0;

	// Values that hold which banks to examine
	uint32 isNotMainBank = (checkAddr >= (uint8*)VRAM_E ? 1 : 0);
	uint32 vramCtrl = (isNotMainBank ? VRAM_EFG_CR : VRAM_CR);
	int vramLock = glGlob->vramLock[isNotMainBank];
	uint32 i = 0, iEnd = (isNotMainBank ? 3 : 4);

	// Fill in the array with only those banks that are not set for Textures or Texture Palettes
	for (; i < iEnd; i++) {
		if (((vramCtrl & 0x83) != 0x83) || (vramLock & 0x1)) {		// VRAM_ENABLE | ( VRAM_x_TEXTURE | VRAM_x_TEX_PALETTE )
			if (isNotMainBank) {
				bankLock[curBank] = (i == 0 ? (uint8*)VRAM_E : (uint8*)VRAM_F + ((i - 1) * 0x4000));
				bankSize[curBank] = (i == 0 ? 0x10000 : 0x4000);
			}
			else {
				bankLock[curBank] = (uint8*)VRAM_A + (i * 0x20000);
				bankSize[curBank] = 0x20000;
			}
			curBank++;
		}
		vramCtrl >>= 8;
		vramLock >>= 1;
	}
	curBank = 0;

	// Retrieve the available area from this block using the address given
	uint32 curBlockSize = block->blockSize - ((uint32)checkAddr - (uint32)block->AddrSet);
	do {
		// Do address adjustments based on locked banks
		if (bankLock[curBank]) {
			// Skip to corresponding bank that address is in
			while (bankLock[curBank] && checkAddr >= (bankLock[curBank] + bankSize[curBank]))
				curBank++;
			do {
				// Examine is address is within locked bank, and push it to next bank if needed
				if (bankLock[curBank] && checkAddr >= bankLock[curBank] && checkAddr < bankLock[curBank] + bankSize[curBank])
					checkAddr = bankLock[curBank] + bankSize[curBank];
				else
					break;
			} while (bankLock[++curBank] != NULL);
			// Continue block and address adjustments
			while (block && checkAddr >= block->AddrSet + block->blockSize)
				block = block->node[3];
			if (!block)	return NULL;
			if (checkAddr < block->AddrSet)
				checkAddr = block->AddrSet;
			// Adjust the blocks available size based on address location within said block
			if (bankLock[curBank] && bankLock[curBank] < block->AddrSet + block->blockSize)
				curBlockSize = (uint32)bankLock[curBank] - (uint32)checkAddr;
			else
				curBlockSize = block->blockSize - ((uint32)checkAddr - (uint32)block->AddrSet);
		}

		// Obtained an aligned address, and adjust the available area that can be used
		uint8* aligned_checkAddr = (uint8*)(((uint32)checkAddr + ((1 << align) - 1)) & (~((1 << align) - 1)));
		uint32 excess = ((uint32)aligned_checkAddr - (uint32)checkAddr);
		curBlockSize -= excess;
		if (curBlockSize >= size) {
			mb->lastExamined = block;
			mb->lastExaminedAddr = aligned_checkAddr;
			mb->lastExaminedSize = size;
			return aligned_checkAddr;
		}
		else {
			if (bankLock[curBank] && bankLock[curBank] < block->AddrSet + block->blockSize) {
				checkAddr = bankLock[curBank] + bankSize[curBank];
				curBlockSize = 0;
			}
			else {
				block = block->node[3];
				if (!block)	return NULL;
				checkAddr = block->AddrSet;
				curBlockSize = block->blockSize;
			}
		}
	} while (block != NULL);
	return NULL;
}

uint32
vramBlock_allocateSpecial(s_vramBlock* mb, uint8* addr, uint32 size) {
	// Simple validity tests. Special allocations require "examination" data
	if (!addr || !size || !mb->lastExamined || !mb->lastExaminedAddr) return 0;
	if (mb->lastExaminedAddr != addr || mb->lastExaminedSize != size) return 0;

	// Can only get here if prior tests passed, meaning a spot is available, and can be allocated
	struct s_SingleBlock* newBlock = (struct s_SingleBlock*)vramBlock__allocateBlock(mb, mb->lastExamined, addr, size);
	if (newBlock) { // with current implementation, it should never be false if it gets to here
		uint32 curBlock;
		// Use a prior index if one exists. Else, obtain a new index
		if (mb->deallocCount)
			curBlock = (uint32)DynamicArrayGet(&mb->deallocBlocks, mb->deallocCount--);
		else
			curBlock = mb->blockCount++;
		DynamicArraySet(&mb->blockPtrs, curBlock, (void*)newBlock);
		// Clear out examination data
		mb->lastExamined = NULL;
		mb->lastExaminedAddr = NULL;
		mb->lastExaminedSize = 0;
		newBlock->indexOut = curBlock;
		return curBlock;
	}
	return 0;
}

uint32
vramBlock_allocateBlock(s_vramBlock* mb, uint32 size, uint8 align) {
	// Simple valid tests, such as if there are no more empty blocks as indicated by "firstEmpty"
	if (mb->firstEmpty == NULL || !size || align >= 8)
		return 0;

	// Grab the first empty block, and begin examination for a valid spot from there
	struct s_SingleBlock* block = mb->firstEmpty;
	uint8* checkAddr = vramBlock_examineSpecial(mb, block->AddrSet, size, align);
	if (!checkAddr) return 0;

	// If execution gets here, then a spot was found, so allocate it
	return vramBlock_allocateSpecial(mb, checkAddr, size);
}

uint32
vramBlock_deallocateBlock(s_vramBlock* mb, uint32 index) {
	// Retrieve the block from the index array, and see if it exists. If it does, and is deallocated (which it should),
	//		remove from index list
	struct s_SingleBlock* block = (struct s_SingleBlock*)DynamicArrayGet(&mb->blockPtrs, index);
	if (block && vramBlock__deallocateBlock(mb, block)) {
		DynamicArraySet(&mb->blockPtrs, index, (void*)0x0);
		DynamicArraySet(&mb->deallocBlocks, ++mb->deallocCount, (void*)index);
		return 1;
	}
	return 0;
}

void
vramBlock_deallocateAll(s_vramBlock* mb) {
	// Reset the entire container
	vramBlock_terminate(mb);
	vramBlock_init(mb);
}


uint8*
vramBlock_getAddr(s_vramBlock* mb, uint32 index) {
	struct s_SingleBlock* getBlock;
	if ((getBlock = (struct s_SingleBlock*)DynamicArrayGet(&mb->blockPtrs, index)))
		return getBlock->AddrSet;
	return NULL;
}


uint32
vramBlock_getSize(s_vramBlock* mb, uint32 index) {
	struct s_SingleBlock* getBlock;
	if ((getBlock = (struct s_SingleBlock*)DynamicArrayGet(&mb->blockPtrs, index)))
		return getBlock->blockSize;
	return 0;
}

uint16* vramGetBank(uint16* addr) {
	//---------------------------------------------------------------------------------
	if (addr >= VRAM_A && addr < VRAM_B)
		return VRAM_A;
	else if (addr >= VRAM_B && addr < VRAM_C)
		return VRAM_B;
	else if (addr >= VRAM_C && addr < VRAM_D)
		return VRAM_C;
	else if (addr >= VRAM_D && addr < VRAM_E)
		return VRAM_D;
	else if (addr >= VRAM_E && addr < VRAM_F)
		return VRAM_E;
	else if (addr >= VRAM_F && addr < VRAM_G)
		return VRAM_F;
	else if (addr >= VRAM_G && addr < VRAM_H)
		return VRAM_G;
	else if (addr >= VRAM_H && addr < VRAM_I)
		return VRAM_H;
	else return VRAM_I;
}
*/

//This updated version fixes the cch flushing issue
int glTexImage2DFixed(int target, int empty1, GL_TEXTURE_TYPE_ENUM type, int sizeX, int sizeY, int empty2, int param, const void* texture) {
	//---------------------------------------------------------------------------------
	uint32 size = 0;
	uint32 typeSizes[9] = { 0, 8, 2, 4, 8, 3, 8, 16, 16 };	// Represents the number of bits per pixels for each format

	if (!glGlob->activeTexture) return 0;

	size = 1 << (sizeX + sizeY + 6);

	switch (type) {
	case GL_RGB:
	case GL_RGBA:
		size = size << 1;
		break;
	case GL_RGB4:
	case GL_COMPRESSED:
		size = size >> 2;
		break;
	case GL_RGB16:
		size = size >> 1;
		break;
	default:
		break;
	}
	if (!size) return 0;

	if (type == GL_NOTEXTURE)
		size = 0;

	gl_texture_data* tex = (gl_texture_data*)DynamicArrayGet(&glGlob->texturePtrs, glGlob->activeTexture);

	// Clear out the texture data if one already exists for the active texture
	if (tex) {
		uint32 texType = ((tex->texFormat >> 26) & 0x07);
		if ((tex->texSize != size) || (typeSizes[texType] != typeSizes[type])) {
			if (tex->texIndexExt)
				vramBlock_deallocateBlock(glGlob->vramBlocks[0], tex->texIndexExt);
			if (tex->texIndex)
				vramBlock_deallocateBlock(glGlob->vramBlocks[0], tex->texIndex);
			tex->texIndex = tex->texIndexExt = 0;
			tex->vramAddr = NULL;
		}
	}


	tex->texSize = size;

	// Allocate a new space for the texture in VRAM
	if (!tex->texIndex) {
		if (type != GL_NOTEXTURE) {
			if (type != GL_COMPRESSED) {
				tex->texIndex = vramBlock_allocateBlock(glGlob->vramBlocks[0], tex->texSize, 3);
			}
			else {
				uint8* vramBAddr = (uint8*)VRAM_B;
				uint8* vramACAddr = NULL;
				uint8* vramBFound, * vramACFound;
				uint32 vramBAllocSize = size >> 1;
				if ((VRAM_B_CR & 0x83) != 0x83)
					return 0;

				// Process of finding a valid spot for compressed textures is as follows...
				//		Examine first available spot in VRAM_B for the header data
				//		Extrapulate where the tile data would go in VRAM_A or VRAM_C if the spot in VRAM_B were used
				//		Check the extrapulated area to see if it is an empty spot
				//			If not, then adjust the header spot in VRAM_B by a ratio amount found by the tile spot
				while (1) {
					// Check designated opening, and return available spot
					vramBFound = vramBlock_examineSpecial(glGlob->vramBlocks[0], vramBAddr, vramBAllocSize, 2);
					// Make sure that the space found in VRAM_B is completely in it, and not extending out of it
					if (vramGetBank((uint16*)vramBFound) != VRAM_B || vramGetBank((uint16*)(vramBFound + vramBAllocSize) - 1) != VRAM_B) {
						return 0;
					}
					// Make sure it is completely on either half of VRAM_B
					if (((uint32)vramBFound - (uint32)VRAM_B < 0x10000) && ((uint32)vramBFound - (uint32)VRAM_B + vramBAllocSize > 0x10000)) {
						vramBAddr = (uint8*)VRAM_B + 0x10000;
						continue;
					}
					// Retrieve the tile location in VRAM_A or VRAM_C
					uint32 offset = ((uint32)vramBFound - (uint32)VRAM_B);
					vramACAddr = (uint8*)(offset >= 0x10000 ? VRAM_B : VRAM_A) + (offset << 1);
					vramACFound = vramBlock_examineSpecial(glGlob->vramBlocks[0], vramACAddr, size, 3);
					if (vramACAddr != vramACFound) {
						// Adjust the spot in VRAM_B by the difference found with VRAM_A/VRAM_C, divided by 2
						vramBAddr += (((uint32)vramACFound - (uint32)vramACAddr) >> 1);
						continue;
					}
					else {
						// Spot found, setting up spots
						tex->texIndex = vramBlock_allocateSpecial(glGlob->vramBlocks[0], vramACFound, size);
						tex->texIndexExt = vramBlock_allocateSpecial(glGlob->vramBlocks[0], vramBlock_examineSpecial(glGlob->vramBlocks[0], vramBFound, vramBAllocSize, 2), vramBAllocSize);
						break;
					}
				}
			}
		}
		if (tex->texIndex) {
			tex->vramAddr = vramBlock_getAddr(glGlob->vramBlocks[0], tex->texIndex);
			tex->texFormat = (sizeX << 20) | (sizeY << 23) | ((type == GL_RGB ? GL_RGBA : type) << 26) | (((uint32)tex->vramAddr >> 3) & 0xFFFF);
		//	printf("Hey\n");
		}
		else {
			tex->vramAddr = NULL;
			tex->texFormat = 0;
			return 0;
		}
	}
	else
	{
		tex->texFormat = (sizeX << 20) | (sizeY << 23) | ((type == GL_RGB ? GL_RGBA : type) << 26) | (tex->texFormat & 0xFFFF);
		
	}
		

	glTexParameter(target, param);

	// Copy the texture data into either VRAM or main memory
	if (type != GL_NOTEXTURE && texture) {
		uint32 vramTemp = VRAM_CR;
		uint16* startBank = vramGetBank((uint16*)tex->vramAddr);
		uint16* endBank = vramGetBank((uint16*)((char*)tex->vramAddr + size - 1));

		do {
			if (startBank == VRAM_A)
				vramSetBankA(VRAM_A_LCD);
			else if (startBank == VRAM_B)
				vramSetBankB(VRAM_B_LCD);
			else if (startBank == VRAM_C)
				vramSetBankC(VRAM_C_LCD);
			else if (startBank == VRAM_D)
				vramSetBankD(VRAM_D_LCD);
			startBank += 0x10000;
		} while (startBank <= endBank);

		if (type == GL_RGB) {
			uint16* src = (uint16*)texture;
			uint16* dest = (uint16*)tex->vramAddr;
			size >>= 1;
			while (size--) {
				*dest++ = *src | 0x8000;
				src++;
			}
		}
		else {
			DC_FlushRange(texture, size);
			dmaCopyWords(0, texture, tex->vramAddr, size);
			if (type == GL_COMPRESSED) {
				vramSetBankB(VRAM_B_LCD);
				DC_FlushRange((char*)texture + tex->texSize, size >> 1);
				dmaCopyWords(0, (char*)texture + tex->texSize, vramBlock_getAddr(glGlob->vramBlocks[0], tex->texIndexExt), size >> 1);
			}
		}
		vramRestorePrimaryBanks(vramTemp);
	}

	return 1;
}