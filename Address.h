#ifndef ADDRESS_H
#define ADDRESS_H

#define IMAGE_BASE 0x00400000   //起始地址
#define SUN_ADDRESS 0x00755E0C //阳光基址
#define SUN_ADDRESS_OFFSET_FIRST 0x868   //阳光基址一级偏移
#define SUN_ADDRESS_OFFSET_SECOND 0x5578 //阳光基址二级偏移
#define SUN_NOT_DECREASE 0x00427694 //减少阳光基址
#define PLANT_LOCK_HP 0x0054BA6A    //减少植物HP基址
#define COOL_DOWN 0x0049CE02   //植物冷却flag基址
#define AUTO_COLLECT 0x0043CC6E    //点击掉落物flag基址
#define DOOM_SHROOM_DEPLOY 0x00474D27    //毁灭菇部署倒计时赋值基址
#define COB_CANNNO_DEPLOY 0x00473196   //玉米加农部署倒计时赋值基址
#define POTATO_MINE_DEPLOY 0x0046C679  //土豆雷部署倒计时赋值基址
#define CHOMPER_DEPLOY 0x0046F90E  //大嘴花部署倒计时赋值基址
#define MAGNET_SHROOM_DEPLOY 0x0046F9EA    //磁力菇部署倒计时赋值基址
#define KERNEL_PULT_LOCK_BUTTER 0x00   //玉米投手锁定黄油
#define CATAPULT_ZOMBIE_ATK 0x0047CB99  //投石车僵尸减少植物HP基址
#define PLANT_ATK_BULLET 0x00472EE4   //植物攻击判定跳转基址
#define PLANT_LOCKED_ATK 0x00472CD9 //锁定植物攻击动作
#define ICE_SURFACE 0x0054659F  //冰面赋值地址
#define GRAVESTONE_GEN 0x00427A39 //墓碑生成地址
#define FOG_GEN 0x00426173//迷雾生成地址
#define BACK_RUNNER 0x0045BFCA //后台运行地址
#define PLANT_DEPLOY_LIMIT 0x0041BD2D   //植物种植限制
#define PURPLE_CARD_LIMIT 0x004293F6    //紫卡限制
#define SHROOM_AWAKEN 0x0046C1C2    //蘑菇免唤醒

#endif // ADDRESS_H
