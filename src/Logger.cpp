#include "Logger.h"
#include <iostream>
#include <QDateTime>
#include <QFile>
#include <QTextStream>
#include <QCoreApplication>

using namespace std;

int Logger::_log_level = DEBUG;

QDateTime Logger::getDateTime(){
    return QDateTime::currentDateTime();
}

QString Logger::getDateFormatted(){
    return Logger::getDateTime().toString("yyyy-MM-dd hh:mm:ss");
}

void Logger::setLogLevel(int level){
    _log_level = level;
}

void Logger::write(QString const &prefix, QString const &message, int act){

    QString string;
    QTextStream stream(&string);
    stream << Logger::getDateFormatted() << " ["+prefix+"] " << message;
    std::string content(stream.string()->toStdString());

    if(act & PRINT){ // Print to the console
        if(act & ERASE)
            cout << "\r";
        cout << content.c_str();

        if(act & ERASE)
            cout << std::flush;
        else
            cout << std::endl << std::flush;
    }else{
        QFile f(qApp->applicationDirPath() + "/error.log");
        if(f.open(QIODevice::WriteOnly | QIODevice::Append)){
            f.write(content.c_str(), content.size());
        }
        else{
            throw std::runtime_error("Impossible to write in log file");
        }
    }

}

void Logger::debug(QString const &message, int act){
    Logger::write("Info", message, act);
}

void Logger::warn(QString const &message, int act){
    Logger::write("Warn", message, act);
}

void Logger::error(QString const &message, int act){
    Logger::write("ERROR", message, act);
}

void Logger::log(int log_level, const QString &message, int act){

    if(_log_level < log_level) return;

    switch (log_level) {
        case ERROR:
            error(message, act);
            break;
        case WARN:
            warn(message, act);
            break;
        case DEBUG:
            debug(message, act);
            break;
        default:
            throw std::runtime_error{"Unknown log level"};
            break;
    }
}
