#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    config = new Config();

    initToolBar();
    initHexView();

    config->load();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::initToolBar() {
    // File
    QMenu *file = ui->MenuBar->addMenu("&File");
    file->addSection("File");
    file->addAction("New", this, SLOT(file_new()), QKeySequence::New);
    file->addAction("Open", this, SLOT(file_open()), QKeySequence::Open);
    file->addAction("Save", this, SLOT(file_save()), QKeySequence::Save);
    file->addAction("Save as...", this, SLOT(file_saveas()), QKeySequence::SaveAs);
    file->addAction("Close", this, SLOT(file_close()), QKeySequence::Close);
    file->addSection("Analyze");
    file->addAction("About the file", this, SLOT(doesntimplemented()), QKeySequence("Ctrl+Shift+I"));
    file->addSeparator();
    file->addAction("Exit", this, SLOT(file_exit()), QKeySequence::Quit);

    // Edit
    QMenu *edit = ui->MenuBar->addMenu("&Edit");
    edit->addSection("History");
    edit->addAction("Undo", this, SLOT(doesntimplemented()), QKeySequence::Undo);
    edit->addAction("Redo", this, SLOT(doesntimplemented()), QKeySequence::Redo);
    edit->addAction("Open history", this, SLOT(doesntimplemented()));
    edit->addSection("Search");
    edit->addAction("Find", this, SLOT(doesntimplemented()), QKeySequence::Find);
    edit->addAction("Replace", this, SLOT(doesntimplemented()), QKeySequence::Replace);
    edit->addSection("Navigation");
    edit->addAction("Goto offset", this, SLOT(edit_gotooffset()), QKeySequence("Ctrl+G"));

    // View
    QMenu *view = ui->MenuBar->addMenu("&View");
    view->addSection("File view");
    view->addAction("Hex", this, SLOT(doesntimplemented()), QKeySequence("Ctrl+Shift+1"));
    view->addAction("Assembler", this, SLOT(doesntimplemented()), QKeySequence("Ctrl+Shift+2"));
    view->addAction("Strings", this, SLOT(doesntimplemented()), QKeySequence("Ctrl+Shift+3"));
    view->addAction("Raw", this, SLOT(doesntimplemented()), QKeySequence("Ctrl+Shift+4"));
    view->addSeparator();
    QMenu *toolbars = view->addMenu("&Toolbars");
    toolbars->addAction("Address section", this, SLOT(view_toolbars_toggleAddress()));
    toolbars->addAction("Hexdecimal section", this, SLOT(view_toolbars_toggleHex()));
    toolbars->addAction("Ascii section", this, SLOT(view_toolbars_toggleAscii()));
    view->addSeparator();
    view->addAction("Save selection preset", this, SLOT(doesntimplemented()));
    view->addAction("Load selection preset", this, SLOT(doesntimplemented()));
    view->addSection("Settings");
    view->addAction("Preferences", this, SLOT(view_preferences()), QKeySequence::Preferences);
    view->addAction("Key bindings", this, SLOT(view_keybindings()));

    // Tools
    QMenu *tools = ui->MenuBar->addMenu("&Tools");
    tools->addSection("About the file");
    tools->addAction("File ripper", this, SLOT(doesntimplemented()), QKeySequence("Ctrl+Shift+R"));
    tools->addSection("Useful utilities");
    tools->addAction("Converter", this, SLOT(tools_converter()), QKeySequence("Ctrl+Shift+C"));
    tools->addAction("ASCII Table", this, SLOT(tools_asciiTable()), QKeySequence("Ctrl+Shift+A"));
    tools->addAction("Assembler commands", this, SLOT(doesntimplemented()), QKeySequence("Ctrl+Shift+B"));

    // About
    QMenu *about = ui->MenuBar->addMenu("&About");
    about->addSection("Help");
    about->addAction("User manual (offline)", this, SLOT(doesntimplemented()), QKeySequence::HelpContents);
    about->addAction("Online forum", this, SLOT(doesntimplemented()));
    about->addSection("About");
    about->addAction("About the program", this, SLOT(about_about()));
    about->addAction("About Qt", this, SLOT(about_aboutqt()));
    about->addAction("License", this, SLOT(doesntimplemented()));
}

