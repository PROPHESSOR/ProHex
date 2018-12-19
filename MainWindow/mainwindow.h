#pragma once

#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>
#include <QInputDialog>
#include <QDebug>
#include <QTranslator>

#include "../DataStorage/datastorage.h"
#include "constants.h"
#include "../Config/config.h"

// Windows
#include "../QHexView/qhexview.h"
#include "../Converter/converter.h"
#include "../AsciiTable/asciitable.h"
#include "../Preferences/preferences.h"
#include "../Strings/strings.h"
#include "../Finder/finder.h"
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

  public slots:
    void edit_findnext(); // TODO:
    void edit_findprev(); // TODO:

  private slots:
    void file_new();
    void file_open();
    void file_save();
    void file_saveas();
    void file_exit();

    void edit_find(); // TODO:
    void edit_replace(); // TODO:
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
    Ui::MainWindow  *ui               = nullptr;
    Config          *m_config         = nullptr;

    QHexView        *m_hexview        = nullptr;
    Converter       *m_converter      = nullptr;
    AsciiTable      *m_asciitable     = nullptr;
    Preferences     *m_preferences    = nullptr;
    Strings         *m_strings        = nullptr;
    Finder          *m_finder         = nullptr;

    QByteArray      *m_searchArray;
    uint16_t         m_searchIndex    = 0;

    void initToolBar();
    void initHexView();

    void openFile(QString path);
    void saveFile();
    void saveFileAs(QString path);

  protected:
    DataStorage     *m_data = nullptr;
    QTranslator     *translator     = nullptr;
};
