f32 CameraZoom = 5.0f;

vec3 CameraPos = { 0.0f, 2.5f, 18.0f };
vec3 CameraLookAt = { 0.0f, 0.0f, 0.0f };
vec3 CameraUp = { 0.0f, 1.0f, 0.0f };
vec3 CameraDir = { 0.0f, 0.0f, 0.0f };

int CameraRotX = 45.0f;
int CameraRotY = 45.0f;

vec3 LinkPos = { 0.0f, -2.0f, 22.0f };
vec3 LinkRot = { 0.0f, 0.0f, 0.0f };

f32 Distance = 5.5f;
f32 Speed = 0.11f;
f32 LinkCamDist = 0.0f;

f32 CamSpeed = 0.115f;


FORCE_INLINE void UpdateCamera()
{
	CameraLookAt = Vec3(LinkPos.x, LinkPos.y + 1.0f, LinkPos.z);
	LinkCamDist = GetDistanceVec3P(&LinkPos, &CameraPos);

	CameraDir = Normalize3(Sub3(LinkPos, CameraPos));

	if (LinkCamDist > Distance * 1.4f)
	{
		CamSpeed = 0.24f;
		CameraPos = Add3(CameraPos, Mul3(CameraDir, Vec3f(CamSpeed)));
	}		
	else if (LinkCamDist < Distance * 1.4f && LinkCamDist > Distance)
	{
		CamSpeed = 0.115f;
		CameraPos = Add3(CameraPos, Mul3(CameraDir, Vec3f(CamSpeed)));
	}
		

	CameraPos.y = 2.0f;
	gluLookAtV(&CameraPos,
		&CameraLookAt,
		&CameraUp);
}

FORCE_INLINE void UpdateLink()
{
	vec3 PrevPos = LinkPos;

	printf("\x1b[7;3H %f", LinkCamDist.Float());

	CameraDir = Mul3(CameraDir, Vec3f(Speed + 0.13f));
	if (KeysHeld & KEY_UP) { LinkPos = Add3P(&LinkPos, &CameraDir); }
	if (KeysHeld & KEY_DOWN) { LinkPos = Sub3P(&LinkPos, &CameraDir); }
	if (KeysHeld & KEY_LEFT) { LinkPos = Add3(LinkPos, Cross3P(&CameraUp, &CameraDir)); }
	if (KeysHeld & KEY_RIGHT) { LinkPos = Sub3(LinkPos, Cross3P(&CameraUp, &CameraDir)); }

	f32 NewDist = GetDistanceVec3P(&LinkPos, &CameraPos);

	if (LinkCamDist <= 4.3f && NewDist < LinkCamDist)
		LinkPos = PrevPos;

	LinkPos.y = -2.0f;
}