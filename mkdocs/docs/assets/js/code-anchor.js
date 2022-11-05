const blobHash = "1cebb77cf68ba53092fbd26d21522a8adbfa2c2a";

function createCodeLink(e) {
    const line = e.data('line');
    const text = e.data('title');
    const file = e.data('file');
    const link = `https://github.com/pro-bitcoin/pro-bitcoin/blob/${blobHash}/${file}#L${line}`;
    e.wrapInner(`<a href="${link}" target="_blank" >${text}</a>`);
}

function processCodeAnchors() {
    $('codeanchor').each(function () {
        createCodeLink($(this));
    });

}

