#include <QCoreApplication>
#include <QObject>
#include "CloudflareScraper.h"


int main(int argc, char* argv[]){
    QCoreApplication app(argc, argv);

    CloudflareScraper *scraper = new CloudflareScraper{};
    QObject::connect(scraper, &CloudflareScraper::success, [](){

    });
    scraper->get(QUrl("http://www.torrent9.biz/"));

    return app.exec();
}
