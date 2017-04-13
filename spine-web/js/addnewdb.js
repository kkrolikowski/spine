$(document).ready(function() {
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
});
