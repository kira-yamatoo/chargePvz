#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QDebug>
#include <Address.h>
#include "tlhelp32.h"
#include <string.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("chargePvz");
    this->setFixedSize(800,600);
    this->setWindowIcon(QIcon());

    char *name=0;
    getProcessHandleByName(name);

    readProcess();

}

MainWindow::~MainWindow()
{
//    init(); //退出时还原游戏
    delete ui;
}

bool MainWindow::readProcess()
{
    DWORD dwPID= 0;
    dwPID= getProcessHandleByName("PlantsVsZombies.exe");

    this->hProcess= OpenProcess(PROCESS_ALL_ACCESS,FALSE,dwPID);
    if(!hProcess){
        QMessageBox::warning(this,"提示","找不到进程,请打开游戏",QMessageBox::Ok);
        return false;
    }
    return true;
}

int MainWindow:: getProcessHandleBywindow(char *name)
{
    DWORD dwPID= 0;
    HWND hwnd= FindWindow(NULL,TEXT(name));
    if(hwnd== NULL){
        qDebug("FindWindow Error");
        return -1;
    }

    return GetWindowThreadProcessId(hwnd, &dwPID);
}

int MainWindow::getProcessHandleByName(char* name)
{
    HANDLE hSnapShot= CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
    if ( hSnapShot == INVALID_HANDLE_VALUE )
    {
        qDebug("CreateToolhelp32Snapshot Error");
        return -1;
    }

    PROCESSENTRY32 stProcess;
    stProcess.dwSize= sizeof(PROCESSENTRY32);
//    memset(&stProcess,0,sizeof (PROCESSENTRY32));

    bool flag= Process32First(hSnapShot, &stProcess);
    while(flag){

        if(lstrcmp(stProcess.szExeFile,name)==0)
        {
            return stProcess.th32ProcessID;
        }
        flag=Process32Next(hSnapShot, &stProcess);

    };

    return -1;
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

    byte bufDisable[] = {0x7E,0x16};   //jle 0049CE1A
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

void MainWindow:: blockIceSurface(bool flag)
{
    DWORD baseAddress= ICE_SURFACE;    //冰面赋值地址
    byte bufDisable[] = {0xC7,0x84,0x90,0x3C,0x06,0x00,0x00,0xB8,0x0B,0x00,0x00};   //mov [eax+edx*4+0000063C],00000BB8 { 3000 }
    byte *bufEnable= replaceWithNop(sizeof (bufDisable));

    hookFunction(flag, baseAddress, bufEnable, bufDisable, sizeof (bufDisable));
}

void MainWindow:: blockGravestone(bool flag)
{
    DWORD baseAddress= GRAVESTONE_GEN;    //墓碑生成地址
    byte bufDisable[] = {0x74,0x96};   //je 004279D1
    byte *bufEnable= replaceWithNop(sizeof (bufDisable));

    hookFunction(flag, baseAddress, bufEnable, bufDisable, sizeof (bufDisable));
}

void MainWindow:: blockFog(bool flag)
{
    DWORD baseAddress= FOG_GEN;    //迷雾生成地址
    byte bufDisable[] = {0x89,0x11};   //mov [ecx],edx
    byte bufEnable[] = {0x89,0x01};   //mov [ecx],eax

    hookFunction(flag, baseAddress, bufEnable, bufDisable, sizeof (bufDisable));
}

void MainWindow:: backRunner(bool flag)
{
    DWORD baseAddress= BACK_RUNNER;    //后台运行地址
    byte bufDisable[] = {0x75,0x0F};   //jne 0045BFDB
    byte bufEnable[] = {0xEB,0x0F};   //jmp 0045BFDB

    hookFunction(flag, baseAddress, bufEnable, bufDisable, sizeof (bufDisable));
}

void MainWindow:: plantDeployLimit(bool flag)
{
    DWORD baseAddress= PLANT_DEPLOY_LIMIT;    //植物种植限制
    byte bufDisable[] = {0x0F,0x84,0x46,0x09,0x00,0x00};   //je 0041C679
    byte bufEnable[] = {0xE9,0x47,0x09,0x00,0x00,0x90};   //jmp 0041C679 nop

    hookFunction(flag, baseAddress, bufEnable, bufDisable, sizeof (bufDisable));
}
void MainWindow:: purpleCardLimit(bool flag)
{
    DWORD baseAddress= PURPLE_CARD_LIMIT;    //紫卡限制
    byte bufDisable[] = {0x75,0x16,0x8D,0x58,0xDF};   //jne 0042940E    lea ebx,[eax-21]
    byte bufEnable[] = {0xE9,0xB4,0x00,0x00,0x00};   //jmp 004294AF

    hookFunction(flag, baseAddress, bufEnable, bufDisable, sizeof (bufDisable));
}

void MainWindow:: shroomAwaken(bool flag)
{
    DWORD baseAddress= SHROOM_AWAKEN;    //蘑菇免唤醒
    byte bufDisable[] = {0x74,0x1F};   //je 0046C1E3
    byte bufEnable[] = {0xEB,0x1F};   //jmp 0046C1E3

    hookFunction(flag, baseAddress, bufEnable, bufDisable, sizeof (bufDisable));
}

void MainWindow:: injectCode()
{
    byte str[]={0x60,
                0x6A,0xFF,
                0x6A,0x03,
                0xB8,0x01,0x00,0x00,0x00,
                0x6A,0x01,
                0x8B,0x3D,0x0C,0x5E,0x75,0x00,
                0x8B,0xBF,0x68,0x08,0x00,0x00,
                0x57,
                0xE8,0x0C,0xF4,0xDE,0xE2,
                0x61,
                0xC3
               };
    int len=sizeof (str);

    LPVOID dataAddr  =  VirtualAllocEx(hProcess, 0 , len,MEM_COMMIT,PAGE_EXECUTE_READWRITE);
    // 在explorer的内存内里申请一块内存来存所用的数据
//    THREADDATA data  =   {TEXT("a.exe"),(WINEXEC)GetProcAddress(user32Handle,"WinExec"),} ;

    WriteProcessMemory(hProcess,dataAddr, &str, len, 0);
    // 把数据写到申请的内存中

    LPVOID codeAddr  =  VirtualAllocEx(hProcess, 0 ,1024*4,MEM_COMMIT,PAGE_EXECUTE_READWRITE);
    // 申请代码的内存区

    WriteProcessMemory(hProcess,codeAddr, &str, 1024*4, 0);
    // 把代码写进去，这时我们己经把我们要用的代码和数据都准备好了。

    HANDLE threadHandle  =  CreateRemoteThread(hProcess,NULL, 0 , (LPTHREAD_START_ROUTINE)codeAddr,dataAddr, 0 ,NULL);
    // 创建远程线程，来执行启动abc.exe的代码。所需的数据都己经在explorer的内存块中，所以不会出问题。

    WaitForSingleObject(threadHandle, INFINITE);
    VirtualFreeEx(hProcess,dataAddr, 0 ,MEM_RELEASE);
    VirtualFreeEx(hProcess,codeAddr, 0 ,MEM_RELEASE);
    CloseHandle(threadHandle);
    CloseHandle(hProcess);
    // 等待执行完毕，释放内存，关闭句柄。

//    LPVOID lpData = VirtualAllocEx(hProcess,
//                                   NULL,
//                                   sizeof(str),
//                                   MEM_COMMIT | MEM_RESERVE,
//                                   PAGE_READWRITE);
//    DWORD dwWriteNum = 0;
//    WriteProcessMemory(hProcess, lpData, &str, sizeof(str), &dwWriteNum);

//    DWORD dwFunSize = 0x2000;
//    LPVOID lpCode = VirtualAllocEx(hProcess,
//                                   NULL,
//                                   dwFunSize,
//                                   MEM_COMMIT,
//                                   PAGE_EXECUTE_READWRITE);
//    WriteProcessMemory(hProcess, lpCode, str, dwFunSize, &dwWriteNum);

//    HANDLE hRemoteThread = CreateRemoteThread(hProcess,
//                                              NULL,
//                                              0,
//                                              (LPTHREAD_START_ROUTINE)lpCode,
//                                              lpData,
//                                              0,
//                                              NULL);
//    WaitForSingleObject(hRemoteThread, INFINITE);

//    CloseHandle(hRemoteThread);

//    CloseHandle(hProcess);
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

void MainWindow::on_blockIceSurfaceCheckBox_stateChanged(int arg1)
{
    //获取进程
    if(!readProcess())
        return;
    blockIceSurface(arg1);

    //释放句柄
    CloseHandle(hProcess);
}

void MainWindow::on_blockGravestoneCheckBox_stateChanged(int arg1)
{
    //获取进程
    if(!readProcess())
        return;
    blockGravestone(arg1);

    //释放句柄
    CloseHandle(hProcess);
}

void MainWindow::on_blockFogCheckBox_stateChanged(int arg1)
{
    //获取进程
    if(!readProcess())
        return;
    blockFog(arg1);

    //释放句柄
    CloseHandle(hProcess);
}

void MainWindow::on_backRunnerCheckBox_stateChanged(int arg1)
{
    //获取进程
    if(!readProcess())
        return;
    backRunner(arg1);

    //释放句柄
    CloseHandle(hProcess);
}

void MainWindow::on_plantDeployLimitCheckBox_stateChanged(int arg1)
{
    //获取进程
    if(!readProcess())
        return;
    plantDeployLimit(arg1);

    //释放句柄
    CloseHandle(hProcess);
}

void MainWindow::on_purpleCardLimitCheckBox_stateChanged(int arg1)
{
    //获取进程
    if(!readProcess())
        return;
    purpleCardLimit(arg1);

    //释放句柄
    CloseHandle(hProcess);
}

void MainWindow::on_shroomAwakenCheckBox_stateChanged(int arg1)
{
    //获取进程
    if(!readProcess())
        return;
    shroomAwaken(arg1);

    //释放句柄
    CloseHandle(hProcess);
}
