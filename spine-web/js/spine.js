$(document).ready(function() {
    var diskusage_free = $("#diskusage_free").attr("data");
    var diskusage_used = $("#diskusage_used").attr("data");
    var ram_free = $("#ram_free").attr("data");
    $("#diskusage_free").circliful({
        icon: "f0a0",
        animationStep: 5,
        foregroundBorderWidth: 5,
        backgroundBorderWidth: 15,
        percent: diskusage_free,
        text: "Wolny obszar"
    });
    $("#diskusage_used").circliful({
        icon: "f0a0",
        animationStep: 5,
        foregroundBorderWidth: 5,
        backgroundBorderWidth: 15,
        percent: diskusage_used,
        text: "Zajęty obszar"
    });
    $("#ram_free").circliful({
        animationStep: 5,
        foregroundBorderWidth: 5,
        backgroundBorderWidth: 15,
        percent: ram_free,
        text: "Wolny RAM"
    });
    $('#addvhost').validator();
    $(document).on('click', '#enable_server_alias', function() {
      $('#server-alias').attr('disabled', ! this.checked);
      if($('#enable_server_alias').is(':checked')) {
        $('.glyphicon-plus').on('click', function() {
          $("#serverAlias").after(
            '<div class="form-group" id="serverAliasPlus">' +
              '<div class="row">' +
                '<label for="server-alias" class="col-sm-2 control-label">ServerAlias</label>' +
                '<div class="col-sm-4">' +
                  '<input type="text" class="form-control" id="server-alias" name="ServerAlias[]" placeholder="*.example.com">' +
                '</div>' +
                '<div>' +
                  '<span class="glyphicon glyphicon-remove" aria-hidden="true"></span>' +
                '</div>' +
              '</div>' +
            '</div>'
          );
        });
      }
      else {
        $('.glyphicon-plus').unbind('click');
      }
      if(! $('#enable_server_alias').checked) {
        $('div').remove('#serverAliasPlus');
      }
    });
    $(document).on('click', '.glyphicon-remove', function() {
      $(this).closest('#serverAliasPlus').remove();
    });
    $(document).on('click', '#enable_htaccess', function() {
      $('#htaccess').attr('disabled', ! this.checked);
    });
    $(document).on('click', '#addvhost-btn', function(e) {
      e.preventDefault();
      $.ajax({
        url: '/apache.php?addvhost',
        method: 'POST',
        data: $('#addvhost').serializeArray(),
        success: function() {
          $.bootstrapGrowl(
            'Konfiguracja apacza zapisana',
            {
              type: 'success',
              align: 'center',
              offset: { from: 'top', amount: 55},
              width: 500
            }
          );
          $('#addvhost')[0].reset();
        }
    });
  });
});
