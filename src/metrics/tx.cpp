#include <algorithm>
#include <logging.h>
#include <metrics/metrics.h>

namespace metrics {
std::unique_ptr<TxMetrics> TxMetrics::make(const std::string& chain, prometheus::Registry& registry, bool noop)
{
    LogPrint(BCLog::METRICS, "creaing TxMetrics on %s\n", chain);
    return noop ? std::make_unique<TxMetrics>() : std::make_unique<TxMetricsImpl>(chain, registry);
}

TxMetricsImpl::TxMetricsImpl(const std::string& chain, prometheus::Registry& registry) : Metrics(chain, registry)
{
    auto& single_transaction_family = FamilyCounter("transaction");
    auto& check_bucket_family = FamilyHistory("transactions_check");
    auto& check_avg_family = FamilyGauge("transactions_check_avg");

    _check_buckets = &check_bucket_family.Add({{"method", "ConnectBlock"}}, prometheus::Histogram::BucketBoundaries{500000, 1000000, 2000000, 4000000, 8000000});
    _check_avg = &check_avg_family.Add({{"method", "ConnectBlock"}});
    auto quantiles = prometheus::Summary::Quantiles{
        {0.50, 0.001},
        {0.75, 0.001},
        {0.90, 0.001},
        {0.99, 0.001}};
    auto window = std::chrono::seconds{120};
    _inputs_timer = &FamilySummary("tx_inputs").Add({}, quantiles, window);
    _orphan_remove_counter = &single_transaction_family.Add({{"result", "orphan-add"}});
    _orphan_add_counter = &single_transaction_family.Add({{"result", "orphan-remove"}});
    std::vector<std::string> reasons = {
        "bad-txns-vin-empty", "bad-txns-vout-empty", "bad-txns-oversize",
        "bad-txns-vout-negative", "bad-txns-vout-toolarge", "bad-txns-txouttotal-toolarge",
        "bad-txns-inputs-duplicate", "bad-cb-length", "bad-txns-prevout-null",
        "coinbase", "not-standard", "tx-size-small", "non-final", "txn-already-in-mempool", "txn-same-nonwitness-data-in-mempool",
        "bip125-replacement-disallowed", "txn-mempool-conflict", "txn-already-known", "bad-txns-inputs-missingorspent",
        "non-BIP68-final", "bad-txns-nonstandard-inputs", "bad-witness-nonstandard", "bad-txns-too-many-sigops",
        "too-long-mempool-chain", "bad-txns-spends-conflicting-tx", "insufficient fee", "too many potential replacements",
        "replacement-adds-unconfirmed", "mempool full", "unknown"};
    for (const auto& item : reasons) {
         std::string lbl = item;
         std::replace(lbl.begin(), lbl.end(), ' ', '-');
        _single_transaction_counter.insert({item, &single_transaction_family.Add({{"result", "rejected"}, {"reason", lbl}})});
    }
    _cache_gauge = &FamilyGauge("tx_cache").Add({});
}

void TxMetricsImpl::IncInvalid(const std::string& reason)
{
    auto found = this->_single_transaction_counter.find(reason);
    if (found == this->_single_transaction_counter.end()) {
        this->_single_transaction_counter.find("unknown")->second->Increment();
        return;
    }
    found->second->Increment();
}

void TxMetricsImpl::CacheSize(double amt)
{
    _cache_gauge->Set(amt);
}
void TxMetricsImpl::InputTime(const double t)
{
    _inputs_timer->Observe(t);
}
void TxMetricsImpl::IncOrphanAdd()
{
    _orphan_add_counter->Increment();
}
void TxMetricsImpl::IncOrphanRemove()
{
    _orphan_remove_counter->Increment();
}
void TxMetricsImpl::IncTransactions(const std::string& type, long amt)
{
}
void TxMetricsImpl::TransactionCheck(int64_t current, double amt)
{
    _check_buckets->Observe((double)current);
    _check_avg->Set(amt);
}
} // namespace metrics