$(document).ready(function() {
  $(document).on('click', '.rm-sysuser', function() {
    var id = $(this).attr('data-id');
    var serverid = $(this).attr('data-serverid');
    var tr = $(this).closest('tr');
    var login = tr.find('td').first().html();
    var message = "Czy na pewno chcesz usunac konto " + login + "?";
    alertify.confirm(message, function(e) {
      if(e) {
        $.ajax({
          url: "/sysusers.php?rmuser=" + id,
          method: "POST",
          data: {serverid},
          success: function() {
            alertify.success("Konto zostało sakasowane");
            tr.remove();
          },
          error: function(xhr) {
            alertify.error(xhr.getResponseHeader('X-Message'));
          }
        });
      }
      else {
        // nie kasujemy
      }
    });
  });
});
