const vec3 CameraPosOff = { 0, F3, F5 };
vec3 CameraPos = { 0, 1144, F8 };
vec3 CameraDir = { 0, 0,-F1 };
vec3 CameraUp = { 0, F1, 0 };

vec3 PlayerPos = { F5, F0, 0 };
vec3 PlayerPosLast = { F5, F0, 0 };
vec3 PlayerDir = { 0, 0,-F1 };
vec3 PlayerNor = { 0, 0,-F1 };
f32 PlayerCameraDist = 0;

f32 PlayerFallSpeed = -900;
f32 PlayerVelocity = 0;

bool ChangedPlayerDir = false;
bool PlayerOnGround = false;
f32 PlayerHitboxSize = 6144;//1.5
f32 CameraMinDist = 13757;
f32 CameraMaxDist = 21949;

typedef enum
{
	KEY_STATE_UP = 1,
	KEY_STATE_DOWN = 8,
	KEY_STATE_LEFT = 16,
	KEY_STATE_RIGHT = 32
} KeyStates;
short KeyState = 0;

void UpdatePlayer()
{
	f32 Speed = Mul(DeltaTime, 4144);//5144==1,5
	vec3 r;
	Mul3(CameraDir, Vec3f(Speed, r), CameraDir);

	f32 PrevPlayerY = PlayerPos[1];
	f32 PrevCameraY = CameraPos[1];


	if (KeysHeld & KEY_RIGHT)
	{
		PlayerDir[0] = CameraDir[0];
		PlayerDir[1] = 0;
		PlayerDir[2] = CameraDir[2];
		Normalize3(PlayerDir);

	//	PlayerNor[0] = -PlayerDir[0];
	//	PlayerNor[1] = -PlayerDir[1];
	//	PlayerNor[2] = -PlayerDir[2];

		Mul3(PlayerDir, Vec3f(Speed, r), PlayerDir);
		Cross3(CameraUp, PlayerDir, r);
		Copy3(r, PlayerDir);
		Add3(PlayerPos, PlayerDir, PlayerPos);

		ChangedPlayerDir = true;
		KeyState |= KEY_STATE_RIGHT;
	}
	else
		KeyState &= ~KEY_STATE_RIGHT;
	if (KeysHeld & KEY_LEFT)
	{
		PlayerDir[0] = -CameraDir[0];
		PlayerDir[1] = 0;
		PlayerDir[2] = -CameraDir[2];
		Normalize3(PlayerDir);

		

		Mul3(PlayerDir, Vec3f(Speed, r), PlayerDir);
		Cross3(CameraUp, PlayerDir, r);
		Copy3(r, PlayerDir);
		Add3(PlayerPos, PlayerDir, PlayerPos);

		ChangedPlayerDir = true;
		KeyState |= KEY_STATE_LEFT;
	}
	else
		KeyState &= ~KEY_STATE_LEFT;
	if (KeysHeld & KEY_DOWN)
	{
		PlayerDir[0] = CameraDir[0];
		PlayerDir[1] = 0;
		PlayerDir[2] = CameraDir[2];
		Normalize3(PlayerDir);

		Mul3(PlayerDir, Vec3f(Speed, r), PlayerDir);
		Add3(PlayerPos, PlayerDir, PlayerPos);
		

		ChangedPlayerDir = true;
		KeyState |= KEY_STATE_DOWN;
	}
	else
		KeyState &= ~KEY_STATE_DOWN;
	if (KeysHeld & KEY_UP)
	{
	//	PlayerDir[0] = -CameraDir[0];
	//	PlayerDir[1] = -CameraDir[1];
	//	PlayerDir[2] = -CameraDir[2];
		PlayerDir[0] = -CameraDir[0];
		PlayerDir[1] = 0;
		PlayerDir[2] = -CameraDir[2];
		Normalize3(PlayerDir);
		Mul3(PlayerDir, Vec3f(Speed, r), PlayerDir);
		Add3(PlayerPos, PlayerDir, PlayerPos);

		

		ChangedPlayerDir = true;
		KeyState |= KEY_STATE_UP;
	}
	else
		KeyState &= ~KEY_STATE_UP;

//	if (KeysDown & KEY_Y)
//	{
//		PlayerVelocity = 2.6;
//	}

//	if (Key[SDL_SCANCODE_Q] && ChangedPlayerDir)
//	{
//		vec3 Dir = Div3(PlayerDir, Vec3f(Speed));
//
//		f32 AbsX = fabsf(Dir[0]);
//		f32 AbsZ = fabsf(Dir[2]);
//
//		mat4 M = RotateYMat4(LoadMat4Identity(), ToRadians(PlayerRotY));
//		M = TranslateMat4(M, PlayerPos);
//		vec4 Pos = { CameraPos[0], CameraPos[1], CameraPos[2], 1.0 };
//
//		Pos = MultiplyVec4Mat4P(&Pos, &M);
//		CameraPos[0] = Pos[0];
//		CameraPos[1] = Pos[1];
//		CameraPos[2] = Pos[2];
//
//		ChangedPlayerDir = false;
//		//	CameraPos = Mul3P(&CameraPos, &Dir);
//	}

//	vec3 Dir = Div3(PlayerDir, Vec3f(Speed));
//	PrintVec3(&CameraPos);

	PlayerPos[1] = PrevPlayerY;
}
/*
#define SHIFT_AMOUNT 12 // 2^16 = 65536
#define SHIFT_MASK ((1 << SHIFT_AMOUNT) - 1) 

void PlayerWallCollision()
{

	vec3 Pos; Copy3(PlayerPos, Pos);
	Pos[1] += F1;
	vec3 Normal = { 0, 0, 0 };
	f32 Radius = PlayerHitboxSize;
	f32 Dist = F0;
	Copy3(PlayerDir, PlayerNor);
//	PlayerNor[1] = F0;
	Normalize3(PlayerNor);
	int Collision = ProccessCollision((u8*)KokiriForestCollision_bin, Pos, Radius, PlayerNor, F100, Normal, &Dist);
//	int Collision = KokiriForestWallCollision(Pos, Radius, PlayerNor, F100, Normal, &Dist);
//	printf("\x1b[14;2H Collis Nor: %f", Test);
	if (Collision == -1)//no collision
	{
		
	}
	else
	{
	//	Normalize3(Normal);
		printf("\x1b[14;2H Collis Nor: %d.%d %d.%d %d.%d", Normal[0] >> SHIFT_AMOUNT, Normal[0] & SHIFT_MASK, Normal[1] >> SHIFT_AMOUNT, Normal[1] & SHIFT_MASK, Normal[2] >> SHIFT_AMOUNT, Normal[2] & SHIFT_MASK);
		printf("\x1b[20;2H Collision Index: %d", Collision);
		
		f32 V = Radius - Dist;
		printf("\x1b[21;2H V: %d", V);
	//	printf("\x1b[13;2H Player Dist: %d, %d", Dist, V);
		vec3 O = { Mul(Normal[0], V), Mul(Normal[1], V), Mul(Normal[2], V) };
		Add3(PlayerPos, O, PlayerPos);
	//	PlayerPos[1] += 1.0;
	}
}
*/

