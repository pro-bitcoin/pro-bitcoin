#!/usr/bin/env python3
# Copyright (c) 2017-2021 The Bitcoin Core developers
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.
""" Metrics functional tests

  * Verify metrics endpoint is running
"""
import http.client
import socket
from typing import Dict
from typing import List
from contextlib import closing
import re
from decimal import Decimal

from test_framework.blocktools import COINBASE_MATURITY
from test_framework.test_framework import BitcoinTestFramework
from test_framework.util import (
    assert_greater_than, assert_equal, find_vout_for_address,
)

ESCAPE_SEQUENCES = {
    '\\\\': '\\',
    '\\n': '\n',
    '\\"': '"',
}
ESCAPING_RE = re.compile(r'\\[\\n"]')


def replace_escape_sequence(match):
    return ESCAPE_SEQUENCES[match.group(0)]


def _replace_escaping(s):
    return ESCAPING_RE.sub(replace_escape_sequence, s)


def _is_character_escaped(s, charpos):
    num_bslashes = 0
    while (charpos > num_bslashes
           and s[charpos - 1 - num_bslashes] == '\\'):
        num_bslashes += 1
    return num_bslashes % 2 == 1


def _parse_labels(labels_string) -> Dict[str, str]:
    labels: Dict[str, str] = {}
    # Return if we don't have valid labels
    if "=" not in labels_string:
        return labels

    escaping = False
    if "\\" in labels_string:
        escaping = True

    # Copy original labels
    sub_labels = labels_string
    try:
        # Process one label at a time
        while sub_labels:
            # The label name is before the equal
            value_start = sub_labels.index("=")
            label_name = sub_labels[:value_start]
            sub_labels = sub_labels[value_start + 1:].lstrip()
            # Find the first quote after the equal
            quote_start = sub_labels.index('"') + 1
            value_substr = sub_labels[quote_start:]

            # Find the last unescaped quote
            i = 0
            while i < len(value_substr):
                i = value_substr.index('"', i)
                if not _is_character_escaped(value_substr, i):
                    break
                i += 1

            # The label value is between the first and last quote
            quote_end = i + 1
            label_value = sub_labels[quote_start:quote_end]
            # Replace escaping if needed
            if escaping:
                label_value = _replace_escaping(label_value)
            labels[label_name.strip()] = label_value

            # Remove the processed label from the sub-slice for next iteration
            sub_labels = sub_labels[quote_end + 1:]
            next_comma = sub_labels.find(",") + 1
            sub_labels = sub_labels[next_comma:].lstrip()

        return labels

    except ValueError:
        raise ValueError("Invalid labels: %s" % labels_string)


class Metric:
    name: str
    labels: Dict
    value: Decimal

    def __init__(self, name: str, labels_map: Dict, value: Decimal):
        self.name = name
        self.labels = labels_map
        self.value = value


def text_fd_to_metric_families(fd: List[bytes]) -> Dict[str, List[Metric]]:
    metrics: Dict[str, List[Metric]] = {}
    for b in fd:
        line = b.decode('utf-8').strip()
        if line.startswith('#') or line == '' or '{' not in line:
            continue

        label_start, label_end = line.index("{"), line.rindex("}")
        # The name is before the labels
        name = line[:label_start].strip()
        # We ignore the starting curly brace
        lbl_str = line[label_start + 1:label_end]
        labels_map = _parse_labels(lbl_str)
        if name not in metrics:
            metrics[name] = []

        v = line[label_end+1:]
        metrics[name].append(Metric(name, labels_map, Decimal(v)))
    return metrics


def find_free_port() -> int:
    with closing(socket.socket(socket.AF_INET, socket.SOCK_STREAM)) as s:
        s.bind(('', 0))
        s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        return s.getsockname()[1]


