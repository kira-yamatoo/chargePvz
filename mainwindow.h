#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#undef   UNICODE
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
    int getProcessHandleByName(char* name);
    int getProcessHandleBywindow(char *name);

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
    void blockIceSurface(bool flag);
    void blockGravestone(bool flag);
    void blockFog(bool flag);
    void backRunner(bool flag);
    void plantDeployLimit(bool flag);
    void purpleCardLimit(bool flag);
    void shroomAwaken(bool flag);

    void injectCode();
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

    void on_blockIceSurfaceCheckBox_stateChanged(int arg1);

    void on_blockGravestoneCheckBox_stateChanged(int arg1);

    void on_blockFogCheckBox_stateChanged(int arg1);

    void on_backRunnerCheckBox_stateChanged(int arg1);

    void on_plantDeployLimitCheckBox_stateChanged(int arg1);

    void on_purpleCardLimitCheckBox_stateChanged(int arg1);

    void on_shroomAwakenCheckBox_stateChanged(int arg1);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
