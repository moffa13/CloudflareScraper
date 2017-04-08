#include <QCoreApplication>
#include <QObject>
#include "CloudflareScraper.h"
#include "Cookies.h"

int main(int argc, char* argv[]){
    QCoreApplication app(argc, argv);

    Cookies c = new Cookies;

    QDir v8_base{qApp->applicationDirPath()};
    v8_base.cd("../../third-party/");

#ifdef Q_OS_WIN
    v8_base.cd("windows");
#else
    v8_base.cd("linux");
#endif

    CloudflareScraper scraper{&c, nullptr, v8_base};

    QObject::connect(&scraper, &CloudflareScraper::success, [](QNetworkReply* reply, QByteArray const& content){
       qDebug() << "Got response";
    });
    scraper.get(QUrl("http://www.torrent9.biz/"));

    return app.exec();
}
