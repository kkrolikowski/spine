$(document).ready(function() {
    var hdd_bar_opt = {
      foreColor:'#5cb85c',
      backColor:'#d9534f',
      horTitle:'Wolne miejsce:'
    };
    $('.freeHDDbar').barIndicator(hdd_bar_opt);
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
    $(document).on('click', '#enable_server_alias, #enable_sa', function() {
      $('#server-alias, #sa').attr('disabled', ! this.checked);
      if($('#enable_server_alias').is(':checked')) {
        $('.new-server-alias').on('click', function() {
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
                  '<span class="glyphicon glyphicon-minus" aria-hidden="true"></span>' +
                '</div>' +
              '</div>' +
              '<div class="col-sm-4 col-sm-offset-6 server-alias-txt">' +
                '<div class="help-block with-errors"></div>' +
              '</div>' +
            '</div>'
          );
        });
      }
      else if($('#enable_sa').is(':checked')) {
        $('.new-server-alias').on('click', function() {
          if($('#sa-arrow').hasClass("fa-arrow-up")) {
            $('#sa-arrow').removeClass("fa-arrow-up").addClass("fa-arrow-down");
          }
          $("#sa-group").after(
            '<div class="form-group col-sm-offset-2 div-sa-new" id="sa-group-new">' +
              '<div class="row">' +
                '<label for="sa-new" class="col-sm-2 control-label">ServerAlias</label>' +
                '<div class="col-sm-4">' +
                  '<input type="text" class="form-control" id="sa-new"' +
                  'data-minlength="3" data-error="Wpisz co najmniej trzy znaki"' +
                  'name="sa[]" placeholder="*.example.com" required>' +
                  '<span class="glyphicon form-control-feedback" aria-hidden="true"></span>' +
                '</div>' +
                '<div>' +
                  '<span class="glyphicon glyphicon-minus" aria-hidden="true"></span>' +
                '</div>' +
                '<div class="col-sm-4 col-sm-offset-6 server-alias-txt">' +
                  '<div class="help-block with-errors"></div>' +
                '</div>' +
              '</div>' +
            '</div>'
          );
        });
      }
      else {
        $('.new-server-alias').unbind('click');
      }
      if(! $('#enable_server_alias').checked) {
        $('div').remove('#serverAliasPlus');
      }
      if(! $('#enable_sa').checked) {
        $('div').remove('#sa-group-new');
      }
    });
    $(document).on('click', '.glyphicon-minus', function() {
      $(this).closest('#serverAliasPlus, #sa-group-new, .access-rule').remove();
    });
    $(document).on('click', '#enable_htaccess, #edit_enable_htaccess', function() {
      $('#htaccess, #htaccess-field').attr('disabled', ! this.checked);
    });
    $(document).on('click', '#password_enable', function() {
      $('button[title="Wybierz konta "]').attr('disabled', ! this.checked);
    });
    $('#vhostOptSelect').DualListBox();
    $('#vhostOptEdit').DualListBox();
    $(document).on('click', '#addvhost-btn', function(e) {
      e.preventDefault();
      var form = $('#addvhost');
      var serverid = form.find('[name="serverid"]').val();
      var sn = form.find('[name="ServerName"]').val();
      var sa = [];
      if($('#enable_server_alias').is(':checked')) {
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
      var access_order = $('[name="access_order"]:checked').val();
      var allow = [];
      var fromhost = [];
      var idx = 0;
      $('#access-list-details > .form-group').each(function() {
        allow.push($(this).find('[name="allow['+ idx +']"]:checked').val());
        fromhost.push($(this).find('[name="from['+ idx +']"]').val());
        idx++;
      });
      var htusers = [];
      $('#htusers-select > option:checked').each(function() {
        htusers.push($(this).val());
      });
      $.ajax({
        url: '/apache.php?addvhost',
        method: 'POST',
        data: {serverid, sn, sa, vhopts, account, htaccess, access_order, allow, fromhost, htusers},
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
      var div = $('#wwwconfig > div');
      $('.modal').hide();
      $('.modal-backdrop').hide();
      $('#sa-group-new').remove();
      $('.access-rule').remove();
      $('.panel-body').hide();
      $('#expandControl').removeClass("fa-angle-down").addClass("fa-angle-up");
      if(tr.length) {
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
      }
      else {
        div.find('h5').remove();
        div.append(
          '<table class="table table-hover">' +
            '<thead>' +
              '<th>Nazwa Strony</th><th class="button-cell">Akcja</th>' +
            '</thead>' +
            '<tbody>' +
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
              '</tr>' +
            '</tbody>' +
          '</table>'
        );
      }
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
      if(response.ServerAlias[0] != "NaN") {
        $('#enable_sa').prop("checked", true);

        $.each(response.ServerAlias, function(i, sa) {
          if(i === 0) {
            $('#sa').val(sa).prop("disabled", false);
          }
          else {
            $("#sa-group").after(
              '<div class="form-group col-sm-offset-2 div-sa-new" id="sa-group-new">' +
                '<div class="row">' +
                  '<label for="sa-new" class="col-sm-2 control-label">ServerAlias</label>' +
                  '<div class="col-sm-4">' +
                    '<input type="text" class="form-control" id="sa-new"' +
                    'data-minlength="3" data-error="Wpisz co najmniej trzy znaki"' +
                    'name="sa['+ i +']" placeholder="*.example.com" value="'+ sa +'" required>' +
                    '<span class="glyphicon form-control-feedback" aria-hidden="true"></span>' +
                  '</div>' +
                  '<div>' +
                    '<span class="glyphicon glyphicon-minus new-sa-minus" aria-hidden="true"></span>' +
                  '</div>' +
                  '<div class="col-sm-4 col-sm-offset-6 server-alias-txt">' +
                    '<div class="help-block with-errors"></div>' +
                  '</div>' +
                '</div>' +
              '</div>'
            );
          }
        });
        if($('#enable_sa').is(':checked')) {
          $('.new-server-alias').on('click', function() {
            $("#sa-group").after(
              '<div class="form-group col-sm-offset-2 div-sa-new" id="sa-group-new">' +
                '<div class="row">' +
                  '<label for="sa-new" class="col-sm-2 control-label">ServerAlias</label>' +
                  '<div class="col-sm-4">' +
                    '<input type="text" class="form-control" id="sa-new"' +
                    'data-minlength="3" data-error="Wpisz co najmniej trzy znaki"' +
                    'name="sa[]" placeholder="*.example.com" required>' +
                    '<span class="glyphicon form-control-feedback" aria-hidden="true"></span>' +
                  '</div>' +
                  '<div>' +
                    '<span class="glyphicon glyphicon-minus new-sa-minus" aria-hidden="true"></span>' +
                  '</div>' +
                  '<div class="col-sm-4 col-sm-offset-6 server-alias-txt">' +
                    '<div class="help-block with-errors"></div>' +
                  '</div>' +
                '</div>' +
              '</div>'
            );
            if($('#sa-arrow').hasClass("fa-arrow-up")) {
              $('#sa-arrow').removeClass("fa-arrow-up").addClass("fa-arrow-down");
            }
          });
        }
        if($('#sa-group-new').length) {
          $('.fa-arrow-up').removeClass("fa-arrow-up").addClass("fa-arrow-down");
        }
        else {
          if($('span').hasClass('.fa-arrow-down')) {
            $(this).removeClass("fa-arrow-down").addClass("fa-arrow-up");
          }
        }
      }
      else {
        $('#enable_sa').prop("checked", false);
      }
      $.each(response.vhost_options, function(i, opt) {
        $('select.unselected > option[value="'+ i +'"]').remove();
        $('select.selected').append($('<option>', {
          value: i,
          text: opt
        }));
      });
      $('#wwwuser-edit').find('option[text="'+ response.user +'"]').attr('selected', 'selected');
      if(response.access_order === "01") {
        $('#edit_orderallow').attr('checked', false);
        $('#edit_orderdeny').attr('checked', true);
      }
      else {
        $('#edit_orderallow').attr('checked', true);
        $('#edit_orderdeny').attr('checked', false);
      }
      var idx = 0;
      $.each(response.access_list, function(accessfrom, permit) {
        if(idx === 0) {
          if(permit == 1) {
            $('#edit_allow_'+ idx).attr('checked', true);
            $('#edit_deny_'+ idx).attr('checked', false);
          }
          else {
            $('#edit_allow_'+ idx).attr('checked', false);
            $('#edit_deny_'+ idx).attr('checked', true);
          }
          $('[name="from['+ idx +']"]').val(accessfrom);
          idx++;
        }
        else {
          $('#edit_access-list-details').append(
            '<div class="form-group access-rule">' +
              '<div class="row col-sm-offset-2" id="edit_accesslist">' +
                '<div class="col-sm-4">' +
                  '<label class="radio-inline" id="edit_allowfrom">' +
                    '<input type="radio" id="edit_allow_'+ idx +'" name="allow['+ idx +']" value="1"> Allow' +
                  '</label>' +
                  '<label class="radio-inline">' +
                    '<input type="radio" id="edit_deny_'+ idx +'" name="allow['+ idx +']" value="0"> Deny' +
                  '</label>' +
                '</div>' +
                '<label for="from" class="col-sm-2 control-label edit_access-from-label">From</label>' +
                '<div class="col-sm-5">' +
                  '<input type="text" class="form-control access-from-input" id="edit_from" name="from['+ idx +']" value="all">' +
                '</div>' +
                '<div>' +
                  '<span class="glyphicon glyphicon-minus vhost-access vhost-access-del" aria-hidden="true"></span>' +
                '</div>' +
              '</div>' +
            '</div>'
          );
          if(permit == 1) {
            $('#edit_allow_'+ idx).attr('checked', true);
            $('#edit_deny_'+ idx).attr('checked', false);
          }
          else {
            $('#edit_allow_'+ idx).attr('checked', false);
            $('#edit_deny_'+ idx).attr('checked', true);
          }
          $('[name="from['+ idx +']"]').val(accessfrom);
          idx++;
        }
      });
      if(response.htaccess != "NaN") {
        $('#htaccess-row').find('[name="htaccess"]').text(response.htaccess).attr("disabled", false);
        $('#edit_enable_htaccess').prop("checked", true);
      }
      $(document).on('click', '#edit_enable_htaccess', function() {
        if(! $('#edit_enable_htaccess').is(':checked')) {
          $('#vhostEditForm').append('<input type="hidden" name="htaccess" value="NaN">');
        }
        else {
          $('#vhostEditForm').find('[name="htaccess"][value="NaN"]').remove();
        }
      });
      bootbox
        .dialog({
          title: '<strong>Edycja witryny: ' + response.ServerName + '</strong>',
          message: $('#vhostEditForm'),
          show: false
        })
        .on('shown.bs.modal', function() {
          $(this).attr("id", "vhost-modal");
          $('#vhostEditForm')
            .show()
        })
        .on('hide.bs.modal', function(e) {
          $('#vhostEditForm > #sa-group-new').remove();
          $('.input-group-btn .fa-arrow-down').removeClass("fa-arrow-down").addClass("fa-arrow-up");
          $('.edit-apache-conf').unbind('click');
          $('.new-server-alias').unbind('click');
          $('#vhostEditForm')[0].reset();
          $('#sa').prop('disabled', true);
          $('#vhostEditForm').hide().appendTo('body');
          $('#dual-list-box-optedit').find('select.selected > option').each(function() {
            var id = $(this).val();
            var text = $(this).text();
            $('#dual-list-box-optedit .selected > option[value="'+ id +'"]').remove();
            $('#dual-list-box-optedit .unselected').append($('<option>', {
              value: id,
              text: text
            }));
            $('#htaccess-row').find('[name="htaccess"]').text("").attr("disabled", "disabled");
            $('#edit_enable_htaccess').prop("checked", false);
          });
          $('.access-rule').remove();
          i = 0;
        })
        .modal('show');
    });
  });
  $(document).on('click', '#editvhost-btn', function() {

    var id = $('#vhostEditForm').find('[name="id"]').val();
    var sa = [];
    var opts = [];
    var htaccess;
    if($('#edit_enable_htaccess').is(':checked')) {
      htaccess = $('#vhostEditForm').find('[name="htaccess"]').val();
    }
    else {
      htaccess = "NaN";
    }
    var serverid = $('#vhostEditForm').find('[name="serverid"]').val();

    if(!htaccess) {
      htaccess = "NaN"
    }

    if($('#enable_sa').is(':checked')) {
      sa.push($('#sa').val());
      $('#sa-new').each(function() {
        sa.push($(this).val());
      });
    }
    else {
      sa.push("NaN");
    }

    $('#dual-list-box-optedit').find('select.selected > option').each(function() {
      opts.push($(this).val());
    });
    var access_order = $('[name="edit_access_order"]:checked').val();
    var allow = [];
    var fromhost = [];
    $('#edit_access-list-details > .form-group').each(function() {
      allow.push($(this).find('input[type="radio"]:checked').val());
      fromhost.push($(this).find('input[type="text"]').val());
    });
    $.ajax({
      url: '/apache.php?edit=' + id,
      method: 'POST',
      data: {serverid, sa, opts, htaccess, access_order, allow, fromhost},
      success: function() {
        $.bootstrapGrowl(
          'Konfiguracja vhosta zapisana',
          {
            type: 'success',
            align: 'center',
            offset: { from: 'top', amount: 55},
            width: 500
          }
        );
      },
      error: function() {
        $.bootstrapGrowl(
          'Blad aktualizaji konfiguracji',
          {
            type: 'danger',
            align: 'center',
            offset: { from: 'top', amount: 55},
            width: 500
          }
        );
      }
    }).success(function() {
      $('.modal').hide();
      $('.modal-backdrop').hide();
      $('#sa-group-new').remove();
      $('.input-group-btn .fa-arrow-down').removeClass("fa-arrow-down").addClass("fa-arrow-up");
      $('.edit-apache-conf').unbind('click');
      $('.new-server-alias').unbind('click');
      $('#vhostEditForm')[0].reset();
      $('#sa').prop('disabled', true);
      $('#vhostEditForm').hide().appendTo('body');
      $('#dual-list-box-optedit').find('select.selected > option').each(function() {
        var id = $(this).val();
        var text = $(this).text();
        $('select.selected > option[value="'+ id +'"]').remove();
        $('select.unselected').append($('<option>', {
          value: id,
          text: text
        }));
        $('#htaccess-row').find('[name="htaccess"]').text("").attr("disabled", "disabled");
        $('#edit_enable_htaccess').prop("checked", false);
      });
      $('.access-rule').remove();
      i = 0;
    });
  });
  $(document).on('click', '#edit-cancel', function() {
    $('.modal').hide();
    $('.modal-backdrop').hide();
    $('#vhostEditForm > #sa-group-new').remove();
    $('.input-group-btn .fa-arrow-down').removeClass("fa-arrow-down").addClass("fa-arrow-up");
    $('.edit-apache-conf').unbind('click');
    $('.new-server-alias').unbind('click');
    $('#vhostEditForm')[0].reset();
    $('#sa').prop('disabled', true);
    $('#vhostEditForm').hide().appendTo('body');
    $('#dual-list-box-optedit select.selected > option').each(function() {
      var id = $(this).val();
      var text = $(this).text();
      $('#dual-list-box-optedit .selected > option[value="'+ id +'"]').remove();
      $('#dual-list-box-optedit .unselected').append($('<option>', {
        value: id,
        text: text
      }));
      $('#htaccess-row').find('[name="htaccess"]').text("").attr("disabled", "disabled");
      $('#edit_enable_htaccess').prop("checked", false);
    });
    $('.access-rule').remove();
    i = 0;
  });
  $(document).on('click', '#sa-arrow-btn', function() {
    if($('#sa-arrow').hasClass("fa-arrow-down")) {
      $('.div-sa-new').hide();
      $('#sa-arrow').removeClass("fa-arrow-down").addClass("fa-arrow-up");
    }
    else {
      $('.div-sa-new').show();
      $('#sa-arrow').removeClass("fa-arrow-up").addClass("fa-arrow-down");
    }
  });
  $(document).on('click', '#access-expandBtn, #edit_access-expandBtn', function() {
    if($('#expandControl, edit_expandControl').hasClass("fa-angle-down")) {
      $('#access-list-details, #edit_access-list-details').hide();
      $('#expandControl, #edit_expandControl').removeClass("fa-angle-down").addClass("fa-angle-up");
    }
    else {
      $('#access-list-details, #edit_access-list-details').show();
      $('#expandControl, #edit_expandControl').removeClass("fa-angle-up").addClass("fa-angle-down");
    }
  });
  var rulesCount = 1;
  $(document).on('click', '.vhost-access-add', function() {
    $('#access-list-details, #edit_access-list-details').append(
      '<div class="form-group access-rule">' +
        '<div class="row col-sm-offset-2" id="accesslist">' +
          '<div class="col-sm-4">' +
            '<label class="radio-inline" id="allowfrom">' +
              '<input type="radio" id="allow_'+ rulesCount +'" name="allow['+ rulesCount +']" value="1" checked> Allow' +
            '</label>' +
            '<label class="radio-inline">' +
              '<input type="radio" id="deny_'+ rulesCount +'" name="allow['+ rulesCount +']" value="0"> Deny' +
            '</label>' +
          '</div>' +
          '<label for="from" class="col-sm-2 control-label access-from-label">From</label>' +
          '<div class="col-sm-5">' +
            '<input type="text" class="form-control access-from-input" id="from" name="from['+ rulesCount +']" value="all">' +
          '</div>' +
          '<div>' +
            '<span class="glyphicon glyphicon-minus vhost-access vhost-access-del" aria-hidden="true"></span>' +
          '</div>' +
        '</div>' +
      '</div>'
    );
    rulesCount++;
  });
  $(document).on('click', '.vhost-access-del', function() {
    $(this).closest('.access-rule').remove();
  });
  $('#apache-config a').on('click', function(e) {
    e.preventDefault();
    $('#wwwusers').hide();
    $('#wwwconfig').show();
  });
  $('#www-users a').on('click', function(e) {
    e.preventDefault();
    $('#wwwconfig').hide();
    $('#wwwusers').show();
  });
  $('.apache-section').ready(function() {
    $(this).find('#wwwconfig').show();
  });
  $(document).on('click', '#new-vhost > button', function() {
    var id = $(this).attr('data-id');
    $('.select-htusers').multiselect({
      nonSelectedText: 'Wybierz konta '
    });
    $('[title="Wybierz konta "]').parent().attr("id", "div-htusers");
    bootbox
      .dialog({
        title: '<strong>Nowa strona</strong>',
        message: $('#addvhost'),
        show: false
      })
      .on('shown.bs.modal', function() {
        $(this).attr("id", "vhost-modal");
        if($('#password_enable').is(':checked')) {
          $('button[title="Wybierz konta "]').attr('disabled', false);
        }
        else {
          $('button[title="Wybierz konta "]').attr('disabled', true);
        }
        $('#addvhost')
          .show()
      })
      .on('hide.bs.modal', function(e) {
        $('#addvhost').hide().appendTo('body');
      })
      .modal('show');
  });
  $(document).on('click', '#new-htuser > button', function() {
    var id = $(this).attr('data-id');
    bootbox
      .dialog({
        title: '<strong>Nowe konto</strong>',
        message: $('#new-htuser-form'),
        show: false
      })
      .on('shown.bs.modal', function() {
        $('#new-htuser-form').show();
        $('#new-htuser-form').validator();
      })
      .on('hide.bs.modal', function(e) {
        $('#new-htuser-form').hide().appendTo('body');
      })
      .modal('show');
  });
  $(document).on('click', '#addhtuser-btn', function(e) {
    e.preventDefault();
    var form = $('#new-htuser-form');
    var serverid = form.find('[name="serverid"]').val();
    $.ajax({
      url: '/apache.php?addhtuser',
      method: 'POST',
      data: form.serializeArray(),
      success: function() {
        $.bootstrapGrowl(
          'Użytkownik został dodany',
          {
            type: 'success',
            align: 'center',
            offset: { from: 'top', amount: 55},
            width: 500
          }
        );
      }
  }).success(function(response) {
    var tr = $('#wwwusers').find('tr').last();
    var div = $('#wwwusers > div:nth-child(2)');
    $('.modal').hide();
    $('.modal-backdrop').hide();
    if(tr.length) {
      tr.after(
        '<tr>' +
          '<td>' +
            response.login +
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
    }
    else {
      div.find('h5').remove();
      div.append(
        '<table class="table table-hover" id="vhost-table">' +
          '<thead>' +
            '<th>Login</th><th class="button-cell">Akcja</th>' +
          '</thead>' +
          '<tbody>' +
            '<tr>' +
              '<td>' +
                response.login +
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
            '</tr>' +
          '</tbody>' +
        '</table>'
      );
    }
    });
  });
});
