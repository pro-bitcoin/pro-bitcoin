[src/validation.cpp#L3534-L3543](https://github.com/pro-bitcoin/pro-bitcoin/blob/1cebb77cf68ba53092fbd26d21522a8adbfa2c2a/src/validation.cpp#L3534-L3543){:target="_blank"}
```cpp
    // Check timestamp against prev
    if (block.GetBlockTime() <= pindexPrev->GetMedianTimePast())
        return state.Invalid(BlockValidationResult::BLOCK_INVALID_HEADER, "time-too-old", "block's timestamp is too early");

    // Check timestamp
    if (block.Time() > now + std::chrono::seconds{MAX_FUTURE_BLOCK_TIME}) {
        return state.Invalid(BlockValidationResult::BLOCK_TIME_FUTURE, "time-too-new", "block timestamp too far in the future");
    }

```