class MetricsTest(BitcoinTestFramework):
    metrics_port: int = 0

    def get_metrics_port(self):
        if self.metrics_port == 0:
            self.metrics_port = find_free_port()
        return self.metrics_port

    def set_test_params(self):
        self.setup_clean_chain = True
        self.num_nodes = 2
        self.extra_args = [ ["-txindex"],
                            [f"-metricsbind={self.get_metrics_port()}", "-txindex"]
                        ]
        # whitelist all peers to speed up tx relay / mempool sync
        for args in self.extra_args:
            args.append("-whitelist=noban@127.0.0.1")

        self.supports_cli = False

    def setup_network(self):
        super().setup_network()
        self.connect_nodes(0, 1)

    def skip_test_if_missing_module(self):
        self.skip_if_no_wallet()

    def check_conf(self, metrics: Dict[str, List[Metric]]):
        assert 'bitcoin_conf' in metrics
        assert_equal(len(metrics['bitcoin_conf']), 67)
        for m in metrics['bitcoin_conf']:
            assert_equal(len(m.labels), 4)
            assert 'chain' in m.labels
            assert 'category' in m.labels
            assert 'name' in m.labels
            assert 'type' in m.labels
            assert_equal(m.labels['chain'], 'test')

    def check_block(self, metrics: Dict[str, List[Metric]]):
        assert 'block_connect_avg' in metrics
        assert 'block_connect_tip' in metrics
        assert 'block_tip' in metrics
        block_tip_types = [
            "size",
            "size-witness",
            "weight",
            "height",
            "version",
            "transactions",
            "sigops",
            "time",
            "header-time",
            "fees",
            "reward",
            "valueout",
            "time-delta-prev",
        ]
        block_connect = [
            "load",
            "connect-total",
            "flush-view",
            "flush-disk",
            "update-tip",
            "fork-check",
            "update-index",
        ]
        for m in metrics['block_tip']:
            assert 'chain' in m.labels
            assert 'type' in m.labels
            assert_equal(len(m.labels), 2)
            assert_equal(m.labels['chain'], 'test')
            assert m.labels['type'] in block_tip_types
            self.log.info('Checking value of %s (%s)', m.labels['type'], m.value)
            assert_greater_than(m.value, 0)

        block_connect_family: List[Metric] = metrics['block_connect_avg']
        block_connect_family.extend(metrics['block_connect_tip'])
        for m in block_connect_family:
            assert_equal(len(m.labels), 2)
            assert 'chain' in m.labels
            assert 'operation' in m.labels
            assert m.labels['operation'] in block_connect
            # ignore checking load operation value, too many false negatives
            if m.labels['operation'] != 'load':
                self.log.info('Checking value of %s (%s)', m.labels['operation'], m.value)
                assert_greater_than(m.value, 0)

    def run_test(self):
        self.log.info("Prepare some blocks with transactions")
        self.generate(self.nodes[0], 4)
        self.generate(self.nodes[0], COINBASE_MATURITY + 1)
        for amount in [1.5, 1.0, 5.0]:
            self.nodes[0].sendtoaddress(self.nodes[1].getnewaddress(), amount)
        self.sync_all()
        addr = self.nodes[1].getnewaddress()
        txid = self.nodes[0].sendtoaddress(addr, 10)
        self.generate(self.nodes[0], 1)
        vout = find_vout_for_address(self.nodes[1], txid, addr)
        rawTx = self.nodes[1].createrawtransaction([{'txid': txid, 'vout': vout}], {self.nodes[1].getnewaddress(): 9.999})
        rawTxSigned = self.nodes[1].signrawtransactionwithwallet(rawTx)
        self.nodes[1].sendrawtransaction(rawTxSigned['hex'])
        self.generateblock(self.nodes[0], output=self.nodes[0].getnewaddress(), transactions=[rawTxSigned['hex']])
        self.sync_all()

        c = http.client.HTTPConnection(f"localhost:{self.get_metrics_port()}")
        c.request("GET", "/metrics")
        body = c.getresponse().readlines()
        assert_greater_than(len(body), 1)
        metrics = text_fd_to_metric_families(body)

        self.check_conf(metrics)
        self.check_block(metrics)


if __name__ == '__main__':
    MetricsTest().main()
