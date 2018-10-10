#pragma once

#include <QMainWindow>
#include <QMessageBox>
#include <QDebug>
#include "qhexview.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

  public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

  private slots:
    void file_new();
    void file_open();
    void file_save();
    void file_saveas();
    void file_close();
    void file_exit();
    void doesntimplemented();

  private:
    Ui::MainWindow *ui;
    QHexView *hexview;
    void initToolBar();
    void initHexView();
};
