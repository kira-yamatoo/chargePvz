#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTimer.h>
#include <QMessageBox>
#include <QDebug>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("chargePvz");
    this->setFixedSize(800,600);
    this->setWindowIcon(QIcon());

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

void MainWindow::addSun()
{
    DWORD sunBaseAddress= 0x00755E0C; //阳光基址
    DWORD offSetFirst= 0x868;   //一级偏移
    DWORD offSetSecond= 0x5578; //二级偏移
    DWORD tempAddr;

    DWORD sunValue=1000;    //阳光值

    DWORD dwSize = 0;
    ReadProcessMemory(hProcess,(void*)(sunBaseAddress), &tempAddr, sizeof(DWORD), &dwSize);
    ReadProcessMemory(hProcess,(void*)(tempAddr+ offSetFirst), &tempAddr, sizeof(DWORD), &dwSize);
    WriteProcessMemory(hProcess,(void*)(tempAddr+ offSetSecond), &sunValue, sizeof(DWORD), &dwSize);
}

void MainWindow::sunNotDecrease(bool flag)
{
    DWORD baseAddress= 0x00427694;    //减少阳光基址
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
    DWORD baseAddress= 0x0054BA6A;    //减少植物HP基址
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
    DWORD baseAddress= 0x0049E944;    //植物冷却flag基址
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
    DWORD baseAddress= 0x0043CC6E;    //点击掉落物flag基址
    byte bufEnable[] = {0x80,0x7B,0x50,0x01};   //cmp byte ptr [ebx+50],01
    byte bufDisable[] = {0x80,0x7B,0x50,0x00};   //cmp byte ptr [ebx+50],00

    if(flag){
        WriteProcessMemory(hProcess,(void*)(baseAddress),bufEnable, sizeof(bufEnable), 0);
    }
    else{
        WriteProcessMemory(hProcess,(void*)(baseAddress), bufDisable, sizeof(bufDisable), 0);
    }
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
