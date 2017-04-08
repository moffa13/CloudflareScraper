#ifndef LOGGER_H
#define LOGGER_H

#include <QDateTime>
#include <QString>

enum ACTION{
    ERASE = 1,
    PRINT = 2,
    PRINT_FILE = 4
};


class Logger
{
    public:
        static QDateTime getDateTime();
        static QString getDateFormatted();
        static void write(QString const &prefix, QString const &message, int act);
        static void info(QString const &message, int act = PRINT);
        static void warn(QString const &message, int act = PRINT);
        static void error(QString const &message, int act = PRINT);
};

#endif // LOGGER_H
