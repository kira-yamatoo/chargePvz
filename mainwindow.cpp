#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QDebug>
#include <MemoryUtil.h>
#include <Address.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("chargePvz");
    this->setFixedSize(800,600);
    this->setWindowIcon(QIcon());

//    test();

    readProcess();

}

MainWindow::~MainWindow()
{
    init(); //退出时还原游戏
    delete ui;
}

bool MainWindow::readProcess()
{
    DWORD dwPID= 0;
    HWND hwnd= FindWindow(NULL,TEXT("Plants vs. Zombies"));
    if(hwnd== NULL){
        QMessageBox::warning(this,"提示","找不到窗口,请打开游戏",QMessageBox::Ok);
        return false;
    }
//    GetModuleHandleA("PlantsVsZombies.exe");

    GetWindowThreadProcessId(hwnd, &dwPID);
    this->hProcess= OpenProcess(PROCESS_ALL_ACCESS,FALSE,dwPID);
    if(hProcess== NULL){
        QMessageBox::warning(this,"提示","找不到进程,请打开游戏",QMessageBox::Ok);
        return false;
    }
    return true;
}

void MainWindow:: init()
{
    //获取进程
    if(!readProcess())
        return;

    sunNotDecrease(false);
    plantLockHP(false);
    coolDown(false);
    autoCollect(false);

    doomShroomDeploy(false);
    cobCannoDeploy(false);
    potatoMineDeploy(false);
    chomperDeploy(false);
    magnetShroomDeploy(false);

    this->ui->sunNotDecreaseCheckBox->setCheckState(Qt::CheckState(false));
    this->ui->plantLockHPCheckBox->setCheckState(Qt::CheckState(false));
    this->ui->coolDownCheckBox->setCheckState(Qt::CheckState(false));
    this->ui->autoCollectCheckBox->setCheckState(Qt::CheckState(false));

    this->ui->doomShroomCheckBox->setCheckState(Qt::CheckState(false));
    this->ui->cobCannoDeployCheckBox->setCheckState(Qt::CheckState(false));
    this->ui->potatoMineDeployCheckBox->setCheckState(Qt::CheckState(false));
    this->ui->chomperDeployCheckBox->setCheckState(Qt::CheckState(false));
    this->ui->magnetShroomDeployCheckBox->setCheckState(Qt::CheckState(false));


    //释放句柄
    CloseHandle(hProcess);
}

byte* MainWindow:: replaceWithNop(int len)
{
    byte nop={0x90};    //传入长度len并返回长度为len的byte数组,并全部使用空指令nop替代
    static byte temp[]={};
    for(int i=0;i<len; i++)
    {
        temp[i]= nop;
    }
    return temp;
}

void  MainWindow:: hookFunction(bool flag, DWORD baseAddress, byte bufEnable[],byte bufDisable[],int len)
{
    if(flag){
        WriteProcessMemory(hProcess,(void*)(baseAddress),bufEnable, len, 0);
    }
    else{
        WriteProcessMemory(hProcess,(void*)(baseAddress), bufDisable, len, 0);
    }
}

void MainWindow::addSun()
{
    DWORD baseAddress= SUN_ADDRESS; //阳光基址
    DWORD offSetFirst= SUN_ADDRESS_OFFSET_FIRST;   //一级偏移
    DWORD offSetSecond= SUN_ADDRESS_OFFSET_SECOND; //二级偏移
    DWORD tempAddr;

    DWORD sunValue;    //阳光值

    ReadProcessMemory(hProcess,(void*)(baseAddress), &tempAddr, sizeof(DWORD), 0);
    ReadProcessMemory(hProcess,(void*)(tempAddr+ offSetFirst), &tempAddr, sizeof(DWORD), 0);
    ReadProcessMemory(hProcess,(void*)(tempAddr+ offSetSecond), &sunValue, sizeof(DWORD), 0);
    if(sunValue<=500){
        sunValue=2333;
        WriteProcessMemory(hProcess,(void*)(tempAddr+ offSetSecond), &sunValue, sizeof(DWORD), 0);
    }
}

