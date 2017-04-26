$(document).ready(function() {
  $('.rmdb').on('click', function() {
    var dbid = $(this).attr('data-id');
    var dbname = $(this).closest('tr > td:nth-child(0)').html();
    var message = "Czy na pewno chcesz usunac baze " + dbname + "?";
    alertify.confirm(message, function(e) {
      if(e) {
        $.ajax({
          url: "/databases.php?rmdb=" + dbid,
          method: "GET",
          success: function() {
            alertify.success("Baza "+ dbname +" zostala skasowana");
            $(this).closest('tr').remove();
            $('#dbname > option:value="'+ dbid +'"').remove();
        });
      }
      else {
        // nie kasujemy
      }
  });
});
