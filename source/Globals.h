u16 SceneToRender = 1;
bool UpdateTimer = true;

vec3 LightPos = { 0.0f, 0.0f, 1.0f };
vec3 LightColor = { 1.0f, 0.449f, 0.001f };

u16 LinkDrawCount = 0;
bool RenderMap = false;

u32 Ticks = 0;
f32 Ticksf32 = 0.0;

u16 Frame = 0;
const u16 MaxFrames = 2;

const f32 FOV = -70.0f;
const f32 Aspect = 1.333333333333333f;
const f32 NearZ = 0.1f;
const f32 FarZ = 1000.0f;

const f32 LightDelay = 0.003f;

const f32 F0 = 0.0f;
const f32 F1 = 1.0f;
const f32 F2 = 2.0f;
const f32 F8 = 8.0f;
const f32 F90 = 90.0f;
const f32 F100 = 100.0f;

const f32 FN0 = -0.0f;
const f32 FN1 = -1.0f;
const f32 FN8 = -8.0f;
const f32 FN90 = -90.0f;