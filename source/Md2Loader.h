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
	s32 CurrentFrame;
	s32 NextFrame;
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
		Model->ST[i][0] = ToF32(STIndices[i].S);
		Model->ST[i][1] = ToF32(STIndices[i].T);
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
	Model->CurrentFrame = 0;
	Model->NextFrame = 1;
	Model->Interpol = F0;

	return 1;
}

void Free(Md2ModelData* Model)
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