[src/primitives/block.h#L20-L30](https://github.com/pro-bitcoin/pro-bitcoin/blob/1cebb77cf68ba53092fbd26d21522a8adbfa2c2a/src/primitives/block.h#L20-L30){:target="_blank"}
```cpp
class CBlockHeader
{
public:
    // header
    int32_t nVersion;
    uint256 hashPrevBlock;
    uint256 hashMerkleRoot;
    uint32_t nTime;
    uint32_t nBits;
    uint32_t nNonce;
```
