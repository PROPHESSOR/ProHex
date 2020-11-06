#pragma once

#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>
#include <QInputDialog>
#include <QDebug>
#include <QTranslator>
#include <QUndoStack>
#include <QUndoView>
#include <QStyleFactory>
#include <QMimeData>

#include "constants.h"
#include "../DataStorage/datastorage.h"
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
    void edit_findnext();
    void edit_findprev();

    void do_replace();
    void do_replaceAll();

    void historyindexchanged();

    void gotoOffset(int64_t);

  private slots:
    void file_new();
    void file_open();
    void file_save();
    void file_saveas();
    void file_exit();

    void edit_undo();
    void edit_redo();
    void edit_openhistory();
    void edit_search();
    void edit_replace();
    void edit_gotooffset();
    void edit_xor();

    void view_toolbars_toggleAddress();
    void view_toolbars_toggleHex();
    void view_toolbars_toggleAscii();
    void view_preferences();

    void tools_analyzer();
    void tools_converter();
    void tools_strings();
    void tools_asciiTable();
    void tools_md5();

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

    QByteArray      *m_searchArray    = nullptr;
    uint16_t         m_searchIndex    = 0;
    QByteArray      *m_replaceArray   = nullptr;

    QUndoStack      *m_undostack      = nullptr;
    QUndoView       *m_undoview       = nullptr;

    void initToolBar();
    void initHexView();

    void openFile(QString path);
    void saveFile();
    void saveFileAs(QString path);

    void setWindowTheme(uint8_t theme);

  protected:
    DataStorage     *m_data         = nullptr;
    QTranslator     *translator     = nullptr;

    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void dropEvent(QDropEvent *event) override;
};
