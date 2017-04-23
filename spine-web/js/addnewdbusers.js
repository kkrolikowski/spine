$(document).ready(function() {

  // tab switching
  $('#db-users a').on('click', function(e) {
    e.preventDefault();
    $('#dbconfig').hide();
    $('#dbprivs').hide();
    $('#dbusers').show();
  });

  // raise a modal window
  $('#new-dbuser > button').on('click', function() {
    bootbox
      .dialog({
        title: '<strong>New Database User</strong>',
        message: $('#newDBuser'),
        show: false
      })
      .on('shown.bs.modal', function() {
        $('#newDBuser')
          .show()
      })
      .on('hide.bs.modal', function(e) {
        $('#newDB').hide().appendTo('body');
      })
      .modal('show');
  });

  // cancel action
  $('#adddbuser-cancel').on('click', function() {
    $('.modal').hide();
    $('.modal-backdrop').hide();
  });

  // add new database user
  $('#adddbuser-btn').on('click', function(e) {
    e.preventDefault();
    $.ajax({
      url: '/databases.php?adduser',
      method: 'POST',
      data: $('#newDBuser').serializeArray(),
      success: function() {
        alertify.success("Account added");
      },
      error: function(xhr) {
        alertify.error(xhr.getResponseHeader('X-Message'));
      }
    }).success(function(r) {
      $('.modal').hide();
      $('.modal-backdrop').hide();
      $('#dblogin').val("");
      $('#dbpass').val("");
      $('#dbpass-confirm').val("");
      if ($('#dbusers h5').length) {
        $('#dbusers h5').remove();
        $('#dbusers > div:nth-child(2) > div').append(
          '<table class="table" id="db-users-table">' +
            '<thead>' +
              '<th>Login</th><th></th>' +
            '</thead>' +
            '<tbody>' +
              '<tr>' +
                '<td>'+ r.login +'</td>' +
                '<td align="right">' +
                  '<div class="btn-group">' +
                    '<button type="button" class="btn btn-danger rmddbuser" data-id="'+ r.id +'" data-serverid="'+ r.serverid +'">Usuń</button>' +
                    '<button type="button" class="btn btn-danger dropdown-toggle" data-toggle="dropdown" aria-haspopup="true" aria-expanded="false">' +
                      '<span class="caret"></span>' +
                      '<span class="sr-only">Toggle Dropdown</span>' +
                    '</button>' +
                    '<ul class="dropdown-menu">' +
                      '<li><a href="#" data-id="'+ r.id +'" class="ch-dbuser-pass">Change password</a></li>' +
                    '</ul>' +
                  '</div>' +
                '</td>' +
              '</tr>' +
            '</tbody>' +
          '</table>'
        );
      }
      else {
        $('#db-users-table > tbody').append(
          '<tr>' +
            '<td>'+ r.login +'</td>' +
            '<td align="right">' +
              '<div class="btn-group">' +
                '<button type="button" class="btn btn-danger rmddbuser" data-id="'+ r.id +'" data-serverid="'+ r.serverid +'">Usuń</button>' +
                '<button type="button" class="btn btn-danger dropdown-toggle" data-toggle="dropdown" aria-haspopup="true" aria-expanded="false">' +
                  '<span class="caret"></span>' +
                  '<span class="sr-only">Toggle Dropdown</span>' +
                '</button>' +
                '<ul class="dropdown-menu">' +
                  '<li><a href="#" data-id="'+ r.id +'" class="ch-dbuser-pass">Change password</a></li>' +
                '</ul>' +
              '</div>' +
            '</td>' +
          '</tr>'
        );
      }
      $('#seldbuser').append('<option value="'+ r.id +'">'+ r.login +'</option>');
    });
  });
});
