[src/util/system.h#L137-L155](https://github.com/pro-bitcoin/pro-bitcoin/blob/1cebb77cf68ba53092fbd26d21522a8adbfa2c2a/src/util/system.h#L137-L155){:target="_blank"}
```cpp
enum class OptionsCategory {
    OPTIONS,
    CONNECTION,
    WALLET,
    WALLET_DEBUG_TEST,
    ZMQ,
    DEBUG_TEST,
    CHAINPARAMS,
    NODE_RELAY,
    BLOCK_CREATION,
    RPC,
    GUI,
    COMMANDS,
    REGISTER_COMMANDS,

    HIDDEN // Always the last option to avoid printing these in the help
};

```
