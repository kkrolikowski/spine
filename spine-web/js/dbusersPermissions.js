function TableMouseOver() {
  $('#db-permissions-table > tbody > tr').each(function() {
    $(this).mouseover(function() {
      $(this).css( 'cursor', 'pointer' );
    })
  });
}
function getPermissionsInfo() {
  var id = $(this).closest('tr').find('button').attr('data-id');
  $.ajax({
    url: '/databases.php?getperm=' + id,
    method: 'GET'
  }).success(function(r) {
    $('#seldb').find('option').removeAttr("selected");
    $('#seldb option[value="'+ r.dbname +'"]').attr('selected', 'selected');
    $('#seldb').val(r.dbname);

    $('#seldbuser').find('option').removeAttr("selected");
    $('#seldbuser option[value="'+ r.dblogin +'"]').attr('selected', 'selected');
    $('#seldbuser').val(r.dblogin);

    var ul = $('#setDBperms div:nth-child(3)').find('.select2-selection__rendered');
    ul.empty();
    $.each(r.grants, function(i, g) {
      var gr = g.toUpperCase();
      ul.append(
        '<li class="select2-selection__choice" title="'+ gr +'"><span class="select2-selection__choice__remove" role="presentation">×</span>'+ gr +'</li>'
      );
    });
    var selectPerms = $('#selperms');
    selectPerms.val(r.grants).trigger('change');
  });
}
$(document).ready(function() {
  // tab switching
  $('#db-privs a').on('click', function(e) {
    e.preventDefault();
    $('#dbusers').hide();
    $('#dbprivs').show();
    $('#dbconfig').hide();
  });

  // permissions select field
  var selectPerms = $('#selperms');
  selectPerms.select2({
    placeholder: "Click here or...",
    width: '100%',
    minimumResultsForSearch: Infinity
  });
  $('#select-all-perms').on('click', function() {
    selectPerms.val(["select", "insert", "update", "delete"]).trigger('change');
  });
  $('#clear-all-perms').on('click', function() {
    selectPerms.val(null).trigger('change');
  });

  // save changes
  $('#save-db-perms').on('click', function(e) {
    e.preventDefault();
    var form = $('#setDBperms');
    $.ajax({
      url: '/databases.php?addperms',
      method: 'POST',
      data: form.serializeArray()
    }).success(function(r) {
      var table = $('#db-permissions-table > tbody');
      table.empty();
      $.each(r, function(i, item) {
        var grants = item.grants.toString();
        var last = grants.slice(-1);
        if(last == ',')
          grants = grants.slice(0, -1);
        table.append(
          '<tr>'+
            '<td>'+ item.dbname +'</td><td>'+ item.dbuser +'</td><td>'+ grants +'</td>'+
            '<td align="right"><button type="button" class="btn btn-danger btn-sm rm-permission" data-id="'+ i +'">Remove</button></td>'+
          '</tr>'
        );
      });
      $('#db-permissions-table').ready(TableMouseOver);
      $('#db-permissions-table > tbody > tr > td:nth-child(-n+3)').on('click', getPermissionsInfo);
    });
  });
  $('#db-permissions-table').ready(TableMouseOver);
  $('#db-permissions-table > tbody > tr > td:nth-child(-n+3)').on('click', getPermissionsInfo);
  $('.rm-permission').on('click', function() {
    var id = $(this).attr('data-id');
    var dbname = $(this).closest('tr').find('td').eq(0).html();
    var dbuser = $(this).closest('tr').find('td').eq(1).html();
    var row = $(this).closest('tr');
    var message = "Kasuje uprawnienia do bazy "+ dbname +" dla "+ dbuser;
    alertify.confirm(message, function(e) {
      if(e) {
        $.ajax({
          url: "/databases.php?rmperm=" + id,
          method: "GET",
          success: function() {
            alertify.success("Uprawnienia zostaly zmienione");
            row.remove();
          }
        });
      }
      else {
        // nie kasujemy
      }
    });
  });
  $('.ch-dbuser-pass').on('click', function() {
    var login = $(this).closest('td').prev('td').html();
    var dbid = $(this).attr('data-id');
    bootbox
      .dialog({
        title: '<strong>Login: '+ login +'</strong>',
        message: $('#chDBuserPass'),
        show: false
      })
      .on('shown.bs.modal', function() {
        $('#chDBuserPass').append(
          '<input type="hidden" name="dbid" value="'+ dbid +'">'
        );
        $('#chDBuserPass')
          .show()
      })
      .on('hide.bs.modal', function(e) {
        $('#chDBuserPass').hide().appendTo('body');
      })
      .modal('show');
  });
  $('#chDBuserPass-cancel').on('click', function() {
    $('.modal').hide();
    $('.modal-backdrop').hide();
  });
  $('#chDBuserPass-btn').on('click', function() {
    // future code
  });
});
