$(document).ready(function() {
  $('.rmdb').on('click', function() {
    var dbid = $(this).attr('data-id');
    var dbname =  $(this).closest('tr').find('td').eq(0).html();
    var row = $(this).closest('tr');
    var message = "Czy na pewno chcesz usunac baze " + dbname + "?";
    alertify.confirm(message, function(e) {
      if(e) {
        $.ajax({
          url: "/databases.php?rmdb=" + dbid,
          method: "GET",
          success: function() {
            alertify.success("Baza "+ dbname +" zostala skasowana");
            row.remove();
            $('#seldb > option[value="'+ dbid +'"]').remove();
            $('#db-permissions-table').find('td[data-dbid="'+dbid+'"]').closest('tr').remove();
        }
      });
      }
      else {
        // nie kasujemy
      }
    });
  });
});
