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

    void addSun();
    void sunNotDecrease(bool flag);
    void plantLockHP(bool flag);
    void coolDown(bool flag);
    void autoCollect(bool flag);
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

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
