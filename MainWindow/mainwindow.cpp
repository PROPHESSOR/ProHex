#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow) {
    m_undostack = new QUndoStack(this);
    connect(m_undostack, SIGNAL(indexChanged(int)), this, SLOT(historyindexchanged()));
    m_config = new Config();

    qDebug() << "System language:" << QLocale::system().name();

    m_config->load();

    translator = new QTranslator();
    translator->load(":locale/locale_" + m_config->getLanguage());

    qApp->installTranslator(translator);

    ui->setupUi(this);

    initToolBar();
    initHexView();

    /*
    uint16_t charWidth     = uint16_t(fontMetrics().width(QLatin1Char('9')));
    uint16_t charHeight    = uint16_t(fontMetrics().height());

    setSizeIncrement(charWidth * 5, charHeight * 1.15);
    */
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::initToolBar() {
    // File
    QMenu *file = ui->MenuBar->addMenu(tr("&File"));
    file->addSection(tr("File"));
    file->addAction(tr("New"), this, SLOT(file_new()), QKeySequence::New);
    file->addAction(tr("Open"), this, SLOT(file_open()), QKeySequence::Open);
    file->addAction(tr("Save"), this, SLOT(file_save()), QKeySequence::Save);
    file->addAction(tr("Save as..."), this, SLOT(file_saveas()), QKeySequence::SaveAs);
    file->addSeparator();
    file->addAction(tr("Exit"), this, SLOT(file_exit()), QKeySequence::Quit);

    // Edit
    QMenu *edit = ui->MenuBar->addMenu(tr("&Edit"));
    edit->addSection(tr("History"));
    edit->addAction(tr("Undo"), this, SLOT(edit_undo()), QKeySequence::Undo);
    edit->addAction(tr("Redo"), this, SLOT(edit_redo()), QKeySequence::Redo);
    edit->addAction(tr("Open history"), this, SLOT(edit_openhistory()), QKeySequence("Ctrl+Shift+H"));
    edit->addSection(tr("Search"));
    edit->addAction(tr("Find"), this, SLOT(edit_search()), QKeySequence::Find);
    edit->addAction(tr("Replace"), this, SLOT(edit_replace()), QKeySequence::Replace);
    edit->addAction(tr("Find next"), this, SLOT(edit_findnext()), QKeySequence("Ctrl+Shift+N"));
    edit->addAction(tr("Find previous"), this, SLOT(edit_findprev()), QKeySequence("Ctrl+Shift+P"));
    edit->addSection(tr("Navigation"));
    edit->addAction(tr("Goto offset"), this, SLOT(edit_gotooffset()), QKeySequence("Ctrl+G"));

    // View
    QMenu *view = ui->MenuBar->addMenu(tr("&View"));
    QMenu *toolbars = view->addMenu(tr("&Toolbars"));
    toolbars->addAction(tr("Address section"), this, SLOT(view_toolbars_toggleAddress()));
    toolbars->addAction(tr("Hexdecimal section"), this, SLOT(view_toolbars_toggleHex()));
    toolbars->addAction(tr("Ascii section"), this, SLOT(view_toolbars_toggleAscii()));
    view->addSection(tr("Settings"));
    view->addAction(tr("Preferences"), this, SLOT(view_preferences()), QKeySequence("Ctrl+P"));
    view->addAction(tr("Key bindings"), this, SLOT(view_keybindings()));

    // Tools
    QMenu *tools = ui->MenuBar->addMenu(tr("&Tools"));
    tools->addSection(tr("About the file"));
    tools->addAction(tr("File analyzer"), this, SLOT(doesntimplemented()), QKeySequence("Ctrl+Shift+I"));
    tools->addSection(tr("Useful utilities"));
    tools->addAction(tr("Converter"), this, SLOT(tools_converter()), QKeySequence("Ctrl+Shift+C"));
    tools->addAction(tr("Strings"), this, SLOT(tools_strings()), QKeySequence("Ctrl+Shift+G"));
    tools->addAction(tr("ASCII Table"), this, SLOT(tools_asciiTable()), QKeySequence("Ctrl+Shift+A"));
    tools->addAction(tr("Assembler commands"), this, SLOT(doesntimplemented()), QKeySequence("Ctrl+Shift+B"));

    // About
    QMenu *about = ui->MenuBar->addMenu(tr("&About"));
    about->addSection(tr("Help"));
    about->addAction(tr("User manual (offline)"), this, SLOT(doesntimplemented()), QKeySequence::HelpContents);
    about->addAction(tr("Online forum"), this, SLOT(doesntimplemented()));
    about->addSection(tr("About"));
    about->addAction(tr("About the program"), this, SLOT(about_about()));
    about->addAction(tr("About Qt"), this, SLOT(about_aboutqt()));
    about->addAction(tr("License"), this, SLOT(doesntimplemented()));
}

