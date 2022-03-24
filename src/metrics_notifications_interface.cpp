#include <consensus/validation.h>
#include <logging.h>
#include <metrics_notifications_interface.h>
#include <version.h>

namespace metrics {
MetricsNotificationsInterface::MetricsNotificationsInterface(BlockMetrics& blockMetrics, MemPoolMetrics& mempoolMetrics, ChainstateManager& chainman) : _blockMetrics(blockMetrics), _memPoolMetrics(mempoolMetrics), _chainman(chainman) {}

void MetricsNotificationsInterface::UpdatedBlockTip(const CBlockIndex* pindexNew, const CBlockIndex* pindexFork, bool fInitialDownload)
{
}

void MetricsNotificationsInterface::TransactionAddedToMempool(const CTransactionRef& tx, uint64_t mempool_sequence)
{
    if (tx->IsNull()) {
        return;
    }
    _memPoolMetrics.Incoming(tx->vin.size(), tx->vout.size(), tx->GetTotalSize(), tx->GetValueOut());
}

void MetricsNotificationsInterface::TransactionRemovedFromMempool(const CTransactionRef& tx, MemPoolRemovalReason reason,
                                                                  uint64_t mempool_sequence)
{
    _memPoolMetrics.Removed(static_cast<size_t>(reason));
}

void MetricsNotificationsInterface::BlockConnected(const std::shared_ptr<const CBlock>& block, const CBlockIndex* pindex)
{
    if (_chainman.ActiveChainstate().IsInitialBlockDownload()) {
        return;
    }
    _blockMetrics.Size(::GetSerializeSize(block, PROTOCOL_VERSION | SERIALIZE_TRANSACTION_NO_WITNESS));
    _blockMetrics.SizeWitness(::GetSerializeSize(block, PROTOCOL_VERSION));
    _blockMetrics.Weight(::GetBlockWeight(*block));
    _blockMetrics.Transactions(pindex->nTx);
    _blockMetrics.Height(pindex->nHeight);
    _blockMetrics.HeaderTime(pindex->GetBlockHeader().GetBlockTime());
    _blockMetrics.Version(pindex->nVersion);
    CAmount nValueOut{0};
    for (auto& tx : block->vtx) {
        if (tx->IsCoinBase()) {
            _blockMetrics.Reward(tx->GetValueOut());
        } else {
            nValueOut += tx->GetValueOut();
        }
    }
    _blockMetrics.ValueOut(nValueOut);
    _blockMetrics.TimeDeltaPrev(pindex->GetBlockTime() - pindex->pprev->GetMedianTimePast());
    LogPrint(BCLog::METRICS, "BlockConnected at %d\n", pindex->nHeight);
}

void MetricsNotificationsInterface::BlockDisconnected(const std::shared_ptr<const CBlock>& block, const CBlockIndex* pindex)
{
}
void MetricsNotificationsInterface::ChainStateFlushed(const CBlockLocator& locator)
{
}
void MetricsNotificationsInterface::BlockChecked(const CBlock& block, const BlockValidationState& state)
{
}

} // namespace metrics