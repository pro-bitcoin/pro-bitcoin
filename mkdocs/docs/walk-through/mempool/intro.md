---
title: Mempool
description: TBD
---

# The Waiting Room

The [Mempool](https://medium.com/coinmonks/bitcoin-mempool-simply-explained-7f76be235e85) is the waiting room for all unconfirmed transactions.
Every Full Node on the network has access to the mempool. A Full Node is any node that stores all transactions and blocks of the Bitcoin Blockchain.

<iframe src="https://grafana.pro-bitcoin.io/d-solo/LfZliOHnk/mempool?orgId=1&refresh=5m&panelId=10" width="100%" height="150"></iframe>

In my opinion, the mempool is the most fascinating and complicated part of bitcoin. Misuse can lead to DDOS or double-spends.

## Configuration


## CTXMemPool

 [CTxMemPool]() stores valid-according-to-the-current-best-chain transactions that may be included in the next block.

 Transactions are added when they are seen on the network, but not all transactions seen are added to the pool. For
example, the following new transactions will not be added to the mempool:

 * a transaction which doesn't meet the minimum fee requirements.
 * a new transaction that double-spends an input of a transaction already in
 * the pool where the new transaction does not meet the Replace-By-Fee
 * requirements as defined in [BIP 125](https://github.com/bitcoin/bips/blob/master/bip-0125.mediawiki).
 * a non-standard transaction.

 ---

 ### [CTxMemPool::mapTx]() and [CTxMemPoolEntry]() bookkeeping

 mapTx is a [boost::multi_index]() that sorts the mempool on 5 criteria:

 * transaction hash (txid)
 * witness-transaction hash (wtxid)
 * descendant feerate [we use max(feerate of tx, feerate of tx with all descendants)]
 * time in mempool
 * ancestor feerate [we use min(feerate of tx, feerate of tx with all unconfirmed ancestors)]

 Note: the term "descendant" refers to in-mempool transactions that depend on this one, while "ancestor" refers to in-mempool transactions that a given transaction depends on.


 Usually when a new transaction is added to the mempool, it has no in-mempool
 children (because any such children would be an orphan).  So in
 addUnchecked(), we:

 * update a new entry's setMemPoolParents to include all in-mempool parents
 * update the new entry's direct parents to include the new tx as a child
 * update all ancestors of the transaction to include the new tx's size/fee

  When a transaction is removed from the mempool, we must:

 * update all in-mempool parents to not track the tx in setMemPoolChildren
 * update all ancestors to not include the tx's size/fees in descendant state
 * update all in-mempool children to not include it as a parent

 These happen in [UpdateForRemoveFromMempool]()
