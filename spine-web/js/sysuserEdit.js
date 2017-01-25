$(document).ready(function() {
  $(document).on('click', '.edit-user', function() {
     var id = $(this).attr('data-id');
     $.ajax({
       url: "/sysusers.php?edit=" + id,
       method: "GET"
     }).success(function(response) {
       $('#edit-sysuser-form')
         .find('[name="id"]').val(response.id).end()
         .find('[name="fullname_edit"]').val(response.fullname).end()
         .find('[name="email_edit"]').val(response.email).end()
         .find('[name="login_edit"]').val(response.login).end();
       $('#password_edit, #confirm_edit').attr('readonly', 'readonly');
       $('#password_edit').attr('placeholder', 'Click here to change password');

       var form = $('#edit-sysuser-form');
       var account_status_checkbox = form.find('[name="isActive_edit"]');
       response.active == 1 ? account_status_checkbox.prop('checked', true) : account_status_checkbox.prop('checked', false);

       var shell_access_checkbox = form.find('[name="shell_edit"]');
       response.shell == 1 ? shell_access_checkbox.prop('checked', true) : shell_access_checkbox.prop('checked', false);

       var sudo_access_checkbox = form.find('[name="sudo_edit"]');
       response.sudo == 1 ? sudo_access_checkbox.prop('checked', true) : sudo_access_checkbox.prop('checked', false);

       if(response.expire > 0) {
          d = new Date(0);
          d.setUTCSeconds(response.expire);
          day = d.getDate();
          month = d.getMonth();
          year = d.getFullYear();

          if(day < 10)
           day = "0"+day;
          if(month < 10)
           month = "0" + month;

          datestring = day + "/" + month + "/" + year;
          form.find('[name="expEnable_edit"]').prop('checked', true);
          form.find('[name="expdate_edit"]')
            .prop("disabled", false)
            .val(datestring).end();

        }
        else {
          form.find('[name="expEnable_edit"]').prop('checked', false)
        }
        if(response.sshkeys != "NaN") {
          form.find('[name="sshkey_enable_edit"]').prop("checked", true);
          var ssh = response.sshkeys;
          var sshkey = ssh.split(",");
          var div = $('#edit_advanced_settings');
          for (var i = 0; i < sshkey.length; i++) {
            if(i == 0) {
              var glyph = 'glyphicon-plus';
              form.find('[name="sshkey_edit[0]"]').prop('disabled', false).val(sshkey[i]);
            }
            else {
              var glyph = 'glyphicon-minus';
              div.append(
                '<div class="row additional-key">' +
                  '<div class="col-xs-4"></div>' +
                  '<div class="col-xs-6">' +
                    '<div class="form-group" id="sshkey_edit">' +
                      '<input type="text" placeholder="ssh public key" class="form-control" ' +
                      'data-minlength="3" data-error="Pole nie może być puste" ' +
                      'name="sshkey_edit['+ i +']" value="'+ sshkey[i] +'" />' +
                    '</div>' +
                  '</div>' +
                  '<div class="col-xs-1">' +
                      '<span class="glyphicon '+ glyph +'"></span>' +
                  '</div>' +
                '</div>'
              );
            }
          }
        }
        else {
          form.find('[name="sshkey_enable_edit"]').prop("checked", false);
        }
       bootbox
         .dialog({
           title: '<strong>Edit account: ' + response.login + '</strong>',
           message: form,
           show: false
         })
         .on('shown.bs.modal', function() {
           $(this).attr("id", "vhost-modal");
           form.show()
         })
         .on('hide.bs.modal', function(e) {
           form[0].reset();
           $(".additional-key").remove();
           $('[name="sshkey_edit[0]"]').prop('disabled', true);
           $('[name="expdate_edit"]').prop('disabled', true);
           $('.edit-user').unbind('click');
           form.hide().appendTo('body');
         })
         .modal('show');
     });
     $(document).on('click', '#password_edit', function() {
       if($('#password_edit, #confirm_edit').is('[readonly]')) {
         $('#password_edit, #confirm_edit').removeAttr('readonly');
         $('#password_edit').removeAttr('placeholder');
         $('#password_edit').attr('placeholder', 'Password');
         $('#confirm_edit').attr('placeholder', 'Confirm');
       }
       else {
         $('#password_edit, #confirm_edit').attr('readonly', 'readonly');
         $('#password_edit, #confirm_edit').removeAttr('placeholder');
         $('#password_edit').attr('placeholder', 'Click here to change password');
       }
     });
   });
   $(document).on('click', '[name="expEnable_edit"]', function() {
     $('#expiration_edit > input').attr('disabled', ! this.checked);
   });
   $('#expiration_edit').datetimepicker({
     locale: 'pl-PL',
     format: 'DD/MM/YYYY'
   });
   $(document).on('click', '#edit_cancel', function() {
     $('.modal').hide();
     $('.modal-backdrop').hide();
     $('#edit-sysuser-form')[0].reset();
     $(".additional-key").remove();
     $('[name="sshkey_edit[0]"]').prop('disabled', true);
     $('[name="expdate_edit"]').prop('disabled', true);
     $('.edit-user').unbind('click');
     form.hide().appendTo('body');
   });
});
