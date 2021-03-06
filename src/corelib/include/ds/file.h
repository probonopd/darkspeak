#ifndef FILE_H

#define FILE_H

#include <deque>
#include <functional>
#include <QUuid>
#include <QObject>
#include <chrono>

#include "ds/conversation.h"
#include "ds/contact.h"
#include "ds/identity.h"

namespace ds {
namespace core {

struct FileData;

class File : public QObject, std::enable_shared_from_this<File>
{
    Q_OBJECT
public:
    using ptr_t = std::shared_ptr<File>;
    using hash_cb_t = std::function<void(const QByteArray& hash, const QString& failReason)>;

    /*
     *  Outgoing states:
     *
     *      CREATED --> HASHING --> WAITING --> OFFERED --> QUEUED --> TRANSFERRING --> DONE
     *                                 IncomingFile  ack/proceed
     *
     *  Incoming states:
     *
     *      OFFERED --> QUEUED --> TRANSFERRING --> HASHING --> DONE
     *           accepted
     *
     */

    enum State {
        FS_CREATED,
        FS_HASHING,
        FS_WAITING,
        FS_OFFERED,
        FS_QUEUED,
        FS_TRANSFERRING,
        FS_DONE,
        FS_FAILED,
        FS_REJECTED,
        FS_CANCELLED
    };
    Q_ENUM(State)

    enum Direction {
        OUTGOING,
        INCOMING
    };
    Q_ENUM(Direction)

    File(QObject& parent);
    File(QObject& parent, std::unique_ptr<FileData> data);

    Q_PROPERTY(int id READ getId CONSTANT)
    Q_PROPERTY(QByteArray fileId READ getFileId CONSTANT)
    Q_PROPERTY(State state READ getState NOTIFY stateChanged)
    Q_PROPERTY(Direction direction READ getDirection CONSTANT)
    Q_PROPERTY(bool active READ isActive NOTIFY isActiveChanged)
    Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString path READ getPath WRITE setPath NOTIFY pathChanged)
    Q_PROPERTY(QString hash READ getPrintableHash NOTIFY hashChanged)
    Q_PROPERTY(QDateTime created READ getCreated CONSTANT)
    Q_PROPERTY(QDateTime ackTime READ getAckTime NOTIFY ackTimeChanged)
    Q_PROPERTY(QDateTime fileTime READ getFileTime NOTIFY fileTimeChanged)
    Q_PROPERTY(qlonglong size READ getSize NOTIFY sizeChanged)
    Q_PROPERTY(float progress READ getProgress NOTIFY bytesTransferredChanged)
    Q_PROPERTY(qlonglong bytesTransferred READ getBytesTransferred NOTIFY bytesTransferredChanged)

    Q_INVOKABLE void cancel();
    Q_INVOKABLE void accept();
    Q_INVOKABLE void reject();

    Q_INVOKABLE void openInDefaultApplication();
    Q_INVOKABLE void openFolder();

    int getId() const noexcept;
    QByteArray getFileId() const noexcept;
    State getState() const noexcept;
    void setState(const State state);
    Direction getDirection() const noexcept;
    QString getName() const noexcept;
    void setName(const QString& name);
    QString getPath() const noexcept;
    QString getDownloadPath() const noexcept;
    void setPath(const QString& path);
    QByteArray getHash() const noexcept;
    QString getPrintableHash() const noexcept;
    void setHash(const QByteArray& hash);
    QDateTime getCreated() const noexcept;
    QDateTime getFileTime() const noexcept;
    QDateTime getAckTime() const noexcept;
    qlonglong getSize() const noexcept;
    void setSize(const qlonglong size);
    qlonglong getBytesTransferred() const noexcept;
    void setBytesTransferred(const qlonglong bytes);
    void addBytesTransferred(const size_t bytes);
    void clearBytesTransferred(); // Before start transfer
    void setAckTime(const QDateTime& when);
    void touchAckTime();
    bool isActive() const noexcept;
    int getConversationId() const noexcept;
    int getContactId() const noexcept;
    int getIdentityId() const noexcept;
    Contact *getContact() const;
    Conversation *getConversation() const;
    quint32 getChannel() const noexcept;
    void setChannel(quint32 channel);
    float getProgress() const noexcept;

    /*! Add the new File to the database. */
    void addToDb();

    /*! Delete from the database */
    void deleteFromDb();

    static File::ptr_t load(QObject& parent, const int dbId);
    static File::ptr_t load(QObject& parent, int conversation, const QByteArray& hash);

    const char *getTableName() const noexcept { return "file"; }

    void asynchCalculateHash(hash_cb_t callback = {});

    // Start an incoming transfer in state FS_QUEUED or FS_OFFERED
    void queueForTransfer();

    void transferComplete();
    void transferFailed(const QString& reason, const State state = FS_CANCELLED);

    // Asyncroneously hash he file and verify that it i cirrect.
    // This is to validate received files before
    // the state is changed to FS_DONE
    void validateHash();

    static bool findUnusedName(const QString& path, QString& unusedPath);

signals:
    void stateChanged();
    void isActiveChanged();
    void nameChanged();
    void pathChanged();
    void hashChanged();
    void ackTimeChanged();
    void fileTimeChanged();
    void sizeChanged();
    void bytesTransferredChanged();
    void transferDone(File *file, bool succeess);

private:
    static QString getSelectStatement(const QString& where);
    static ptr_t load(QObject& parent, const std::function<void(QSqlQuery&)>& prepare);
    void flushBytesAdded();

    int id_ = 0;
    std::unique_ptr<FileData> data_;
    quint32 channel_ = 0;
    qlonglong bytesAdded_ = {};
    std::unique_ptr<std::chrono::steady_clock::time_point> nextFlush_;
};

struct FileData {
    File::State state = File::FS_CREATED;
    File::Direction direction = File::OUTGOING;
    int identity = 0;
    int contact = 0;
    int conversation = 0;
    QByteArray fileId;
    QByteArray hash;
    QString name; // The adverticed name, may be something else than then the real name
    QString path; // Full path with actual name
    qlonglong size = {};
    qlonglong bytesTransferred = {}; // REST offset
    QDateTime fileTime;
    QDateTime createdTime;
    QDateTime ackTime;
};


}}

Q_DECLARE_METATYPE(ds::core::File *)
Q_DECLARE_METATYPE(ds::core::FileData)

#endif // FILE_H
