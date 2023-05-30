bool IntersectRayTriangle(f32* ray_origin, f32* ray_direction, f32* v0, f32* v1, f32* v2, f32* Dist)
{      
	/*
	const f32 EPSILON = ToF32(0.0000001f);//too inacuarte
	//const f32 EPSILON = 1;
	vec3 e1; Sub3(v1, v0, e1);
	vec3 e2; Sub3(v2, v0, e2);   
	vec3 h; Cross3(ray_direction, e2, h);

	f32 a = Dot3(e1, h);
	if ((a == -F0) || (a == F0)) return false;
	//if (a > -EPSILON && a < EPSILON) return false;
	f32 f = Div(F1, a);
	vec3 s; Sub3(ray_origin, v0, s);
	f32 u = Mul(f, Dot3(s, h));
	if ((u < F0) || (u > F1)) return false;
	vec3 q; Cross3(s, e1, q);
	f32 v = Mul(f, Dot3(ray_direction, q));
	if ((v < F0) || ((v + u) > F1)) return false;
	f32 t = Mul(f, Dot3(e2, q));
	// if (t <= EPSILON) return FALSE; // line intersection but no ray intersection
	
	if (t > EPSILON)
	{
		*Dist = t;
		return true;
	}
	return false;
	*/
	//const float Eps = FLT_EPSILON;
	REG_DIVCNT = DIV_64_32;
	const f32 EPSILON = 1;
	vec3 e1; Sub3(v1, v0, e1);
	vec3 e2; Sub3(v2, v0, e2);  
	vec3 h;  Cross3(ray_direction, e2, h); 
	
	f32 a = Dot3(e1, h);
	if ((a == -F0) || (a == F0)) return false;
	while(REG_DIVCNT & DIV_BUSY);
	REG_DIV_NUMER = ((int64)F1) << 12;
	REG_DIV_DENOM_L = a;
//    if (a > -EPSILON && a < EPSILON) return false;
//	f32 f = Div(F1, a);
	vec3 s; Sub3(ray_origin, v0, s);
	while(REG_DIVCNT & DIV_BUSY);
	f32 u = Mul(REG_DIV_RESULT_L, Dot3(s, h));
	if ((u < F0) || (u > F1)) return false;
	vec3 q; Cross3(s, e1, q);
	f32 v = Mul(REG_DIV_RESULT_L, Dot3(ray_direction, q));
	if ((v < F0) || ((v + u) > F1)) return false;
	f32 t = Mul(REG_DIV_RESULT_L, Dot3(e2, q));
	if (t <= EPSILON) return FALSE; // line intersection but no ray intersection
	if (t > EPSILON)
	{
		*Dist = t;
		return true;
	}
	return false;
}

bool RayVsTriangle(vec3 Pos, vec3 RayDir, f32 TriScale, vec3 A, vec3 B, vec3 C, f32* Dist)
{
	A[0] = Mul(A[0], TriScale);
	A[1] = Mul(A[1], TriScale);
	A[2] = Mul(A[2], TriScale);
		
	B[0] = Mul(B[0], TriScale);
	B[1] = Mul(B[1], TriScale);
	B[2] = Mul(B[2], TriScale);
		
	C[0] = Mul(C[0], TriScale);
	C[1] = Mul(C[1], TriScale);
	C[2] = Mul(C[2], TriScale);

	bool Hit = IntersectRayTriangle(Pos, RayDir, A, B, C, Dist);

	if (Hit)
		return true;      

	return false;
}
/*
#pragma pack(push)
#pragma pack(1)

typedef struct
{
	u32 CollisionType;
	f32 Data[2];
	u32 VertexCount;
} CollisionHeader;

typedef struct
{
	s32 v[3];
} CollisionVertex;

#pragma pack(pop)

typedef enum
{
	COLLISION_TYPE_SPHERE = 0,
	COLLISION_TYPE_MESH = 1,
} CollisionTypes;
*/
typedef struct
{
	u32 xy;
	u32 zn;
} VecF32;

