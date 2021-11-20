#include <boost/test/unit_test.hpp>
#include <metrics/container.h>
#include <test/util/setup_common.h>

BOOST_FIXTURE_TEST_SUITE(metrics_tests, BasicTestingSetup)


BOOST_AUTO_TEST_CASE(metrics_config)
{
    std::map<std::string, prometheus::MetricType> metric_names;
    auto& reg = metrics::Registry();
    BOOST_CHECK(reg.Collect().size() > 0);

    for (auto m : reg.Collect()) {
        metric_names.insert({m.name, m.type});
    }
    std::vector<std::string> expected_names = {
        "bitcoin_boot_time",
        "block_avg",
        "block_connect",
        "block_tip",
        "initial_block_download",
        "mempool",
        "mempool_changes",
        "mempool_timer",
        "mempool_orphans",
        "net_bandwidth",
        "net_connection",
        "net_max_outbound",
        "net_ping",
        "net_ping_problem",
        "net_socket",
        "peer_connection",
        "peer_permission",
        "peer_process",
        "peers_banned",
        "peers_discourage",
        "peer_send",
        "peers_known",
        "peers_misbehave",
        "peer_process",
        "peer_validation_result",
        "transaction",
        "transactions_check_avg",
        "tx_cache",
        "tx_inputs",
    };
    for (auto m : expected_names) {
        auto found = metric_names.find(m);
        BOOST_CHECK_MESSAGE(found != metric_names.end(), strprintf("metric %s is not found", m));
    }
}
BOOST_AUTO_TEST_SUITE_END()
