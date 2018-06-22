#include "mynetwork.h"

MyNetwork::MyNetwork(QObject *parent) : QObject(parent)
{
    m_udpSocket = new QUdpSocket(this);
    if(!m_udpSocket->bind(udpPort, QUdpSocket::ShareAddress )) //
        qDebug() << "m_udpSocket : " + m_udpSocket->errorString()  ;
    else
        qDebug() <<"UDP bind 127.0.0.1:" +QString::number(udpPort)+" LISTENING." ;

    connect(m_udpSocket, SIGNAL(readyRead()), this, SLOT(readyReadUDP()));

}

void MyNetwork::sendUDP(QString msg)
{
    if(enable_debug_udp)
        qDebug() << "sendUDP: " << msg ;
    QByteArray datagram = msg.toLatin1() ;
    m_udpSocket->writeDatagram(datagram.data(), datagram.size(),
                             QHostAddress::Broadcast, udpPort);
}

void MyNetwork::readyReadUDP()
{
}
