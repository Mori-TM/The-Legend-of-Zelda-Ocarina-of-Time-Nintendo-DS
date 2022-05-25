float CameraZoom = 5.0;

vec3 CameraPos = { 0.0, 2.5, 18.0 };
vec3 CameraLookAt = { 0.0, 0.0, 0.0 };
vec3 CameraUp = { 0.0, 1.0, 0.0 };
vec3 CameraDir = { 0.0, 0.0, 0.0 };

int CameraRotX = 45.0;
int CameraRotY = 45.0;

vec3 LinkPos = { 0.0, -2.0, 22.0 };
vec3 LinkRot = { 0.0, 0.0, 0.0 };

float Distance = 7.0;
float Speed = 0.11;
float LinkCamDist = 0.0;

FORCE_INLINE void UpdateCamera()
{
   	CameraLookAt = Vec3(LinkPos.x, LinkPos.y + 1.0, LinkPos.z);
    LinkCamDist = GetDistanceVec3P(&LinkPos, &CameraPos);

	CameraDir = Normalize3(Sub3(LinkPos, CameraPos));

	if (LinkCamDist > Distance * 1.4)
		CameraPos = Add3(CameraPos, Mul3(CameraDir, Vec3f(Speed + 0.13)));
	else if (LinkCamDist < Distance * 1.4 && LinkCamDist > Distance)
		CameraPos = Add3(CameraPos, Mul3(CameraDir, Vec3f(Speed + 0.05)));

	CameraPos.y = 2.0;
	gluLookAtV(&CameraPos,
			   &CameraLookAt,
			   &CameraUp);
}

FORCE_INLINE void UpdateLink()
{
	vec3 PrevPos = LinkPos;
	
	printf("\x1b[7;3H %f", LinkCamDist);

	CameraDir = Mul3(CameraDir, Vec3f(Speed + 0.13));
	if (KeysHeld & KEY_UP) { LinkPos = Add3P(&LinkPos, &CameraDir); }
	if (KeysHeld & KEY_DOWN) { LinkPos = Sub3P(&LinkPos, &CameraDir); }
	if (KeysHeld & KEY_LEFT) { LinkPos = Add3(LinkPos, Cross3P(&CameraUp, &CameraDir)); }
	if (KeysHeld & KEY_RIGHT) { LinkPos = Sub3(LinkPos, Cross3P(&CameraUp, &CameraDir)); }

	float NewDist = GetDistanceVec3P(&LinkPos, &CameraPos);

	if (LinkCamDist <= 4.3 && NewDist < LinkCamDist)
		LinkPos = PrevPos;

	LinkPos.y = -2.0;
}