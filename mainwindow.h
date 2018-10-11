#pragma once

#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>
#include <QInputDialog>
#include <QDebug>
#include "qhexview.h"
#include "constants.h"
#include "converter.h"

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

    void edit_gotooffset();

    void tools_converter();

    void about_about();
    void about_aboutqt();
    void doesntimplemented();

  private:
    Ui::MainWindow *ui;
    QHexView *hexview;
    Converter *converter = 0x0;

    void initToolBar();
    void initHexView();
    void openFile(QString path);
};
