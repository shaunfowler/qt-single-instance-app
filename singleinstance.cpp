#include "singleinstance.h"

SingleInstance::SingleInstance(QObject *parent) : QObject(parent)
{
    connect(&mServer, SIGNAL(newConnection()), this, SLOT(newConnection()));
}

SingleInstance::~SingleInstance()
{
    mServer.close();
}

void SingleInstance::listen(QString name)
{
    mServer.removeServer(name);
    mServer.listen(name);

    qDebug() << "Listening for: " << name;
    qDebug() << mServer.errorString();
}

bool SingleInstance::hasPrevious(QString name, QStringList args)
{
    qDebug() << "Checking for previous instance...";

    QLocalSocket socket;
    socket.connectToServer(name, QLocalSocket::ReadWrite);

    if(socket.waitForConnected())
    {
        qDebug() << "Sending args to previous instance...";

        QByteArray buffer;
        foreach(QString item, args)
        {
            buffer.append(item + "\n");
        }
        socket.write(buffer);
        socket.waitForBytesWritten();

        return true;
    }

    qDebug() << socket.errorString();

    return false;
}

void SingleInstance::newConnection()
{
    emit newInstance();

    mSocket = mServer.nextPendingConnection();
    connect(mSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));
}

void SingleInstance::readyRead()
{
    qDebug() << "Args: " << mSocket->readAll();
    mSocket->close();
    mSocket->deleteLater();
}
