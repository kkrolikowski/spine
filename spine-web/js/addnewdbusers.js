$(document).ready(function() {

  // tab switching
  $('#db-users a').on('click', function(e) {
    e.preventDefault();
    $('#dbconfig').hide();
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
});
