[src/init.cpp#L1265-L1269](https://github.com/pro-bitcoin/pro-bitcoin/blob/1cebb77cf68ba53092fbd26d21522a8adbfa2c2a/src/init.cpp#L1265-L1269){:target="_blank"}
```cpp
    node.connman = std::make_unique<CConnman>(GetRand<uint64_t>(),
                                              GetRand<uint64_t>(),
                                              *node.addrman, *node.netgroupman, args.GetBoolArg("-networkactive", true));

```
