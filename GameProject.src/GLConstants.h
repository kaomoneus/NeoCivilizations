enum {FOCUS_GAME_MENU, FOCUS_WORLD, FOCUS_MENU};

const int FOG_DISTANCE = 100;

//Цвета команд:
#define TEAM_COLOR_0 255,0,0
#define TEAM_COLOR_1 0,255,0
#define TEAM_COLOR_2 0,0,255
#define TEAM_COLOR_3 0,255,255
#define TEAM_COLOR_4 255,0,255
#define TEAM_COLOR_5 255,255,0

const unsigned char TEAM_COLORS[6][3] = {{TEAM_COLOR_0}, {TEAM_COLOR_1}, {TEAM_COLOR_2},
										 {TEAM_COLOR_3}, {TEAM_COLOR_4}, {TEAM_COLOR_5}};



//Цвета ландшафта:
#define LAND_COLOR_0 25,0,100
#define LAND_COLOR_1 150,200,200
#define LAND_COLOR_2 60,150,100
#define LAND_COLOR_3 0,255,255
#define LAND_COLOR_4 255,0,255
#define LAND_COLOR_5 255,255,0

//Указатель:
const int POINTER_MOVE = 1;
const int POINTER_ATTACK = 2;
const int POINTER_SELECT = 3;

const int CURSOR_WORLD = 1;
const int CURSOR_MENU = 1;
const int CURSOR_PICK = 2;

const int CAMERA_MAX_HEIGHT = 50;
const int CAMERA_MIN_HEIGHT = 5;

//Ограничения:
const int MAX_LANDTEX = 6;
const int MAX_MODELTEX = 3;
const int MAX_WEAPONTEX = 5;
const int MAX_PARTS = 53;
const int MAX_ACTIONS = 2;
const int MAX_STRINGS = 10;
const int MAX_TEX_SIZE = 64;
const int MAX_LANDTEX_SIZE = 16;
const int MAX_VARIATIONS = 5;
const int MAX_MENU = 4;
const int MAX_IMAGES = 7;
const int MAX_PANELS = 6;
const int MAX_CONMENU = 1;
const int MAX_LINES = 256;
const int MAX_BUTTONS = 8;
const int MAX_BTN_TEX = 3;
const int MAX_CURSORS = 10;
const int MAX_MISCTEX = 2;

const int MAX_LIGHTS = 10;
const int MAX_MODELS = 1;

const int HINT_TIME = 5;

//Разрешение и масштаб:
const int ResolutionX = 800;
const int ResolutionY = 600;
const float InterfaceScale = 1;

const float MINIMAP_SCALE = 0.8;

const int CONSOLE_VISIBLE_TIME = 5;

const int SPLASH_WIDTH = 334;
const int SPLASH_HEIGHT = 222;

const int MAP_RESOLUTION = 1;
const int MAP_TEXTURES_RESOLUTION = 1;


const int VISIBLE_DISTANCE = 200;

const int SLOW_GAME_SPEED   = 2;
const int NORMAL_GAME_SPEED = 5;
const int FAST_GAME_SPEED   = 15;