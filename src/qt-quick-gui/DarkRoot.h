#pragma once

#include <memory>

#include <QtCore>

#include "darkspeak/darkspeak.h"
#include "darkspeak/Api.h"
#include "darkspeak/Config.h"
#include "ContactsModel.h"


// Todo: Make signals for online/offline state change
// Todo: Make it possible to edit my_info_;

using namespace std;
using namespace darkspeak;
using namespace war;

class SettingsData;
class ImageProvider;

/*! Interface to QML.
 *
 * The gui app has one instance of this class. QML use it
 * to instatiate models and access properties of the IM layer.
 */
class DarkRoot : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString programName READ programName)
public:
    explicit DarkRoot(darkspeak::Api& api,
                      const std::shared_ptr<darkspeak::Config>& config,
                      QObject *parent = nullptr);

    QString programName() const {
        return "DarkSpeak";
    }

    Q_INVOKABLE ContactsModel *contactsModel();
    Q_INVOKABLE SettingsData *settings();

    Q_INVOKABLE void goOnline();
    Q_INVOKABLE void goOffline();
    Q_INVOKABLE void copyToClipboard(QString text);
    Q_INVOKABLE void acceptFile(const QString buddyHandle, const QString fileId);
    Q_INVOKABLE void rejectFile(const QString buddyHandle, const QString fileId);

    void SetImageProvider(ImageProvider *provider) {
        image_provider_ = provider;
    }

    ImageProvider *GetImageProvider() {
        return image_provider_;
    }

private:
    darkspeak::Api& api_;
    std::shared_ptr<darkspeak::Config> config_;
    ImageProvider *image_provider_ = nullptr;
};