void MainWindow::sunNotDecrease(bool flag)
{
    DWORD baseAddress= SUN_NOT_DECREASE;    //减少阳光基址

    byte bufDisable[] = {0x29,0xDE};   //sub esi,ebx
    byte *bufEnable= replaceWithNop(sizeof (bufDisable));

    hookFunction(flag, baseAddress, bufEnable, bufDisable, sizeof (bufDisable));
}

void MainWindow:: plantLockHP(bool flag)
{
    DWORD baseAddress= PLANT_LOCK_HP;    //减少植物HP基址

    byte bufDisable[] = {0x83,0x46,0x40,0xFC};   //add dword ptr [esi+40],-04
    byte *bufEnable= replaceWithNop(sizeof (bufDisable));

    hookFunction(flag, baseAddress, bufEnable, bufDisable, sizeof (bufDisable));
}

void MainWindow:: coolDown(bool flag)
{
    DWORD baseAddress= COOL_DOWN;    //植物冷却flag基址

    byte bufDisable[] = {0xC6,0x43,0x48,0x00};   //mov byte ptr [ebx+48],00
    byte *bufEnable= replaceWithNop(sizeof (bufDisable));

    hookFunction(flag, baseAddress, bufEnable, bufDisable, sizeof (bufDisable));
}

void MainWindow:: autoCollect(bool flag)
{
    DWORD baseAddress= AUTO_COLLECT;    //点击掉落物flag基址

    byte bufDisable[] = {0x80,0x7B,0x50,0x00};   //cmp byte ptr [ebx+50],00
    byte bufEnable[] = {0x80,0x7B,0x50,0x01};   //cmp byte ptr [ebx+50],01

    hookFunction(flag, baseAddress, bufEnable, bufDisable, sizeof (bufDisable));
}

void MainWindow:: doomShroomDeploy(bool flag)
{
    DWORD baseAddress= DOOM_SHROOM_DEPLOY;    //毁灭菇部署倒计时赋值基址
    byte bufDisable[] = {0xC7,0x40,0x18,0x50,0x46,0x00,0x00};   //mov [eax+18],00004650
    byte *bufEnable= replaceWithNop(sizeof (bufDisable));

    hookFunction(flag, baseAddress, bufEnable, bufDisable, sizeof (bufDisable));
}

void MainWindow:: cobCannoDeploy(bool flag)
{
    DWORD baseAddress= COB_CANNNO_DEPLOY;    //玉米加农部署倒计时赋值基址

    byte bufDisable[] = {0xC7,0x47,0x54,0xB8,0x0b,0x00,0x00};   //mov [edi+54],00000BB8
    byte *bufEnable= replaceWithNop(sizeof (bufDisable));

    hookFunction(flag, baseAddress, bufEnable, bufDisable, sizeof (bufDisable));
}

void MainWindow:: potatoMineDeploy(bool flag)
{
    DWORD baseAddress= POTATO_MINE_DEPLOY;    //土豆雷部署倒计时赋值基址

    byte bufDisable[] = {0xC7,0x40,0x54,0xDC,0x05,0x00,0x00};   //mov [eax+54],000005DC
    byte *bufEnable= replaceWithNop(sizeof (bufDisable));

    hookFunction(flag, baseAddress, bufEnable, bufDisable, sizeof (bufDisable));
}

void MainWindow:: chomperDeploy(bool flag)
{
    DWORD baseAddress= CHOMPER_DEPLOY;    //大嘴花部署倒计时赋值基址

    byte bufDisable[] = {0xC7,0x47,0x54,0xA0,0x0F,0x00,0x00};   //mov [edi+54],00000FA0
    byte *bufEnable= replaceWithNop(sizeof (bufDisable));

    hookFunction(flag, baseAddress, bufEnable, bufDisable, sizeof (bufDisable));
}

void MainWindow:: magnetShroomDeploy(bool flag)
{
    DWORD baseAddress= MAGNET_SHROOM_DEPLOY;    //磁力菇部署倒计时赋值基址

    byte bufDisable[] = {0xC7,0x46,0x54,0xDC,0x05,0x00,0x00};   //mov [esi+54],000005DC
    byte *bufEnable= replaceWithNop(sizeof (bufDisable));

    hookFunction(flag, baseAddress, bufEnable, bufDisable, sizeof (bufDisable));
}

