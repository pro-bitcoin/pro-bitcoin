[src/script/interpreter.cpp#L2072-L2087](https://github.com/pro-bitcoin/pro-bitcoin/blob/1cebb77cf68ba53092fbd26d21522a8adbfa2c2a/src/script/interpreter.cpp#L2072-L2087){:target="_blank"}
```cpp
{
    if (witversion == 0) {
        if (witprogram.size() == WITNESS_V0_KEYHASH_SIZE)
            return 1;

        if (witprogram.size() == WITNESS_V0_SCRIPTHASH_SIZE && witness.stack.size() > 0) {
            CScript subscript(witness.stack.back().begin(), witness.stack.back().end());
            return subscript.GetSigOpCount(true);
        }
    }

    // Future flags may be implemented here.
    return 0;
}

```
