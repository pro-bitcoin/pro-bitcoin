#!/usr/bin/env python3
# Copyright (c) 2017-2021 The Bitcoin Core developers
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.
""" Metrics functional tests

  * Verify metrics endpoint is running
"""
import requests

from test_framework.test_framework import BitcoinTestFramework
from test_framework.util import (
    assert_greater_than,
)


class MetricsTest(BitcoinTestFramework):
    # TODO generating a random port
    metrics_port = 59142

    def get_metrics_port(self):
        return self.metrics_port


    def set_test_params(self):
        self.setup_clean_chain = True
        self.num_nodes = 1
        self.extra_args = [[f"-metricsbind={self.get_metrics_port()}"]]


    def setup_network(self):
        """Setup the test network topology

        Often you won't need to override this, since the standard network topology
        (linear: node0 <-> node1 <-> node2 <-> ...) is fine for most tests.

        If you do override this method, remember to start the nodes, assign
        them to self.nodes, connect them and then sync."""

        self.setup_nodes()


    def run_test(self):
        metrics = requests.get(f"http://localhost:{self.get_metrics_port()}/metrics").text;
        assert_greater_than(len(metrics), 1)
        assert 'bitcoin_conf{' in metrics


if __name__ == '__main__':
    MetricsTest().main()
