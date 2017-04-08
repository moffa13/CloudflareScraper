#include "Cookies.h"

Cookies::Cookies(QObject *parent) : QNetworkCookieJar(parent)
{

}

QNetworkCookie const* Cookies::getCookie(QString name) const{
    auto cookies = allCookies();
    for(int i{0}; i < cookies.length(); ++i){
        QNetworkCookie const& cookie = cookies.at(i);
        if(cookie.name() == name){
            return &cookie;
        }
    }
    return nullptr;
}
