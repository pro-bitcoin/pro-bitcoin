#include <cassert>
#include <logging.h>
#include <metrics/container.h>


namespace metrics {

PeerMetrics& Container::Peer() const
{
    assert(this->_peerMetrics);
    return *this->_peerMetrics;
}
NetMetrics& Container::Net() const
{
    assert(this->_netMetrics);
    return *this->_netMetrics;
}
TxMetrics& Container::Tx() const
{
    assert(this->_txMetrics);
    return *this->_txMetrics;
}
BlockMetrics& Container::Block() const
{
    assert(this->_blocks_metrics);
    return *this->_blocks_metrics;
}
MemPoolMetrics& Container::MemPool() const
{
    assert(this->_mempool_metrics);
    return *this->_mempool_metrics;
}
ConfigMetrics& Container::Config() const
{
    assert(this->_cfg_metrics);
    return *this->_cfg_metrics;
}

RpcMetrics& Container::Rpc() const
{
    assert(this->_rpc_metrics);
    return *this->_rpc_metrics;
}

void Container::Init(const std::string& chain, bool noop)
{
    LogPrint(BCLog::METRICS, "Init metrics on %s\n", chain);
    // check if already init
    if (_init.exchange(true)) {
        return;
    }

    _peerMetrics = PeerMetrics::make(chain, *prom_registry, noop);
    _netMetrics = NetMetrics::make(chain, *prom_registry, noop);
    _txMetrics = TxMetrics::make(chain, *prom_registry, noop);
    _blocks_metrics = BlockMetrics::make(chain, *prom_registry, noop);
    //_utxo_metrics =  std::make_unique<UtxoMetrics>(chain, *prom_registry);
    _mempool_metrics = MemPoolMetrics::make(chain, *prom_registry, noop);
    _cfg_metrics = std::make_unique<ConfigMetrics>(chain, *prom_registry);
    _rpc_metrics = RpcMetrics::make(chain, *prom_registry, noop);
}

Container& Container::Instance()
{
    static Container c;
    return c;
}

void Init(const std::string& bind, const std::string& chain, bool noop)
{
    if (!bind.empty() && !noop) {
        exposer = std::make_shared<prometheus::Exposer>(bind);
        exposer->RegisterCollectable(prom_registry);
    }
    Container::Instance().Init(chain, noop);
}

prometheus::Registry& Registry()
{
    return *prom_registry;
}
} // namespace metrics