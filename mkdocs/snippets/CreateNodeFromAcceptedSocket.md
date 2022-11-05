[src/net.cpp#L949-L966](https://github.com/pro-bitcoin/pro-bitcoin/blob/1cebb77cf68ba53092fbd26d21522a8adbfa2c2a/src/net.cpp#L949-L966){:target="_blank"}
```cpp
void CConnman::CreateNodeFromAcceptedSocket(std::unique_ptr<Sock>&& sock,
                                            NetPermissionFlags permission_flags,
                                            const CAddress& addr_bind,
                                            const CAddress& addr)
{
    int nInbound = 0;
    int nMaxInbound = nMaxConnections - m_max_outbound;
    metricsContainer->Net().IncConnection("accept");
    AddWhitelistPermissionFlags(permission_flags, addr);
    if (NetPermissions::HasFlag(permission_flags, NetPermissionFlags::Implicit)) {
        NetPermissions::ClearFlag(permission_flags, NetPermissionFlags::Implicit);
        if (gArgs.GetBoolArg("-whitelistforcerelay", DEFAULT_WHITELISTFORCERELAY)) NetPermissions::AddFlag(permission_flags, NetPermissionFlags::ForceRelay);
        if (gArgs.GetBoolArg("-whitelistrelay", DEFAULT_WHITELISTRELAY)) NetPermissions::AddFlag(permission_flags, NetPermissionFlags::Relay);
        NetPermissions::AddFlag(permission_flags, NetPermissionFlags::Mempool);
        NetPermissions::AddFlag(permission_flags, NetPermissionFlags::NoBan);
    }

```
