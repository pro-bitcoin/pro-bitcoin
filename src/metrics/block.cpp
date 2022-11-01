#include <logging.h>
#include <metrics/block.h>
#include <prometheus/histogram.h>

namespace metrics {
std::unique_ptr<BlockMetrics> BlockMetrics::make(const std::string& chain, prometheus::Registry& registry, bool noop)
{
    LogPrint(BCLog::METRICS, "creating TxMetrics on %s\n", chain);
    return noop ? std::make_unique<BlockMetrics>() : std::make_unique<BlockMetricsImpl>(chain, registry);
}

BlockMetricsImpl::BlockMetricsImpl(const std::string& chain, prometheus::Registry& registry) : Metrics(chain, registry)
{
    auto& family = FamilyGauge("block_tip");
    auto& family_block_connect_avg = FamilyGauge("block_connect_avg");
    auto& family_block_connect_tip = FamilyGauge("block_connect_tip");
    for (const auto& type : _block_types) {
        _block_tip_gauge.insert({type, &family.Add({{"type", type}})});
    }
    for (const auto& op : _block_operations) {
        _block_connect_avg.insert({op, &family_block_connect_avg.Add({{"operation", op}})});
        _block_connect_tip.insert({op, &family_block_connect_tip.Add({{"operation", op}})});
    }
}

void BlockMetricsImpl::setTip(const std::string& type, double amt)
{
    auto found = this->_block_tip_gauge.find(type);
    if (found == this->_block_tip_gauge.end()) {
        return;
    }
    found->second->Set(amt);
}

void BlockMetricsImpl::setConnect(const std::string& type, double current, double avg)
{
    auto found = this->_block_connect_avg.find(type);
    if (found != this->_block_connect_avg.end()) {
        found->second->Set(avg);
    }
    auto foundTip = this->_block_connect_tip.find(type);
    if (foundTip != this->_block_connect_tip.end()) {
        foundTip->second->Set(current);
    }
}

void BlockMetricsImpl::Size(size_t amt)
{
    this->setTip("size", (double)amt);
}

void BlockMetricsImpl::SizeWitness(size_t amt)
{
    this->setTip("size-witness", (double)amt);
}

void BlockMetricsImpl::Height(int amt)
{
    this->setTip("height", (double)amt);
}

void BlockMetricsImpl::Weight(size_t amt)
{
    this->setTip("weight", (double)amt);
}

void BlockMetricsImpl::Version(double amt)
{
    this->setTip("version", (double)amt);
}

void BlockMetricsImpl::Transactions(size_t amt)
{
    this->setTip("transactions", (double)amt);
    auto now = std::time(nullptr);
    this->setTip("time", (double)now);
}

void BlockMetricsImpl::SigOps(int64_t amt)
{
    this->setTip("sigops", (double)amt);
}

void BlockMetricsImpl::HeaderTime(int64_t amt)
{
    this->setTip("header-time", (double)amt);
}

void BlockMetricsImpl::Reward(int64_t amt)
{
    this->setTip("reward", (double)amt);
}

void BlockMetricsImpl::Fees(int64_t amt)
{
    this->setTip("fees", (double)amt);
}

void BlockMetricsImpl::ValueOut(double amt)
{
    this->setTip("valueout", amt);
}
void BlockMetricsImpl::TimeDeltaPrev(int64_t amt)
{
    this->setTip("time-delta-prev", double(amt));
}
void BlockMetricsImpl::ConnectLoadBlockDisk(int64_t current, double avg)
{
    this->setConnect("load", current, avg);
}
void BlockMetricsImpl::ConnectBlock(int64_t current, double avg)
{
    this->setConnect("connect", current, avg);
}
void BlockMetricsImpl::ConnectFlushView(int64_t current, double avg)
{
    this->setConnect("flush-view", current, avg);
}
void BlockMetricsImpl::ConnectFlushDisk(int64_t current, double avg)
{
    this->setConnect("flush-disk", current, avg);
}
void BlockMetricsImpl::ConnectUpdateTip(int64_t current, double avg)
{
    this->setConnect("update-tip", current, avg);
}
void BlockMetricsImpl::ConnectForkCheck(int64_t current, double avg)
{
    this->setConnect("fork-check", current, avg);
}
void BlockMetricsImpl::ConnectUpdateIndex(int64_t current, double avg)
{
    this->setConnect("update-index", current, avg);
}
void BlockMetricsImpl::ConnectTotal(int64_t current, double avg)
{
    this->setConnect("connect-total", current, avg);
}
void BlockMetricsImpl::ConnectTransactionsCheck(int64_t current, double avg)
{
    this->setConnect("transactions-check", current, avg);
}
} // namespace metrics