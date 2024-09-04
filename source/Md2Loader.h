#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <float.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

typedef struct
{
	s16 S;
	s16 T;
} STIndex;

typedef struct
{
	u8 V[3];
	u8 NormalIndex;
} FramePoint;

typedef struct
{
	float Scale[3];
	float Translate[3];
	s8 Name[16];
	FramePoint Fp[1];
} Md2Frame;

typedef struct
{
	u16 MeshIndex[3];
	u16 STIndex[3];
} Mesh;

typedef struct
{
	u16 MeshIndex[3];
	u16 STIndex[3];
	u16 NormalIndex[3];
} MeshExt;

typedef struct
{
	s32 Ident;
	s32 Version;
	s32 SkinWidth;
	s32 SkinHeight;
	s32 FrameSize;
	s32 NumSkins;
	s32 NumXYZ;
	s32 NumST;
	s32 NumTris;
	s32 NumGLCmds;
	s32 NumFrames;
	s32 OffsetSkins;
	s32 OffsetST;
	s32 OffsetTris;
	s32 OffsetFrames;
	s32 OffsetGLCmds;
	s32 OffsetEnd;
} ModelHeader;

typedef struct
{
	s32 NumFrames;
	s32 NumPoints;
	s32 NumTriangles;
	s32 NumST;
	s32 FrameSize;
	s32 TexWidth;
	s32 TexHeight;
	
	bool HoldFirstAnim;
	s32 CurrentFrame;
	s32 NextFrame;
	
	s32 CurrentFrameBlend;
	s32 NextFrameBlend;
	
	s32 CurrentFrame2;
	s32 NextFrame2;
	
	f32 Interpol;
	MeshExt* TriIndex;
	
	vec2* ST;
	vec3* PointList;
	vec3* NormalList;
} Md2ModelData;

void GenerateNormals(u32 VertexCount, vec3* Vertices, u32 IndexCount, MeshExt* Indices, vec3* Normals, u32 j)
{
	register vec3* PointList = &Vertices[VertexCount * j];
	register vec3* NormalList = &Normals[VertexCount * j];

	register u32 i;
	for (i = 0; i < VertexCount; i++)
	{
		NormalList[i][0] = (s32)0;
		NormalList[i][1] = (s32)0;
		NormalList[i][2] = (s32)0;
	}

	vec3* V0;
	vec3* V1;
	vec3* V2;

	vec3 A;
	vec3 B;
	vec3 Normal;

	for (i = 0; i < IndexCount; i++)
	{
		V0 = &PointList[Indices[i].MeshIndex[0]];
		V1 = &PointList[Indices[i].MeshIndex[2]];
		V2 = &PointList[Indices[i].MeshIndex[1]];

		Sub3(*V1, *V0, A);
		Sub3(*V2, *V0, B);
		Cross3(A, B, Normal);
		Normalize3(Normal);

		NormalList[Indices[i].MeshIndex[0]][0] += Normal[0];
		NormalList[Indices[i].MeshIndex[0]][1] += Normal[1];
		NormalList[Indices[i].MeshIndex[0]][2] += Normal[2];

		NormalList[Indices[i].MeshIndex[2]][0] += Normal[0];
		NormalList[Indices[i].MeshIndex[2]][1] += Normal[1];
		NormalList[Indices[i].MeshIndex[2]][2] += Normal[2];

		NormalList[Indices[i].MeshIndex[1]][0] += Normal[0];
		NormalList[Indices[i].MeshIndex[1]][1] += Normal[1];
		NormalList[Indices[i].MeshIndex[1]][2] += Normal[2];
	}

	for (i = 0; i < VertexCount; i++)
		Normalize3(NormalList[i]);
}

