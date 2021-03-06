// This file is part of Agros2D.
//
// Agros2D is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
//
// Agros2D is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Agros2D.  If not, see <http://www.gnu.org/licenses/>.
//
// hp-FEM group (http://hpfem.org/)
// University of Nevada, Reno (UNR) and University of West Bohemia, Pilsen
// Email: agros2d@googlegroups.com, home page: http://hpfem.org/agros2d/

#include "remotecontrol.h"

#include "pythonengine_agros.h"


ScriptEngineRemoteLocal::ScriptEngineRemoteLocal()
{  
    qDebug() << serverName();
    // server
    removeServer(serverName());
    if (!listen(serverName()))
    {
        qWarning() << tr("Error: Unable to start the server (agros2d-server): %1.").arg(errorString());
        return;
    }

    connect(this, SIGNAL(newConnection()), this, SLOT(connected()));
}

ScriptEngineRemoteLocal::~ScriptEngineRemoteLocal()
{
}

void ScriptEngineRemoteLocal::connected()
{
    m_command = "";

    m_server_socket = nextPendingConnection();
    connect(m_server_socket, SIGNAL(readyRead()), this, SLOT(readCommand()));
    connect(m_server_socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
}

void ScriptEngineRemoteLocal::readCommand()
{
    QTextStream in(m_server_socket);

    QString comm = in.readAll();
    qDebug() << "com: " << comm;
    if (comm.startsWith("client:"))
    {
        m_clientName = comm.right(comm.length() - 7);
        qDebug() << m_clientName;
    }
    else
        m_command = comm;
}

void ScriptEngineRemoteLocal::disconnected()
{
    m_server_socket->deleteLater();

    if (!m_command.isEmpty())
    {
        bool successfulRun = currentPythonEngineAgros()->runScript(m_command);
    }

    m_client_socket = new QLocalSocket();
    connect(m_client_socket, SIGNAL(error(QLocalSocket::LocalSocketError)), this, SLOT(displayError(QLocalSocket::LocalSocketError)));

    m_client_socket->connectToServer(clientName());
    if (m_client_socket->waitForConnected(1000))
    {
        ErrorResult result = currentPythonEngineAgros()->parseError();

        QTextStream out(m_client_socket);
        out << result.error();
        out.flush();
        m_client_socket->waitForBytesWritten();
    }
    else
    {
        displayError(QLocalSocket::ConnectionRefusedError);
    }

    delete m_client_socket;
}

void ScriptEngineRemoteLocal::displayError(QLocalSocket::LocalSocketError socketError)
{
    switch (socketError) {
    case QLocalSocket::ServerNotFoundError:
        qWarning() << tr("Server error: The host was not found.");
        break;
    case QLocalSocket::ConnectionRefusedError:
        qWarning() << tr("Server error: The connection was refused by the peer. Make sure the agros2d-client server is running.");
        break;
    default:
        qWarning() << tr("Server error: The following error occurred: %1.").arg(m_client_socket->errorString());
    }
}

QString ScriptEngineRemoteLocal::clientName()
{
    return m_clientName;
}

QString ScriptEngineRemoteLocal::serverName()
{
    return QString("agros2d-server-%1").arg(QString::number(QCoreApplication::applicationPid()));
}
