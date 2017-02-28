$(document).ready(function() {
  $("#smtp_auth_enable").on('click', function() {
    if(this.checked) {
      $('.advanced_smtp_settings').show();
    }
    else {
      $('.advanced_smtp_settings').hide();
    }
  });
  $('#smtp_ssl').on('click', function() {
    if(this.checked)
      $('#smtpport').val("465");
    else
      $('#smtpport').val("25");
  });
});
