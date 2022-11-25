---
title: P2P network
description: TBD
---

# P2P Network

<iframe src="https://grafana.pro-bitcoin.io/d-solo/VkX8f0N7k/blocks?orgId=1&theme=dark&panelId=21" width="200" height="150"></iframe>
<iframe src="https://grafana.pro-bitcoin.io/d-solo/VkX8f0N7k/blocks?orgId=1&theme=dark&panelId=23" width="200" height="150"></iframe>
<iframe src="https://grafana.pro-bitcoin.io/d-solo/VkX8f0N7k/blocks?orgId=1&theme=dark&panelId=26" width="250" height="150"></iframe>


## Protocols

  Bitcoin supports transports, Ipv4, Ipv6, [Tor](https://github.com/pro-bitcoin/pro-bitcoin/blob/prometheus/doc/tor.md), [I2P](https://github.com/pro-bitcoin/pro-bitcoin/blob/prometheus/doc/i2p.md)

  Here are the current protocol connections: 

<iframe src="https://grafana.pro-bitcoin.io/d-solo/VkX8f0N7k/blocks?orgId=1&theme=dark&panelId=10" width="100%" height="250"></iframe>

  At the time of this writing most connections where Ipv4.


## Inbound/Outbound 



## CNode

{%
include-markdown "../../../snippets/CNode.md"
%}

The `CNode` class represents a Node


## CConnman

{%
include-markdown "../../../snippets/CConnman.md"
%}

Class CConnman handles all connections and coordinates messages to/from other nodes. Initialization occurs in `src/init.cpp`

{%
include-markdown "../../../snippets/CConnmanInit.md"
%}

{%
include-markdown "../../../snippets/CConnmanOptions.md"
%}

CConnman has multiple threads handling various tasks

* ThreadOpenAddedConnections
* ThreadMessageHandler
* ThreadI2PAcceptIncoming
* ThreadDNSAddressSeed
* ThreadSocketHandler


## Accepting Connections
 
  CConnman::SocketHandlerListening

{%
include-markdown "../../../snippets/CreateNodeFromAcceptedSocket.md"
%}

Here you see that a connection will be accepted, but first bitcoin checks if [banned](https://github.com/pro-bitcoin/pro-bitcoin/blob/1cebb77cf68ba53092fbd26d21522a8adbfa2c2a/src/net.cpp#L995),
[discouraged](https://github.com/pro-bitcoin/pro-bitcoin/blob/1cebb77cf68ba53092fbd26d21522a8adbfa2c2a/src/net.cpp#L995) or reached inbound [limit](https://github.com/pro-bitcoin/pro-bitcoin/blob/1cebb77cf68ba53092fbd26d21522a8adbfa2c2a/src/net.cpp#L1009)
