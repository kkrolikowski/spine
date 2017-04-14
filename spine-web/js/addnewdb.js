$(document).ready(function() {

/*
        Display form section
*/

  $('#new-db > button').on('click', function() {
    var serverid = $(this).attr('data-id');

    // get vhosts available to connect with database
    $.ajax({
      url: '/apache.php?vhlist=' + serverid,
      method: 'GET'
    }).success(function(rsp) {
      $.each(rsp, function(id, sn) {
        $('#vhlist').append('<option value="'+ id +'">'+ sn +'</option>');
      });

      // raise a modal window with new DB form
      bootbox
        .dialog({
          title: '<strong>New Database</strong>',
          message: $('#newDB'),
          show: false
        })
        .on('shown.bs.modal', function() {
          $('#newDB')
            .show()
        })
        .on('hide.bs.modal', function(e) {
          $('#vhlist option:gt(0)').remove();
          $('#newDB').hide().appendTo('body');
        })
        .modal('show');
    });
  });

  // cancel action
  $('#adddb-cancel').on('click', function() {
    $('.modal').hide();
    $('.modal-backdrop').hide();
    $('#vhlist option:gt(0)').remove();
  });

/*
          Add / retrieve data section
*/
  $('#adddb-btn').on('click', function() {
    $.ajax({
      url: '/databases.php?add',
      method: 'POST',
      data: $('#newDB').serializeArray(),
      success: function() {
        alertify.success("Database added");
      }
    }).success(function(rsp) {
      $('.modal').hide();
      $('.modal-backdrop').hide();
      if($('#dbconfig h5').length) {
        $('#dbconfig h5').remove();
        $('#dbconfig > div:nth-child(2)').append(
          '<table class="table table-hover" id="db-table" data-id="'+ rsp.serverid +'">' +
            '<thead>' +
              '<tr>' +
                '<th>Baza</th><th>Strona WWW</th><th>Akcja</th>' +
              '</tr>' +
            '</thead>' +
            '<tbody>' +
              '<tr>' +
                '<td>'+ rsp.dbname +'</td>' +
                '<td><a href="http://'+ rsp.vhost +'/" target="_blank">'+ rsp.vhost +'</a></td>' +
                '<td class="button-cell">' +
                  '<div class="btn-group">' +
                    '<button type="button" class="btn btn-danger rmdb" data-id="'+ rsp.id +'" data-serverid="'+ rsp.serverid +'">Usuń</button>' +
                    '<button type="button" class="btn btn-danger dropdown-toggle" data-toggle="dropdown" aria-haspopup="true" aria-expanded="false">' +
                      '<span class="caret"></span>' +
                      '<span class="sr-only">Toggle Dropdown</span>' +
                    '</button>' +
                    '<ul class="dropdown-menu">' +
                      '<li><a href="#" data-id="'+ rsp.id +'" class="editdb">Edytuj</a></li>' +
                    '</ul>' +
                  '</div>' +
                '</td>' +
              '</tr>' +
            '</tbody>' +
          '</table>'
        );
      }
      else {
        var vhostCell;
        if(rsp.vhost == "None") {
          vhostCell = '<td>'+ rsp.vhost +'</td>';
        }
        else {
          vhostCell = '<td><a href="http://'+ rsp.vhost +'/" target="_blank">'+ rsp.vhost +'</a></td>';
        }
        $('#db-table > tbody').append(
          '<tr>' +
            '<td>'+ rsp.dbname +'</td>' +
            vhostCell +
            '<td class="button-cell">' +
              '<div class="btn-group">' +
                '<button type="button" class="btn btn-danger rmdb" data-id="'+ rsp.id +'" data-serverid="'+ rsp.serverid +'">Usuń</button>' +
                '<button type="button" class="btn btn-danger dropdown-toggle" data-toggle="dropdown" aria-haspopup="true" aria-expanded="false">' +
                  '<span class="caret"></span>' +
                  '<span class="sr-only">Toggle Dropdown</span>' +
                '</button>' +
                '<ul class="dropdown-menu">' +
                  '<li><a href="#" data-id="'+ rsp.id +'" class="editdb">Edytuj</a></li>' +
                '</ul>' +
              '</div>' +
            '</td>' +
          '</tr>'
        );
      }
    });
  });
});
