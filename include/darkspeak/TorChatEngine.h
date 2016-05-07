#pragma once

#include <future>

#include <boost/property_tree/ptree.hpp>

#include "war_error_handling.h"

#include "darkspeak/ImProtocol.h"

namespace darkspeak {
namespace impl {

class TorChatPeer;

/*! Tor Chat Protocol implementation
 *
 * One instance will handle one hidden service (TC id)
 */
class TorChatEngine : public ImProtocol
{
public:
    struct Request;
    struct ExceptionNotAllowed : public war::ExceptionBase {};
    struct ExceptionDisconnectNow : public war::ExceptionBase {};

    enum class Direction {
        INCOMING,
        OUTGOING
    };

    struct Command {
        using cmd_fn_t =  std::function<void (const Request&)>;
        enum class Verb {
            UNKNOWN,
            ADD_ME,
            CLIENT,
            FILEDATA,
            FILEDATA_ERROR,
            FILEDATA_OK,
            FILENAME,
            FILE_STOP_SENDING,
            MESSAGE,
            PING,
            PONG,
            PROFILE_AVATAR,
            PROFILE_AVATAR_ALPHA,
            PROFILE_NAME,
            PROFILE_TEXT,
            REMOVE_ME,
            STATUS,
            VERSION,
        };

        enum class Valid {
            GREETING, // Initial connect / ping
            HANDSHAKE, // before pong
            AUTHENTICATED, // after pong
            ACCEPTED // when the upper layer have confirmed the buddy
        };

        Command(Verb ve,
                int a = 0,
                cmd_fn_t fn = nullptr,
                Valid va = Valid::AUTHENTICATED)
        : verb{ve}, args{a}, valid{va}, execute{fn} {}

        const Verb verb;
        const int args = 0;
        const Valid valid = Valid::AUTHENTICATED;
        const cmd_fn_t execute;
    };

    struct Request {
        const Command *cmd = nullptr;
        std::vector<std::string> params;
        std::string command_name;
        TorChatPeer *peer = nullptr;
        Direction direction = Direction::INCOMING;
        boost::asio::yield_context *yield = nullptr;
    };

    TorChatEngine(ImProtocol::get_pipeline_fn_t fn,
        const boost::property_tree::ptree& properties);

    ~TorChatEngine();

    // IM protocol implementation
    void Connect(Api::Buddy::ptr_t buddy) override;
    void SetInfo(const Api::Info& info) override;
    void SendMessage(Api::Buddy& buddy, const Message& msg) override;
    void SendFile(Api::Buddy& buddy, const File& file,
        FileMonitor::ptr_t monitor) override;
    void Disconnect(Api::Buddy& buddy) override;
    void SetMonitor(EventMonitor::ptr_t monitor) override;
    void Listen(boost::asio::ip::tcp::endpoint endpoint) override;
    void Shutdown() override;

private:
    void Listen_(boost::asio::ip::tcp::endpoint endpoint,
                 std::promise<void>& promise);

    void Accept(boost::asio::ip::tcp::endpoint endpoint,
        boost::asio::yield_context ctx);

    void ConnectToPeer(TorChatPeer& peer, war::Pipeline& pipeline,
                       boost::asio::yield_context& yield);

    void OnAccepted(war::Pipeline& pipeline,
                    std::shared_ptr< boost::asio::ip::tcp::socket > socket,
                    boost::asio::yield_context yield);

    std::vector<EventMonitor::ptr_t> GetMonitors();

    bool VerifyPing(boost::string_ref line, std::string& id,
                    std::string& cookie);

    void ProcessRequests(
        std::weak_ptr<TorChatPeer> weak_peer,
        Direction direction,
        boost::asio::yield_context& yield);

    // For spawn
    void StartProcessingRequests(
        std::weak_ptr<TorChatPeer> weak_peer,
        Direction direction,
        boost::asio::yield_context yield);

    // For spawn
    void StartConnectToPeer(std::string& peer_id,
                            war::Pipeline& pipeline,
                            boost::asio::yield_context yield);

    std::shared_ptr<TorChatPeer> GetPeer(const std::string& id);

    void OnAddMe(const Request& req);
    void OnClient(const Request& req);
    void OnFileData(const Request& req);
    void OnFileDataError(const Request& req);
    void OnFileDataOk(const Request& req);
    void OnFilename(const Request& req);
    void OnFileStopSending(const Request& req);
    void OnPing(const Request& req);
    void OnPong(const Request& req);
    void OnProfileAvatar(const Request& req);
    void OnProfileName(const Request& req);
    void OnProfileText(const Request& req);
    void OnRemoveMe(const Request& req);
    void OnStatus(const Request& req);
    void OnVersion(const Request& req);

    bool DoSendPing(TorChatPeer& peer, boost::asio::yield_context& yield);
    bool DoSendPong(TorChatPeer& peer, boost::asio::yield_context& yield);
    bool DoSendStatus(TorChatPeer& peer, boost::asio::yield_context& yield);
    bool DoSend(const std::string& command,
                std::initializer_list<std::string> args,
                TorChatPeer& peer, boost::asio::yield_context& yield);
    static const std::string& ToString(Api::Status status);

    boost::asio::ip::tcp::endpoint GetTorEndpoint() const;

    Request Parse(boost::string_ref request) const;

    std::vector<std::weak_ptr<EventMonitor>> event_monitors_;
    std::unique_ptr<boost::asio::ip::tcp::acceptor> acceptor_;
    war::Pipeline& pipeline_;
    boost::property_tree::ptree config_;
    std::size_t connect_timeout_ = 1000 * 60 * 2; // 2 minutes in milliseconds
    std::map<std::string, std::shared_ptr<TorChatPeer>> peers_;
    std::map<std::string, Command> commands_;

    Api::Info local_info_;
};

} // impl
} // darkspeak