s8 Md2Load(u8* Buffer, u32 FileLength, s32 TexWidth, s32 TexHeight, Md2ModelData* Model)
{
	ModelHeader* Header;

	STIndex* STIndices;
	Md2Frame* FrameData;
	vec3* PointList;
	MeshExt* TriIndex;
	Mesh* BufIndex;
	s32 i;
	s32 j;
	//Buffer += 52;
	Header = (ModelHeader*)Buffer;
	//	Header->NumFrames = 25;
	//	printf("d: %d\n", Header->NumXYZ);
	Model->PointList = (vec3*)malloc(Header->NumXYZ * Header->NumFrames * sizeof(vec3));
	//-----//
	Model->NormalList = (vec3*)malloc(Header->NumXYZ * Header->NumFrames * sizeof(vec3));
	//-----//
	Model->NumPoints = Header->NumXYZ;
	Model->NumFrames = Header->NumFrames;
	Model->FrameSize = Header->FrameSize;

	for (j = 0; j < Header->NumFrames; j++)
	{
		FrameData = (Md2Frame*)&Buffer[Header->OffsetFrames + Header->FrameSize * j];

		PointList = (vec3*)&Model->PointList[Header->NumXYZ * j];
		for (i = 0; i < Header->NumXYZ; i++)
		{
			//This can be optimised
			PointList[i][0] = ToF32(FrameData->Scale[0] * FrameData->Fp[i].V[0] + FrameData->Translate[0]);
			PointList[i][1] = ToF32(FrameData->Scale[1] * FrameData->Fp[i].V[1] + FrameData->Translate[1]);
			PointList[i][2] = ToF32(FrameData->Scale[2] * FrameData->Fp[i].V[2] + FrameData->Translate[2]);
		}
	}

	Model->ST = (vec2*)malloc(Header->NumST * sizeof(vec2));
	Model->NumST = Header->NumST;

	STIndices = (STIndex*)&Buffer[Header->OffsetST];
	for (i = 0; i < Header->NumST; i++)
	{
		Model->ST[i][0] = ToF32(STIndices[i].S * 2);
		Model->ST[i][1] = ToF32(STIndices[i].T * 2);
	}

	TriIndex = (MeshExt*)malloc(Header->NumTris * sizeof(MeshExt));

	Model->NumTriangles = Header->NumTris;
	Model->TriIndex = TriIndex;

	BufIndex = (Mesh*)&Buffer[Header->OffsetTris];

	//why this loop it doesnt do anything
	for (j = 0; j < Model->NumFrames; j++)
	{
		for (i = 0; i < Header->NumTris; i++)
		{
			TriIndex[i].MeshIndex[0] = BufIndex[i].MeshIndex[0];
			TriIndex[i].MeshIndex[1] = BufIndex[i].MeshIndex[1];
			TriIndex[i].MeshIndex[2] = BufIndex[i].MeshIndex[2];

			TriIndex[i].STIndex[0] = BufIndex[i].STIndex[0];
			TriIndex[i].STIndex[1] = BufIndex[i].STIndex[1];
			TriIndex[i].STIndex[2] = BufIndex[i].STIndex[2];

			TriIndex[i].NormalIndex[0] = BufIndex[i].MeshIndex[0];
			TriIndex[i].NormalIndex[1] = BufIndex[i].MeshIndex[1];
			TriIndex[i].NormalIndex[2] = BufIndex[i].MeshIndex[2];
		}

		GenerateNormals(Header->NumXYZ, Model->PointList, Header->NumTris, TriIndex, Model->NormalList, j);
	}
	//	free(Buffer);
	Model->HoldFirstAnim = false;
	Model->CurrentFrame = 0;
	Model->NextFrame = 1;
	Model->Interpol = F0;
	
	Model->CurrentFrameBlend = 0;
	Model->NextFrameBlend = 1;
	
	Model->CurrentFrame2 = -1;
	Model->NextFrame2 = 1;

	return 1;
}

void Md2Destroy(Md2ModelData* Model)
{
	free(Model->TriIndex);
	free(Model->ST);
	free(Model->PointList);
	free(Model->NormalList);
}

struct
{
	Md2ModelData* Model;
	vec3 Vertex[3];
	vec2 TexCoord[3];
	vec3 Normal[3];

	vec3* PointList;
	vec3* NextPointList;
	vec3* NormalList;
	
	vec3* PointListBlend;
	vec3* NextPointListBlend;
	vec3* NormalListBlend;
	
	vec3* PointList2;
	vec3* NextPointList2;
	vec3* NormalList2;
	
	bool CurrentlyBlending;
} Md2Proceess;

