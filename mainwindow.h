#pragma once

#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>
#include <QInputDialog>
#include <QDebug>
#include "qhexview.h"
#include "datastorage.h"
#include "constants.h"
#include "converter.h"
#include "config.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

  public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void statusBarMessage(const QString &);
    void update(int8_t mode);

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
    void view_toolbars_toggleAddress();
    void view_toolbars_toggleHex();
    void view_toolbars_toggleAscii();

  private:
    Ui::MainWindow  *ui         = nullptr;
    QHexView        *hexview    = nullptr;
    Converter       *converter  = nullptr;
    Config          *config     = nullptr;


    void initToolBar();
    void initHexView();
    void openFile(QString path);

  protected:
    DataStorage *m_data = nullptr;
};
