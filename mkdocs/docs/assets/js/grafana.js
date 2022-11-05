const _dashboards = {
    "blocks": "qtQCmdN7z",
    "config": "V9BHdTN7z"
};

function processGrafanaIframes() {
    $('grafana').each(function () {
        loadGrafanaIframe($(this));
    });
}

function loadGrafanaIframe(e) {
    const dash = e.data('dashboard');
    const dashId = _dashboards[e.data('dashboard')];
    const panel = e.data('panel');
    if (!dashId || !panel) {
        return
    }
    const fromQuery = e.data('from') === undefined ? "" : `&from=${e.data('from')}`;
    const toQuery = e.data('to') === undefined ? "" : `&to=${e.data('to')}`;
    const url = `https://grafana.pro-bitcoin.io/d-solo/${dashId}/${dash}?orgId=1&theme=dark&panelId=${panel}${fromQuery}${toQuery}`;
    const width = e.data('width') || 600;
    const height = e.data('height') || 200;

    e.wrapInner(`<iframe src="${url}" width=${width} height=${height}></iframe>`);
}
