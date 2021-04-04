const int FILE_NAME_LENGTH = 8;


const int MAX_FRAMES = 30;

const int MAX_MODEL_FRAMES=MAX_FRAMES;



//Таймер:
const int TimerInterval = 3;// время между кадрами
const TIMER_INTERVAL = TimerInterval;

static short int MAP_WIDTH  =  200;
static short int MAP_HEIGHT = 200;

const int LAND_SCALE=1;

const int TARGET_BLAST_FRAMES = 8;
//-------------------------------------------------------------
const int ACT_MOVE = 0;
const int ACT_ATTACK = 1;
const int ACT_WAIT = 2;




const ANALIS_INTERVAL = 0;
const SET_ANGELS_INTERVAL = 0;
const DO_AI_INTERVAL = 20;

const int SCALE = 1; //масштаб     сейчас 1:1 один пиксел к одному метру


static short int NUMBER_OF_UNITS = 25;
static short int NUMBER_OF_GROUPS = NUMBER_OF_UNITS;
static short int NUMBER_OF_TEAMS = 4;
static short int NUMBER_OF_STANDARTS = 1;
static short int NUMBER_OF_WEAPONS   = 2;
//const int OBJECT_WIDTH = 3;
const MAX_ENEMIES = 2;
const POSSIBLY_XY_MISTAKE = 0;
const int MAP_PASSABLY_DELTA = 70;
const int POSSIBLY_ANGLE_TURN = 2;
const int MAN_TEAM = 0; 


const NUMBER_OF_KVADRATS = 8; // количество квадаратов по одной из осей, на которые(про квадраты) будет разделена карта
// на самом деле карта будет разделена на (NUMBER_OF_KVADRATS)^2 квадаратов, точнеее равных между собой
// четырех угольников и подобных карте.

static int   MEN_IN_KVADRAT = (MAP_WIDTH*MAP_HEIGHT)/(NUMBER_OF_KVADRATS*NUMBER_OF_KVADRATS);
static int   KVADRAT_WIDTH = MAP_WIDTH/NUMBER_OF_KVADRATS;
static int   KVADRAT_HEIGHT = MAP_HEIGHT/NUMBER_OF_KVADRATS;


const PART_IN_CYRCLE = 16;
const SECOND_MAX_SIGHT = 60;
const GENERAL_MAX_SIGHT = 800;


enum { DEATH, NETRAL, WAIT, ATTACK, ATTACKED};
enum {DEL,PUT};
enum {Ms_SEE};
enum {WAIT_ACT=-1,ATTACK_ACT,GOBACK_ACT};
enum TWEAPON_TYPE {WP_GUN,WP_HOWITZER, WP_LASER};
enum {AGRESSIVE,PASSIVE};
enum TATTACK_MODE {NOT_ATTACK,ATTACK_NOT_MOVE,ATTACK_AND_MOVE};
enum {MAN,AI};


const float Pi                = 3.1415926535897932384626433832795;
const float OGIR              = Pi/180;              //One Gradus In Radians - O.G.I.R.
const float G                 = 0.08;
//const int NULL =0;

const DEBUG_TEAMS_INTERVAL = MAP_WIDTH/(NUMBER_OF_TEAMS+2);


const CRASH_RADIUS = 8;

const int POSSIBLY_PUT_INTERVAL = 1;
 