void Md2Start(u32 StartFrame, u32 EndFrame)
{
	if (StartFrame > Md2Proceess.Model->CurrentFrame)
		Md2Proceess.Model->CurrentFrame = StartFrame;

	if (Md2Proceess.Model->Interpol >= F1)
	{
		Md2Proceess.Model->Interpol = F0;
		Md2Proceess.Model->CurrentFrame++;

		if (Md2Proceess.Model->CurrentFrame >= EndFrame)
			Md2Proceess.Model->CurrentFrame = StartFrame;

		Md2Proceess.Model->NextFrame = Md2Proceess.Model->CurrentFrame + 1;

		if (Md2Proceess.Model->NextFrame >= EndFrame)
			Md2Proceess.Model->NextFrame = StartFrame;
	}

	Md2Proceess.PointList = &Md2Proceess.Model->PointList[Md2Proceess.Model->NumPoints * Md2Proceess.Model->CurrentFrame];
	Md2Proceess.NextPointList = &Md2Proceess.Model->PointList[Md2Proceess.Model->NumPoints * Md2Proceess.Model->NextFrame];
	Md2Proceess.NormalList = &Md2Proceess.Model->NormalList[Md2Proceess.Model->NumPoints * Md2Proceess.Model->CurrentFrame];
}

void Md2StartBlend(u32 FirstStart, u32 FirstEnd, u32 BlendFrames, u32 SecondStart, u32 SecondEnd)
{

//	if (Md2Proceess.Model->CurrentFrame2 - BlendFrames > SecondStart )
//	if (Md2Proceess.Model->CurrentFrame2 == -1)
	if (!Md2Proceess.Model->HoldFirstAnim)
	{
		if (FirstStart > Md2Proceess.Model->CurrentFrame)
			Md2Proceess.Model->CurrentFrame = FirstStart;

		if (Md2Proceess.Model->Interpol >= F1)
		{
			Md2Proceess.Model->CurrentFrame++;

			if (Md2Proceess.Model->CurrentFrame >= FirstEnd)
			{
				
				
			}

			Md2Proceess.Model->NextFrame = Md2Proceess.Model->CurrentFrame + 1;

			if (Md2Proceess.Model->NextFrame >= FirstEnd)
			{
				Md2Proceess.Model->NextFrame = FirstStart;
				Md2Proceess.Model->HoldFirstAnim = true;
			}
		}

		Md2Proceess.PointList = &Md2Proceess.Model->PointList[Md2Proceess.Model->NumPoints * Md2Proceess.Model->CurrentFrame];
		Md2Proceess.NextPointList = &Md2Proceess.Model->PointList[Md2Proceess.Model->NumPoints * Md2Proceess.Model->NextFrame];
		Md2Proceess.NormalList = &Md2Proceess.Model->NormalList[Md2Proceess.Model->NumPoints * Md2Proceess.Model->CurrentFrame];
		
		Md2Proceess.CurrentlyBlending = false;
		Md2Proceess.Model->CurrentFrameBlend = 0;
	}
	
	{
	
		if (Md2Proceess.Model->CurrentFrame + BlendFrames >= FirstEnd)
		{
			if (SecondStart > Md2Proceess.Model->CurrentFrame2)
				Md2Proceess.Model->CurrentFrame2 = SecondStart;

			if (Md2Proceess.Model->Interpol >= F1)
			{
				Md2Proceess.Model->CurrentFrame2++;

				if (Md2Proceess.Model->CurrentFrame2 >= SecondEnd)
				{
				
								
					
					
				}

				Md2Proceess.Model->NextFrame2 = Md2Proceess.Model->CurrentFrame2 + 1;

				if (Md2Proceess.Model->NextFrame2 >= SecondEnd)
				{
				
					Md2Proceess.Model->NextFrame2 = SecondStart;
					
					Md2Proceess.Model->CurrentFrame2 = SecondStart;		
					Md2Proceess.Model->CurrentFrame = FirstStart;
					Md2Proceess.Model->NextFrame = FirstStart;
					
					Md2Proceess.Model->CurrentFrameBlend = FirstStart;
					
					Md2Proceess.Model->HoldFirstAnim = false;
					
					
					Md2Proceess.Model->CurrentFrameBlend = 0;
					Md2Proceess.CurrentlyBlending = false;
				}
			}

			if (Md2Proceess.Model->HoldFirstAnim)
			{
				Md2Proceess.PointList = &Md2Proceess.Model->PointList[Md2Proceess.Model->NumPoints * Md2Proceess.Model->CurrentFrame2];
				Md2Proceess.NextPointList = &Md2Proceess.Model->PointList[Md2Proceess.Model->NumPoints * Md2Proceess.Model->NextFrame2];
				Md2Proceess.NormalList = &Md2Proceess.Model->NormalList[Md2Proceess.Model->NumPoints * Md2Proceess.Model->CurrentFrame2];
				Md2Proceess.CurrentlyBlending = false;
				Md2Proceess.Model->CurrentFrameBlend = 0;
			}
			else
			{
				Md2Proceess.PointList2 = &Md2Proceess.Model->PointList[Md2Proceess.Model->NumPoints * Md2Proceess.Model->CurrentFrame2];
				Md2Proceess.NextPointList2 = &Md2Proceess.Model->PointList[Md2Proceess.Model->NumPoints * Md2Proceess.Model->NextFrame2];
				Md2Proceess.NormalList2 = &Md2Proceess.Model->NormalList[Md2Proceess.Model->NumPoints * Md2Proceess.Model->CurrentFrame2];
				Md2Proceess.CurrentlyBlending = true;
				Md2Proceess.Model->CurrentFrameBlend++;
			}
			
		}
		else
		{
		//	Md2Proceess.Model->CurrentFrame2 = -1;
		}
	}


	if (Md2Proceess.Model->Interpol >= F1)
	{
	
		Md2Proceess.Model->Interpol = F0;
		
	}

//	Md2Start(FirstStart, BlendStart);

/*
	{
		if (FirstStart > Md2Proceess.Model->CurrentFrame)
			Md2Proceess.Model->CurrentFrame = FirstStart;

		if (Md2Proceess.Model->Interpol >= F1)
		{
			Md2Proceess.Model->Interpol = F0;
			Md2Proceess.Model->CurrentFrame++;

			if (Md2Proceess.Model->CurrentFrame >= FirstEnd)
				Md2Proceess.Model->CurrentFrame = SecondStart;

			if (Md2Proceess.Model->CurrentFrame >= SecondEnd)
				Md2Proceess.Model->CurrentFrame = FirstStart;
				
			

			Md2Proceess.Model->NextFrame = Md2Proceess.Model->CurrentFrame + 1;

			if (Md2Proceess.Model->NextFrame >= FirstEnd)
				Md2Proceess.Model->NextFrame = SecondStart;

			if (Md2Proceess.Model->NextFrame >= SecondEnd)
				Md2Proceess.Model->NextFrame = FirstStart;
		}

		Md2Proceess.PointList = &Md2Proceess.Model->PointList[Md2Proceess.Model->NumPoints * Md2Proceess.Model->CurrentFrame];
		Md2Proceess.NextPointList = &Md2Proceess.Model->PointList[Md2Proceess.Model->NumPoints * Md2Proceess.Model->NextFrame];
		Md2Proceess.NormalList = &Md2Proceess.Model->NormalList[Md2Proceess.Model->NumPoints * Md2Proceess.Model->CurrentFrame];
	}
	*/
	/*
	{
		if (StartFrame > Md2Proceess.Model->CurrentFrame)
			Md2Proceess.Model->CurrentFrame = StartFrame;

		if (Md2Proceess.Model->Interpol >= F1)
		{
			Md2Proceess.Model->Interpol = F0;
			Md2Proceess.Model->CurrentFrame++;

			if (Md2Proceess.Model->CurrentFrame >= EndFrame)
				Md2Proceess.Model->CurrentFrame = StartFrame;

			Md2Proceess.Model->NextFrame = Md2Proceess.Model->CurrentFrame + 1;

			if (Md2Proceess.Model->NextFrame >= EndFrame)
				Md2Proceess.Model->NextFrame = StartFrame;
		}

		Md2Proceess.PointList = &Md2Proceess.Model->PointList[Md2Proceess.Model->NumPoints * Md2Proceess.Model->CurrentFrame];
		Md2Proceess.NextPointList = &Md2Proceess.Model->PointList[Md2Proceess.Model->NumPoints * Md2Proceess.Model->NextFrame];
		Md2Proceess.NormalList = &Md2Proceess.Model->NormalList[Md2Proceess.Model->NumPoints * Md2Proceess.Model->CurrentFrame];
	}
	*/
}

