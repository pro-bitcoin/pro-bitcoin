#ifndef BITCOIN_METRICS_NET_H
#define BITCOIN_METRICS_NET_H

#include <metrics/metrics.h>

namespace metrics {
class NetMetrics
{
public:
    virtual ~NetMetrics() {}
    static std::unique_ptr<NetMetrics> make(const std::string& chain, prometheus::Registry& registry, bool noop);
    virtual void IncConnection(const std::string& type){};
    virtual void ConnectionGauge(NetConnectionType netConnection, uint amt){};
    virtual void BandwidthGauge(NetDirection direction, const std::string& msg, uint64_t amt){};
    virtual void PingTime(long amt){};
    virtual void IncPingProblem(){};
    virtual void MaxOutbound(int64_t amt){};
    virtual void MaxOutboundStartTime(int64_t amt){};
};
class NetMetricsImpl : virtual public NetMetrics, Metrics
{
protected:
    void initBandwidth();
    std::map<NetConnectionType, prometheus::Gauge*> _connections_gauge;
    std::map<const std::string, prometheus::Gauge*> _bandwidth_gauge_tx;
    std::map<const std::string, prometheus::Gauge*> _bandwidth_gauge_rx;
    prometheus::Summary* _ping_timer;
    prometheus::Counter* _ping_problem_counter;
    std::map<const std::string, prometheus::Counter*> _connection_counter;
    prometheus::Gauge* _max_outbound_gauge;
    prometheus::Gauge* _max_outbound_start_gauge;

public:
    ~NetMetricsImpl(){};
    explicit NetMetricsImpl(const std::string& chain, prometheus::Registry& registry);
    void IncConnection(const std::string& type) override;
    void ConnectionGauge(NetConnectionType netConnection, uint amt) override;
    void BandwidthGauge(NetDirection direction, const std::string& msg, uint64_t amt) override;
    void PingTime(long amt) override;
    void IncPingProblem() override;
    void MaxOutbound(int64_t amt) override;
    void MaxOutboundStartTime(int64_t amt) override;
};

} // namespace metrics

#endif // BITCOIN_METRICS_NET_H