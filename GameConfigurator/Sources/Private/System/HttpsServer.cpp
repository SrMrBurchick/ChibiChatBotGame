#include "System/HttpsServer.h"
#include "System/Logger.h"

#include <QTcpServer>
#include <QRegularExpression>
#include <QUrl>
#include <QUrlQuery>
#include <QTcpSocket>
#include <QHttpServer>
#include <QRegularExpression>
#include <QNetworkAccessManager>

CBHttpsServer::CBHttpsServer(QObject *Parent)
    :QObject(Parent)
{
    Server = new QTcpServer();
    HTTPServer = new QHttpServer();
    QObject::connect(Server.get(), &QTcpServer::newConnection, this, &CBHttpsServer::HandleConnection);
}

void CBHttpsServer::Start()
{
    if (!Server->listen(QHostAddress::LocalHost, Port)/*  || !HTTPServer->bind(Server.get()) */) {
        LOG_CRITICAL("TOBI PIZDA!");
    }
}

void CBHttpsServer::Stop()
{
    if (!Server.isNull()) {
        Server->close();
        HTTPServer.clear();
    }
}

void CBHttpsServer::HandleConnection()
{
    QTcpSocket* Client = Server->nextPendingConnection();

    QObject::connect(Client, &QTcpSocket::readyRead, this, [this, Client]() {
        QString Request(Client->readAll());
        QStringList Chunks = Request.split("\n");
        QString Patern = "Referer: ";
        QString Code;
        for (const QString& Chunk : Chunks) {
            if (Chunk.contains(Patern)) {
                QUrlQuery Query(QUrl(Chunk.mid(Patern.length()).trimmed()));
                Code = Query.queryItemValue("code");
                if (!Code.isEmpty()) {
                    emit codeReceived(Code);
                }
            }
        }

        Client->write("HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nToken received!");
        Client->disconnectFromHost();
    });

    QObject::connect(Client, &QTcpSocket::disconnected, Client, &QTcpSocket::deleteLater);
}
