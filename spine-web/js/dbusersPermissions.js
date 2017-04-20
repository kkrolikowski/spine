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
          '<tr><td>'+ item.dbname +'</td><td>'+ item.dbuser +'</td><td>'+ grants +'</td></tr>'
        );
      });
    });
  });
});
