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

    virtual void TipLoadBlockDisk(int64_t current, double avg){};
    virtual void TipConnectBlock(int64_t current, double avg){};
    virtual void TipFlushView(int64_t current, double avg){};
    virtual void TipFlushDisk(int64_t current, double avg){};
    virtual void TipUpdate(int64_t current, double avg){};

    virtual void ForkCheck(int64_t current, double avg){};
    virtual void UpdateIndex(int64_t current, double avg){};
};

class BlockMetricsImpl : virtual public BlockMetrics, Metrics
{
protected:
    void set(const std::string& type, double amt);
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
            "difficulty",
            "valueout"};
    std::map<const std::string, prometheus::Gauge*> _block_tip_gauge;
    std::vector<prometheus::Gauge*> _block_connect_avg;
    std::vector<prometheus::Gauge*> _block_connect_tip;

public:
    ~BlockMetricsImpl(){};
    explicit BlockMetricsImpl(const std::string& chain, prometheus::Registry& registry);
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
    void Difficulty(double amt) override;
    virtual void ValueOut(double amt) override;

    void TipLoadBlockDisk(int64_t current, double avg) override;
    void TipConnectBlock(int64_t current, double avg) override;
    void TipFlushView(int64_t current, double avg) override;
    void TipFlushDisk(int64_t current, double avg) override;
    void TipUpdate(int64_t current, double avg) override;

    void ForkCheck(int64_t current, double avg) override;
    void UpdateIndex(int64_t current, double avg) override;
};
}
#endif // BITCOIN_METRICS_BLOCK_H
