#pragma once

#include <QString>
#include <QList>
#include <QVariantHash>
#include <QJsonObject>
#include <QSslCertificate>

#define DEFAULT_REDIS_PORT 6379
#define DEFAULT_SSH_PORT 22
#define DEFAULT_TIMEOUT_IN_MS 60000

namespace RedisClient {

class Connection;

class ConnectionConfig
{
public:
    static const char DEFAULT_NAMESPACE_SEPARATOR = ':';
    static const char DEFAULT_KEYS_GLOB_PATTERN = '*';

public:
    ConnectionConfig(const QString & host = "", const QString & name = "",
                     const int port = DEFAULT_REDIS_PORT);
    ConnectionConfig & operator = (const ConnectionConfig & other);
    ConnectionConfig(const QVariantHash& options);

    QString name() const;
    QString host() const;
    QString auth() const;
    int port() const;
    int executeTimeout() const;
    int connectionTimeout() const;

    QList<QSslCertificate> sslCaCertificates() const;
    QString sslPrivateKeyPath() const;
    QString sslLocalCertPath() const;

    bool isNull() const;
    bool useSshTunnel() const;
    bool useAuth() const;
    bool useSsl() const;
    bool isValid() const;
    bool isSshPasswordUsed();

    QVariantHash getInternalParameters();

    template <class T> inline T param(const QString& p) const
    {
       if (m_parameters.contains(p)) return m_parameters[p].value<T>();
       return T();
    }

    template <class T> inline void setParam(const QString& key, T p)
    {
        m_parameters.insert(key, p);
    }

    QWeakPointer<Connection> getOwner() const;
    QString getSshPrivateKey();
    QString keysPattern() const;

    void setOwner(QWeakPointer<Connection>);
    void setSshTunnelSettings(QString host, QString user, QString pass,
                              int port = DEFAULT_SSH_PORT,
                              QString sshPrivatekey = "");    

    QJsonObject toJsonObject();
    static ConnectionConfig fromJsonObject(const QJsonObject& config);

protected:
    QString getValidPathFromParameter(const QString& param) const;
private:
    QWeakPointer<Connection> m_owner;
    QVariantHash m_parameters;
};
}

/**
 * Methods for backward compatibility with Qt 5.4
 */
QJsonObject QJsonObjectFromVariantHash(const QVariantHash &hash);
QVariantHash QJsonObjectToVariantHash(const QJsonObject &o);
