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
      var newrow = '<tr><td>'+ r.dbname +'</td><td>'+ r.dbuser +'</td><td>'+ r.grants +'</td></tr>';
      var row = table.find('td:contains("'+ r.dbname +'")').parent();
      var dbname_cell = table.find('td:contains("'+ r.dbname +'")');
      var dbuser_cell = dbname_cell.closest('td:contains("'+ r.dbuser +'")');
      if(dbname_cell.length && dbuser_cell.length)
        row.replaceWith(newrow);
      else
        table.append(newrow);
    });
  });
});
