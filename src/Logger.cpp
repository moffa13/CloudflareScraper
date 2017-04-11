#include "Logger.h"
#include <iostream>
#include <QCoreApplication>
#include <QDateTime>
#include <QFile>
#include <QTextStream>

using namespace std;

int Logger::_log_level = DEBUG;

void Logger::write(QString const &prefix, QString const &message, int act){

    QString string;
    QTextStream stream(&string);
    stream << Logger::getDateFormatted() << " ["+prefix+"] " << message;
    std::string content(stream.string()->toStdString());

    auto &os = (act & ERROR_OUT) ? cerr : cout;

    if(act & PRINT){ // Print to the console
        if(act & ERASE)
            os << "\r";
        os << content.c_str();

        if(act & ERASE)
            os << std::flush;
        else
            os << std::endl << std::flush;
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
    act |= ERROR_OUT;
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
