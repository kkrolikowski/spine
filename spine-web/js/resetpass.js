$(document).ready(function() {
  $(document).on('click', '.reset-pass', function() {
    var userid = $(this).attr('data-id');
    $.ajax({
      url: "/sysusers.php?resetpass=" + userid,
      method: "GET",
      success: function(resp) {
        alertify.success(resp);
      }
    });
  });
});
