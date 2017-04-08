#include "Logger.h"
#include <iostream>
#include <QDateTime>
#include <QFile>
#include <QTextStream>
#include <QCoreApplication>

using namespace std;

QDateTime Logger::getDateTime(){
    return QDateTime::currentDateTime();
}

QString Logger::getDateFormatted(){
    return Logger::getDateTime().toString("yyyy-MM-dd hh:mm:ss");
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

void Logger::info(QString const &message, int act){
    Logger::write("Info", message, act);
}

void Logger::warn(QString const &message, int act){
    Logger::write("Warn", message, act);
}

void Logger::error(QString const &message, int act){
    Logger::write("ERROR", message, act);
}
