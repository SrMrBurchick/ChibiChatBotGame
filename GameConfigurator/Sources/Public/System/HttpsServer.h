#pragma once

#include <QObject>
#include <QPointer>

class QTcpServer;
class QHttpServer;

class CBHttpsServer : public QObject {
    Q_OBJECT

public:
    explicit CBHttpsServer(QObject *parent = nullptr);

    void Start();
    void Stop();

signals:
    void codeReceived(const QString& AppCode);

protected:
    void HandleConnection();

private:
    quint16 Port = 3000;
    QPointer<QTcpServer> Server = nullptr;
    QPointer<QHttpServer> HTTPServer = nullptr;
};
