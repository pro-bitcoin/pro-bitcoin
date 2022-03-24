#ifndef BITCOIN_METRICS_BLOCK_H
#define BITCOIN_METRICS_BLOCK_H
#include <metrics/metrics.h>

namespace metrics {
class BlockMetrics
{
public:
    static std::unique_ptr<BlockMetrics> make(const std::string& chain, prometheus::Registry& registry, bool noop);
    virtual ~BlockMetrics(){};
    virtual void Size(size_t amt){};
    virtual void SizeWitness(size_t amt){};
    virtual void Height(int amt){};
    virtual void Weight(size_t amt){};
    virtual void Version(double amt){};
    virtual void Transactions(size_t amt){};
    virtual void SigOps(int64_t amt) {}
    virtual void HeaderTime(int64_t amt) {}
    virtual void Reward(int64_t amt){};
    virtual void Fees(int64_t amt){};
    virtual void Difficulty(double amt){};
    virtual void ValueOut(double amt){};
    virtual void TimeDeltaPrev(int64_t amt){};

    virtual void ConnectLoadBlockDisk(int64_t current, double avg){};
    virtual void ConnectBlockTotal(int64_t current, double avg){};
    virtual void ConnectFlushView(int64_t current, double avg){};
    virtual void ConnectFlushDisk(int64_t current, double avg){};
    virtual void ConnectUpdate(int64_t current, double avg){};

    virtual void ConnectForkCheck(int64_t current, double avg){};
    virtual void ConnectUpdateIndex(int64_t current, double avg){};
};

class BlockMetricsImpl : virtual public BlockMetrics, Metrics
{
protected:
    void setTip(const std::string& type, double amt);
    void setConnect(const std::string& type, double current, double avg);
    std::vector<std::string> _block_types{
        "size",
        "size-witness",
        "weight",
        "height",
        "version",
        "transactions",
        "sigops",
        "time",
        "header-time",
        "fees",
        "reward",
        "valueout",
        "time-delta-prev"};
    std::vector<std::string> _block_operations{
        "load",
        "connect-total",
        "flush-view",
        "flush-disk",
        "update-tip",
        "fork-check",
        "update-index",
    };
    std::map<const std::string, prometheus::Gauge*> _block_tip_gauge;
    std::map<const std::string, prometheus::Gauge*> _block_connect_avg;
    std::map<const std::string, prometheus::Gauge*> _block_connect_tip;

public:
    ~BlockMetricsImpl(){};
    explicit BlockMetricsImpl(const std::string& chain, prometheus::Registry& registry);

    // TIP of blockchain
    void Size(size_t amt) override;
    void SizeWitness(size_t amt) override;
    void Height(int amt) override;
    void Weight(size_t amt) override;
    void Version(double amt) override;
    void Transactions(size_t amt) override;
    void SigOps(int64_t amt) override;
    void HeaderTime(int64_t amt) override;
    void Reward(int64_t amt) override;
    void Fees(int64_t amt) override;
    void ValueOut(double amt) override;
    void TimeDeltaPrev(int64_t amt) override;

    // Timers for connecting a block
    void ConnectLoadBlockDisk(int64_t current, double avg) override;
    void ConnectBlockTotal(int64_t current, double avg) override;
    void ConnectFlushView(int64_t current, double avg) override;
    void ConnectFlushDisk(int64_t current, double avg) override;
    void ConnectUpdate(int64_t current, double avg) override;
    void ConnectForkCheck(int64_t current, double avg) override;
    void ConnectUpdateIndex(int64_t current, double avg) override;
};
}
#endif // BITCOIN_METRICS_BLOCK_H
