#ifndef BITCOIN_METRICS_CONTAINER_H
#define BITCOIN_METRICS_CONTAINER_H

#include <cassert>
#include <metrics/block.h>
#include <metrics/mempool.h>
#include <metrics/metrics.h>
#include <metrics/net.h>
#include <metrics/peer.h>
#include <metrics/rpc.h>
#include <metrics/tx.h>

namespace metrics {
static auto prom_registry = std::make_shared<prometheus::Registry>();
static std::shared_ptr<prometheus::Exposer> exposer;

class Container
{
protected:
    std::unique_ptr<PeerMetrics> _peerMetrics;
    std::unique_ptr<NetMetrics> _netMetrics;
    std::unique_ptr<TxMetrics> _txMetrics;
    std::unique_ptr<BlockMetrics> _blocks_metrics;
    std::unique_ptr<MemPoolMetrics> _mempool_metrics;
    std::unique_ptr<ConfigMetrics> _cfg_metrics;
    std::unique_ptr<RpcMetrics> _rpc_metrics;
    std::atomic<bool> _init{false};

public:
    Container();
    ~Container() {}
    void Init(const std::string& chain, bool noop);
    PeerMetrics& Peer();
    NetMetrics& Net();
    TxMetrics& Tx();
    BlockMetrics& Block();
    //UtxoMetrics& Utxo();
    MemPoolMetrics& MemPool();
    ConfigMetrics& Config();
    RpcMetrics& Rpc();
};

void Init(const std::string& bind, const std::string& chain, bool noop = false);
Container* Instance();
prometheus::Registry& Registry();
} // namespace metrics

#endif // BITCOIN_METRICS_CONTAINER_H
