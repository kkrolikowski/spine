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
    $(document).on('click', '#enable_server_alias', function() {
      $('#server-alias').attr('disabled', ! this.checked);
      if($('#enable_server_alias').is(':checked')) {
        $('.glyphicon-plus').on('click', function() {
          $("#serverAlias").after(
            '<div class="form-group" id="serverAliasPlus">' +
              '<div class="row">' +
                '<label for="server-alias" class="col-sm-2 control-label">ServerAlias</label>' +
                '<div class="col-sm-4">' +
                  '<input type="text" class="form-control" id="server-alias"' +
                  'data-minlength="3" data-error="Wpisz co najmniej trzy znaki"' +
                  'name="ServerAlias[]" placeholder="*.example.com" required>' +
                  '<span class="glyphicon form-control-feedback" aria-hidden="true"></span>' +
                '</div>' +
                '<div>' +
                  '<span class="glyphicon glyphicon-remove" aria-hidden="true"></span>' +
                '</div>' +
              '</div>' +
              '<div class="col-sm-4 col-sm-offset-6 server-alias-txt">' +
                '<div class="help-block with-errors"></div>' +
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
    $('#vhostOptSelect').DualListBox();
    $(document).on('click', '#addvhost-btn', function(e) {
      e.preventDefault();
      var form = $('#addvhost');
      var serverid = form.find('[name="serverid"]').val();
      var sn = form.find('[name="ServerName"]').val();
      var sa = [];
      if($('#enable_server_alias').attr('checked')) {
        form.find('[name="ServerAlias[]"]').each(function() {
          sa.push($(this).val());
        });
      }
      else {
        sa.push("NaN");
      }
      var vhopts = [];
      $('select.selected > option').each(function() {
        vhopts.push($(this).val());
      });
      var account = form.find('[name="account"]').val();
      var htaccess;
      if(form.find('[name="htaccess"]').val()) {
        htaccess = form.find('[name="htaccess"]').val();
      }
      else {
        htaccess = "NaN";
      }
      $.ajax({
        url: '/apache.php?addvhost',
        method: 'POST',
        data: {serverid, sn, sa, vhopts, account, htaccess},
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
          $('select.selected > option').each(function() {
            $(this).appendTo('select.unselected');
          });
        }
    }).success(function(response) {
      var tr = $('#wwwconfig').find('tr').last();
      tr.after(
        '<tr>' +
          '<td>' +
            '<a href="http://'+ response.ServerName +'/" target="_blank">'+ response.ServerName +'</a>' +
          '</td>' +
          '<td class="button-cell">' +
            '<div class="btn-group">' +
              '<button type="button" class="btn btn-danger" data-id="'+ response.id +'">Usuń</button>' +
              '<button type="button" class="btn btn-danger dropdown-toggle" data-toggle="dropdown" aria-haspopup="true" aria-expanded="false">' +
                '<span class="caret"></span>' +
                '<span class="sr-only">Toggle Dropdown</span>' +
              '</button>' +
              '<ul class="dropdown-menu">' +
                '<li><a href="#" data-id="'+ response.id +'" class="edit-apache-conf">Edytuj</a></li>' +
              '</ul>' +
            '</div>' +
          '</td>' +
        '</tr>'
      );
    });
  });
  $('#addvhost-btn').attr('disabled', true);
  $('#servername').keyup(function() {
    if($(this).val().lenght != 0)
      $('#addvhost-btn').attr('disabled', false);
    else {
      $('#addvhost-btn').attr('disabled', true);
    }
  });
  $('#addvhost').validator();
  $(document).on('click', '.edit-apache-conf', function() {
    var id = $(this).attr('data-id');
    $.ajax({
      url: "/apache.php?vhostid=" + id,
      method: "GET"
    }).success(function(response) {
      $('#vhostEditForm')
        .find('[name="id"]').val(response.id).end()
        .find('[name="ServerName"]').val(response.ServerName).end();
      bootbox
        .dialog({
          title: 'Konfiguracja witryny',
          message: $('#vhostEditForm'),
          show: false
        })
        .on('shown.bs.modal', function() {
          $('#vhostEditForm')
            .show()
        })
        .on('hide.bs.modal', function(e) {
          $('#vhostEditForm').hide().appendTo('body');
        })
        .modal('show');
    });
  });
});