#define SHIFT_AMOUNT 12 // 2^16 = 65536
#define SHIFT_MASK ((1 << SHIFT_AMOUNT) - 1) 

void PlayerGroundCollision(u32* CollisionMesh)
{	
	PlayerPos[1] += PlayerVelocity;
	PlayerVelocity += Mul(PlayerFallSpeed, DeltaTime);
	
	vec3 Pos; Copy3(PlayerPos, Pos);
	Pos[1] += F1;//2548;
	vec3 T = { F2, F2, F2 };
	Div3(Pos, T, Pos);

	vec3 Normal = { 0, 0, 0};
	vec3 Dir = { 0, -F1, 0 };
	f32 Dist = F100;
	
	s8 Collision = ProccessCollision((u8*)CollisionMesh, Pos, Dir, F100, Normal, &Dist);//F200
	Mul3(Pos, T, Pos);
//	if (Normal[1] <= 1229)
//		return;
//	int Collision = KokiriForestGroundCollision(Pos, Radius, Dir, F100, Normal, &Dist);
//	if (Collision != -1)
//	{
//	//	f32 V = Radius - Dist;
//	//	vec3 O = { Normal[0] * V, Normal[1] * V, Normal[2] * V };
//	//	PlayerPos = Add3P(&PlayerPos, &O);
//		PlayerPos[1] = Dist - F1;
//	}
	
	
	
//	if (Dist >= 7144)//no collision
//	{
//		
//	}
//	if (Dist <= 7000 && PlayerVelocity < 0)

	if (Collision == COLLISION_TYPE_NO_AABB)
	{}
	else if (Collision == COLLISION_TYPE_NO_COLLISION_BUT_AABB)
	{}
	else
	{
		printf("\x1b[14;2H Collis Nor: %d.%d %d.%d %d.%d", Normal[0] >> SHIFT_AMOUNT, Normal[0] & SHIFT_MASK, Normal[1] >> SHIFT_AMOUNT, Normal[1] & SHIFT_MASK, Normal[2] >> SHIFT_AMOUNT, Normal[2] & SHIFT_MASK);
		printf("\x1b[20;2H Collision Index: %d", Collision);
		
		Dist = Mul(Dist, T[0]);
		if (Dist < PlayerHitboxSize)
		{
			PlayerVelocity = F0;
			PlayerOnGround = true;
			PlayerPos[1] -= Dist - F1;
		}			
		else
			PlayerOnGround = false;
	//	
	}
/*
	if (Collision != -1)//Ther is a collision
	{
		printf("\x1b[14;2H Collis Nor: %d.%d %d.%d %d.%d", Normal[0] >> SHIFT_AMOUNT, Normal[0] & SHIFT_MASK, Normal[1] >> SHIFT_AMOUNT, Normal[1] & SHIFT_MASK, Normal[2] >> SHIFT_AMOUNT, Normal[2] & SHIFT_MASK);
		printf("\x1b[20;2H Collision Index: %d", Collision);
	//	printf("\x1b[13;2H Player Dist: %d, %d", Dist, V);
	//	if (Normal[1] < 0.9)
	//	{
	//		f32 V = Radius - Dist;
	//		vec3 O = { Normal[0] * V, Normal[1] * V, Normal[2] * V };
	//		PlayerPos = Add3P(&PlayerPos, &O);
	//	}		
		PlayerPos[1] -= Dist - PlayerHitboxSize;
	//	PlayerPos[1] = Dist - 4096;
		PlayerOnGround = true;
		PlayerVelocity = F0;
	}
	else
	{
		PlayerOnGround = false;
	}

	if (Collision == -1)
		PlayerVelocity = F0;
		*/
}

