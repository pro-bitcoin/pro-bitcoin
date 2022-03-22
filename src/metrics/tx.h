#ifndef BITCOIN_METRICS_TX_H
#define BITCOIN_METRICS_TX_H

#include <metrics/metrics.h>

namespace metrics {
class TxMetrics
{
public:
    virtual ~TxMetrics() {}
    static std::unique_ptr<TxMetrics> make(const std::string& chain, prometheus::Registry& registry, bool noop);
    virtual void IncInvalid(const std::string& reason){};
    virtual void InputTime(double t){};
    virtual void IncOrphanAdd(){};
    virtual void IncOrphanRemove(){};
    virtual void CacheSize(double amt){};
    virtual void IncTransactions(const std::string& type, long amt){};
    virtual void TransactionCheck(int64_t current, double amt){};
};

class TxMetricsImpl : virtual public TxMetrics, Metrics
{
protected:
    std::map<const std::string, prometheus::Counter*> _single_transaction_counter;
    prometheus::Summary* _inputs_timer;
    prometheus::Counter* _orphan_remove_counter;
    prometheus::Counter* _orphan_add_counter;
    prometheus::Counter* _accepted_counter;
    prometheus::Gauge* _cache_gauge;
    prometheus::Histogram* _check_buckets;
    prometheus::Gauge* _check_avg;

public:
    ~TxMetricsImpl() {}
    explicit TxMetricsImpl(const std::string& chain, prometheus::Registry& registry);
    void IncInvalid(const std::string& reason) override;
    void InputTime(double t) override;
    void IncOrphanAdd() override;
    void IncOrphanRemove() override;
    void CacheSize(double amt) override;
    void IncTransactions(const std::string& type, long amt) override;
    void TransactionCheck(int64_t current, double amt) override;
};


} // namespace metrics
#endif // BITCOIN_METRICS_TX_H
