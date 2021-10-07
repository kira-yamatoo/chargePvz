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

    test();

    readProcess();

}

MainWindow::~MainWindow()
{
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

    this->ui->sunNotDecreaseCheckBox->setCheckState(Qt::CheckState(false));
    this->ui->plantLockHPCheckBox->setCheckState(Qt::CheckState(false));
    this->ui->coolDownCheckBox->setCheckState(Qt::CheckState(false));
    this->ui->autoCollectCheckBox->setCheckState(Qt::CheckState(false));


    //释放句柄
    CloseHandle(hProcess);
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
    byte bufEnable[] = {0x01,0xDE};   //add esi,ebx
    byte bufDisable[] = {0x29,0xDE};   //sub esi,ebx

    if(flag){
        WriteProcessMemory(hProcess,(void*)(baseAddress),bufEnable, sizeof(bufEnable), 0);
    }
    else{
        WriteProcessMemory(hProcess,(void*)(baseAddress), bufDisable, sizeof(bufDisable), 0);
    }
}

void MainWindow:: plantLockHP(bool flag)
{
    DWORD baseAddress= PLANT_LOCK_HP;    //减少植物HP基址
    byte bufEnable[] = {0x83,0x46,0x40,0x00};   //add dword ptr [esi+40],0
    byte bufDisable[] = {0x83,0x46,0x40,0xFC};   //add dword ptr [esi+40],-04

    if(flag){
        WriteProcessMemory(hProcess,(void*)(baseAddress),bufEnable, sizeof(bufEnable), 0);
    }
    else{
        WriteProcessMemory(hProcess,(void*)(baseAddress), bufDisable, sizeof(bufDisable), 0);
    }
}

void MainWindow:: coolDown(bool flag)
{
    DWORD baseAddress= COOL_DOWN;    //植物冷却flag基址
    byte bufEnable[] = {0xC6,0x43,0x48,0x01};   //mov byte ptr [ebx+48],01
    byte bufDisable[] = {0xC6,0x43,0x48,0x00};   //mov byte ptr [ebx+48],00

    if(flag){
        WriteProcessMemory(hProcess,(void*)(baseAddress),bufEnable, sizeof(bufEnable), 0);
    }
    else{
        WriteProcessMemory(hProcess,(void*)(baseAddress), bufDisable, sizeof(bufDisable), 0);
    }
}

void MainWindow:: autoCollect(bool flag)
{
    DWORD baseAddress= AUTO_COLLECT;    //点击掉落物flag基址
    byte bufEnable[] = {0x80,0x7B,0x50,0x01};   //cmp byte ptr [ebx+50],01
    byte bufDisable[] = {0x80,0x7B,0x50,0x00};   //cmp byte ptr [ebx+50],00

    if(flag){
        WriteProcessMemory(hProcess,(void*)(baseAddress),bufEnable, sizeof(bufEnable), 0);
    }
    else{
        WriteProcessMemory(hProcess,(void*)(baseAddress), bufDisable, sizeof(bufDisable), 0);
    }
}

void MainWindow::test()
{
    int a = 40;
    int b;
    asm ("movl %1, %%eax; \
    shr %%eax; \
    movl %%eax, %0;"
    : "=r" (b)
    : "r" (a)
    : "%eax");
    printf ("a = %d\nb = %d\n", a, b);

//    __asm__("movl %esp,%eax");


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
