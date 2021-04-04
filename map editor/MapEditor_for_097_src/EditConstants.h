const WORD_LENGTH = 24;
const STRING_LENGTH = 5*WORD_LENGTH;
const char NULL_NAME[WORD_LENGTH] = "NULL";


// Константы для определения типа файла.
 enum TChangeType{WEAPON,TYPE};
 char WEAPONS[WORD_LENGTH]      = "Weapons";
 char OBJECT_TYPES[WORD_LENGTH] = "Standarts";
//-----------------------------------------------------------
//char O_LIVE[WORD_LENGTH] = "Live";




// Зарезервированные слова для оружия.
const char NAME_WEAPON_NAME[WORD_LENGTH]      = "Name";
const char NAME_WEAPON_FILE_NAME[WORD_LENGTH] = "FileName";
const char NAME_LASER[WORD_LENGTH]            = "IT_LASER";
const char NAME_TARGET[WORD_LENGTH]           = "IT_TARGET";
const char NAME_TARGET_SPEED[WORD_LENGTH]     = "TARGET_SPEED";
const char NAME_TARGET_DAMAGE[WORD_LENGTH]    = "TARGET_DAMAGE";
const char NAME_TARGET_RADIUS[WORD_LENGTH]    = "TARGET_RADIUS";
const char NAME_TARGET_ROOF[WORD_LENGTH]      = "TARGET_ROOF";

const char NAME_LASER_RANGE[WORD_LENGTH]      = "LASER_RANGE";
const char NAME_LASER_DAMAGE[WORD_LENGTH]     = "LASER_DAMAGE";
//-----------------------------------------------------------

// Зарезервированные слова для типов объектов.
//-----------------------------------------------------------

const char NAME_OBJECT_NAME[WORD_LENGTH]            = "Name";
const char NAME_OBJECT_FILE_NAME[WORD_LENGTH]       = "FileName";
const char NAME_OBJECT_LIVE[WORD_LENGTH]            = "LIVE";
const char NAME_OBJECT_SIGHT[WORD_LENGTH]           = "SIGHT";
const char NAME_OBJECT_SPEED[WORD_LENGTH]           = "SPEED";
const char NAME_OBJECT_ANGLE_SPEED[WORD_LENGTH]     = "ANGLE_SPEED";
const char NAME_OBJECT_ATTACK_PERIOD[WORD_LENGTH]   = "ATTACK_PERIOD";
const char NAME_OBJECT_ARMOR[WORD_LENGTH]           = "ARMOR";
const char NAME_OBJECT_WEAPON[WORD_LENGTH]          = "WEAPON";
const char NAME_OBJECT_WIDTH[WORD_LENGTH]           = "WIDTH";
const char NAME_OBJECT_THINKING[WORD_LENGTH]        = "THINKING";
const char NAME_OBJECT_NVO[WORD_LENGTH]             = "NVO";
const char NAME_OBJECT_N[WORD_LENGTH]               = "VICTORY_PROBALITY";
const char NAME_OBJECT_CAN_ATTACK[WORD_LENGTH]      = "CAN_ATTACK";
const char NAME_OBJECT_CAN_ATTACK_MOVE[WORD_LENGTH] = "CAN_ATTACK_MOVE";
const char NAME_OBJECT_CAN_MOVE[WORD_LENGTH]        = "CAN_MOVE";
const char NAME_OBJECT_MAX_MODEL_FRAMES[WORD_LENGTH]= "MODEL_FRAMES";



// Сообщения редактора карт
//------------------------------------------------------------
const AnsiString BRUSH_ERROR = "Значение радиуса кисти некорректно. Отменено.";
const AnsiString  LOAD_ERROR = "Неверный формат данных. Загрузка преостановлена.";
