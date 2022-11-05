---
title: TX SigOps
description: Signature Operations
hide:
  - toc
---

# Signature Operations (a.k.a SigOps)

Signatures are CPU expensive operations for nodes, especially low powered ones.
To reduce the denial-of-service attack vector, nodes limit the amount of signatures per block.

{%
include-markdown "../../../snippets/MaxSigOps.md"
%}


Every node validates signatures. If someone can craft a set of transactions such that nodes spend most of them time checking sigs then
one could possibly achieve consensus failure/breakdown. If a node is too busy verifying signatures, it **cannot** keep of with the chain.

---

## Tip SigOps

Current block tip cost:

<iframe src="https://grafana.pro-bitcoin.io/d-solo/qtQCmdN7z/blocks?orgId=1&theme=dark&panelId=29" width="350" height="100"></iframe>

The percentage of blocks sigops to  <codeanchor data-file="src/consensus/consensus.h" data-line="17" data-title="MAX_BLOCK_SIGOPS_COST"></codeanchor> over time:

<iframe src="https://grafana.pro-bitcoin.io/d-solo/qtQCmdN7z/blocks?orgId=1&theme=dark&panelId=24" width="100%" height="250"></iframe>

<iframe src="https://grafana.pro-bitcoin.io/d-solo/qtQCmdN7z/blocks?orgId=1&theme=dark&panelId=86" width="100%" height="250"></iframe>

## Verification

{%
include-markdown "../../../snippets/GetTransactionSigOpCost.md"
%}

  Note the use of <codeanchor data-file="src/consensus/consensus.h" data-line="21" data-title="WITNESS_SCALE_FACTOR"></codeanchor> (value of 3).
  Legacy transactions have an explicit higher cost then SegWit transactions.

---

### Witness SigOps
{%
include-markdown "../../../snippets/WitnessSigOps.md"
%}

 For SegWit transactions using a simple [p2swh]() i.e. sending coins from one hash to another, the cost is only **1**.

 The constant <codeanchor data-file="src/script/interpreter.h" data-line="226" data-title="WITNESS_V0_KEYHASH_SIZE"></codeanchor> is only 20 bytes long

---

### Legacy

  TBD
