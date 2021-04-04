#include "EditConstants.h"
struct TWeapon
        {
                int  NameLen;
                char Name[32];
		bool Laser;
		bool Target;
		int TargetSpeed;
		int TargetDamage;
		int TargetRadius;
		int TargetRoof;

		int LaserRange;
		int LaserDamage;

		};

struct TGStandart
		{
                    int  NameLen;
                    char Name[32];
		    char FileName[20];
	        int	 Live,
                         Sight,
			 Speed,
			 AngleSpeed,
			 AttackPeriod,
                         Armor,
			 IndexWeapon,
			 Width,
			 Thinking,
			 NVO,
			 N;

                    bool CanAttack,
			 CanAttackMove,
			 CanMove;
                    int  MaxModelFrames;

		};
