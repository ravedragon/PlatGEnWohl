#include "localserver.h"

#include <QFile>
#include <QStringList>
#include <QtDebug>

#include "../common_features/mainwinconnect.h"

#include "../networking/engine_intproc.h"

/**
 * @brief LocalServer::LocalServer
 *  Constructor
 */
LocalServer::LocalServer()
{

}

/**
 * @brief LocalServer::~LocalServer
 *  Destructor
 */
LocalServer::~LocalServer()
{
  server->close();
  for(int i = 0; i < clients.size(); ++i)
  {
    clients[i]->close();
  }
}

/**
 * -----------------------
 * QThread requred methods
 * -----------------------
 */

/**
 * @brief run
 *  Initiate the thread.
 */
void LocalServer::run()
{
  server = new QLocalServer();

  QObject::connect(server, SIGNAL(newConnection()), this, SLOT(slotNewConnection()));
  QObject::connect(this, SIGNAL(privateDataReceived(QString)), this, SLOT(slotOnData(QString)));

#ifdef Q_OS_UNIX
  // Make sure the temp address file is deleted
  QFile address(QString("/tmp/" LOCAL_SERVER_NAME));
  if(address.exists()){
    address.remove();
  }
#endif

  QString serverName = QString(LOCAL_SERVER_NAME);
  server->listen(serverName);
  while(server->isListening() == false){
    server->listen(serverName);
    msleep(100);
  }
  exec();
}

/**
 * @brief LocalServer::exec
 *  Keeps the thread alive. Waits for incomming connections
 */
void LocalServer::exec()
{
  while(server->isListening())
  {
    msleep(100);
    server->waitForNewConnection(100);
    for(int i = 0; i < clients.size(); ++i)
    {
      if(clients[i]->waitForReadyRead(100))
      {
        QByteArray data = clients[i]->readAll();
        emit privateDataReceived(QString::fromUtf8(data));
      }
      else
      {
          if(!clients[i]->isOpen())
          {
              QLocalSocket* tmp = clients[i];
              clients.removeAt(i);
              delete tmp;
              i--;
          }
      }
    }
  }
}



/**
 * -------
 * SLOTS
 * -------
 */



/**
 * @brief LocalServer::slotNewConnection
 *  Executed when a new connection is available
 */
void LocalServer::slotNewConnection()
{
  clients.push_front(server->nextPendingConnection());
}



/**
 * @brief LocalServer::slotOnData
 *  Executed when data is received
 * @param data
 */
void LocalServer::slotOnData(QString data)
{
  qDebug() << data;
  QStringList args = data.split('\n');
  foreach(QString c, args)
  {
      if(c.startsWith("CMD:", Qt::CaseInsensitive))
      {
        onCMD(c);
      }
      else
      {
        emit dataReceived(c);
      }
  }
}



/**
 * -------
 * Helper methods
 * -------
 */
void LocalServer::onCMD(QString data)
{
  //  Trim the leading part from the command
  if(data.startsWith("CMD:"))
  {
    data.remove("CMD:");

    QStringList commands;
    commands << "showUp";
    commands << "CONNECT_TO_ENGINE";
    commands << "ENGINE_CLOSED";

      switch(commands.indexOf(data)){
        case 0:
        {
              emit showUp();
              qApp->setActiveWindow(MainWinConnect::pMainWin);
              if(!MainWinConnect::pMainWin->isMaximized())
                MainWinConnect::pMainWin->showNormal();
              else
                MainWinConnect::pMainWin->showMaximized();
              MainWinConnect::pMainWin->raise();
              break;
        }
        case 1:
        {
              MainWinConnect::pMainWin->showMinimized();
              qApp->setActiveWindow(MainWinConnect::pMainWin);
              if(MainWinConnect::pMainWin->activeChildWindow()==1)
              {
                  IntEngine::engineSocket->sendLevelData(
                         MainWinConnect::pMainWin->activeLvlEditWin()->LvlData
                     );
              }
              break;
        }
        case 2:
        {
              MainWinConnect::pMainWin->showNormal();
              qApp->setActiveWindow(MainWinConnect::pMainWin);
              if(MainWinConnect::pMainWin->activeChildWindow()==1)
              {
                  MainWinConnect::pMainWin->activeLvlEditWin()->setFocus();
                  MainWinConnect::pMainWin->activeLvlEditWin()->raise();
              }
              IntEngine::quit();
              break;
        }
        default:
          emit acceptedCommand(data);
      }
  }
  else
      emit acceptedCommand(data);
}
