#ifndef HTTPMGR_H
#define HTTPMGR_H

#include "singleton.h"

#include <QString>
#include <QUrl>
#include <QObject>
#include <QNetworkAccessManager>
#include <QJsonObject>
#include <QJsonDocument>

// 学习C++知识点：CRTP
class HttpMgr : public QObject, public Singleton<HttpMgr>, public std::enable_shared_from_this<HttpMgr>
{
    Q_OBJECT
private:
    // Singleton::GetSingleton()的new<HttpMgr>需要调用HttpMgr的构造函数，设置为友元
    friend class Singleton<HttpMgr>;
    HttpMgr();

private:
    QNetworkAccessManager _manager;

private slots:
    void slot_http_finish(ReqId id, QString res, ErrorCodes err, Modules mod);

signals:
    void sig_http_finish(ReqId id, QString res, ErrorCodes err, Modules mod);
    void sig_reg_mod_finish(ReqId id, QString res, ErrorCodes err);

public:
    ~HttpMgr();
    void PostHttpReq(QUrl url, QJsonObject json, ReqId req_id, Modules mod);
};

#endif // HTTPMGR_H
