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
    std::pair<std::string, std::string> _chain_lbl;
    prometheus::Registry& _registry;
    Metrics(const std::string& chain, prometheus::Registry& registry);
};

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