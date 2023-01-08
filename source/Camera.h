const vec3 CameraPosOff = { 0.0, 3.0, 5.0 };
vec3 CameraPos = { 0.0, 3.0, 8.0 };
vec3 CameraDir = { 0.0, 0.0,-1.0 };
vec3 CameraUp = { 0.0, 1.0, 0.0 };
vec3 PlayerPos = { 5.0, 0.0, 0.0 };
vec3 PlayerDir = { 0.0, 0.0,-1.0 };
vec3 PlayerNor = { 0.0, 0.0,-1.0 };
f32 PlayerCameraDist = 0.0;

f32 PlayerFallSpeed = -0.25;
f32 PlayerVelocity = 0.0;

bool ChangedPlayerDir = false;

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
	f32 Speed = DeltaTime * 1.5;
	CameraDir = Mul3(CameraDir, Vec3f(Speed));

	f32 PrevPlayerY = PlayerPos.y;
	f32 PrevCameraY = CameraPos.y;


	if (KeysHeld & KEY_RIGHT)
	{
		PlayerDir.x = CameraDir.x;
		PlayerDir.y = 0.0;
		PlayerDir.z = CameraDir.z;
		Normalize3P(&PlayerDir);

	//	PlayerNor.x = -PlayerDir.x;
	//	PlayerNor.y = -PlayerDir.y;
	//	PlayerNor.z = -PlayerDir.z;

		PlayerDir = Mul3(PlayerDir, Vec3f(Speed));
		PlayerDir = Cross3P(&CameraUp, &PlayerDir);
		PlayerPos = Add3P(&PlayerPos, &PlayerDir);

		ChangedPlayerDir = true;
		KeyState |= KEY_STATE_RIGHT;
	}
	else
		KeyState &= ~KEY_STATE_RIGHT;
	if (KeysHeld & KEY_LEFT)
	{
		PlayerDir.x = -CameraDir.x;
		PlayerDir.y = 0.0;
		PlayerDir.z = -CameraDir.z;
		Normalize3P(&PlayerDir);

		

		PlayerDir = Mul3(PlayerDir, Vec3f(Speed));
		PlayerDir = Cross3P(&CameraUp, &PlayerDir);
	//	PlayerDir.x = -PlayerDir.x;
	//	PlayerDir.y = -PlayerDir.y;
	//	PlayerDir.z = -PlayerDir.z;

		PlayerPos = Add3P(&PlayerPos, &PlayerDir);

		ChangedPlayerDir = true;
		KeyState |= KEY_STATE_LEFT;
	}
	else
		KeyState &= ~KEY_STATE_LEFT;
	if (KeysHeld & KEY_DOWN)
	{
		PlayerDir.x = CameraDir.x;
		PlayerDir.y = 0.0;
		PlayerDir.z = CameraDir.z;
		Normalize3P(&PlayerDir);

		PlayerDir = Mul3(PlayerDir, Vec3f(Speed));
	//	PlayerDir = CameraDir;
		PlayerPos = Add3P(&PlayerPos, &PlayerDir);

		

		ChangedPlayerDir = true;
		KeyState |= KEY_STATE_DOWN;
	}
	else
		KeyState &= ~KEY_STATE_DOWN;
	if (KeysHeld & KEY_UP)
	{
	//	PlayerDir.x = -CameraDir.x;
	//	PlayerDir.y = -CameraDir.y;
	//	PlayerDir.z = -CameraDir.z;
		PlayerDir.x = -CameraDir.x;
		PlayerDir.y = 0.0;
		PlayerDir.z = -CameraDir.z;
		Normalize3P(&PlayerDir);
		PlayerDir = Mul3(PlayerDir, Vec3f(Speed));
		PlayerPos = Add3P(&PlayerPos, &PlayerDir);

		

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
//		f32 AbsX = fabsf(Dir.x);
//		f32 AbsZ = fabsf(Dir.z);
//
//		mat4 M = RotateYMat4(LoadMat4Identity(), ToRadians(PlayerRotY));
//		M = TranslateMat4(M, PlayerPos);
//		vec4 Pos = { CameraPos.x, CameraPos.y, CameraPos.z, 1.0 };
//
//		Pos = MultiplyVec4Mat4P(&Pos, &M);
//		CameraPos.x = Pos.x;
//		CameraPos.y = Pos.y;
//		CameraPos.z = Pos.z;
//
//		ChangedPlayerDir = false;
//		//	CameraPos = Mul3P(&CameraPos, &Dir);
//	}

//	vec3 Dir = Div3(PlayerDir, Vec3f(Speed));
//	PrintVec3(&CameraPos);

	PlayerPos.y = PrevPlayerY;
}

