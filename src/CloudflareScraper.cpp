#include "Logger.h"
#include "CloudflareScraper.h"
#include "CloudflareException.h"
#include <algorithm>
#include <ctime>
#include <iterator>
#include <random>
#include <QCoreApplication>
#include <QDir>
#include <QNetworkCookie>
#include <QRegExp>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QRegularExpressionMatchIterator>
#include <QString>
#include <QStringList>
#include <QThread>
#include <QUrlQuery>

QSet<QString> CloudflareScraper::RANDOM_UA_LIST = {
    "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/56.0.2924.87 Safari/537.36",
    "Mozilla/5.0 (Windows NT 10.0; WOW64; rv:51.0) Gecko/20100101 Firefox/51.0",
    "Mozilla/5.0 (Windows NT 6.3; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/56.0.2924.87 Safari/537.36",
    "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/56.0.2924.87 Safari/537.36"
};

inline void CloudflareScraper::addUA(QString const& ua, bool update){ RANDOM_UA_LIST.insert(ua); if(update) setRandomUA(); }
inline void CloudflareScraper::removeUA(QString const& ua, bool update) { RANDOM_UA_LIST.remove(ua); if(update) setRandomUA(); }

/**
 * Returns a number between a and b, which are included
 * @brief CloudflareScraper::random
 * @param a
 * @param b
 */
unsigned CloudflareScraper::random(unsigned a, unsigned b){
    std::mt19937 random_gen;
    random_gen.seed(std::random_device()());
    std::uniform_int_distribution<std::mt19937::result_type> rnd(a, b);
    return rnd(random_gen);
}

QString CloudflareScraper::getUA() const{
    return _current_ua;
}

CloudflareScraper::CloudflareScraper(Cookies *cookies, QObject *parent, QDir const& v8_path) :
    QObject(parent),
    m_cookies(cookies),
    m_am(new QNetworkAccessManager(this)),
    _v8_path(v8_path)
{
    setRandomUA(); // Set a random user-agent
    Logger::log(DEBUG, "Using useragent: " + _current_ua);
    m_am->setCookieJar(m_cookies);
}

void CloudflareScraper::setRandomUA(){
    auto size = RANDOM_UA_LIST.size();
    auto rand = random(0, size - 1);
    QSet<QString>::iterator it = RANDOM_UA_LIST.begin() + rand;
    _current_ua = *it;
}

void CloudflareScraper::setUA(QString const& ua, bool add){
    _current_ua = ua;
    if(add)
        addUA(ua, false);
}

CloudflareScraper::CloudflareScraper(CloudflareScraper const& rhs) : CloudflareScraper(){
    _current_ua = rhs._current_ua;
    Cookies *c = new Cookies(this);
    c->setAllCookies(rhs.getCookies()->getAllCookies());
    setCookies(c);
}

CloudflareScraper& CloudflareScraper::operator =(CloudflareScraper const &rhs){
    if(&rhs != this){
        _current_ua = rhs._current_ua;
        Cookies *c = new Cookies(this);
        c->setAllCookies(rhs.getCookies()->getAllCookies());
        setCookies(c);
    }
    return *this;
}

void CloudflareScraper::get(QUrl const &url, bool force){

    if(m_cookies.isNull())
        throw CloudflareException{"No cookies jar provided"};

    checkForGoogleV8();

    if(m_busy && !force) return; // Do not accept requests while still processing one

    m_busy = true;
    m_last_url = url;

    QNetworkRequest r{url};
    r.setHeader(QNetworkRequest::UserAgentHeader, getUA());

    connect(m_am, SIGNAL(finished(QNetworkReply*)), this, SLOT(gotResponse(QNetworkReply*)), Qt::UniqueConnection);

    follow_replies(m_am->get(r));

}

/**
 * Sets _google_v8 to the right executable d8 file
 * @brief CloudflareScraper::checkForGoogleV8
 * @throws CloudflareException if google_v8 hasn't been found
 */
void CloudflareScraper::checkForGoogleV8(){

    if(!_google_v8.isEmpty()) return;

    QList<QDir> whereToFind;
    whereToFind.append(_v8_path);
    whereToFind.append(QDir(QCoreApplication::instance()->applicationDirPath()));

    bool found = false;
    QString google_v8;

    for(QList<QDir>::iterator it = whereToFind.begin(); it != whereToFind.end(); ++it){
        #ifdef Q_OS_WIN32
            google_v8 = it->absoluteFilePath("d8.exe");
        #else
            google_v8 = it->absoluteFilePath("d8");
        #endif

        {
            QFile f{google_v8};
            if(f.exists()){
                #ifndef Q_OS_WIN32
                f.setPermissions(f.permissions() | (QFile::ExeUser | QFile::ExeGroup));
                #endif
                found = true;
                break;
            }
        }
    }

    if(!found){
        throw CloudflareException{QString{"Google v8 not found"}.toStdString()};
    }

    _google_v8 = google_v8;
}

