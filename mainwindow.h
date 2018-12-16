#pragma once

#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>
#include <QInputDialog>
#include <QDebug>
#include <QTranslator>

#include "datastorage.h"
#include "constants.h"
#include "config.h"

// Windows
#include "qhexview.h"
#include "converter.h"
#include "asciitable.h"
#include "preferences.h"
#include "strings.h"
//\\ Windows

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
    void file_exit();

    void edit_gotooffset();

    void view_toolbars_toggleAddress();
    void view_toolbars_toggleHex();
    void view_toolbars_toggleAscii();
    void view_preferences();
    void view_keybindings();

    void tools_converter();
    void tools_strings();
    void tools_asciiTable();

    void about_about();
    void about_aboutqt();
    void doesntimplemented();

  private:
    Ui::MainWindow  *ui             = nullptr;
    Config          *config         = nullptr;

    QHexView        *hexview        = nullptr;
    Converter       *converter      = nullptr;
    AsciiTable      *asciitable     = nullptr;
    Preferences     *preferences    = nullptr;
    Strings         *strings        = nullptr;

    void initToolBar();
    void initHexView();

    void openFile(QString path);
    void saveFile();
    void saveFileAs(QString path);

  protected:
    DataStorage *m_data = nullptr;
    QTranslator     *translator     = nullptr;
};
