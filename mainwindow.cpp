#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
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
    file->addAction("Open fileripper", this, SLOT(doesntimplemented()));
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
    edit->addAction("Goto offset", this, SLOT(doesntimplemented()));

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

    // About
    QMenu *about = ui->MenuBar->addMenu("&About");
    about->addSection("Help");
    about->addAction("User manual (offline)", this, SLOT(doesntimplemented()));
    about->addAction("Online forum", this, SLOT(doesntimplemented()));
    about->addSection("About");
    about->addAction("About the program", this, SLOT(doesntimplemented()));
    about->addAction("About Qt", this, SLOT(doesntimplemented()));
    about->addAction("License", this, SLOT(doesntimplemented()));
}

void MainWindow::initHexView() {
    hexview = new QHexView;
    setCentralWidget(hexview);
}

void MainWindow::file_new() {
    qDebug() << "File->New";
    doesntimplemented();
}

void MainWindow::file_open() {
    qDebug() << "File->Open";
    doesntimplemented();
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
    doesntimplemented();
}

void MainWindow::file_exit() {
    qDebug() << "File->Exit";
    doesntimplemented();
}

void MainWindow::doesntimplemented() {
    QMessageBox::information(this, "Not implemented", "Hey! This feature doesn't implemented yet!");
}
