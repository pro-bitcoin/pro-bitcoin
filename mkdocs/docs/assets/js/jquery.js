$(document).ready(function () {
    $('a[href^="http"]').not('a[href*=pro-bitcoin]').attr('target','_blank');
});

$( window ).on( "load", function() {
    processCodeAnchors();
    $('a[href^="http"]').not('a[href*=pro-bitcoin]').attr('target','_blank');
});
