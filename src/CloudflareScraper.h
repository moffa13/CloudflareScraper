#ifndef CLOUDFLARESCRAPER_H
#define CLOUDFLARESCRAPER_H

#include "Logger.h"
#include "CloudflareScraper_export.h"
#include "Cookies.h"
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

class DLL_API CloudflareScraper : public QObject
{

    Q_OBJECT

    public:
        CloudflareScraper(Cookies *cookies = nullptr, QObject *parent = nullptr, QDir const& v8_path = QDir(QCoreApplication::applicationDirPath()));
        CloudflareScraper(CloudflareScraper const& rhs);
        CloudflareScraper& operator=(CloudflareScraper const& rhs);
        void setUA(QString const& ua, bool add = false);
        void get(QUrl const &url, bool force = false);
        inline void get() { get(m_last_url); }
        inline QUrl getLastUrl() const { return m_last_url; }
        inline void setCookies(Cookies* cookies) {
            m_cookies = cookies;
            m_am->setCookieJar(m_cookies);
        }
        inline QPointer<Cookies> getCookies() const { return m_cookies; }
        inline bool isBusy() const { return m_busy; }
        inline void addUA(QString const& ua, bool update = true);
        inline void removeUA(QString const& ua, bool update = true);
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
        void checkForGoogleV8();
        QString getJSAlgorithm(QByteArray const &raw);
        QString _google_v8;
        static unsigned random(unsigned a, unsigned b);
    Q_SIGNALS:
        void success(QNetworkReply*, QByteArray const &content);
        void error(QString const& msg = QString(), bool recoverable = false);
    private Q_SLOTS:
        void gotResponse(QNetworkReply*);
};

#endif // CLOUDFLARESCRAPER_H