void Md2UpdateBlend(u32 i, u32 FirstEnd, u32 BlendFrames)
{
	vec3* P0;
	vec3* P1;
	vec3* P2;
	vec3* P3;
	vec3 r;
	
	vec3 Step3;
	f32 Step = Div((Md2Proceess.Model->CurrentFrameBlend * 4096), BlendFrames * 4096);
	Vec3f(Step, Step3);			
	
//	vec3 m;
//	vec3 n;
	vec3 Inter = { Md2Proceess.Model->Interpol, Md2Proceess.Model->Interpol, Md2Proceess.Model->Interpol };

	//Blend key frames
	if (Md2Proceess.CurrentlyBlending)
	{
		for (u8 j = 0; j < 3; j++)
		{
			P0 = &Md2Proceess.PointList[Md2Proceess.Model->TriIndex[i].MeshIndex[j]];
			P1 = &Md2Proceess.NextPointList[Md2Proceess.Model->TriIndex[i].MeshIndex[j]];

			P2 = &Md2Proceess.PointList2[Md2Proceess.Model->TriIndex[i].MeshIndex[j]];
			P3 = &Md2Proceess.NextPointList2[Md2Proceess.Model->TriIndex[i].MeshIndex[j]];

			vec3 V0;
			Add3(*P0, Mul3(Inter, Sub3(*P1, *P0, r), r), V0);
			
			vec3 V1;
			Add3(*P2, Mul3(Inter, Sub3(*P3, *P2, r), r), V1);
			
		//	Vec3f(Div(Md2Proceess.Model->CurrentFrameBlend * 4096, BlendFrames * 4096), t);
		//	Add3(V0, Mul3(t, Sub3(V1, V0, r), r), Md2Proceess.Vertex[j]);
			
			/*
			Vec3f(BlendFrames * 4096, r);
			
			Div3(V0, r, m);
			Div3(V1, r, n);
			
			Vec3f(Md2Proceess.Model->CurrentFrameBlend * 4096, t);
			Vec3f((BlendFrames - Md2Proceess.Model->CurrentFrameBlend) * 4096, r);
			
			Mul3(n, t, V1);
			Mul3(m, r, V0);

			Add3(V0, V1, Md2Proceess.Vertex[j]);
			*/
			
			
			Lerp3(V0, V1, Step3, Md2Proceess.Vertex[j]);
			
			//Md2Proceess.Vertex[j]
			
			Copy2(Md2Proceess.Model->ST[Md2Proceess.Model->TriIndex[i].STIndex[j]], Md2Proceess.TexCoord[j]);
			Copy3(Md2Proceess.NormalList[Md2Proceess.Model->TriIndex[i].NormalIndex[j]], Md2Proceess.Normal[j]);
		}
	}
	//no Blending
	else
	{
		for (u8 j = 0; j < 3; j++)
		{
			P0 = &Md2Proceess.PointList[Md2Proceess.Model->TriIndex[i].MeshIndex[j]];
			P1 = &Md2Proceess.NextPointList[Md2Proceess.Model->TriIndex[i].MeshIndex[j]];

			Add3(*P0, Mul3(Inter, Sub3(*P1, *P0, r), r), Md2Proceess.Vertex[j]);
			
			Copy2(Md2Proceess.Model->ST[Md2Proceess.Model->TriIndex[i].STIndex[j]], Md2Proceess.TexCoord[j]);
			Copy3(Md2Proceess.NormalList[Md2Proceess.Model->TriIndex[i].NormalIndex[j]], Md2Proceess.Normal[j]);
		}
	}

	
}


