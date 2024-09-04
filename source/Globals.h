bool UpdateTimer = true;

vec3 LightPos = { 0, 0, 0 };
vec3 LightColor = { 4096, 1839, 4 };

bool RenderMap = false;

u32 Ticks = 0;
f32 Ticksf32 = 0;

//#define FOV				-286720	//70.0
#define FOV				204800	//50.0
#define ASPECT_RATIO	5461	//1.3333333
#define NEAR_Z			410		//0.1
#define FAR_Z			4096000	//1000.0
#define LIGHT_DELAY		12
