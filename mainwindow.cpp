#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    initToolBar();
    initHexView();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::initToolBar() {
    // File
    QMenu *file = ui->MenuBar->addMenu("&File");
    file->addSection("File");
    file->addAction("New", this, SLOT(file_new()));
    file->addAction("Open", this, SLOT(file_open()));
    file->addAction("Save", this, SLOT(file_save()));
    file->addAction("Save as...", this, SLOT(file_saveas()));
    file->addAction("Close", this, SLOT(file_close()));
    file->addSection("Analyze");
    file->addAction("About the file", this, SLOT(doesntimplemented()));
    file->addSeparator();
    file->addAction("Exit", this, SLOT(file_exit()));

    // Edit
    QMenu *edit = ui->MenuBar->addMenu("&Edit");
    edit->addSection("History");
    edit->addAction("Undo", this, SLOT(doesntimplemented()));
    edit->addAction("Redo", this, SLOT(doesntimplemented()));
    edit->addAction("Open history", this, SLOT(doesntimplemented()));
    edit->addSection("Search");
    edit->addAction("Find", this, SLOT(doesntimplemented()));
    edit->addAction("Replace", this, SLOT(doesntimplemented()));
    edit->addSection("Navigation");
    edit->addAction("Goto offset", this, SLOT(edit_gotooffset()));

    // View
    QMenu *view = ui->MenuBar->addMenu("&View");
    view->addSection("File view");
    view->addAction("Hex", this, SLOT(doesntimplemented()));
    view->addAction("Assembler", this, SLOT(doesntimplemented()));
    view->addAction("Strings", this, SLOT(doesntimplemented()));
    view->addAction("Raw", this, SLOT(doesntimplemented()));
    view->addSeparator();
    view->addAction("Save selection preset", this, SLOT(doesntimplemented()));
    view->addAction("Load selection preset", this, SLOT(doesntimplemented()));
    view->addSection("Settings");
    view->addAction("Preferences", this, SLOT(doesntimplemented()));
    view->addAction("Key bindings", this, SLOT(doesntimplemented()));

    // Tools
    QMenu *tools = ui->MenuBar->addMenu("&Tools");
    tools->addSection("About the file");
    tools->addAction("File ripper", this, SLOT(doesntimplemented()));
    tools->addSection("Useful utilities");
    tools->addAction("Converter", this, SLOT(tools_converter()));
    tools->addAction("ASCII Table", this, SLOT(doesntimplemented()));
    tools->addAction("Assembler commands", this, SLOT(doesntimplemented()));

    // About
    QMenu *about = ui->MenuBar->addMenu("&About");
    about->addSection("Help");
    about->addAction("User manual (offline)", this, SLOT(doesntimplemented()));
    about->addAction("Online forum", this, SLOT(doesntimplemented()));
    about->addSection("About");
    about->addAction("About the program", this, SLOT(about_about()));
    about->addAction("About Qt", this, SLOT(about_aboutqt()));
    about->addAction("License", this, SLOT(doesntimplemented()));
}

void MainWindow::initHexView() {
    hexview = new QHexView(this, m_data);
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
    hexview->setData(new DataStorageArray(array));
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
    exit(0);
    doesntimplemented();
}

void MainWindow::edit_gotooffset() {
    qDebug() << "Edit->Goto offset";
    bool confirmed = false;
    std::size_t offset = QInputDialog::getInt(this, "Offset", "Enter the offset", 0, 0, 2147483647, 1, &confirmed);

    if(confirmed) {
        hexview->showFromOffset(offset);
    }
}

void MainWindow::tools_converter() {
    qDebug() << "Tools->Converter";
    if(converter == 0x0) {
        converter = new Converter();
    }
    converter->show();
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
