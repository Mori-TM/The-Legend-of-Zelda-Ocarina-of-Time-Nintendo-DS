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

#define F0		0
#define F1		4096
#define F2		8192
#define F3		12288
#define F4		16384
#define F5		20480
#define F6		24576
#define F8		32768
#define F9		36864
#define F10		40960
#define F20		81920
#define F31		126976
#define F50		204800
#define F90		368640
#define F100	409600
#define F191	782336
#define F200	819200
#define F255	1044480
