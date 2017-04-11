#include "CloudflareScraper.h"
#include "Cookies.h"
#include "Logger.h"
#include <iostream>
#include <QCoreApplication>
#include <QObject>
#include <QTimer>

int main(int argc, char* argv[]){
    QCoreApplication app(argc, argv);

    Cookies *c = new Cookies;

    QDir v8_base{qApp->applicationDirPath()};
    v8_base.cd("../../third-party/");

#ifdef Q_OS_WIN
    v8_base.cd("windows");
#else
    v8_base.cd("linux");
#endif

    CloudflareScraper::setLogLevel(ERROR);
    CloudflareScraper scraper{c, nullptr, v8_base};

    QObject::connect(&scraper, &CloudflareScraper::success, [c](QNetworkReply* reply, QByteArray const& content){
        Q_UNUSED(reply);
        std::cout << content.toStdString() << std::flush;
        c->deleteLater();
        qApp->exit(0);
    });

    QTimer::singleShot(0, [&scraper, argc, argv](){
        if(argc > 1)
            scraper.get(QUrl(argv[1]));
        else{
            Logger::error("No url specified, please use CloudflareScraper.exe <url>");
            qApp->exit(1);
        }
    });

    return app.exec();
}
