#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow) {
    m_undostack = new QUndoStack(this);
    connect(m_undostack, SIGNAL(indexChanged(int)), this, SLOT(historyindexchanged()));
    m_config = new Config();

    qDebug() << "System language:" << QLocale::system().name();

    m_config->load();
    setWindowTheme(m_config->getWindowTheme());
    setAcceptDrops(true);

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
    edit->addSection(tr("Misc"));
    edit->addAction(tr("XOR encryption"), this, SLOT(edit_xor()), QKeySequence("Ctrl+Shift+X"));

    // View
    QMenu *view = ui->MenuBar->addMenu(tr("&View"));
    QMenu *toolbars = view->addMenu(tr("&Toolbars"));
    toolbars->addAction(tr("Address section"), this, SLOT(view_toolbars_toggleAddress()));
    toolbars->addAction(tr("Hexdecimal section"), this, SLOT(view_toolbars_toggleHex()));
    toolbars->addAction(tr("Ascii section"), this, SLOT(view_toolbars_toggleAscii()));
    view->addSection(tr("Settings"));
    view->addAction(tr("Preferences"), this, SLOT(view_preferences()), QKeySequence("Ctrl+P"));

    // Tools
    QMenu *tools = ui->MenuBar->addMenu(tr("&Tools"));
    tools->addSection(tr("About the file"));
    tools->addAction(tr("File analyzer"), this, SLOT(tools_analyzer()), QKeySequence("Ctrl+Shift+I"));
    tools->addSection(tr("Useful utilities"));
    tools->addAction(tr("Converter"), this, SLOT(tools_converter()), QKeySequence("Ctrl+Shift+C"));
    tools->addAction(tr("Strings"), this, SLOT(tools_strings()), QKeySequence("Ctrl+Shift+G"));
    tools->addAction(tr("ASCII Table"), this, SLOT(tools_asciiTable()), QKeySequence("Ctrl+Shift+A"));
    tools->addAction(tr("MD5 Hash"), this, SLOT(tools_md5()), QKeySequence("Ctrl+Shift+M"));

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

void MainWindow::setWindowTheme(uint8_t theme) {
    if(theme == 1) { // Dark
        qApp->setStyle(QStyleFactory::create("Fusion"));

        QPalette darkPalette;
        darkPalette.setColor(QPalette::Window, QColor(53,53,53));
        darkPalette.setColor(QPalette::WindowText, Qt::white);
        darkPalette.setColor(QPalette::Base, QColor(25,25,25));
        darkPalette.setColor(QPalette::AlternateBase, QColor(53,53,53));
        darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
        darkPalette.setColor(QPalette::ToolTipText, Qt::white);
        darkPalette.setColor(QPalette::Text, Qt::white);
        darkPalette.setColor(QPalette::Button, QColor(53,53,53));
        darkPalette.setColor(QPalette::ButtonText, Qt::white);
        darkPalette.setColor(QPalette::BrightText, Qt::red);
        darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));

        darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
        darkPalette.setColor(QPalette::HighlightedText, Qt::black);

        qApp->setPalette(darkPalette);

        qApp->setStyleSheet("QToolTip { color: #ffffff; background-color: #2a82da; border: 1px solid white; }");
    }
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event) {
    qDebug() << "MainWindow::dragEnterEvent" << event;

    if (event->mimeData()->hasText()) return event->acceptProposedAction();

    event->dropAction();
}

void MainWindow::dragMoveEvent(QDragMoveEvent *event) {
    event->acceptProposedAction();
}

void MainWindow::dragLeaveEvent(QDragLeaveEvent *event) {
    event->accept();
}

void MainWindow::dropEvent(QDropEvent *event) {
    qDebug() << "MainWindow::dropEvent" << event;

    const QMimeData *mimeData = event->mimeData();
    qDebug() << mimeData;

    qDebug() << mimeData->hasColor() << mimeData->hasText() << mimeData->hasHtml() << mimeData->hasImage() << mimeData->hasUrls();

    if (mimeData->hasUrls()) {
        if (mimeData->urls().length() > 1) {
            QMessageBox::warning(this, tr("Too many files"), tr("Only one file is supported!"));
            return;
        }

        QUrl url = mimeData->urls()[0];

        if (!url.isLocalFile()) {
            QMessageBox::warning(this, tr("Not a local file"), tr("Only local files are supported!"));
            return;
        }

        openFile(url.path());
    } else if (mimeData->hasText()) {
        file_new();

        m_data = new DataStorage(mimeData->data("text/plain"), "");

        m_hexview->setData(m_data);
        m_hexview->update();
    }
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

void MainWindow::edit_xor() {
    qDebug() << "Edit->XOR";
    if(m_data == nullptr) return;

    QString key = QInputDialog::getText(this, tr("XOR"), tr("Enter the XOR key"));
    qDebug() << key;

    if(!key.isEmpty()) m_undostack->push(new UndoXOR(m_data, key));
}

void MainWindow::view_toolbars_toggleAddress() {
    m_config->setViewShowAddress(!m_config->getViewShowAddress());
    m_hexview->recalcView();
    m_hexview->update();
}

void MainWindow::view_toolbars_toggleHex() {
    m_config->setViewShowHex(!m_config->getViewShowHex());
    m_hexview->recalcView();
    m_hexview->update();
}

void MainWindow::view_toolbars_toggleAscii() {
    m_config->setViewShowAscii(!m_config->getViewShowAscii());
    m_hexview->recalcView();
    m_hexview->update();
}

void MainWindow::view_preferences() {
    qDebug() << "View->Preferences";

    if(m_preferences == nullptr) {
        m_preferences = new Preferences(nullptr, m_config);
    }

    m_preferences->show();
    m_preferences->load();
}

void MainWindow::tools_analyzer() {
    qDebug() << "Tools->File analyzer";
    if(m_data == nullptr || m_data->size() < 2) return;

    QString output = "";

    output += tr("File name: ") + m_data->getAssociatedFile() + "\n";
    output += tr("File size: ") + QString::number(m_data->size()) + tr(" bytes\n");
    output += tr("File type: ") + (m_data->at(0) > 33 && m_data->at(m_data->size() - 2) > 33 ? tr("Text") : tr("Binary"));

    QMessageBox::information(this, tr("File analyzer"), output);
}

void MainWindow::tools_converter() {
    qDebug() << "Tools->Converter";

    if(m_converter == nullptr) {
        m_converter = new Converter();
        connect(m_hexview, SIGNAL(valueChanged(uint8_t)), m_converter, SLOT(on_innerValue(uint8_t)));
    }

    m_converter->show();
    m_converter->calculate();
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

void MainWindow::tools_md5() {
    qDebug() << "Tools->MD5 Hash";
    if(m_data == nullptr || !m_data->size()) return;
    QMessageBox::information(this, tr("MD5 Hash"), m_data->getMD5Hash());
}

void MainWindow::about_about() {
    qDebug() << "About->About the program";
    QMessageBox::about(this, tr("About the program"), PROHEX + " v." + PROHEX_VERSION + " <br/>Copyright (c) PROPHESSOR 2018-2020");
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
