/*
 * Platformer Game Engine by Wohlstand, a free platform for game making
 * Copyright (c) 2014 Vitaly Novichkov <admin@wohlnet.ru>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "crashhandler.h"
#include <ui_crashhandler.h>
#include "dev_console/devconsole.h"
#include <QDesktopServices>

//temp
//#include <QMessageBox>

#ifdef _WIN32
    #include <windows.h>
    #include <dbghelp.h>
#elif __linux__
    #include <execinfo.h>
    #include <unistd.h>
#endif
#include <signal.h>

QString CrashHandler::getStacktrace()
{
    #ifdef _WIN32
        unsigned int   i;
        void         * stack[ 100 ];
        unsigned short frames;
        SYMBOL_INFO  * symbol;
        HANDLE         process;

        process = GetCurrentProcess();

        SymInitialize( process, NULL, TRUE );

        frames               = CaptureStackBackTrace( 0, 100, stack, NULL );
        symbol               = ( SYMBOL_INFO * )calloc( sizeof( SYMBOL_INFO ) + 256 * sizeof( char ), 1 );
        symbol->MaxNameLen   = 255;
        symbol->SizeOfStruct = sizeof( SYMBOL_INFO );

        QString bkTrace("");
        for( i = 0; i < frames; i++ )
        {
            char buf[500];
            SymFromAddr( process, ( DWORD64 )( stack[ i ] ), 0, symbol );

            sprintf(buf, "%i: %s - 0x%0llx\n", frames - i - 1, symbol->Name, symbol->Address );
            bkTrace += buf;
        }
        return bkTrace;
    #elif __linux__
        void *array[400];
        size_t size;

        char **strings;
        size = backtrace(array, 10);

        strings = backtrace_symbols(array, size);

        QString bkTrace("");
        for(int j = 0; j < size; j++)
             bkTrace += QString(strings[j])+"\n";
        return bkTrace;
    #else
        return QString("");
    #endif
}

CrashHandler::CrashHandler(QString &crashText, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CrashHandler)
{
    ui->setupUi(this);
    ui->crashData->setText(crashText);
}


CrashHandler::~CrashHandler()
{
    delete ui;
}

void CrashHandler::crashByFlood()
{
    QString crashMsg = QApplication::tr("We're sorry, but PGE Editor has crashed. \nReason: Out of memory!\n\n"
                                        "To prevent this, try closing other uneccessary programs to free up more memory.");

    if(DevConsole::isConsoleShown())
        DevConsole::closeIfPossible();

    crashMsg += QString("\n\n") + getStacktrace();

    CrashHandler* crsh = new CrashHandler(crashMsg);
    crsh->exec();

    exit(EXIT_FAILURE);
}

void CrashHandler::crashByUnhandledException()
{
    std::exception_ptr unhandledException = std::current_exception();
    try{
        std::rethrow_exception(unhandledException);
    }
    catch(const std::exception& e)
    {
        QString crashMsg = QApplication::tr("We're sorry, but PGE Editor has crashed. \nReason: %1\n\nPlease inform our forum staff so we can try to fix this problem, Thank you\n\nForum link: engine.wohlnet.ru/forum").arg(e.what());

        if(DevConsole::isConsoleShown())
            DevConsole::closeIfPossible();

        crashMsg += QString("\n\n") + getStacktrace();

        CrashHandler* crsh = new CrashHandler(crashMsg);
        crsh->exec();
    }

    exit(EXIT_FAILURE);
}

void CrashHandler::crashBySIGSERV(int /*signalid*/)
{
    QString crashMsg = QApplication::tr("We're sorry, but PGE Editor has crashed. \nReason: Signal Segmentation Violation [SIGSERV]\n\n");

    if(DevConsole::isConsoleShown())
        DevConsole::closeIfPossible();

    crashMsg += QString("\n\n") + getStacktrace();

    CrashHandler* crsh = new CrashHandler(crashMsg);
    crsh->exec();

    exit(EXIT_FAILURE);
}

void CrashHandler::initCrashHandlers()
{
    std::set_new_handler(&crashByFlood);
    std::set_terminate(&crashByUnhandledException);
    signal(SIGSEGV, &crashBySIGSERV);
}

void CrashHandler::on_pgeForumButton_clicked()
{
    QDesktopServices::openUrl(QUrl("http://engine.wohlnet.ru/forum/"));
}

void CrashHandler::on_exitButton_clicked()
{
    this->close();
}
