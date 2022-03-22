#ifndef BITCOIN_METRICS_METRICS_H
#define BITCOIN_METRICS_METRICS_H
#include <optional>
#include <prometheus/counter.h>
#include <prometheus/exposer.h>
#include <prometheus/gauge.h>
#include <prometheus/histogram.h>
#include <prometheus/registry.h>
#include <prometheus/summary.h>

namespace metrics {

enum NetDirection {
    RX,
    TX
};

enum NetConnectionType {
    TOTAL,
    SPV,
    FULL,
    INBOUND,
    OUTBOUND,
    IPV4,
    IPV6,
    TOR,
    I2P,
};

enum MemPoolType {
    POOL_SIZE,
    POOL_TRANSACTION_BYTES,
    POOl_USAGE,
    POOL_FEE,
    POOL_UPDATE,
};

// see protocol.h
static std::vector<std::string> netMsgTypes{
    "version",
    "verack",
    "addr",
    "addrv2",
    "sendaddrv2",
    "inv",
    "getdata",
    "merkleblock",
    "getblocks",
    "getheaders",
    "tx",
    "headers",
    "block",
    "getaddr",
    "mempool",
    "ping",
    "pong",
    "notfound",
    "filterload",
    "filteradd",
    "filterclear",
    "sendheaders",
    "feefilter",
    "sendcmpct",
    "cmpctblock",
    "getblocktxn",
    "blocktxn",
    "getcfilters",
    "cfilter",
    "getcfheaders",
    "cfheaders",
    "getcfcheckpt",
    "cfcheckpt",
    "wtxidrelay",
};

class Metrics
{
public:
    prometheus::Family<prometheus::Summary>& FamilySummary(const std::string&);
    prometheus::Family<prometheus::Counter>& FamilyCounter(const std::string&, const std::map<std::string, std::string>& labels = {});
    prometheus::Family<prometheus::Gauge>& FamilyGauge(const std::string&, const std::map<std::string, std::string>& labels = {});
    prometheus::Family<prometheus::Histogram>& FamilyHistory(const std::string& name, const std::map<std::string, std::string>& labels = {});
    static inline std::pair<std::string, std::string> GAUGE_LABEL = {"mtype", "gauge"};
    static inline std::pair<std::string, std::string> COUNTER_LABEL = {"mtype", "counter"};
    static inline std::pair<std::string, std::string> SUMMARY_LABEL = {"mtype", "summary"};
    static inline std::pair<std::string, std::string> HISTOGRAM_LABEL = {"mtype", "histogram"};
    std::pair<std::string, std::string> _chain_lbl;
    prometheus::Registry& _registry;
    Metrics(const std::string& chain, prometheus::Registry& registry);
};

struct BlockTimerOp {
    BlockTimerOp(std::string name, prometheus::Histogram::BucketBoundaries buckets);
    std::string name() const;
    prometheus::Histogram::BucketBoundaries buckets() const;
    std::string _name;
    prometheus::Histogram::BucketBoundaries _buckets;
};

static BlockTimerOp BLOCK_LOAD = BlockTimerOp("load", prometheus::Histogram::BucketBoundaries{500, 5000, 50000, 100000});
static BlockTimerOp BLOCK_CONNECT = BlockTimerOp("connect", prometheus::Histogram::BucketBoundaries{500000, 1000000, 2000000, 4000000, 8000000});
static BlockTimerOp BLOCK_FLUSH_VIEW = BlockTimerOp("flush-view", prometheus::Histogram::BucketBoundaries{100, 200, 500, 1000});
static BlockTimerOp BLOCK_FLUSH_DISK = BlockTimerOp("flush-disk", prometheus::Histogram::BucketBoundaries{10, 25, 75, 100});
static BlockTimerOp BLOCK_UPDATE_TIP = BlockTimerOp("update-tip", prometheus::Histogram::BucketBoundaries{25, 75, 100, 200});
static BlockTimerOp BLOCK_FORK_CHK = BlockTimerOp("fork-check", prometheus::Histogram::BucketBoundaries{10000, 50000, 100000, 200000});
static BlockTimerOp BLOCK_UPDATE_INDEX = BlockTimerOp("update-index", prometheus::Histogram::BucketBoundaries{5000, 10000, 15000, 20000});

class ConfigMetrics : Metrics
{
private:
    prometheus::Family<prometheus::Gauge>* _config;
    prometheus::Gauge* _ibd;
    std::vector<std::string> _categories{
        "options",
        "connection",
        "wallet",
        "wallet-debug-test",
        "zmq",
        "debug",
        "chainparams",
        "node-relay",
        "block",
        "rpc",
        "gui",
        "commands",
        "register-commands",
    };

public:
    explicit ConfigMetrics(const std::string& chain, prometheus::Registry& registry);
    void Set(const std::string& cfg, size_t category, const std::string type, int64_t value);
    void SetU(const std::string& cfg, size_t category, const std::string type, uint64_t value);
    void SetFlag(const std::string& cfg, size_t category, bool value);
    void SetIBD(const bool value);
};







} // namespace metrics

#endif // BITCOIN_METRICS_METRICS_H