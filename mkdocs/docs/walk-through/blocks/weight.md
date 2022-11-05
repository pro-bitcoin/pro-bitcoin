---
title: Block Weight
description: TBD
hide:
  - toc
---

# Block Weight

  Block size have a maximum byte size of (currently) 4 million bytes. This is so blocks do not become so large that nodes fail catch up to the tip.
  Bitcoin, on purpose, designed the chain to have small a block size in part so low resource nodes (i.e. raspberry pie) can validate the entire chain.
  For comparison, the fork [BSV](https://bitcoinsv.io/documentation/miners/system-requirements/) would like nodes to have 64mb (actually 128) of RAM with 10 cores.

  Originally, transactions and blocks were measured in "real" bytes, and blocks were limited to 1MB or 1 million bytes.
  After the [SegWit]() upgrade, transactions and blocks are measured by weight units, and blocks are limited to 4 million weight units.
  Weight units are abbreviated as ‘wu’ and is defined as <CodeAnchor line={21} file={'src/consensus/consensus.h'}>WITNESS_SCALE_FACTOR</CodeAnchor>

---

## Size Matters

  Here is a [bar graph](https://grafana.com/docs/grafana/latest/visualizations/bar-gauge-panel/) of block size as viewed by legacy non-SegWit nodes:

<iframe src="https://grafana.pro-bitcoin.io/d-solo/qtQCmdN7z/blocks?orgId=1&theme=dark&panelId=30" width="450" height="150"></iframe>

  And for SegWit:

<iframe src="https://grafana.pro-bitcoin.io/d-solo/qtQCmdN7z/blocks?orgId=1&theme=dark&panelId=78" width="450" height="200"></iframe>

  Transactions [signatures]() are a somewhat significant portion of the total size. SegWit upgrade moved the signature data to the [coinbase transaction]().
  The protocol allowed miners to add extra data to their coinbase transaction as defined in [bip-0141](https://github.com/bitcoin/bips/blob/master/bip-0141.mediawiki#commitment-structure)
  Legacy nodes will see extra data in the coinbase transaction and simply ignore it.

{%
include-markdown "../../../snippets/GetBlockWeight.md"
%}

  Let's take an example block, [726580](https://mempool.space/block/00000000000000000005cb3ff57fa42f262b0987019ccd0da0b1827e0eb4e13f?showDetails=true#details).

```shell
bitcoing-cli getblock 00000000000000000005cb3ff57fa42f262b0987019ccd0da0b1827e0eb4e13f |  jq '. | {strippedsize,size,weight}'
```
```json
{
  "strippedsize": 843497,
  "size": 1463110,
  "weight": 3993601
}
```

*  <codeanchor data-title="strippedsize" data-line="173" data-file="src/rpc/blockchain.cpp"></codeanchor>  is the legacy size
*  <codeanchor data-title="size" data-line="174" data-file="src/rpc/blockchain.cpp"></codeanchor> includes SegWit transactions
*  <codeanchor data-title="weight" data-line="175" data-file="src/rpc/blockchain.cpp"></codeanchor>  is `GetBlockWeight`

```text
3993601 = (843497 * 3) + 1463110
```

  In all, the cost of legacy transactions is **3 times** more than SegWit.


---

## Validation
{%
include-markdown "../../../snippets/ValidateBlockWeight.md"
%}

---

# References

* [MIT OpenCourseWare Segregated Witness](https://www.youtube.com/watch?v=7o5shPC0R2k)