void MainWindow::initHexView() {
    hexview = new QHexView(this, m_data, config, statusBar());
    setCentralWidget(hexview);
}

void MainWindow::openFile(QString path) {
    qDebug() << "MainWindow::openFile(" << path << ")";
    QFile file(path);

    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, "Can't open the file", "Can't open the file \"" + path + "\" for reading!");
        return;
    }

    hexview->clear();

    QByteArray array = file.readAll();
    hexview->setData(new DataStorage(array));
    hexview->update();
}

void MainWindow::file_new() {
    qDebug() << "File->New";
    doesntimplemented();
}

void MainWindow::file_open() {
    qDebug() << "File->Open";
    QString dir = QDir::currentPath();

    QString filename = QFileDialog::getOpenFileName(this, "Select file", dir);
    if(!filename.isEmpty()) {
        openFile(filename);

        QFileInfo info(filename);
        setWindowTitle(info.fileName() + " - " + PROHEX);
    }
}

void MainWindow::file_save() {
    qDebug() << "File->Save";
    doesntimplemented();
}

void MainWindow::file_saveas() {
    qDebug() << "File->Save as...";
    doesntimplemented();
}

void MainWindow::file_close() {
    qDebug() << "File->Close";
    hexview->clear();
    hexview->close();
    setWindowTitle(PROHEX);
}

void MainWindow::file_exit() {
    qDebug() << "File->Exit";
    // TODO: Save confirm
    config->save();
    exit(0);
}

void MainWindow::edit_gotooffset() {
    qDebug() << "Edit->Goto offset";
    bool confirmed = false;
    uint64_t offset = uint64_t(QInputDialog::getInt(this, "Offset", "Enter the offset", 0, 0, 2147483647, 1, &confirmed));

    if(confirmed) hexview->showFromOffset(offset);
}

void MainWindow::view_toolbars_toggleAddress() {
    config->setViewShowAddress(!config->getViewShowAddress());
    hexview->recalcView();
    hexview->repaint();
};

void MainWindow::view_toolbars_toggleHex() {
    config->setViewShowHex(!config->getViewShowHex());
    hexview->recalcView();
    hexview->repaint();
};

void MainWindow::view_toolbars_toggleAscii() {
    config->setViewShowAscii(!config->getViewShowAscii());
    hexview->recalcView();
    hexview->repaint();
};

void MainWindow::view_preferences() {
    qDebug() << "View->Preferences";

    if(preferences == nullptr) {
        preferences = new Preferences(nullptr, config);
    }

    preferences->show();
}

void MainWindow::view_keybindings() {
    qDebug() << "View->Key bindings";

    if(preferences == nullptr) {
        preferences = new Preferences(nullptr, config);
    }

    preferences->showTab(1);
}

void MainWindow::tools_converter() {
    qDebug() << "Tools->Converter";

    if(converter == nullptr) {
        converter = new Converter();
    }

    converter->show();
}

void MainWindow::tools_asciiTable() {
    qDebug() << "Tools->ASCII Table";
    if(asciitable == nullptr) {
        asciitable = new AsciiTable();
    }
    asciitable->show();
}

void MainWindow::about_about() {
    qDebug() << "About->About the program";
    QMessageBox::about(this, "About the program", PROHEX + " v." + PROHEX_VERSION + " <br/>Copyright (c) PROPHESSOR 2018");
}

void MainWindow::about_aboutqt() {
    qDebug() << "About->About Qt";
    QMessageBox::aboutQt(this, "About Qt");
}

void MainWindow::doesntimplemented() {
    QMessageBox::information(this, "Not implemented", "Hey! This feature doesn't implemented yet!");
}

void MainWindow::statusBarMessage(const QString &message) { // TODO: Remove
    statusBar()->showMessage(message);
}

void MainWindow::update(int8_t mode) {
    switch(mode) {
        case 1: // QHexView updated
            qDebug() << "MainWindow::update called by QHexView";
            break;
        default:
            qDebug() << "Undefined mode " << mode << "in MainWindow::update";
            break;
    }
}