void MainWindow::initHexView() {
    m_hexview = new QHexView(this, m_data, m_config, statusBar(), m_undostack);
    setCentralWidget(m_hexview);
}

void MainWindow::openFile(QString path) {
    qDebug() << "MainWindow::openFile(" << path << ")";
    QFile file(path);

    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, tr("Can't open the file"), tr("Can't open the file \"") + path + tr("\" for reading!"));
        return;
    }

    m_hexview->clear();

    QByteArray array = file.readAll();
    m_data = new DataStorage(array, path);
    m_searchIndex = 0;
    m_hexview->setData(m_data);
    m_hexview->update();
}

void MainWindow::saveFile() {
    qDebug() << "MainWindow::saveFile()";
    if(!m_data->isFileAssociated()) return file_saveas();

    saveFileAs(m_data->getAssociatedFile());
}

void MainWindow::saveFileAs(QString path) {
    qDebug() << "MainWindow::saveFileAs(" << path << ")";

    QFile file(path);

    if(!file.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(this, tr("Can't open the file"), tr("Can't open the file \"") + path + tr("\" for writing!"));
        return;
    }

    file.write(m_data->getAllData());
}

void MainWindow::file_new() {
    qDebug() << "File->New";
    m_hexview->clear();
    setWindowTitle(PROHEX);

    QByteArray array(1, 0x0);
    m_data = new DataStorage(array, "");
    m_hexview->setData(m_data);
    m_hexview->update();
}

void MainWindow::file_open() {
    qDebug() << "File->Open";
    QString dir = QDir::currentPath();

    QString filename = QFileDialog::getOpenFileName(this, tr("Select file"), dir);
    if(!filename.isEmpty()) {
        openFile(filename);

        QFileInfo info(filename);
        setWindowTitle(info.fileName() + " - " + PROHEX);
    }
}

void MainWindow::file_save() {
    qDebug() << "File->Save";
    if(m_data->isFileAssociated()) saveFile();
    else file_saveas();
}

void MainWindow::file_saveas() {
    qDebug() << "File->Save as...";
    QString dir = QDir::currentPath();

    QString filename = QFileDialog::getSaveFileName(this, tr("Select file to save"), dir);
    if(!filename.isEmpty()) {
        saveFileAs(filename);
        m_data->setAssociatedFile(filename);

        QFileInfo info(filename);
        setWindowTitle(info.fileName() + " - " + PROHEX);
    }
}

