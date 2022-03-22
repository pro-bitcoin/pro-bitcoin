#ifndef BITCOIN_METRICS_MEMPOOL_H
#define BITCOIN_METRICS_MEMPOOL_H

#include <metrics/metrics.h>

namespace metrics {

class MemPoolMetrics
{
public:
    virtual ~MemPoolMetrics() {}
    static std::unique_ptr<MemPoolMetrics> make(const std::string& chain, prometheus::Registry& registry, bool noop);
    virtual void AcceptTime(long amt){};
    virtual void Transactions(MemPoolType type, long amt){};
    virtual void Incoming(size_t in, size_t out, unsigned int byte_size, int64_t amt){};
    virtual void Removed(size_t reason){};
    virtual void Orphans(size_t map, size_t outpoint){};

    virtual std::optional<double> GetRemoved(size_t reason) { return std::nullopt; };
};
class MemPoolMetricsImpl : virtual public MemPoolMetrics, Metrics
{
protected:
    prometheus::Histogram* _accept_pool_timer;
    std::vector<prometheus::Gauge*> _mempool_gauge;
    prometheus::Counter* _incoming_counter;
    prometheus::Counter* _vin_incoming_counter;
    prometheus::Counter* _vout_incoming_counter;
    prometheus::Counter* _incoming_size_counter;
    prometheus::Counter* _incoming_amt_counter;
    std::vector<prometheus::Counter*> _removed_counter;
    prometheus::Counter* _removed_unknown_counter;
    prometheus::Gauge* _orphan_size_gauge;
    prometheus::Gauge* _orphan_outpoint_gauge;

public:
    explicit MemPoolMetricsImpl(const std::string& chain, prometheus::Registry& registry);
    ~MemPoolMetricsImpl(){};
    void AcceptTime(long amt) override;
    void Transactions(MemPoolType type, long amt) override;
    void Incoming(size_t in, size_t out, unsigned int byte_size, int64_t amt) override;
    void Removed(size_t reason) override;
    void Orphans(size_t map, size_t outpoint) override;

    std::optional<double> GetRemoved(size_t reason) override;
};


} // namespace metrics


#endif // BITCOIN_METRICS_MEMPOOL_H
