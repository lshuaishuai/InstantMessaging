#include "registerdialog.h"
#include "ui_registerdialog.h"
#include "global.h"
#include "httpmgr.h"

RegisterDialog::RegisterDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::RegisterDialog)
{
    ui->setupUi(this);
    ui->pass_edit->setEchoMode(QLineEdit::Password);
    ui->confirm_edit->setEchoMode(QLineEdit::Password);
    ui->err_tip->setProperty("state", "normal");
    repolish(ui->err_tip);
    connect(HttpMgr::GetInstance().get(), &HttpMgr::sig_reg_mod_finish, this, &RegisterDialog::slot_reg_mod_finish);
    initHttpHandlers();
}

void RegisterDialog::initHttpHandlers()
{
    // 在获取验证码回包的逻辑
    _handlers.insert(ReqId::ID_GET_VARIFY_CODE, [this](const QJsonObject& jsonObj){
        int error = jsonObj["error"].toInt();
        if(error != ErrorCodes::SUCCESS)
        {
            showTip(tr("The param error."), false);
            return;
        }
        auto email = jsonObj["email"].toString();
        showTip(tr("The verification code has been sent to your email， please check it."), true);
        qDebug() << "email is " << email;
    });
}

RegisterDialog::~RegisterDialog()
{
    delete ui;
}

void RegisterDialog::on_send_code_clicked()
{
    // 19707765070@163.com
    auto email = ui->email_edit->text();
    QRegularExpression regex(R"((\w+)(\.|_)?(\w*)@(\w+)(\.(\w+))+)");
    bool match = regex.match(email).hasMatch();
    if(match)
    {
        // send http varify code
    }
    else
    {
        showTip(tr("The email entered is incorrect!!!"), false);
    }
}

void RegisterDialog::slot_reg_mod_finish(ReqId id, QString res, ErrorCodes err)
{
    if(err != ErrorCodes::SUCCESS)
    {
        showTip(tr("Network request error!"), false);
        return;
    }

    // 解析JSON
    QJsonDocument jsonDoc = QJsonDocument::fromJson(res.toUtf8());
    if(jsonDoc.isNull())
    {
        showTip(tr("JSON parsing failed!"), false);
        return;
    }
    // JSON parsing error
    if(!jsonDoc.isObject())
    {
        showTip(tr("JSON parsing failed!"), false);
        return;
    }
    _handlers[id](jsonDoc.object());
    return;
}

void RegisterDialog::showTip(QString str, bool b_ok)
{
    if(b_ok)
        ui->err_tip->setProperty("state", "normal");
    else
        ui->err_tip->setProperty("state", "err");

    ui->err_tip->setText(str);
    repolish(ui->err_tip);
}
