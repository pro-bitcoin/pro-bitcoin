---
title: Install
description: Installing a prometheus bitcoin node
---

# Prerequisites

  You should be comfortable with the shell, gnu make and basic linux system administration
  In addition to all build [requirements](https://github.com/bitcoin/bitcoin/blob/master/doc/build-unix.md) for bitcoin-core, you need [prometheus-cpp](https://github.com/jupp0r/prometheus-cpp)
  installed as well.
  For Arch Linux users there is a [aur](https://aur.archlinux.org/packages/prometheus-cpp-git) package you can use.

  **Note**, building has only been tested on Linux.

## Build

```shell

git clone  https://github.com/pro-bitcoin/pro-bitcoin.git
cd pro-bitcoin
./autogen.sh
LIBS="-lz"  ./configure \
    --prefix=/usr \
    --sbindir=/usr/bin \
    --libexecdir=/usr/lib/bitcoin \
    --sysconfdir=/etc \
    --sharedstatedir=/usr/share/bitcoin \
    --localstatedir=/var/lib/bitcoin \
    --enable-hardening \
    --enable-tests \
    --disable-gui-tests \
    --enable-bench \
    --enable-glibc-back-compat \
    --enable-threadlocal \
    --enable-zmq \
    --enable-wallet --disable-libs --enable-usdt \
    --with-gui=no
make
```

`LIBS=-lz` is for prometheus web server to compress metrics responses using `libz`

## Config File

pro-bitcoin uses the same configuration as bitcoin-core, however there are too additional arguments

* `metrics=1` - this will enable metrics framework, disabled by default
* `metricsbind=localhost:18335` - bind metrics endpoint to port 18335 (default testnet)

For mainnet the default port is `8335`


## Run

```shell
./src/bitcoind -chain=test -metricsbind=localhost:18335 -metrics=1
```

You should see a log entry like this:
```
Bound metrics endpoint to localhost:18335/metrics
```

## Verify

```shell
curl -s http://localhost:18335/metrics
```

Example output:

```text
# TYPE block_connect_tip gauge
block_connect_tip{chain="test",operation="transactions-check"} 0
block_connect_tip{chain="test",operation="update-index"} 0
block_connect_tip{chain="test",operation="update-tip"} 0
block_connect_tip{chain="test",operation="flush-view"} 0
block_connect_tip{chain="test",operation="connect"} 0
block_connect_tip{chain="test",operation="fork-check"} 0
block_connect_tip{chain="test",operation="flush-disk"} 0
block_connect_tip{chain="test",operation="connect-total"} 0
block_connect_tip{chain="test",operation="load"} 0
```


## Debug

You can enable a debug flag `debug=metrics` which displays the metrics that have been registered

## Prometheus

There's plenty of [documentation](https://lmgtfy.app/?q=Prometheus+grafana+install) on how to install prometheus with grafana.
Add the following to your [scrape_configs](https://prometheus.io/docs/prometheus/latest/configuration/configuration/)  within prometheus.yml:
```yaml
  - job_name: "btc"
    static_configs:
      - targets: ["localhost:18335"]
```

