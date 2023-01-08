#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <float.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

namespace Md2
{
	/*
	typedef unsigned char u8;
	typedef char s8;	
	typedef unsigned short u16;
	typedef short s16;
	typedef unsigned int u32;
	typedef int s32;
	*/

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
	} Frame;

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
	} ModelData;

	void GenerateNormals(u32 VertexCount, vec3* Vertices, u32 IndexCount, MeshExt* Indices, vec3* Normals, u32 j)
	{
		register vec3* PointList = &Vertices[VertexCount * j];
		register vec3* NormalList = &Normals[VertexCount * j];

		register u32 i;
		for (i = 0; i < VertexCount; i++)
		{
			NormalList[i].x = (s32)0;
			NormalList[i].y = (s32)0;
			NormalList[i].z = (s32)0;
		}

		vec3* V0;
		vec3* V1;
		vec3* V2;

		vec3 A;
		vec3 B;
		register vec3 Normal;

		for (i = 0; i < IndexCount; i++)
		{
			V0 = &PointList[Indices[i].MeshIndex[0]];
			V1 = &PointList[Indices[i].MeshIndex[2]];
			V2 = &PointList[Indices[i].MeshIndex[1]];

			A = Sub3P(V1, V0);
			B = Sub3P(V2, V0);
			Normal = Cross3P(&A, &B);
			Normalize3P(&Normal);

			NormalList[Indices[i].MeshIndex[0]].x += Normal.x;
			NormalList[Indices[i].MeshIndex[0]].y += Normal.y;
			NormalList[Indices[i].MeshIndex[0]].z += Normal.z;
															  
			NormalList[Indices[i].MeshIndex[2]].x += Normal.x;
			NormalList[Indices[i].MeshIndex[2]].y += Normal.y;
			NormalList[Indices[i].MeshIndex[2]].z += Normal.z;
															  
			NormalList[Indices[i].MeshIndex[1]].x += Normal.x;
			NormalList[Indices[i].MeshIndex[1]].y += Normal.y;
			NormalList[Indices[i].MeshIndex[1]].z += Normal.z;
		}

		for (i = 0; i < VertexCount; i++)
			Normalize3P(&NormalList[i]);
	}

	s8 Load(u8* Buffer, u32 FileLength, s32 TexWidth, s32 TexHeight, ModelData* Model)
	{		
		ModelHeader* Header;

		STIndex* STIndices;
		Frame* FrameData;
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
			FrameData = (Frame*)&Buffer[Header->OffsetFrames + Header->FrameSize * j];

			PointList = (vec3*)&Model->PointList[Header->NumXYZ * j];
			for (i = 0; i < Header->NumXYZ; i++)
			{
				PointList[i].x = FrameData->Scale[0] * FrameData->Fp[i].V[0] + FrameData->Translate[0];
				PointList[i].y = FrameData->Scale[1] * FrameData->Fp[i].V[1] + FrameData->Translate[1];
				PointList[i].z = FrameData->Scale[2] * FrameData->Fp[i].V[2] + FrameData->Translate[2];
			}
		}

		Model->ST = (vec2*)malloc(Header->NumST * sizeof(vec2));
		Model->NumST = Header->NumST;

		STIndices = (STIndex*)&Buffer[Header->OffsetST];
		for (i = 0; i < Header->NumST; i++)
		{
			Model->ST[i].x = (float)STIndices[i].S;
			Model->ST[i].y = (float)STIndices[i].T;
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
		Model->Interpol = 0.0;

		return 1;
	}

	void Free(ModelData* Model)
	{
		free(Model->TriIndex);
		free(Model->ST);
		free(Model->PointList);
		free(Model->NormalList);
	}

	class Proccess
	{
	public:
		ModelData* Model;
		vec3 Vertex[3];
		vec2 TexCoord[3];
		vec3 Normal[3];

	private:
		f32 X0, Y0, Z0;
		f32 X1, Y1, Z1;
		vec3* PointList;
		vec3* NextPointList;
		vec3* NormalList;

	public:
		void Start(u32 StartFrame, u32 EndFrame)
		{
			if (StartFrame > Model->CurrentFrame)
				Model->CurrentFrame = StartFrame;

			if (Model->Interpol >= 1.0)
			{
				Model->Interpol = 0.0;
				Model->CurrentFrame++;

				if (Model->CurrentFrame >= EndFrame)
					Model->CurrentFrame = StartFrame;

				Model->NextFrame = Model->CurrentFrame + 1;

				if (Model->NextFrame >= EndFrame)
					Model->NextFrame = StartFrame;
			}

			PointList = &Model->PointList[Model->NumPoints * Model->CurrentFrame];
			NextPointList = &Model->PointList[Model->NumPoints * Model->NextFrame];
			NormalList = &Model->NormalList[Model->NumPoints * Model->CurrentFrame];
		}

		void End(f32 Percent)
		{
			Model->Interpol += Percent;
		}

		void Update(u32 i)
		{
			register vec3* P0;
			register vec3* P1;
			register vec3 Inter = { Model->Interpol, Model->Interpol, Model->Interpol };
			
			for (u8 j = 0; j < 3; j++)
			{
				P0 = &PointList[Model->TriIndex[i].MeshIndex[j]];
				P1 = &NextPointList[Model->TriIndex[i].MeshIndex[j]];

				Vertex[j] = Add3(*P0, Mul3(Inter, Sub3P(P1, P0)));

				TexCoord[j] = Model->ST[Model->TriIndex[i].STIndex[j]];		
				Normal[j] = NormalList[Model->TriIndex[i].NormalIndex[j]];
			}
			
/*
			P0 = &PointList[Model->TriIndex[i].MeshIndex[0]];
			P1 = &NextPointList[Model->TriIndex[i].MeshIndex[0]];

			Vertex[0] = Add3(*P0, Mul3(Inter, Sub3P(P1, P0)));

			TexCoord[0] = Model->ST[Model->TriIndex[i].STIndex[0]];		
			Normal[0] = NormalList[Model->TriIndex[i].NormalIndex[0]];

			
			P0 = &PointList[Model->TriIndex[i].MeshIndex[1]];
			P1 = &NextPointList[Model->TriIndex[i].MeshIndex[1]];

			Vertex[1] = Add3(*P0, Mul3(Inter, Sub3P(P1, P0)));

			TexCoord[1] = Model->ST[Model->TriIndex[i].STIndex[1]];		
			Normal[1] = NormalList[Model->TriIndex[i].NormalIndex[1]];


			P0 = &PointList[Model->TriIndex[i].MeshIndex[2]];
			P1 = &NextPointList[Model->TriIndex[i].MeshIndex[2]];

			Vertex[2] = Add3(*P0, Mul3(Inter, Sub3P(P1, P0)));

			TexCoord[2] = Model->ST[Model->TriIndex[i].STIndex[2]];		
			Normal[2] = NormalList[Model->TriIndex[i].NormalIndex[2]];
*/			
			/*
			X0 = PointList[Model->TriIndex[i].MeshIndex[0]].x;
			Y0 = PointList[Model->TriIndex[i].MeshIndex[0]].y;
			Z0 = PointList[Model->TriIndex[i].MeshIndex[0]].z;

			X1 = NextPointList[Model->TriIndex[i].MeshIndex[0]].x;
			Y1 = NextPointList[Model->TriIndex[i].MeshIndex[0]].y;
			Z1 = NextPointList[Model->TriIndex[i].MeshIndex[0]].z;

			Vertex[0].x = X0 + Model->Interpol * (X1 - X0);
			Vertex[0].y = Y0 + Model->Interpol * (Y1 - Y0);
			Vertex[0].z = Z0 + Model->Interpol * (Z1 - Z0);

			//---------//
			X0 = PointList[Model->TriIndex[i].MeshIndex[2]].x;
			Y0 = PointList[Model->TriIndex[i].MeshIndex[2]].y;
			Z0 = PointList[Model->TriIndex[i].MeshIndex[2]].z;

			X1 = NextPointList[Model->TriIndex[i].MeshIndex[2]].x;
			Y1 = NextPointList[Model->TriIndex[i].MeshIndex[2]].y;
			Z1 = NextPointList[Model->TriIndex[i].MeshIndex[2]].z;

			Vertex[2].x = X0 + Model->Interpol * (X1 - X0);
			Vertex[2].y = Y0 + Model->Interpol * (Y1 - Y0);
			Vertex[2].z = Z0 + Model->Interpol * (Z1 - Z0);

			//---------//
			X0 = PointList[Model->TriIndex[i].MeshIndex[1]].x;
			Y0 = PointList[Model->TriIndex[i].MeshIndex[1]].y;
			Z0 = PointList[Model->TriIndex[i].MeshIndex[1]].z;

			X1 = NextPointList[Model->TriIndex[i].MeshIndex[1]].x;
			Y1 = NextPointList[Model->TriIndex[i].MeshIndex[1]].y;
			Z1 = NextPointList[Model->TriIndex[i].MeshIndex[1]].z;

			Vertex[1].x = X0 + Model->Interpol * (X1 - X0);
			Vertex[1].y = Y0 + Model->Interpol * (Y1 - Y0);
			Vertex[1].z = Z0 + Model->Interpol * (Z1 - Z0);

			TexCoord[0] = Model->ST[Model->TriIndex[i].STIndex[0]];
			TexCoord[2] = Model->ST[Model->TriIndex[i].STIndex[2]];
			TexCoord[1] = Model->ST[Model->TriIndex[i].STIndex[1]];
			
			
			Normal[0] = NormalList[Model->TriIndex[i].NormalIndex[0]];
			Normal[2] = NormalList[Model->TriIndex[i].NormalIndex[2]];
			Normal[1] = NormalList[Model->TriIndex[i].NormalIndex[1]];
			*/
		}
	};
	
}