#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    _login_dlg = new LoginDialog();
    setCentralWidget(_login_dlg);
    _login_dlg->show();

    // create and register connect
    connect(_login_dlg, &LoginDialog::switchRegister, this, &MainWindow::SlotSwitch);
    _register_dlg = new RegisterDialog();

}

MainWindow::~MainWindow()
{
    delete ui;
    if(_login_dlg)
    {
        delete _login_dlg;
        _login_dlg = nullptr;
    }
    if(_register_dlg)
    {
        delete _register_dlg;
        _register_dlg = nullptr;
    }
}

void MainWindow::SlotSwitch()
{
    setCentralWidget(_register_dlg);
    _login_dlg->hide();
    _register_dlg->show();
}
