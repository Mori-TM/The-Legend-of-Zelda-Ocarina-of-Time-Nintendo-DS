FORCE_INLINE void CreatePack(u32 Index, u32* List, u32* Off, u32* CmdOff)
{
	(*Off)++;
	*CmdOff += 5;
	List[*CmdOff] = FIFO_COMMAND_PACK(FIFO_TEX_COORD, FIFO_NORMAL, FIFO_VERTEX16, FIFO_NOP);
	SetTexCoordToList(List, *Off, Md2Proceess.TexCoord[Index]);
	SetNormalToList(List, *Off, Md2Proceess.Normal[Index]);
	SetVertexToList(List, *Off, Md2Proceess.Vertex[Index]);
}

bool AnimationBlended = false;
/*
void AnimationBlend(u32 FirstStart, u32 FirstEnd, u32 BlendStart, u32 BlendEnd, u32 SecondStart, u32 SecondEnd)
{
	Md2Start(20, 38);
	AnimationBlended = true;
}

void AnimationUpdate(u32 i)
{
	if (AnimationBlended)
	{
		//	Md2Update(Index);
		vec3* P0;
		vec3* P1;
		vec3 r;
		vec3 Inter = { Md2Proceess.Model->Interpol, Md2Proceess.Model->Interpol, Md2Proceess.Model->Interpol };

		for (u8 j = 0; j < 3; j++)
		{
			P0 = &Md2Proceess.PointList[Md2Proceess.Model->TriIndex[i].MeshIndex[j]];
			P1 = &Md2Proceess.NextPointList[Md2Proceess.Model->TriIndex[i].MeshIndex[j]];

			Add3(*P0, Mul3(Inter, Sub3(*P1, *P0, r), r), Md2Proceess.Vertex[j]);
			
			Copy2(Md2Proceess.Model->ST[Md2Proceess.Model->TriIndex[i].STIndex[j]], Md2Proceess.TexCoord[j]);
			Copy3(Md2Proceess.NormalList[Md2Proceess.Model->TriIndex[i].NormalIndex[j]], Md2Proceess.Normal[j]);
		}
	}
	else
	{
		Md2Update(i);
	}
}
*/
void AnimationUpdateList(u32* List, u32 NumTriangles, u32 FirstEnd, u32 BlendFrames)
{
	u32 Off = 0;
	u32 CmdOff = 7;
	register u32 i = 0;
	Md2UpdateBlend(i, FirstEnd, BlendFrames);

	SetTexCoordToList(List, Off, Md2Proceess.TexCoord[0]);
	SetNormalToList(List, Off, Md2Proceess.Normal[0]);
	SetVertexToList(List, Off, Md2Proceess.Vertex[0]);
	List[CmdOff] = FIFO_COMMAND_PACK(FIFO_TEX_COORD, FIFO_NORMAL, FIFO_VERTEX16, FIFO_NOP);
	CreatePack(2, List, &Off, &CmdOff);
	CreatePack(1, List, &Off, &CmdOff);

	for (i = 1; i < NumTriangles; i++)
	{
		Md2UpdateBlend(i, FirstEnd, BlendFrames);
		
		CreatePack(0, List, &Off, &CmdOff);
		CreatePack(2, List, &Off, &CmdOff);
		CreatePack(1, List, &Off, &CmdOff);
	}
	
	AnimationBlended = false;
}

