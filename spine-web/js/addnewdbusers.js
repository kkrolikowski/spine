$(document).ready(function() {

  // tab switching
  $('#db-users a').on('click', function(e) {
    e.preventDefault();
    $('#dbconfig').hide();
    $('#dbusers').show();
  });
});