//stylus = mouse
//D-Pad = wasd
//L-Shoulder = Q
void UpdateCamera()
{
	Sub3(CameraPos, PlayerPos, CameraDir);
	Normalize3(CameraDir);
	

//	if (Frame == 0)
//	{
	UpdatePlayer();
	//	PlayerWallCollision();
//	}
		
//	else if (Frame == 1)
	
	
//	PlayerWallCollision();
	PlayerGroundCollision(KokiriForestCollisionModel);
	
	CameraPos[1] = PlayerPos[1] + ToF32(2.4);
	
	PlayerCameraDist = GetDistanceVec3(CameraPos, PlayerPos);
	
	if (PlayerCameraDist > CameraMaxDist)
	{
		vec3 Dir; Sub3(PlayerPos, CameraPos, Dir);
		Dir[1] = 0;
		Normalize3(Dir);
		f32 Speed = Mul(DeltaTime, 4144);//5144==1,5
		vec3 r;
		Mul3(Dir, Vec3f(Speed, r), Dir);
	
		Add3(CameraPos, Dir, CameraPos);
	}
	
	if (KeyState & KEY_STATE_UP)
	{
		PlayerCameraDist = GetDistanceVec3(CameraPos, PlayerPos);

		if (PlayerCameraDist > CameraMaxDist)
		{
		//	Add3(CameraPos, PlayerDir, CameraPos);
		//	PlayerCameraDist = GetDistanceVec3P(&CameraPos, &PlayerPos);
		}
			
	}
	else if (KeyState & KEY_STATE_DOWN)
	{
		PlayerCameraDist = GetDistanceVec3(CameraPos, PlayerPos);
		
		if (PlayerCameraDist < CameraMinDist)
		{
			Add3(CameraPos, PlayerDir, CameraPos);
		//	PlayerCameraDist = GetDistanceVec3P(&CameraPos, &PlayerPos);
		}
	}
	
	printf("\x1b[12;0H Last Ply Pos: %d %d %d", PlayerPosLast[0], PlayerPosLast[1], PlayerPosLast[2]);
	printf("\x1b[13;0H Ply Pos: %d %d %d", PlayerPos[0], PlayerPos[1], PlayerPos[2]);
	
	//	CameraPos = Add3P(&CameraPosOff, &PlayerPos);
	//This is for animating water

	

}