/**
 * Connect every reply and emit error when error signal is emitted
 * @brief follow_replies
 */
void CloudflareScraper::follow_replies(QNetworkReply* reply){

}

void CloudflareScraper::gotResponse(QNetworkReply* reply){

    disconnect(m_am, 0, 0, 0);

    int code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    auto locationHeader = reply->header(QNetworkRequest::LocationHeader);

    if(locationHeader.isValid()){ // If the location header exists, we have to redirect
        QUrl dest{locationHeader.toUrl()};
        Logger::warn(QString::number(code) + " redirect : " + dest.toString());
        if(dest.toString().isEmpty()){
            m_busy = false;
            emit error(QString{}, true);
            return;
        }

        get(dest, true);
        return;
    }

    QByteArray const content = reply->readAll();

    if(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 503
            && content.contains("jschl_vc") && content.contains("jschl_answer")){
        Logger::log(DEBUG, "It is secured with cloudflare !");
        Logger::log(DEBUG, "Bypassing...");
        hack(reply->url(), content);
    }else{
        Logger::log(DEBUG, "It is NOT secured !");
        m_busy = false;
        emit success(reply, content);
    }

}

QString CloudflareScraper::getJSAlgorithm(QByteArray const &raw){
    QRegExp reg("setTimeout\\(function\\(\\)\\{\\s+(var s,t,o,p,b,r,e,a,k,i,n,g,f.+\\r?\\n[\\s\\S]+a\\.value =.+)\\r?\\n");
    reg.setMinimal(true);
    reg.indexIn(raw, 0);
    QString first{reg.cap(1)};

    QRegularExpression r("a\\.value = (parseInt\\(.+\\)).+");

    QRegularExpression r2("\\s{3,}[a-z](?: = |\\.).+");

    first = first.replace(r, "res=\\1");
    first = first.replace(r2, "");

    return first;
}

QPointer<QProcess> CloudflareScraper::evalJS(QString const &js){
    QPointer<QProcess> process(new QProcess(this));

    QStringList args; args << "-e" << js + ";print(res);";

    connect(process, &QProcess::readyReadStandardOutput, [process, this](){
        m_secret_token = process->readAllStandardOutput().trimmed().toLong();
    });
    process->start(_google_v8, args);
    return process;
}

void CloudflareScraper::hack(QUrl const &url, QByteArray const& resp){
    QThread::sleep(5);

    QString final_url{QString{"%1://%2/cdn-cgi/l/chk_jschl"}.arg(url.scheme()).arg(url.host())};
    QString host{url.host()};

    QString js_eval{getJSAlgorithm(resp)};

    auto proc = evalJS(js_eval);

    proc->waitForFinished(-1);
    proc->close();
    proc->deleteLater();

    QRegExp regex("name=\"jschl_vc\" value=\"(\\w+)\"");
    regex.indexIn(resp, 0);
    QString jschl_vc = regex.cap(1);
    regex.setPattern("name=\"pass\" value=\"(.+)\"");
    regex.setMinimal(true);
    regex.indexIn(resp, 0);
    QString pass = regex.cap(1);

    if(jschl_vc.isEmpty() || pass.isEmpty()){
        emit error("Unexpected exception, maybe cloudflare's changed its method");
    }

    QNetworkCookie const* cfduid_cookie = m_cookies->getCookie("__cfduid");
    if(cfduid_cookie == nullptr){
        emit error("Didn't get __cfduid cookie");
    }

    m_secret_token += host.length();

    QUrlQuery getData;
    getData.addQueryItem("jschl_vc", jschl_vc);
    getData.addQueryItem("pass", pass);
    getData.addQueryItem("jschl_answer", QString::number(m_secret_token));
    QNetworkRequest r(final_url + "?" + getData.toString(QUrl::FullyEncoded).toUtf8());
    r.setHeader(QNetworkRequest::UserAgentHeader, getUA());
    r.setRawHeader("Referer", url.toString().toUtf8());
    r.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    Logger::log(DEBUG, "GET : " + r.url().toString());

    connect(m_am, &QNetworkAccessManager::finished, [this, url](){

        disconnect(m_am, 0, 0, 0);

        QNetworkCookie const* cfclearance_cookie = m_cookies->getCookie("cf_clearance");
        if(cfclearance_cookie != nullptr){
            Logger::log(DEBUG, "GOT cf_clearance, OK");
        }else{
            Logger::log(ERROR, "Didn't get cf_clearance, failed");
            emit error("Didn't get cf_clearance", true);
        }

        get(url, true); // Now we've got the cookies, we call again the get method but this time it will directly return content

    });

    follow_replies(m_am->get(r)); // The first request, which will be used to get jschl_vc, pass
}


