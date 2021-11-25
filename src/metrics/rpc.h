#ifndef BITCOIN_METRICS_RPC_H
#define BITCOIN_METRICS_RPC_H
#include <metrics/metrics.h>
#include <optional>

namespace metrics {

class RpcMetrics
{
public:
    virtual ~RpcMetrics() {}
    static std::unique_ptr<RpcMetrics> make(const std::string& chain, prometheus::Registry& registry, bool noop);
    virtual void ObserveMethod(const std::string& method, double amt){};
    virtual void IncrementError(const std::string& method){};
    virtual bool HasMethod(const std::string& method) { return false; };
    virtual std::optional<double> GetErrorCount(const std::string& method) { return 0; };
};

class RpcMetricsImpl : virtual public RpcMetrics, Metrics
{
protected:
    std::map<std::string, prometheus::Summary*> _method_counter;
    std::map<std::string, prometheus::Counter*> _error_counter;
    prometheus::Family<prometheus::Summary>* _method_family;
    prometheus::Family<prometheus::Counter>* _error_family;

public:
    ~RpcMetricsImpl(){};
    explicit RpcMetricsImpl(const std::string& chain, prometheus::Registry& registry);
    void ObserveMethod(const std::string& method, double amount) override;
    void IncrementError(const std::string& method) override;
    bool HasMethod(const std::string& method) override;
    std::optional<double> GetErrorCount(const std::string& method) override;
};

} // namespace metrics

#endif