void MainWindow:: plantAtkBullet(bool flag)
{
    DWORD baseAddress= PLANT_ATK_BULLET;    //植物攻击判定跳转基址
    byte bufDisable[] = {0x0f,0x85,0x98,0xFE,0xFF,0xFF};   //jne 00472D82
    byte *bufEnable= replaceWithNop(sizeof (bufDisable));

    hookFunction(flag, baseAddress, bufEnable, bufDisable, sizeof (bufDisable));
}

void MainWindow::test()
{
//    int a = 40;
//    int b;
//    asm ("movl %1, %%eax; \
//    shr %%eax; \
//    movl %%eax, %0;"
//    : "=r" (b)
//    : "r" (a)
//    : "%eax");
//    printf ("a = %d\nb = %d\n", a, b);

//    __asm__("alloc(newmen,20)");
//    __asm__("createthread(newmen)");
//    __asm__("newmen:");
//    __asm__("pushad");
//    __asm__("push -1");
//    __asm__("push 4");
//    __asm__("mov eax,0");
//    __asm__("mov ebp,[731C50]");
//    __asm__("mov ebp,[ebp+868]");
//    __asm__("push ebp");
//    __asm__("call 004105A0");
//    __asm__("popad");
//    __asm__("ret");

}


/**
 * --------------------------UI相关--------------------------------------------------------------
 */
void MainWindow::on_addSunButton_clicked()
{
    //获取进程
    if(!readProcess())
        return;
    addSun();

    //释放句柄
    CloseHandle(hProcess);

}

void MainWindow::on_sunNotDecreaseCheckBox_stateChanged(int arg1)
{
    //获取进程
    if(!readProcess())
        return;
    sunNotDecrease(arg1);

    //释放句柄
    CloseHandle(hProcess);
}

void MainWindow::on_plantLockHPCheckBox_stateChanged(int arg1)
{
    //获取进程
    if(!readProcess())
        return;
    plantLockHP(arg1);

    //释放句柄
    CloseHandle(hProcess);
}

void MainWindow::on_coolDownCheckBox_stateChanged(int arg1)
{
    //获取进程
    if(!readProcess())
        return;
    coolDown(arg1);

    //释放句柄
    CloseHandle(hProcess);
}

void MainWindow::on_autoCollectCheckBox_stateChanged(int arg1)
{
    //获取进程
    if(!readProcess())
        return;
    autoCollect(arg1);

    //释放句柄
    CloseHandle(hProcess);
}

void MainWindow::on_actionInit_triggered()
{
    init();
}

void MainWindow::on_doomShroomCheckBox_stateChanged(int arg1)
{
    //获取进程
    if(!readProcess())
        return;
    doomShroomDeploy(arg1);

    //释放句柄
    CloseHandle(hProcess);
}

void MainWindow::on_cobCannoDeployCheckBox_stateChanged(int arg1)
{
    //获取进程
    if(!readProcess())
        return;
    cobCannoDeploy(arg1);

    //释放句柄
    CloseHandle(hProcess);
}

void MainWindow::on_potatoMineDeployCheckBox_stateChanged(int arg1)
{
    //获取进程
    if(!readProcess())
        return;
    potatoMineDeploy(arg1);

    //释放句柄
    CloseHandle(hProcess);
}

void MainWindow::on_chomperDeployCheckBox_stateChanged(int arg1)
{
    //获取进程
    if(!readProcess())
        return;
    chomperDeploy(arg1);

    //释放句柄
    CloseHandle(hProcess);
}

void MainWindow::on_magnetShroomDeployCheckBox_stateChanged(int arg1)
{
    //获取进程
    if(!readProcess())
        return;
    magnetShroomDeploy(arg1);

    //释放句柄
    CloseHandle(hProcess);
}

void MainWindow::on_plantAtkBulletCheckBox_stateChanged(int arg1)
{
    //获取进程
    if(!readProcess())
        return;
    plantAtkBullet(arg1);

    //释放句柄
    CloseHandle(hProcess);
}
