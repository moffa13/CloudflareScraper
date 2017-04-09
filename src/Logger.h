#ifndef LOGGER_H
#define LOGGER_H

#include <QDateTime>
#include <QString>

enum ACTION{
    ERASE = 1,
    PRINT = 2,
    PRINT_FILE = 4
};

enum LOGLEVEL{
    NOTHING = 0,
    ERROR = 1,
    WARN = 2,
    DEBUG = 3
};


class Logger
{
    public:
        static QDateTime getDateTime();
        static QString getDateFormatted();
        static void setLogLevel(int level);
        static void write(QString const &prefix, QString const &message, int act);
        static void error(QString const &message, int act = PRINT);
        static void warn(QString const &message, int act = PRINT);
        static void debug(QString const &message, int act = PRINT);
        static void log(int log_level, QString const& message, int act = PRINT);
    private:
        static int _log_level;
};

#endif // LOGGER_H
