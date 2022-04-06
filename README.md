# Pro Bitcoin

[![Build Status](https://api.cirrus-ci.com/github/pro-bitcoin/pro-bitcoin.svg)](https://cirrus-ci.com/github/pro-bitcoin/pro-bitcoin)

A [prometheus](https://prometheus.io/docs/introduction/overview/) integrated bitcoin-core node running version [23.0](https://github.com/bitcoin/bitcoin/tree/v23.0), inspired by [https://statoshi.info](https://statoshi.info/?orgId=1)

## Building

* Install [prometheus-cpp](https://github.com/jupp0r/prometheus-cpp) (for arch linux: [AUR](https://aur.archlinux.org/packages/prometheus-cpp-git) package)

```shell
./autogen.sh
LIBS="-lz" ./configure <your config options>
make
```

## Usage

Default metrics scrape url at `http://localhost:8335/metrics` (18335 for testnet)

## Config

Two new configuration fields are available

| config | desc |
| ----   | --------------|
| `metrics`      | boolean 0=disable 1=enable (default) |
| `metricsbind`  | bind address and port default: localhost:8335 for main net or localhost:18335 |


## Demo

Check out [https://pro-bitcoin.io](https://pro-bitcoin.io) for a running demo with live data

## Why?

Mostly curiosity. I really wanted to explor what's really happening with bitcoin nodes, especially network communication and consensus.
I learning by tinkering and with the help of [grafana](http://docs.grafana.org/guides/getting_started/) and [prometheus](https://github.com/jupp0r/prometheus-cpp), I built a visual aid to explore the code

## Special Thanks

* [jupp0r](https://github.com/jupp0r)
* [jlopp](https://github.com/jlopp)
