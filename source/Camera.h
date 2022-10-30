const vec3 CameraPosOff = { 0.0, 3.0, 5.0 };
vec3 CameraPos = { 0.0, 2.5, 8.0 };
vec3 CameraDir = { 0.0, 0.0,-1.0 };
vec3 CameraUp = { 0.0, 1.0, 0.0 };
vec3 PlayerPos = { 5.0, -2.0, 0.0 };
vec3 PlayerDir = { 0.0, 0.0,-1.0 };
//f32 PlayerRotY = 0.0;
vec2 PlayerRot = { 0.0, 0.0 };
f32 PlayerCameraDist = 0.0;

bool ChangedPlayerDir = false;

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
		PlayerDir = Mul3(PlayerDir, Vec3f(Speed));
		PlayerDir = Cross3P(&CameraUp, &PlayerDir);
		PlayerPos = Add3P(&PlayerPos, &PlayerDir);

		ChangedPlayerDir = true;
	}
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
	}
	if (KeysHeld & KEY_DOWN)
	{
		PlayerDir.x = CameraDir.x;
		PlayerDir.y = 0.0;
		PlayerDir.z = CameraDir.z;
		Normalize3P(&PlayerDir);
		PlayerDir = Mul3(PlayerDir, Vec3f(Speed));
	//	PlayerDir = CameraDir;
		PlayerPos = Add3P(&PlayerPos, &PlayerDir);

		if (PlayerCameraDist < 5.8)
			CameraPos = Add3P(&CameraPos, &PlayerDir);

		ChangedPlayerDir = true;
	}
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

		if (PlayerCameraDist > 7.8)
			CameraPos = Add3P(&CameraPos, &PlayerDir);

		ChangedPlayerDir = true;
	}

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
	CameraPos.y = PrevCameraY;
}

//stylus = mouse
//D-Pad = wasd
//L-Shoulder = Q
void UpdateCamera()
{
	PlayerCameraDist = GetDistanceVec3P(&CameraPos, &PlayerPos);
	//	Uint8* Key = SDL_GetKeyboardState(NULL);




	CameraDir = Sub3(CameraPos, PlayerPos);
	Normalize3P(&CameraDir);
//	LightPos = { CameraDir.x, 0.0, CameraDir.z };
//	Normalize3P(&LightPos);
//	LightPos = CameraDir;
	PlayerRot = { -CameraDir.x * 2.0, -CameraDir.z * 2.0 };
	Normalize2P(&PlayerRot);
	UpdatePlayer();

	PlayerPos.y += 1.0;
	gluLookAtV(&CameraPos, &PlayerPos, &CameraUp);
	PlayerPos.y -= 1.0;
	//	CameraPos = Add3P(&CameraPosOff, &PlayerPos);
}