void PlayerWallCollision()
{
	vec3 Pos = PlayerPos;
	Pos.y += 1.0;
	vec3 Normal = {0.0, 0.0, 0.0};
	f32 Radius = 2.5f;
	f32 Dist = F0;
	PlayerNor = PlayerDir;
//	PlayerNor.y = F0;
	Normalize3P(&PlayerNor);
	int Collision = KokiriForestWallCollision(&Pos, Radius, &PlayerNor, 100.0f, &Normal, &Dist);
//	printf("\x1b[17;2H Collis Nor: %d %d %d", Normal.x.Fixed, Normal.y.Fixed, Normal.z.Fixed);

	if (Collision == -1)//no collision
	{
		
	}
	else
	{
		
		f32 V = Radius - Dist;
	//	printf("\x1b[13;2H Player Dist: %d, %d", Dist.Fixed, V.Fixed);
		vec3 O = { Normal.x * V, Normal.y * V, Normal.z * V };
		PlayerPos = Add3P(&PlayerPos, &O);
	//	PlayerPos.y += 1.0;
	}
}

void PlayerGroundCollision()
{	
	vec3 Pos = PlayerPos;
	Pos.y += 1.0;
	vec3 Normal = {0.0, 0.0, 0.0};
	f32 Radius = 1.5f;
	f32 Dist = F0;
	vec3 Dir = { 0.0, -1.0, 0.0 };
	int Collision = KokiriForestGroundCollision(&Pos, Radius, &Dir, 100.0f, &Normal, &Dist);	
//	if (Collision != -1)
//	{
//	//	f32 V = Radius - Dist;
//	//	vec3 O = { Normal.x * V, Normal.y * V, Normal.z * V };
//	//	PlayerPos = Add3P(&PlayerPos, &O);
//		PlayerPos.y = Dist - F1;
//	}

	if (Collision == -1)//no collision
	{
		PlayerPos.y += PlayerVelocity;
		PlayerVelocity += PlayerFallSpeed * DeltaTime;
	}
	else
	{
		printf("\x1b[17;2H Collis Nor: %d %d %d", Normal.x.Fixed, Normal.y.Fixed, Normal.z.Fixed);
		
	//	printf("\x1b[13;2H Player Dist: %d, %d", Dist.Fixed, V.Fixed);
	//	if (Normal.y < 0.9)
	//	{
	//		f32 V = Radius - Dist;
	//		vec3 O = { Normal.x * V, Normal.y * V, Normal.z * V };
	//		PlayerPos = Add3P(&PlayerPos, &O);
	//	}		

		PlayerVelocity = F0;
	}

	
}

//stylus = mouse
//D-Pad = wasd
//L-Shoulder = Q
void UpdateCamera()
{
	CameraDir = Sub3(CameraPos, PlayerPos);
	Normalize3P(&CameraDir);
	

//	if (Frame == 0)
//	{
	UpdatePlayer();
	//	PlayerWallCollision();
//	}
		
//	else if (Frame == 1)
	
	
	PlayerWallCollision();
	PlayerGroundCollision();
	
	CameraPos.y = PlayerPos.y + 3.5;

	if (KeyState & KEY_STATE_UP)
	{
		PlayerCameraDist = GetDistanceVec3P(&CameraPos, &PlayerPos);

		if (PlayerCameraDist > 7.8)
		{
			CameraPos = Add3P(&CameraPos, &PlayerDir);
		//	PlayerCameraDist = GetDistanceVec3P(&CameraPos, &PlayerPos);
		}
			
	}
	else if (KeyState & KEY_STATE_DOWN)
	{
		PlayerCameraDist = GetDistanceVec3P(&CameraPos, &PlayerPos);
		
		if (PlayerCameraDist < 5.8)
		{
			CameraPos = Add3P(&CameraPos, &PlayerDir);
		//	PlayerCameraDist = GetDistanceVec3P(&CameraPos, &PlayerPos);
		}
	}

	
	PlayerPos.y += 1.0;
	gluLookAtV(&CameraPos, &PlayerPos, &CameraUp);
	PlayerPos.y -= 1.0;

	//	CameraPos = Add3P(&CameraPosOff, &PlayerPos);
	//This is for animating water

	vec2 Time = { DeltaTime * 0.05f, DeltaTime * 0.05f };

	for (int j = 0; j <  KokiriForestVertexCounts[33]; j += 3)
	{
		vec2 TexA = GetTexCoordFromList((u32*)waterModel, j);
		vec2 TexB = GetTexCoordFromList((u32*)waterModel, j + 1);
		vec2 TexC = GetTexCoordFromList((u32*)waterModel, j + 2);
	//	f32 Multi = 0.0019531f;
	//	vec2 Time = { DeltaTime * Multi, DeltaTime * Multi };
		
		TexA = Sub2P(&TexA, &Time);
		TexB = Sub2P(&TexB, &Time);
		TexC = Sub2P(&TexC, &Time);
		SetTexCoordToList((u32*)waterModel, j	   , TexA);
		SetTexCoordToList((u32*)waterModel, j + 1, TexB);
		SetTexCoordToList((u32*)waterModel, j + 2, TexC);
	//	if (SphereRayVsTriangle(Pos, Radius, RayDir, TriScale, &A, &B, &C, Normal, Dist)) return 34;
	}
	
}