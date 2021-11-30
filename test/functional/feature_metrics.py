#!/usr/bin/env python3
# Copyright (c) 2017-2021 The Bitcoin Core developers
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.
""" Metrics functional tests

  * Verify metrics endpoint is running
"""
import http.client

from test_framework.test_framework import BitcoinTestFramework
from test_framework.util import (
    assert_greater_than,
)

import requests

def deadcode():
    return 0
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
        self.setup_nodes()


    def run_test(self):
        c = http.client.HTTPConnection(f"localhost:{self.get_metrics_port()}")
        c.request("GET", "/metrics")
        metrics = c.getresponse().read()
        assert_greater_than(len(metrics), 1);
        assert 'bitcoin_conf{' in str(metrics)


if __name__ == '__main__':
    MetricsTest().main()