void Md2End(f32 Percent)
{
	Md2Proceess.Model->Interpol += Percent;
}

void Md2Update(u32 i)
{
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


s8 Md2LoadFromFile(const char* FileName, s32 TexWidth, s32 TexHeight, Md2ModelData* Model)
{
	int Length = 0;
	char* Buffer = LoadFile(FileName, &Length);
	if (!Buffer)
		return 0;
		
	s8 Success = Md2Load(Buffer, Length, TexWidth, TexHeight, Model);
		
	free(Buffer);
	
	return Success;
/*
	FILE* File = fopen(FileName, "rb");
	if (File)
	{
		fseek(File, 0, SEEK_END);
		int Length = ftell(File);
		fseek(File, 0, SEEK_SET);

		char* Buffer = (char*)malloc(Length + 1);
		if (!Buffer)
			return 0;
			
		if (fread(Buffer, 1, Length, File) != Length)
			printf("Failed reading: %s\n", FileName);
			
		Buffer[Length] = 0;
		printf("Length: %d\n", Length);
	//	while (1);
		s8 Success = Md2Load(Buffer, Length, TexWidth, TexHeight, Model);
		
		free(Buffer);

		fclose(File);
		return Success;
	}
	
	printf("Failed to Load: %s\n", FileName);
	return 0;
	*/
}