---
hide:
  - toc
title: Configuration
description: TBD
---

# Configuration

Bitcoin configuration is rather complex and long. Below you will find almost all configuration as defined in [bitcoind -help](https://man.archlinux.org/man/community/bitcoin-daemon/bitcoind.1.en)

---

## Categories

**Bitcoind** splits the flags/config into "categories".  These are defined in `src/util/system.h` as enum **OptionsCategory**

{%
include-markdown "../../snippets/OptionsCategory.md"
%}

Below are the options for RPC category:

<iframe src="https://grafana.pro-bitcoin.io/d-solo/V9BHdTN7z/blocks?orgId=1&amp;theme=dark&amp;panelId=39" width="100%" height="200"></iframe>

---

## ArgsManager

<codeanchor data-title="ArgsManager" data-file="src/util/system.h" data-line="172"></codeanchor> is the main class that handles parsing (e.g. command line and bitcoin.conf) and defining names, category and type (boolean) of flags/config.
For example, I added a metrics config to an instance of ArgsManager. I wanted to be able to enable/disable metrics reporting.

{%
include-markdown "../../snippets/MetricsInit.md"
%}

To get the value of a flag you use `args.GetBoolArg("-metrics", true)`. The second parameter is the `default` value if not set or defined

As of version `22.x` there is a global <codeanchor data-title="ArgsManager" data-file="src/util/system.h" data-line="503"></codeanchor> which is referenced throughout the code

---

## Init

<codeanchor data-title="AppInitMain" data-file="src/init.cpp" data-line="1097"></codeanchor> is where the action is at, if you want to know how bitcoind initializes and runs

## Metrics Config

The metrics `bitcoin_conf` is a metric defining most configuration values:
{%
include-markdown "../../snippets/MetricsConfig.md"
%}

See [config](https://grafana.pro-bitcoin.io/d/V9BHdTN7z/configuration?orgId=1) dashboard for complete visual

## Dynamic Configuration

 `// TODO`