typedef struct
{
	VecF32 Min;
	VecF32 Max;
} AABB;

typedef struct
{
	u32 VertexOffset;
	u32 VertexCount;
} CollisionOffset;

typedef struct
{
	u32 MeshCount;
	CollisionOffset MeshOffsets[16];
	AABB AABBs[16];
} CollisionHeader;

typedef enum
{
	COLLISION_TYPE_NO_AABB = -1,
	COLLISION_TYPE_NO_COLLISION_BUT_AABB = -2,
} CollisionType;

/*
For future Moritz STL models are not y axis up and memory allignment is very important
*/
//TriData CollisionTris[8];
//TriData
s8 ProccessCollision(u8* CollisionMesh, vec3 Pos, vec3 RayDir, f32 TriScale, vec3 Normal, f32* Dist)
{
	/*
	vec3 A;
	vec3 B;
	vec3 C;
	int j = 0;
	for (j = 0; j <  999; j += 3)
	{
		GetVertexFromList((u32*)KokiriForestgrass_bin, j, A);
		GetVertexFromList((u32*)KokiriForestgrass_bin, j + 1, B);
		GetVertexFromList((u32*)KokiriForestgrass_bin, j + 2, C);
		if (SphereRayVsTriangle(Pos, Radius, RayDir, TriScale, A, B, C, Normal, Dist)) return 0;
	}
	*/
	
	CollisionHeader* Header = (CollisionHeader*)CollisionMesh;
	CollisionMesh += sizeof(CollisionHeader);
	
	f32 SmallestDist = F100;
	register v16 x;
	register v16 y;
	register v16 z;
	register u32 i;
	
	vec3 A;
	vec3 B;
	vec3 C;
	s8 Index = -1;
	
	for (i = 0; i < Header->MeshCount; i++)
	{
		x = (Header->AABBs[i].Min.xy & 0xFFFF);
		y = (Header->AABBs[i].Min.xy >> 16) & 0xFFFF; // & 0xFFFF opt
		z = (Header->AABBs[i].Min.zn & 0xFFFF);	    

		A[0] = (s32)x;
		A[1] = (s32)y;
		A[2] = (s32)z;
		
		x = (Header->AABBs[i].Max.xy & 0xFFFF);
		y = (Header->AABBs[i].Max.xy >> 16) & 0xFFFF; // & 0xFFFF opt
		z = (Header->AABBs[i].Max.zn & 0xFFFF);	    

		B[0] = (s32)x;
		B[1] = (s32)y;
		B[2] = (s32)z;
		
		A[0] = Mul(A[0], TriScale);
		A[1] = Mul(A[1], TriScale);
		A[2] = Mul(A[2], TriScale);
			
		B[0] = Mul(B[0], TriScale);
		B[1] = Mul(B[1], TriScale);
		B[2] = Mul(B[2], TriScale);
		
		if( Pos[0] > A[0] && Pos[0] < B[0] &&
			Pos[1] > A[1] && Pos[1] < B[1] &&
			Pos[2] > A[2] && Pos[2] < B[2])
			{
				Index = i;
				break;
			}               
	}
	
	printf("\x1b[18;0H Mesh Count: %d/%d", Header->MeshCount, (int)i);
	
	if (Index == -1)
		return COLLISION_TYPE_NO_AABB;
	
	u32* p = CollisionMesh;
	u32 f = Header->MeshOffsets[Index].VertexOffset * 2;
	u32 t = Header->MeshOffsets[Index].VertexCount * 2 + f;
	
	
	//printf("\x1b[19;0H Tri Count: %d/%d", t / 3, Header->MeshOffsets[Index]);
	u16 PossibleInteractions = 0;

	v16 nx;
	v16 ny;
	v16 nz;

	for (i = f; i < t; i += 6)
	{                     
		x = (p[i] & 0xFFFF);
		y = (p[i] >> 16) & 0xFFFF;

		z = (p[i + 1] & 0xFFFF);
		
		A[0] = (s32)x;
		A[1] = (s32)y;
		A[2] = (s32)z;
		
		x = (p[i + 2] & 0xFFFF);
		y = (p[i + 2] >> 16) & 0xFFFF;
		z = (p[i + 3] & 0xFFFF);
		
		
		B[0] = (s32)x;
		B[1] = (s32)y;
		B[2] = (s32)z;
		
		x = (p[i + 4] & 0xFFFF);
		y = (p[i + 4] >> 16) & 0xFFFF;
		z = (p[i + 5] & 0xFFFF);
		
		
		C[0] = (s32)x;
		C[1] = (s32)y;
		C[2] = (s32)z;
		
		if (RayVsTriangle(Pos, RayDir, TriScale, A, B, C, Dist)) 
		{
			if ((*Dist) < SmallestDist)
			{
			//	nx = (p[i + 1] >> 16) & 0xFFFF;
			//	ny = (p[i + 3] >> 16) & 0xFFFF;
			//	nz = (p[i + 5] >> 16) & 0xFFFF;				

				SmallestDist = *Dist;
				Normal[0] = (p[i + 1] >> 16) & 0xFFFF;
				Normal[1] = (p[i + 3] >> 16) & 0xFFFF;
				Normal[2] = (p[i + 5] >> 16) & 0xFFFF;
				PossibleInteractions = i;
			}

		//	PossibleInteractions++;
		}
	}
	
	printf("\x1b[19;2H Possible Interactions: %d", PossibleInteractions);

	/*
	printf("\x1b[19;2H Mesh Count: %d/%d", Header->MeshCount, (int)sizeof(v16));
	
	register u32* List = (u32*)CollisionMesh;
	register v16 x;
	register v16 y;
	register v16 z;
	register u32 i;
	
	v16 nx;
	v16 ny;
	v16 nz;
	//register v16 w;
	
	
//    u8 CollisionCount = 0;
//    CollisionTris[0].Dist = 1000000;
	
	vec3 A;
	vec3 B;
	vec3 C;
	
	f32 SmallestDist = 1000000;

	for (i = 0; i < Header->VertexCount * 2; i += 6)
	{
		x = (List[i] & 0xFFFF);
		y = (List[i] >> 16) & 0xFFFF; // & 0xFFFF opt
		z = (List[i + 1] & 0xFFFF);
		

		A[0] = (s32)x;
		A[1] = (s32)y;
		A[2] = (s32)z;
		
		x = (List[i + 2] & 0xFFFF);
		y = (List[i + 2] >> 16) & 0xFFFF; // & 0xFFFF opt
		z = (List[i + 3] & 0xFFFF);
		

		B[0] = (s32)x;
		B[1] = (s32)y;
		B[2] = (s32)z;
		
		x = (List[i + 4] & 0xFFFF);
		y = (List[i + 4] >> 16) & 0xFFFF; // & 0xFFFF opt
		z = (List[i + 5] & 0xFFFF);
		

		C[0] = (s32)x;
		C[1] = (s32)y;
		C[2] = (s32)z;
		
		if (SphereRayVsTriangle(Pos, Radius, RayDir, TriScale, A, B, C, Normal, Dist)) 
		{
			ny = (List[i + 3] >> 16) & 0xFFFF;
			
			if (*Dist < SmallestDist)
			{
				nx = (List[i + 1] >> 16) & 0xFFFF;
				
				nz = (List[i + 5] >> 16) & 0xFFFF;
				
				SmallestDist = *Dist;
				Normal[0] = nx;
				Normal[1] = ny;
				Normal[2] = nz;
			}
			//CollisionTris[CollisionCount++];
			
			
		//    return i;
		}
	}
	*/
	if (SmallestDist >= F100 - 1)
		return COLLISION_TYPE_NO_COLLISION_BUT_AABB;
	
	*Dist = SmallestDist;
	return Index;
}