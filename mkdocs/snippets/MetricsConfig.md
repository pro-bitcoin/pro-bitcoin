[src/init.cpp#L1426-L1435](https://github.com/pro-bitcoin/pro-bitcoin/blob/1cebb77cf68ba53092fbd26d21522a8adbfa2c2a/src/init.cpp#L1426-L1435){:target="_blank"}
```cpp
    configMetrics.SetFlag("alertnotify", static_cast<size_t>(OptionsCategory::OPTIONS), args.GetArg("-alertnotify", "") != "");
    configMetrics.SetFlag("acceptnonstdtxn", static_cast<size_t>(OptionsCategory::NODE_RELAY), args.GetBoolArg("-acceptnonstdtxn", !chainparams.RequireStandard()));
    std::string blockfilterindex_value = args.GetArg("-blockfilterindex", DEFAULT_BLOCKFILTERINDEX);
    configMetrics.SetFlag("blockfilterindex", static_cast<size_t>(OptionsCategory::OPTIONS), blockfilterindex_value != DEFAULT_BLOCKFILTERINDEX);
    configMetrics.SetFlag("blocksonly", static_cast<size_t>(OptionsCategory::NODE_RELAY), ignores_incoming_txs);
    configMetrics.SetFlag("blocknotify", static_cast<size_t>(OptionsCategory::OPTIONS), args.GetArg("-blocknotify", "") != "");
    configMetrics.SetFlag("checkpoints", static_cast<size_t>(OptionsCategory::DEBUG_TEST), fCheckpointsEnabled);
    configMetrics.SetFlag("checkblockindex", static_cast<size_t>(OptionsCategory::DEBUG_TEST), fCheckBlockIndex);
    configMetrics.SetFlag("coinstatsindex", static_cast<size_t>(OptionsCategory::OPTIONS), args.GetBoolArg("-coinstatsindex", DEFAULT_COINSTATSINDEX));
```
