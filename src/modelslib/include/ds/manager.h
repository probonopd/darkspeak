#ifndef MANAGER_H
#define MANAGER_H


#include <QSettings>
#include <QMetaType>
#include <QtCore>

#include "ds/dscert.h"
#include "ds/dsengine.h"
#include "ds/identity.h"
#include "ds/logmodel.h"
#include "ds/identitiesmodel.h"

#ifndef PROGRAM_VERSION
    #define PROGRAM_VERSION "develop"
#endif

namespace ds {
namespace models {

class Manager : public QObject
{
    Q_OBJECT
public:
    using AppState = ds::core::DsEngine::State;
    using OnlineState = ds::core::ProtocolManager::State;

    Q_PROPERTY(QString programName READ getProgramName)
    Q_PROPERTY(QString programNameAndVersion READ getProgramNameAndVersion)
    Q_PROPERTY(int currentPage READ getCurrentPage WRITE setCurrentPage NOTIFY currentPageChanged)

    Q_PROPERTY(int appState READ getAppState NOTIFY appStateChanged)
    Q_PROPERTY(int onlineState READ getOnlineState NOTIFY onlineStateChanged)
    Q_PROPERTY(int online READ isOnline NOTIFY onlineChanged)
    Q_PROPERTY(QUrl onlineStatusIcon READ getOnlineStatusIcon NOTIFY onlineStatusIconChanged)

    Q_INVOKABLE LogModel *logModel();
    Q_INVOKABLE IdentitiesModel *identitiesModel();
    Q_INVOKABLE void textToClipboard(QString text);

    Manager();

public slots:
    AppState getAppState() const;
    OnlineState getOnlineState() const;
    bool isOnline() const;
    QString getProgramName() const;
    QString getProgramNameAndVersion() const;
    int getCurrentPage();
    void setCurrentPage(int page);
    QUrl getOnlineStatusIcon() const;
    void goOnline();
    void goOffline();

signals:
    void appStateChanged(const AppState state);
    void onlineStateChanged(const OnlineState state);
    void currentPageChanged(int page);
    void onlineStatusIconChanged();
    void onlineChanged();

private:
    AppState app_state_ = ds::core::DsEngine::State::INITIALIZING;
    OnlineState online_state_ = ds::core::ProtocolManager::State::OFFLINE;
    std::unique_ptr<ds::core::DsEngine> engine_;
    std::unique_ptr<LogModel> log_;
    std::unique_ptr<IdentitiesModel> identities_;
    int page_ = 3; // Home
};

}} // namespaces

#endif // MANAGER_H
