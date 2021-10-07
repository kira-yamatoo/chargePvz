#ifndef ADDRESS_H
#define ADDRESS_H

#define IMAGE_BASE 0x00400000   //起始地址
#define SUN_ADDRESS 0x00755E0C; //阳光基址
#define SUN_ADDRESS_OFFSET_FIRST 0x868;   //阳光基址一级偏移
#define SUN_ADDRESS_OFFSET_SECOND 0x5578; //阳光基址二级偏移
#define SUN_NOT_DECREASE 0x00427694 //减少阳光基址
#define PLANT_LOCK_HP 0x0054BA6A    //减少植物HP基址
#define COOL_DOWN 0x0049E944;   //植物冷却flag基址
#define AUTO_COLLECT 0x0043CC6E;    //点击掉落物flag基址



#endif // ADDRESS_H