void MainWindow::file_exit() {
    qDebug() << "File->Exit";
    QMessageBox::StandardButton confirm = QMessageBox::question(this, tr("Save"), tr("Save changes?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

    switch(confirm) {
        case QMessageBox::Yes:
            file_save();
            break;
        default:
            break;
    }

    m_config->save();
    exit(0);
}

void MainWindow::edit_undo() {
    qDebug() << "Edit->Undo";
    m_undostack->undo();
}

void MainWindow::edit_redo() {
    qDebug() << "Edit->Redo";
    m_undostack->redo();
}

void MainWindow::edit_openhistory() {
    if(m_undoview == nullptr) {
        m_undoview = new QUndoView(m_undostack);
        m_undoview->setWindowTitle(tr("History"));
    }

    m_undoview->show();
}

void MainWindow::edit_search() {
    if(m_finder == nullptr) {
        m_searchArray = new QByteArray;
        m_replaceArray = new QByteArray;
        m_finder = new Finder(nullptr, m_searchArray, m_replaceArray);
        connect(m_finder->ui->findNext, SIGNAL(clicked()), this, SLOT(edit_findnext()));
        connect(m_finder->ui->findPrev, SIGNAL(clicked()), this, SLOT(edit_findprev()));
        connect(m_finder->ui->replace, SIGNAL(clicked()), this, SLOT(do_replace()));
        connect(m_finder->ui->replaceAll, SIGNAL(clicked()), this, SLOT(do_replaceAll()));
    }

    m_finder->show();
    m_finder->setMode(Finder::MODE_SEARCH);
}

void MainWindow::edit_replace() {
    edit_search();
    m_finder->setMode(Finder::MODE_REPLACE);
}

void MainWindow::edit_findnext() {
    if(m_data == nullptr || m_searchArray == nullptr) return;
    qDebug() << "MainWindow::edit_findnext()";
    int64_t index = m_data->find(m_searchArray, m_searchIndex, true);

    if(index == -1) {
        if(m_searchIndex) {
            statusBarMessage(tr("Out of file! Search position reset to begin"));
            m_searchIndex = 0;
            edit_findnext();
        } else {
            statusBarMessage(tr("Not found"));
        }
        return;
    }

    m_hexview->showFromOffset(index * 2);
    m_hexview->select(index * 2, (index + m_searchArray->size()) * 2);
    m_hexview->update();
    m_searchIndex = index + 1;
}

void MainWindow::edit_findprev() {
    if(m_data == nullptr || m_searchArray == nullptr) return;
    qDebug() << "MainWindow::edit_findprev()";
    int64_t index = m_data->find(m_searchArray, m_searchIndex, false);

    if(index == -1) {
        if(m_searchIndex) {
            statusBarMessage(tr("Out of file! Search position reset to end"));
            m_searchIndex = m_data->size() - 1;
            edit_findprev();
        } else {
            statusBarMessage(tr("Not found"));
        }
        return;
    }

    m_hexview->showFromOffset(index * 2);
    m_hexview->select(index * 2, (index + m_searchArray->size()) * 2);
    m_hexview->update();
    m_searchIndex = index - 1;
}

void MainWindow::do_replace() {
    qDebug() << "MainWindow::do_replace";
    bool result = m_data->findReplace(m_searchArray, m_replaceArray, m_searchIndex - 1);
    if(result) {
        m_hexview->update();
        statusBarMessage(tr("Replaced!"));
    } else {
        statusBarMessage(tr("Can't replace at this search position! Try to Find Next or Find Prev before replace"));
    }
}

void MainWindow::do_replaceAll() {
    qDebug() << "MainWindow::do_replaceAll";

    bool result = m_data->findReplace(m_searchArray, m_replaceArray);
    if(result) {
        m_hexview->update();
        statusBarMessage(tr("Replaced!"));
    } else {
        statusBarMessage(tr("Can't replace!"));
    }
}

void MainWindow::edit_gotooffset() {
    qDebug() << "Edit->Goto offset";
    if(m_data == nullptr) return;
    bool confirmed = false;
    int64_t offset = int64_t(QInputDialog::getInt(this, tr("Offset"), tr("Enter the offset"), 0, 0, 2147483647, 1, &confirmed));

    if(confirmed) m_hexview->showFromOffset(offset);
}

void MainWindow::view_toolbars_toggleAddress() {
    m_config->setViewShowAddress(!m_config->getViewShowAddress());
    m_hexview->recalcView();
    m_hexview->repaint();
}

void MainWindow::view_toolbars_toggleHex() {
    m_config->setViewShowHex(!m_config->getViewShowHex());
    m_hexview->recalcView();
    m_hexview->repaint();
}

void MainWindow::view_toolbars_toggleAscii() {
    m_config->setViewShowAscii(!m_config->getViewShowAscii());
    m_hexview->recalcView();
    m_hexview->repaint();
}

void MainWindow::view_preferences() {
    qDebug() << "View->Preferences";

    if(m_preferences == nullptr) {
        m_preferences = new Preferences(nullptr, m_config);
    }

    m_preferences->show();
}

void MainWindow::view_keybindings() {
    qDebug() << "View->Key bindings";

    if(m_preferences == nullptr) {
        m_preferences = new Preferences(nullptr, m_config);
    }

    m_preferences->showTab(1);
}

void MainWindow::tools_converter() {
    qDebug() << "Tools->Converter";

    if(m_converter == nullptr) {
        m_converter = new Converter();
    }

    m_converter->show();
}

void MainWindow::tools_strings() {
    qDebug() << "Tools->Strings";

    if(m_strings == nullptr) {
        m_strings = new Strings();
    }

    this->setCursor(Qt::WaitCursor);
    statusBarMessage(tr("Searching for readable strings..."));
    m_strings->generateList(m_data);
    m_strings->show();
    statusBarMessage(tr("OK"));
    this->setCursor(Qt::ArrowCursor);
}

void MainWindow::tools_asciiTable() {
    qDebug() << "Tools->ASCII Table";
    if(m_asciitable == nullptr) {
        m_asciitable = new AsciiTable();
    }
    m_asciitable->show();
}

void MainWindow::about_about() {
    qDebug() << "About->About the program";
    QMessageBox::about(this, tr("About the program"), PROHEX + " v." + PROHEX_VERSION + " <br/>Copyright (c) PROPHESSOR 2018");
}

void MainWindow::about_aboutqt() {
    qDebug() << "About->About Qt";
    QMessageBox::aboutQt(this, tr("About Qt"));
}

void MainWindow::doesntimplemented() {
    QMessageBox::information(this, tr("Not implemented"), tr("Hey! This feature doesn't implemented yet!"));
}

void MainWindow::historyindexchanged() {
    qDebug() << "MainWindow::historyindexchanged";
    m_hexview->update();
}

void MainWindow::statusBarMessage(const QString &message) { // TODO: Remove
    statusBar()->showMessage(message);
}

void MainWindow::update(int8_t mode) { // TODO:
    switch(mode) {
        case 1: // QHexView updated
            qDebug() << "MainWindow::update called by QHexView";
            break;
        default:
            qDebug() << "Undefined mode " << mode << "in MainWindow::update";
            break;
    }
}
