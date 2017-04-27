$(document).ready(function() {

/*
        Display form section
*/

  // tab switching
  $('#db-config a').on('click', function(e) {
    e.preventDefault();
    $('#dbusers').hide();
    $('#dbprivs').hide();
    $('#dbconfig').show();
  });

  $('#new-db > button').on('click', function() {
    var serverid = $(this).attr('data-id');

    // get vhosts available to connect with database
    $.ajax({
      url: '/apache.php?vhlist=' + serverid,
      method: 'GET'
    }).success(function(rsp) {
      if(rsp) {
        $.each(rsp, function(id, sn) {
          $('#vhlist').append('<option value="'+ id +'">'+ sn +'</option>');
        });
      }
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
    $('.modal').modal('hide');
    $('#vhlist option:gt(0)').remove();
    $('#newDB').hide().appendTo('body');
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
      },
      error: function(xhr) {
        alertify.error(xhr.getResponseHeader('X-Message'));
      }
    }).success(function(rsp) {
      $('.modal').modal('hide');
      $('.modal-backdrop').hide();
      $('#dbname').val("");
      $('#vhlist option:gt(0)').remove();
      $('#newDB').hide().appendTo('body');
      $('#vhlist').find('option:contains("None")').attr('selected', 'selected');
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
                  '<button type="button" class="btn btn-danger rmdb" data-id="'+ rsp.id +'" data-serverid="'+ rsp.serverid +'">Usuń</button>' +
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
          button = '<button type="button" class="btn btn-danger rmdb" data-id="'+ rsp.id +'" data-serverid="'+ rsp.serverid +'">Usuń</button>';
        }
        else {
          vhostCell = '<td><a href="http://'+ rsp.vhost +'/" target="_blank">'+ rsp.vhost +'</a></td>';
          button = '<button type="button" class="btn btn-danger rmdb" data-id="'+ rsp.id +'" data-serverid="'+ rsp.serverid +'" disabled>Usuń</button>';
        }
        $('#db-table > tbody').append(
          '<tr>' +
            '<td>'+ rsp.dbname +'</td>' +
            vhostCell +
            '<td class="button-cell">'+ button +'</td>' +
          '</tr>'
        );
      }
      $('#seldb').append('<option value="'+ rsp.id +'">'+ rsp.dbname +'</option>');
    });
  });
});
