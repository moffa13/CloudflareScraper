#ifndef CLOUDFLARESCRAPER_H
#define CLOUDFLARESCRAPER_H

#include "CloudflareScraper_export.h"
#include "Cookies.h"
#include "Logger.h"
#include <memory>
#include <QByteArray>
#include <QCoreApplication>
#include <QDir>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
#include <QPointer>
#include <QProcess>
#include <QString>

#ifndef NO_DLL
class DLL_API CloudflareScraper : public QObject
#else
class CloudflareScraper : public QObject
#endif
{

    Q_OBJECT

    public:
        CloudflareScraper(Cookies *cookies = nullptr, QObject *parent = nullptr, QDir const& v8_path = QDir(QCoreApplication::applicationDirPath()));
        CloudflareScraper(CloudflareScraper const& rhs);
        inline void addUA(QString const& ua, bool update = true){ RANDOM_UA_LIST.insert(ua); if(update) setRandomUA(); }
        inline void removeUA(QString const& ua, bool update = true) { RANDOM_UA_LIST.remove(ua); if(update) setRandomUA(); }
        void get(QUrl const &url, bool force = false);
        inline void get() { get(m_last_url); }
        inline QUrl getLastUrl() const { return m_last_url; }
        inline void setCookies(Cookies* cookies) {
            m_cookies = cookies;
            m_am->setCookieJar(m_cookies);
        }
        inline QPointer<Cookies> getCookies() const { return m_cookies; }
        inline bool isBusy() const { return m_busy; }
        static inline void setLogLevel(int level) { Logger::setLogLevel(level); }
    private:
        static QSet<QString> RANDOM_UA_LIST;
        QString _current_ua;
        void setRandomUA();
        QPointer<QNetworkAccessManager> m_am;
        void follow_replies(QNetworkReply* reply);
        QUrl m_last_url;
        bool m_busy = false;
        QDir _v8_path;
        QPointer<Cookies> m_cookies;
        bool m_follow_redirects = true;
        long m_secret_token = 0;
        QString getUA() const;
        QPointer<QProcess> evalJS(QString const &js);
        void hack(QUrl const &url, QByteArray const& resp);
        QString getJSAlgorithm(QByteArray const &raw);
        static unsigned random(unsigned a, unsigned b);
    Q_SIGNALS:
        void success(QNetworkReply*, QByteArray const &content);
        void error();
    private Q_SLOTS:
        void gotResponse(QNetworkReply*);
};

#endif // CLOUDFLARESCRAPER_H
