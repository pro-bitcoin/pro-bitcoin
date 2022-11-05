---
title: Block Headers
description: TBD
hide:
  - toc
---

# Block Headers

## Intro

Class [CBlockHeader](https://doxygen.bitcoincore.org/class_c_block_header.html) is a compact yet powerful data structure. The [MerkelRoot](https://en.m.wikipedia.org/wiki/Merkle_tree) field allows
[SPV](https://btcinformation.org/en/developer-guide#simplified-payment-verification-spv) nodes to check
existence of transactions with low overhead. A node with no blocks downloads all block Headers from connected peers.
Depending on your internet connection, this should take a few minutes. Based on the headers only, nodes are able to see the correct chain (most proof-of-work)

{%
include-markdown "../../../snippets/CBlockHeader.md"
%}

---

## Time

  The block time is a [Unix epoch time](https://en.wikipedia.org/wiki/Unix_time)
  when the miner started hashing the header (according to the miner).
  Must be strictly greater than the median time of the previous 11 blocks.
  Full nodes will not accept blocks with headers more than two hours in the future according to their clock.
  The main reason for time is adjustment of [difficulty](https://btcinformation.org/en/developer-reference#target-nbits) in addition for [transactions](/transactions) to indicate when a particular transaction is valid.

{%
include-markdown "../../../snippets/ContextualCheckBlockHeader.md"
%}

---

### Too new
  Block Tip header time as indicated by the miner, compared to when my node saw the block announcement

<iframe src="https://grafana.pro-bitcoin.io/d-solo/qtQCmdN7z/blocks?orgId=1&amp;theme=dark&amp;panelId=40" width="250" height="100"></iframe>

  And here is the same but over time:

<iframe src="https://grafana.pro-bitcoin.io/d-solo/qtQCmdN7z/blocks?orgId=1&amp;theme=dark&amp;panelId=37" width="100%" height="200"></iframe>

  X-Axis is time and Y-Axis is not relevant. Each color segment is a new block. Essentially you are looking at the result of

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


If the incoming block has a time less than `GetMedianTimePast` than it is rejected.

---

## References

* [block dashboard](https://grafana.pro-bitcoin.io/d/qtQCmdN7z/blocks?orgId=1&refresh=5m)
* [Headers](https://btcinformation.org/en/developer-reference#block-headers)
* [CBlockHeader](https://doxygen.bitcoincore.org/class_c_block_header.html)
* Block Header Time [MIT OpenCourseWare - 5. Synchronization Process and Pruning](https://youtu.be/1Qws70XGSq4?t=690)

