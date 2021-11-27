#include <logging.h>
#include <metrics/rpc.h>
#include <sync.h>

Mutex g_rpc_mutex;

namespace metrics {
std::unique_ptr<RpcMetrics> RpcMetrics::make(const std::string& chain, prometheus::Registry& registry, bool noop)
{
    LogPrint(BCLog::METRICS, "creaing TxMetrics on %s\n", chain);
    return noop ? std::make_unique<RpcMetrics>() : std::make_unique<RpcMetricsImpl>(chain, registry);
}

RpcMetricsImpl::RpcMetricsImpl(const std::string& chain, prometheus::Registry& registry) : Metrics(chain, registry)
{
    _method_family = &FamilySummary("rpc_access");
    _error_family = &FamilyCounter("rpc_error");
}

void RpcMetricsImpl::ObserveMethod(const std::string& method, double amt)
{
    auto counter = _method_counter.find(method);
    if (counter == this->_method_counter.end()) {
        if (this->_method_counter.size() > MAX_ACCESS_METHODS) {
            LogPrint(BCLog::METRICS, "reached max number of metrics for rpc_access (%s)\n", method);
            return;
        }
        {
            LOCK(g_rpc_mutex);
            LogPrint(BCLog::METRICS, "creaing rpc_access metric with method \"%s\"\n", method);
            auto quant = prometheus::Summary::Quantiles{
                {0.75, 0.001},
                {0.95, 0.001},
                {0.99, 0.001}};
            this->_method_counter.insert({method, &_method_family->Add({{"method", method}}, quant)});
            counter = _method_counter.find(method);
        }
    }
    counter->second->Observe(amt);
}

void RpcMetricsImpl::IncrementError(const std::string& method)
{
    auto counter = _error_counter.find(method);
    if (counter == this->_error_counter.end()) {
        if (this->_method_counter.size() > MAX_ERROR_METHODS) {
            LogPrint(BCLog::METRICS, "reached max number of metrics for rpc_error (%s)\n", method);
            return;
        }
        {
            LOCK(g_rpc_mutex);
            this->_error_counter.insert({method, &_error_family->Add({{"method", method}})});
            counter = _error_counter.find(method);
        }
    }
    counter->second->Increment();
}

bool RpcMetricsImpl::HasMethod(const std::string& method)
{
    auto counter = _method_counter.find(method);
    return counter != this->_method_counter.end();
}

std::optional<double> RpcMetricsImpl::GetErrorCount(const std::string& method)
{
    auto counter = _error_counter.find(method);
    if (counter == this->_error_counter.end()) {
        return std::nullopt;
    }
    return counter->second->Value();
}
} // namespace metrics