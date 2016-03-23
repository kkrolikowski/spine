$(document).ready(function() {
    var usage_free = $("#diskusage_free").attr("data");
    var usage_used = $("#diskusage_used").attr("data");
    $("#diskusage_free").circliful({
        icon: "f0a0",
        animationStep: 5,
        foregroundBorderWidth: 5,
        backgroundBorderWidth: 15,
        percent: usage_free,
        text: "Wolny obszar"
    });
    $("#diskusage_used").circliful({
        icon: "f0a0",
        animationStep: 5,
        foregroundBorderWidth: 5,
        backgroundBorderWidth: 15,
        percent: usage_used,
        text: "Zajęty obszar"
    });
});
