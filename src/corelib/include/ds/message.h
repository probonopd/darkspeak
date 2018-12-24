#ifndef MESSAGE_H
#define MESSAGE_H

#include <QByteArray>
#include <QDateTime>

#include "ds/dscert.h"

namespace ds {
namespace core {

enum class Encoding
{
    US_ACSII,
    UTF8
};

enum class Direction {
    OUTGOING,
    INCOMING
};

class Message
{
public:
    Direction direction = Direction::OUTGOING;
    QByteArray conversation;
    QByteArray message_id;
    QDateTime composed_time;
    QDateTime sent_time;
    QDateTime received_time;
    QByteArray content;
    QByteArray sender;
    Encoding encoding = Encoding::US_ACSII;
    QByteArray signature;

    void init();
    void sign(const crypto::DsCert& cert);
    bool validate(const crypto::DsCert& cert) const;
};

}} // Namespaces

#endif // MESSAGE_H
