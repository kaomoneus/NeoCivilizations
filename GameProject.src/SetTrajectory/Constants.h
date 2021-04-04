const int FILE_NAME_LENGTH = 8;


enum {FOCUS_INTERFACE, FOCUS_WORLD, FOCUS_MENU};



//Цвета команд:
#define TEAM_COLOR_0 255,0,0
#define TEAM_COLOR_1 0,255,0
#define TEAM_COLOR_2 0,0,255
#define TEAM_COLOR_3 0,255,255
#define TEAM_COLOR_4 255,0,255
#define TEAM_COLOR_5 255,255,0

//Цвета ландшафта:
#define LAND_COLOR_0 25,0,100
#define LAND_COLOR_1 150,200,200
#define LAND_COLOR_2 60,150,100
#define LAND_COLOR_3 0,255,255
#define LAND_COLOR_4 255,0,255
#define LAND_COLOR_5 255,255,0

//Таймер:
const int TimerInterval = 3;// время между кадрами
const TIMER_INTERVAL = TimerInterval;

//Указатель:
const int POINTER_MOVE = 1;
const int POINTER_ATTACK = 2;
const int POINTER_SELECT = 3;

const int CURSOR_WORLD = 0;
const int CURSOR_MENU = 1;

//Ограничения:
const int MAX_LANDTEX = 7;
const int MAX_MODELTEX = 3;
const int MAX_WEAPONTEX = 5;
const int MAX_FRAMES = 9;
const int MAX_PARTS = 53;
const int MAX_ACTIONS = 2;
const int MAX_STRINGS = 1;
const int MAX_MODEL_FRAMES=MAX_FRAMES;
const int MAX_TEX_SIZE = 64;
const int MAX_LANDTEX_SIZE = 16;
const int MAX_VARIATIONS = 5;
const int MAX_MENU = 3;
const int MAX_IMAGES = 6;
const int MAX_LINES = 256;
const int MAX_BUTTONS = 4;
const int MAX_BTN_TEX = 3;
const int MAX_CURSORS = 10;

const int MAX_LIGHTS = 10;
const int MAX_MODELS = 1;

//Разрешение и масштаб:
const int ResolutionX = 800;
const int ResolutionY = 600;
const int MAP_WIDTH = 200;
const int MAP_HEIGHT = 200;

const int LAND_SCALE=1;
const float MINIMAP_SCALE = 0.8;

const int TARGET_BLAST_FRAMES = 5;
//-------------------------------------------------------------
const int ACT_MOVE = 0;
const int ACT_ATTACK = 1;
const int ACT_WAIT = 2;




const ANALIS_INTERVAL = 0;
const SET_ANGELS_INTERVAL = 0;
const DO_AI_INTERVAL = 20;

const int SCALE = 1; //масштаб     сейчас 1:1 один пиксел к одному метру

const int NUMBER_OF_UNITS = 20;
const int NUMBER_OF_GROUPS = 10;
const int NUMBER_OF_TEAMS = 2;
const int NUMBER_OF_STANDARTS = 4;
const int OBJECT_WIDTH = 2;
const MAX_ENEMIES = 2;
const POSSIBLY_XY_MISTAKE = 5;
const int MAP_PASSABLY_DELTA = 2;
const int POSSIBLY_ANGLE_TURN = 2;
const int MAN_TEAM = 0; 


const NUMBER_OF_KVADRATS = 8; // количество квадаратов по одной из осей, на которые(про квадраты) будет разделена карта
// на самом деле карта будет разделена на (NUMBER_OF_KVADRATS)^2 квадаратов, точнеее равных между собой
// четырех угольников и подобных карте.

const MEN_IN_KVADRAT = (MAP_WIDTH*MAP_HEIGHT)/(NUMBER_OF_KVADRATS*NUMBER_OF_KVADRATS);
const KVADRAT_WIDTH = MAP_WIDTH/NUMBER_OF_KVADRATS;
const KVADRAT_HEIGHT = MAP_HEIGHT/NUMBER_OF_KVADRATS;


const PART_IN_CYRCLE = 128;
const SECOND_MAX_SIGHT = 60;
const GENERAL_MAX_SIGHT = 150;


enum { DEATH, NETRAL, WAIT, ATTACK, ATTACKED};
enum {DEL,PUT};
enum {Ms_SEE};
enum {WAIT_ACT=-1,ATTACK_ACT,GOBACK_ACT};
enum {WP_GUN};
enum {AGRESSIVE,PASSIVE};
enum {NOT_ATTACK,ATTACK_NOT_MOVE,ATTACK_AND_MOVE};
enum {MAN,AI};


const float Pi = 3.1415926535897932384626433832795;
const float OGIR = Pi/180;              //One Gradus In Radians - O.G.I.R.

//const int NULL =0;

const DEBUG_TEAMS_INTERVAL = MAP_WIDTH/(NUMBER_OF_TEAMS+2);

 