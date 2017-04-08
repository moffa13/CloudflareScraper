#ifndef COOKIES_H
#define COOKIES_H

#include <QNetworkCookieJar>
#include <QNetworkCookie>
#include <QList>
#include <QString>
#include <QObject>

class Cookies : public QNetworkCookieJar
{
    public:
        Cookies(QObject *parent = 0);
        QList<QNetworkCookie> getAllCookies() { return allCookies(); }
        void setAllCookies(const QList<QNetworkCookie> & cookieList) { QNetworkCookieJar::setAllCookies(cookieList); }
        QNetworkCookie const* getCookie(QString name) const;
};

#endif // COOKIES_H
