#include <logging.h>
#include <metrics/block.h>
#include <prometheus/histogram.h>

namespace metrics {
#define P_BLK_LOAD 0
#define P_BLK_CONNECT 1
#define P_BLK_FLUSH_VIEW 2
#define P_BLK_FLUSH_DISK 3
#define P_BLK_UPDATE_TIP 4
#define P_BLK_FORK_CHECK 5
#define P_BLK_INDEX 6

std::unique_ptr<BlockMetrics> BlockMetrics::make(const std::string& chain, prometheus::Registry& registry, bool noop)
{
    LogPrint(BCLog::METRICS, "creaing TxMetrics on %s\n", chain);
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
    _block_connect_avg = {
        &family_block_connect_avg.Add({{"operation", BLOCK_LOAD.name()}}),
        &family_block_connect_avg.Add({{"operation", BLOCK_CONNECT.name()}}),
        &family_block_connect_avg.Add({{"operation", BLOCK_FLUSH_VIEW.name()}}),
        &family_block_connect_avg.Add({{"operation", BLOCK_FLUSH_DISK.name()}}),
        &family_block_connect_avg.Add({{"operation", BLOCK_UPDATE_TIP.name()}}),
        &family_block_connect_avg.Add({{"operation", BLOCK_FORK_CHK.name()}}),
        &family_block_connect_avg.Add({{"operation", BLOCK_UPDATE_INDEX.name()}}),
    };
    _block_connect_tip = {
        &family_block_connect_tip.Add({{"operation", BLOCK_LOAD.name()}}),
        &family_block_connect_tip.Add({{"operation", BLOCK_CONNECT.name()}}),
        &family_block_connect_tip.Add({{"operation", BLOCK_FLUSH_VIEW.name()}}),
        &family_block_connect_tip.Add({{"operation", BLOCK_FLUSH_DISK.name()}}),
        &family_block_connect_tip.Add({{"operation", BLOCK_UPDATE_TIP.name()}}),
        &family_block_connect_tip.Add({{"operation", BLOCK_FORK_CHK.name()}}),
        &family_block_connect_tip.Add({{"operation", BLOCK_UPDATE_INDEX.name()}}),
    };
}

void BlockMetricsImpl::set(const std::string& type, double amt)
{
    auto found = this->_block_tip_gauge.find(type);
    if (found == this->_block_tip_gauge.end()) {
        return;
    }
    found->second->Set(amt);
}

void BlockMetricsImpl::Size(size_t amt)
{
    this->set("size", (double)amt);
}

void BlockMetricsImpl::SizeWitness(size_t amt)
{
    this->set("size-witness", (double)amt);
}

void BlockMetricsImpl::Height(int amt)
{
    this->set("height", (double)amt);
}

void BlockMetricsImpl::Weight(size_t amt)
{
    this->set("weight", (double)amt);
}

void BlockMetricsImpl::Version(double amt)
{
    this->set("version", (double)amt);
}

void BlockMetricsImpl::Transactions(size_t amt)
{
    this->set("transactions", (double)amt);
    auto now = std::time(nullptr);
    this->set("time", (double)now);
}

void BlockMetricsImpl::SigOps(int64_t amt)
{
    this->set("sigops", (double)amt);
}

void BlockMetricsImpl::HeaderTime(int64_t amt)
{
    this->set("header-time", (double)amt);
}

void BlockMetricsImpl::Reward(int64_t amt)
{
    this->set("reward", (double)amt);
}

void BlockMetricsImpl::Fees(int64_t amt)
{
    this->set("fees", (double)amt);
}

void BlockMetricsImpl::Difficulty(double amt)
{
    this->set("difficulty", amt);
}

void BlockMetricsImpl::ValueOut(double amt)
{
    this->set("valueout", amt);
}

void BlockMetricsImpl::TipLoadBlockDisk(int64_t current, double avg)
{
    _block_connect_avg[P_BLK_LOAD]->Set(avg);
    _block_connect_tip[P_BLK_LOAD]->Set(current);
}
void BlockMetricsImpl::TipConnectBlock(int64_t current, double avg)
{
    _block_connect_avg[P_BLK_CONNECT]->Set(avg);
    _block_connect_tip[P_BLK_CONNECT]->Set(current);
}
void BlockMetricsImpl::TipFlushView(int64_t current, double avg)
{
    _block_connect_avg[P_BLK_FLUSH_VIEW]->Set(avg);
    _block_connect_tip[P_BLK_FLUSH_VIEW]->Set(current);
}
void BlockMetricsImpl::TipFlushDisk(int64_t current, double avg)
{
    _block_connect_avg[P_BLK_FLUSH_DISK]->Set(avg);
    _block_connect_tip[P_BLK_FLUSH_DISK]->Set(current);
}
void BlockMetricsImpl::TipUpdate(int64_t current, double avg)
{
    _block_connect_avg[P_BLK_UPDATE_TIP]->Set(avg);
    _block_connect_tip[P_BLK_UPDATE_TIP]->Set(current);
}
void BlockMetricsImpl::ForkCheck(int64_t current, double avg)
{
    _block_connect_avg[P_BLK_FORK_CHECK]->Set(avg);
    _block_connect_tip[P_BLK_FORK_CHECK]->Set(current);
}
void BlockMetricsImpl::UpdateIndex(int64_t current, double avg)
{
    _block_connect_avg[P_BLK_INDEX]->Set(avg);
    _block_connect_tip[P_BLK_INDEX]->Set(current);
}
} // namespace metrics