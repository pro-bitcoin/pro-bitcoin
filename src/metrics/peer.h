#ifndef BITCOIN_METRICS_PEER_H
#define BITCOIN_METRICS_PEER_H

#include <metrics/metrics.h>

namespace metrics {
class PeerMetrics
{
public:
    virtual ~PeerMetrics() {}
    static std::unique_ptr<PeerMetrics> make(const std::string& chain, prometheus::Registry& registry, bool noop);
    virtual void ProcessMsgType(const std::string& msg_type, long amt){};
    virtual void IncTxValidationResult(int state){};
    virtual void IncDiscourage(){};
    virtual void IncMisbehaveAmount(int amt){};
    virtual void ConnectionType(int type, uint amt){};
    virtual void Permission(const std::string& permission, uint amt){};
    virtual void Known(size_t amt){};
    virtual void SendMessageTime(long amt){};
    virtual void PushMsgType(const std::string& msg_type){};
    virtual void Banned(unsigned long amt){};
};
class PeerMetricsImpl : virtual public PeerMetrics, Metrics
{
protected:
    void initConnections();
    std::vector<prometheus::Gauge*> _connections_gauge;
    std::map<const std::string, prometheus::Histogram*> _process_msg_timer;
    std::map<const std::string, prometheus::Counter*> _push_msg_counter;
    std::vector<prometheus::Counter*> _tx_validations;
    prometheus::Counter* _bad_peer_counter;
    prometheus::Counter* _misbehave_counter;
    prometheus::Gauge* _known_peers_gauge;
    prometheus::Gauge* _banned_gauge;
    prometheus::Summary* _send_msg_timer;
    std::map<const std::string, prometheus::Gauge*> _permission_gauge;

public:
    ~PeerMetricsImpl(){};
    explicit PeerMetricsImpl(const std::string& chain, prometheus::Registry& registry);
    void ProcessMsgType(const std::string& msg_type, long amt) override;
    void IncTxValidationResult(int state) override;
    void IncDiscourage() override;
    void IncMisbehaveAmount(int amt) override;
    void ConnectionType(int type, uint amt) override;
    void Permission(const std::string& permission, uint amt) override;
    void Known(size_t amt) override;
    void SendMessageTime(long amt) override;
    void PushMsgType(const std::string& msg_type) override;
    void Banned(unsigned long amt) override;
};
} // namespace metrics
#endif // BITCOIN_METRICS_PEER_H
