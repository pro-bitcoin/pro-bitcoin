$(document).ready(function () {
    console.log('document ready');
    $('a[href^="http"]').not('a[href*=pro-bitcoin]').attr('target','_blank');
});

$( window ).on( "load", function() {
    console.log('window load');
    //processGrafanaIframes();
    processCodeAnchors();
    $('a[href^="http"]').not('a[href*=pro-bitcoin]').attr('target','_blank');

});
