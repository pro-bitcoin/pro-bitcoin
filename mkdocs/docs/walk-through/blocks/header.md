---
title: Block Headers
description: TBD
hide:
  - toc
---

# Block Headers

## Introduction

The [CBlockHeader](https://doxygen.bitcoincore.org/class_c_block_header.html) class is a vital and efficient data structure within the Bitcoin Core node software. This class comprises a compact representation of a block's metadata, including its Merkle Root. The [Merkel Root](https://en.m.wikipedia.org/wiki/Merkle_tree) field allows field is essential for enabling Simplified Payment Verification ([SPV](https://btcinformation.org/en/developer-guide#simplified-payment-verification-spv)) nodes to verify transaction existence with minimal overhead.

When a new node joins the network, it initially downloads all block headers from its connected peers rather than the entire block data. To retrieve block headers, nodes can use the getheaders Remote Procedure Call (RPC) command.


{%
include-markdown "../../../snippets/CBlockHeader.md"
%}

### Bootstrapping Block Headers

When a new node starts up, it follows a process to bootstrap the block headers:

1. The new node establishes connections to multiple peers in the network.
2. Once connected, it sends a `version` message to its peers, which includes the height of the locally known blockchain.
3. Upon receiving the `version` message, the connected peers respond with their `version` messages, containing their respective blockchain heights.
4. The new node identifies the peer with the highest blockchain height and issues a `getheaders` RPC call to that peer, requesting headers starting from its locally known best block.
5. The connected peer replies with a `headers` message, containing up to 2,000 block headers.
6. The new node validates the received headers to ensure they meet the consensus rules, then updates its local header chain with the valid headers.
7. If the received headers are less than 2,000, the new node has caught up with the longest chain of headers. Otherwise, it repeats steps 4-6 to request more headers from the peer until it is fully synchronized with the network.

By following this process, the new node can efficiently bootstrap the block headers and determine the most reliable blockchain with the most proof-of-work.


---

### Time

The `time` field in the block header represents a Unix epoch timestamp indicating when the miner began hashing the header, as reported by the miner. This timestamp serves two primary purposes: adjusting the mining [difficulty](https://btcinformation.org/en/developer-reference#target-nbits)  and providing a reference point for transaction validity.

For a block to be considered valid, its timestamp must meet the following criteria:

1. It must be strictly greater than the median time of the previous 11 blocks. This requirement helps prevent miners from manipulating the blockchain's timestamps and ensures a fair distribution of block rewards.
2. The block's timestamp cannot be more than two hours in the future, according to the full node's clock. This constraint prevents miners from submitting blocks with significantly skewed timestamps that could disrupt the network.

{%
include-markdown "../../../snippets/ContextualCheckBlockHeader.md"
%}

By adhering to these validation rules, the Bitcoin network maintains a consistent and secure chain of blocks, ensuring that mining difficulty is adjusted accordingly and transactions are time-stamped correctly.

---

### Too new
  Here is the current block time as compared to when my node saw.

<iframe src="https://grafana.pro-bitcoin.io/d-solo/qtQCmdN7z/blocks?orgId=1&amp;theme=dark&amp;panelId=40" width="250" height="100"></iframe>

  And here is the same but over time:

<iframe src="https://grafana.pro-bitcoin.io/d-solo/qtQCmdN7z/blocks?orgId=1&amp;theme=dark&amp;panelId=37" width="100%" height="200"></iframe>

  X-Axis is time and Y-Axis are not relevant. Each color segment is a new block. Essentially you are looking at the result of

  ```cpp
  auto now = std::time(nullptr);
  auto diff = now - currentBlock->GetBlockTime();
  ```

  The colors indicate time difference buckets. For instance <span class="red">red</span> indicates it took greater than 60 seconds for this node to
  see the block. <span class="orange">Orange</span> is greater than 30 seconds. The **black** or transparent background is less than 30 seconds.

  Typically, one would expect that the node time to be greater than miner time (block header time). However, that is not always the case:

<iframe src="https://grafana.pro-bitcoin.io/d-solo/qtQCmdN7z/blocks?orgId=1&theme=dark&panelId=74&from=1648902600000&to=1648911600000" width="100%" height="200"></iframe>

  Apparently, on 2022-04-02 block [730,106](https://mempool.space/block/00000000000000000004a901eb03c4de9ff5905f7cb9f240181afe0f3826de84) had a time in the future by about 21 seconds as compared to my node.

---

### Too old

Your node stores the block header time and metadata when the block was seen. Bitcoin then calculates the average time from the past 11 previous blocks

{%
include-markdown "../../../snippets/GetMedianTimePast.md"
%}


If the incoming block has a time less than `GetMedianTimePast` then it is rejected.

---

## References

* [block dashboard](https://grafana.pro-bitcoin.io/d/qtQCmdN7z/blocks?orgId=1&refresh=5m)
* [Headers](https://btcinformation.org/en/developer-reference#block-headers)
* [CBlockHeader](https://doxygen.bitcoincore.org/class_c_block_header.html)
* Block Header Time [MIT OpenCourseWare - 5. Synchronization Process and Pruning](https://youtu.be/1Qws70XGSq4?t=690)

