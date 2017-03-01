$(document).ready(function() {
  $("#smtp_auth_enable").on('click', function() {
    if(this.checked) {
      $('.advanced_smtp_settings').show();
    }
    else {
      $('.advanced_smtp_settings').hide();
      $('#smtplogin').val("");
      $('#smtppass').val("");
      $("#smtp_ssl").prop('checked', false);
    }
  });
  $('#smtp_ssl').on('click', function() {
    if(this.checked)
      $('#smtpport').val("465");
    else
      $('#smtpport').val("25");
  });
  $('#smtp_settings_save').on('click', function() {
    var form = $('#settings_smtp');
    $.ajax({
      url: "/settings.php?smtp",
      method: "POST",
      data: form.serializeArray(),
      success: function() {
        alertify.success("SMTP configuration saved");
      }
    });
  });
});
