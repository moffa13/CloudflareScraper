#ifndef CLOUDFLARESCRAPER_H
#define CLOUDFLARESCRAPER_H

#include "CloudflareScraper_export.h"
#include "Cookies.h"
#include "Logger.h"
#include <memory>
#include <QByteArray>
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
        CloudflareScraper(Cookies *cookies = nullptr, QObject *parent = nullptr, QDir const& v8_path = QDir("."));
        CloudflareScraper(CloudflareScraper const& rhs);
        void get(QUrl const &url, bool force = false);
        inline void get() { get(m_last_url); }
        inline QUrl getLastUrl() const { return m_last_url; }
        inline void setCookies(Cookies* cookies) {
            m_cookies = cookies;
            m_am->setCookieJar(m_cookies);
        }
        inline quint8 getRandomUaN() const{ return m_current_ua_n; }
        inline void setRandomUaN(quint8 n){ m_current_ua_n = n; m_current_ua = &RANDOM_UA[m_current_ua_n]; }
        inline QPointer<Cookies> getCookies() const { return m_cookies; }
        inline bool isBusy() const { return m_busy; }
        static inline void setLogLevel(int level) { Logger::setLogLevel(level); }
    private:
        static const QString RANDOM_UA[];
        QPointer<QNetworkAccessManager> m_am;
        void follow_replies(QNetworkReply* reply);
        QUrl m_last_url;
        bool m_busy = false;
        QDir _v8_path;
        QPointer<Cookies> m_cookies;
        QString const *m_current_ua;
        quint8 m_current_ua_n;
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
