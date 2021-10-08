#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <Windows.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    bool readProcess();
    void init();
    byte* replaceWithNop(int len);
    void hookFunction(bool flag, DWORD baseAddress, byte bufEnable[],byte bufDisable[],int len);

    void addSun();
    void sunNotDecrease(bool flag);
    void plantLockHP(bool flag);
    void coolDown(bool flag);
    void autoCollect(bool flag);
    void doomShroomDeploy(bool flag);
    void cobCannoDeploy(bool flag);
    void potatoMineDeploy(bool flag);
    void chomperDeploy(bool flag);
    void magnetShroomDeploy(bool flag);
    void plantAtkBullet(bool flag);

    void test();


public:
    HANDLE hProcess=NULL;
    QString windowName="Plants vs. Zombies";

private slots:
    void on_addSunButton_clicked();

    void on_sunNotDecreaseCheckBox_stateChanged(int arg1);

    void on_plantLockHPCheckBox_stateChanged(int arg1);

    void on_coolDownCheckBox_stateChanged(int arg1);

    void on_autoCollectCheckBox_stateChanged(int arg1);

    void on_actionInit_triggered();

    void on_doomShroomCheckBox_stateChanged(int arg1);

    void on_cobCannoDeployCheckBox_stateChanged(int arg1);

    void on_potatoMineDeployCheckBox_stateChanged(int arg1);

    void on_chomperDeployCheckBox_stateChanged(int arg1);

    void on_magnetShroomDeployCheckBox_stateChanged(int arg1);

    void on_plantAtkBulletCheckBox_stateChanged(int arg1);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
