<?php /* Smarty version Smarty-3.1-DEV, created on 2017-03-02 00:29:48
         compiled from "./templates/main.tpl" */ ?>
<?php /*%%SmartyHeaderCode:32474853458970837d95589-41320044%%*/if(!defined('SMARTY_DIR')) exit('no direct access allowed');
$_valid = $_smarty_tpl->decodeProperties(array (
  'file_dependency' => 
  array (
    '48ad11f68be179e1830edf3ce56b82b0712baeff' => 
    array (
      0 => './templates/main.tpl',
      1 => 1488410984,
      2 => 'file',
    ),
  ),
  'nocache_hash' => '32474853458970837d95589-41320044',
  'function' => 
  array (
  ),
  'version' => 'Smarty-3.1-DEV',
  'unifunc' => 'content_58970837ecce40_21197956',
  'variables' => 
  array (
    'wwwuser' => 0,
    'id' => 0,
    'user' => 0,
    'htpasswd' => 0,
    'htuser' => 0,
    'HostMenu' => 0,
    'srv' => 0,
    'basicInfo' => 0,
    'smtp_settings' => 0,
    'sysinfo' => 0,
    'hostsrv' => 0,
    'sname' => 0,
    'state' => 0,
    'sysuser' => 0,
    'info' => 0,
    'userid' => 0,
    'EmptySiteList' => 0,
    'websites' => 0,
    'website' => 0,
    'EmptyUserList' => 0,
    'htusers' => 0,
    'login' => 0,
    'HostTotalCount' => 0,
    'FreeTotalGB' => 0,
    'websiteCount' => 0,
    'SrvHDDFree' => 0,
    'hostname' => 0,
    'allHostStatus' => 0,
    'Logs' => 0,
    'entry' => 0,
  ),
  'has_nocache_code' => false,
),false); /*/%%SmartyHeaderCode%%*/?>
<?php if ($_valid && !is_callable('content_58970837ecce40_21197956')) {function content_58970837ecce40_21197956($_smarty_tpl) {?><?php if (!is_callable('smarty_function_math')) include '/usr/share/php/smarty3/plugins/function.math.php';
?><!DOCTYPE html>
<html lang="en">

<head>

    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <meta name="description" content="">
    <meta name="author" content="">

    <title>Spine</title>

        <!-- jQuery -->
    <script src="../bower_components/jquery/dist/jquery.min.js"></script>

    <!-- Bootstrap Core JavaScript -->
    <script src="../bower_components/bootstrap/dist/js/bootstrap.min.js"></script>

    <!-- Metis Menu Plugin JavaScript -->
    <script src="../bower_components/metisMenu/dist/metisMenu.min.js"></script>

    <!-- Custom Theme JavaScript -->
    <script src="../dist/js/sb-admin-2.js"></script>
    <!-- Bootstrap Core CSS -->
    <link href="../bower_components/bootstrap/dist/css/bootstrap.min.css" rel="stylesheet">

    <!-- MetisMenu CSS -->
    <link href="../bower_components/metisMenu/dist/metisMenu.min.css" rel="stylesheet">

    <!-- Timeline CSS -->
    <link href="../dist/css/timeline.css" rel="stylesheet">

    <!-- Custom CSS -->
    <link href="../dist/css/sb-admin-2.css" rel="stylesheet">

    <!-- Morris Charts CSS -->
    <link href="../bower_components/morrisjs/morris.css" rel="stylesheet">

    <!-- Custom Fonts -->
    <link href="../bower_components/font-awesome/css/font-awesome.min.css" rel="stylesheet" type="text/css">

    <!-- Bootbox - wyskakujace okienka -->
    <script type="text/javascript" src="https://cdnjs.cloudflare.com/ajax/libs/bootbox.js/4.4.0/bootbox.min.js"></script>

    <!-- Form validator plugin -->
    <script type="text/javascript" src="https://cdnjs.cloudflare.com/ajax/libs/1000hz-bootstrap-validator/0.9.0/validator.min.js"></script>

    <!-- JQuery - easing -->
    <script type="text/javascript" src="https://cdnjs.cloudflare.com/ajax/libs/jquery-easing/1.3/jquery.easing.min.js"></script>

    <!-- Shuttle box plugin -->
    <script src="js/dual-list-box.min.js"></script>

    <!-- HTML5 Shim and Respond.js IE8 support of HTML5 elements and media queries -->
    <!-- WARNING: Respond.js doesn't work if you view the page via file:// -->
    <!--[if lt IE 9]>
        <script src="https://oss.maxcdn.com/libs/html5shiv/3.7.0/html5shiv.js"></script>
        <script src="https://oss.maxcdn.com/libs/respond.js/1.4.2/respond.min.js"></script>
    <![endif]-->

      <!-- Wykres kolowy -->
      <link href="css/jquery.circliful.css" rel="stylesheet" type="text/css" />
      <script src="js/jquery.circliful.min.js"></script>

      <!-- Wizualizacja wolnego miejsca na serwerach -->
      <link href="css/bi-style.css" rel="stylesheet" type="text/css" />
      <script src="js/jquery-barIndicator.js"></script>

      <!-- Zabezpieczenie stron haslem -->
      <script type="text/javascript" src="https://cdnjs.cloudflare.com/ajax/libs/bootstrap-multiselect/0.9.13/js/bootstrap-multiselect.js"></script>
      <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/bootstrap-multiselect/0.9.13/css/bootstrap-multiselect.css">

      <!-- Potwierdzanie akcji -->
      <script type="text/javascript" src="https://cdnjs.cloudflare.com/ajax/libs/alertify.js/0.3.11/alertify.min.js"></script>
      <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/alertify.js/0.3.11/alertify.core.min.css">
      <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/alertify.js/0.3.11/alertify.default.min.css">

      <!-- Customowy skrypt strony i css -->
      <script src="js/spine.js"></script>
      <script src="js/sysuserEdit.js"></script>
      <script src="js/sysuserRemove.js"></script>
      <script src="js/smtp_settings.js"></script>
      <script src="js/resetpass.js"></script>

      <link href="/css/custom.css" rel="stylesheet">

      <!-- Monitoring -->
      <script src="js/monitoring.js"></script>

      <!-- Plugin do logow -->
      <script type="text/javascript" src="https://cdnjs.cloudflare.com/ajax/libs/datatables/1.10.12/js/jquery.dataTables.min.js"></script>
      <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/datatables/1.10.12/css/dataTables.bootstrap.min.css">

      <!-- Wlaczanie / wylaczanie monitorowania hosta -->
      <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/bootstrap-switch/3.3.2/css/bootstrap3/bootstrap-switch.min.css">
      <script type="text/javascript" src="https://cdnjs.cloudflare.com/ajax/libs/bootstrap-switch/3.3.2/js/bootstrap-switch.min.js"></script>

      <!-- Wykresy utylizacji interfejsow sieciowych -->
      <script type="text/javascript" src="https://cdnjs.cloudflare.com/ajax/libs/d3/3.5.17/d3.min.js"></script>
      <script type="text/javascript" src="https://cdnjs.cloudflare.com/ajax/libs/epoch/0.8.4/js/epoch.min.js"></script>
      <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/epoch/0.8.4/css/epoch.min.css" />

      <!-- Wybor daty expiracji konta -->
      <script src="https://cdnjs.cloudflare.com/ajax/libs/moment.js/2.15.2/moment.min.js"></script>
      <script src="https://cdnjs.cloudflare.com/ajax/libs/bootstrap-datetimepicker/4.17.43/js/bootstrap-datetimepicker.min.js"></script>
      <script src="https://cdnjs.cloudflare.com/ajax/libs/moment.js/2.15.2/locale/pl.js"></script>
      <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/bootstrap-datetimepicker/4.17.43/css/bootstrap-datetimepicker.min.css" />


</head>

<body onload="watch()">
  <!-- BEGIN: Sekcja formularzy edycji danych -->
    <!-- BEGIN: Edycja konfiguracji virtualhostow -->
    <form id="vhostEditForm" method="post" class="form-horizontal" role="form" style="display: none;">
      <input type="hidden" name="id" value="">
      <input type="hidden" name="serverid" value="<?php echo $_GET['serverid'];?>
">
      <div class="form-group">
        <div class="row">
          <div class="col-sm-offset-2 enable-option">
            <label class="checkbox-inline checkbox-placement">
              <input type="checkbox" id="enable_sa" value="enable_sa"> <strong>Konfiguracja ServerAlias</strong>
            </label>
          </div>
        </div>
      </div>
      <div class="form-group" id="sa-group">
        <div class="row">
          <label for="sa" class="col-sm-2 control-label">ServerAlias</label>
          <div class="col-sm-5">
            <div class="input-group">
              <span class="input-group-btn">
                 <button class="btn btn-default" type="button" id="sa-arrow-btn"><span class="fa fa-arrow-up" id="sa-arrow"></span></button>
              </span>
              <input type="text" class="form-control" id="sa"
              data-minlength="3" data-error="Wpisz co najmniej trzy znaki"
              name="sa[0]" placeholder="*.example.com" required disabled>
            </div>
            <span class="glyphicon form-control-feedback" aria-hidden="true"></span>
          </div>
          <div >
            <span class="glyphicon glyphicon-plus new-server-alias" aria-hidden="true"></span>
          </div>
          <div class="col-sm-4 col-sm-offset-6 server-alias-txt">
            <div class="help-block with-errors"></div>
          </div>
        </div>
      </div>
      <div class="form-group">
        <div class="row">
          <label for="vhostOptEdit" class="col-sm-2 control-label">Opcje</label>
          <div class="col-sm-8 options-edit">
            <select  multiple="multiple" class="form-control" id="vhostOptEdit" name="optname"
              data-source="data/vhost_options.json"
              data-title="optedit"
              data-value='id'
              data-text='vhostopt'>
            </select>
          </div>
          <div class="col-sm-4">
            <div class="help-block with-errors"></div>
          </div>
        </div>
      </div>
      <div class="form-group">
        <div class="row">
          <label for="wwwuser_edit" class="col-sm-2 control-label">Konto</label>
          <div class="col-sm-4" id="edit-wwwuser">
            <select class="form-control" id="wwwuser-edit" name="account" disabled>
              <?php  $_smarty_tpl->tpl_vars['user'] = new Smarty_Variable; $_smarty_tpl->tpl_vars['user']->_loop = false;
 $_smarty_tpl->tpl_vars['id'] = new Smarty_Variable;
 $_from = $_smarty_tpl->tpl_vars['wwwuser']->value; if (!is_array($_from) && !is_object($_from)) { settype($_from, 'array');}
foreach ($_from as $_smarty_tpl->tpl_vars['user']->key => $_smarty_tpl->tpl_vars['user']->value){
$_smarty_tpl->tpl_vars['user']->_loop = true;
 $_smarty_tpl->tpl_vars['id']->value = $_smarty_tpl->tpl_vars['user']->key;
?>
              <option value="<?php echo $_smarty_tpl->tpl_vars['id']->value;?>
"><?php echo $_smarty_tpl->tpl_vars['user']->value;?>
</option>
              <?php } ?>
            </select>
          </div>
        </div>
      </div>
      <div class="form-group">
        <div class="row">
          <label for="www_access_order" class="col-sm-2 control-label">Dostęp do strony</label>
          <div class="col-sm-8" id="edit_accessbox">
            <div class="panel panel-default">
              <div class="panel-heading">
                <i class="fa fa-shield fa-fw"></i> Polityka Dostępu
                <label class="radio-inline default-vhost-access" style="margin-top: -5px;">
                  <input type="radio" id="edit_orderallow" name="edit_access_order" value="10" checked> Allow
                </label>
                <label class="radio-inline default-vhost-access" style="margin-top: -5px;">
                  <input type="radio" id="edit_orderdeny" name="edit_access_order" value="01"> Deny
                </label>
                <div class="pull-right" id="edit_accessListsExpand">
                  <button type="button" class="btn btn-default btn-xs" id="edit_access-expandBtn">
                    <span class="fa fa-angle-up" id="edit_expandControl"></span>
                  </button>
                </div>
              </div>
              <div class="panel-body" id="edit_access-list-details" style="display: none;">
                <div class="form-group">
                  <div class="row col-sm-offset-2" id="edit_accesslist">
                    <div class="col-sm-4">
                      <label class="radio-inline" id="edit_allowfrom">
                        <input type="radio" id="edit_allow_0" name="allow[0]" value="1" checked> Allow
                      </label>
                      <label class="radio-inline">
                        <input type="radio" id="edit_deny_0" name="allow[0]" value="0"> Deny
                      </label>
                    </div>
                    <label for="from" class="col-sm-2 control-label edit_access-from-label">From</label>
                    <div class="col-sm-5">
                      <input type="text" class="form-control access-from-input" id="edit_from" name="from[0]" value="all">
                    </div>
                    <div>
                      <span class="glyphicon glyphicon-plus vhost-access vhost-access-add" aria-hidden="true"></span>
                    </div>
                  </div>
                </div>
              </div>
            </div>
          </div>
        </div>
      </div>
      <div class="form-group">
        <div class="row">
          <div class="col-sm-offset-2 enable-htusers-option">
            <label class="checkbox-inline">
              <input type="checkbox" id="edit_password_enable" value="password_enable"> <strong>Zabezpieczenie hasłem</strong>
            </label>
          </div>
        </div>
      </div>
      <div class="form-group">
        <div class="col-sm-offset-2 col-sm-4">
          <div id="edit-select-htusers-div" class="select-htusers-group">
            <select id="edit-select-htusers" name="htusers" class="form-control select-htusers" multiple="multiple">
              <?php if ($_smarty_tpl->tpl_vars['htpasswd']->value=="NaN"){?>
               <option value="0" disabled>Brak kont</option>
               <?php }else{ ?>
               <?php  $_smarty_tpl->tpl_vars['htuser'] = new Smarty_Variable; $_smarty_tpl->tpl_vars['htuser']->_loop = false;
 $_smarty_tpl->tpl_vars['id'] = new Smarty_Variable;
 $_from = $_smarty_tpl->tpl_vars['htpasswd']->value; if (!is_array($_from) && !is_object($_from)) { settype($_from, 'array');}
foreach ($_from as $_smarty_tpl->tpl_vars['htuser']->key => $_smarty_tpl->tpl_vars['htuser']->value){
$_smarty_tpl->tpl_vars['htuser']->_loop = true;
 $_smarty_tpl->tpl_vars['id']->value = $_smarty_tpl->tpl_vars['htuser']->key;
?>
               <option value="<?php echo $_smarty_tpl->tpl_vars['id']->value;?>
"><?php echo $_smarty_tpl->tpl_vars['htuser']->value;?>
</option>
               <?php } ?>
              <?php }?>
            </select>
          </div>
        </div>
      </div>
      <div class="form-group">
        <div class="row">
          <div class="col-sm-offset-2 enable-option">
            <label class="checkbox-inline checkbox-placement">
              <input type="checkbox" id="edit_enable_htaccess" value="enable_htaccess"> <strong>Konfiguracja .htaccess</strong>
            </label>
          </div>
        </div>
      </div>
      <div class="form-group">
        <div class="row" id="htaccess-row">
          <label for="htaccess-field" class="col-sm-2 control-label">.htaccess</label>
          <div class="col-sm-6">
            <textarea class="form-control htaccess-group" id="htaccess-field" name="htaccess" rows="10" disabled></textarea>
          </div>
        </div>
      </div>
      <div class="form-group">
        <div class="row">
          <div class="col-sm-offset-3">
            <button type="button" class="btn btn-primary" id="editvhost-btn">Zapisz konfigurację</button>
            <button type="button" class="btn btn-default" id="edit-cancel">Anuluj</button>
          </div>
        </div>
      </div>
    </form>
    <!-- END: Edycja konfiguracji virtualhostow -->
    <!-- BEGIN: Nowy virtualhost -->
    <form role="form" class="form-horizontal" data-toggle="validator" id="addvhost" style="display: none;">
      <input type="hidden" name="serverid" value="<?php echo $_GET['serverid'];?>
">
      <div class="form-group">
          <div class="row">
          <label for="servername" class="col-sm-2 control-label">ServerName</label>
          <div class="col-sm-4">
            <input type="text" class="form-control" id="servername"
            data-minlength="3" data-error="Wpisz co najmniej trzy znaki"
            name="ServerName" placeholder="example.com"  required>
            <span class="glyphicon form-control-feedback" aria-hidden="true"></span>
          </div>
          <div class="col-sm-4">
            <div class="help-block with-errors"></div>
          </div>
        </div>
      </div>
      <div class="form-group">
        <div class="row">
          <div class="col-sm-offset-2 enable-option">
            <label class="checkbox-inline add-vhost-checkbox">
              <input type="checkbox" id="enable_server_alias" value="enable_server_alias"> <strong>Konfiguracja ServerAlias</strong>
            </label>
          </div>
        </div>
      </div>
      <div class="form-group" id="serverAlias">
        <div class="row">
          <label for="server-alias" class="col-sm-2 control-label">ServerAlias</label>
          <div class="col-sm-4">
            <input type="text" class="form-control" id="server-alias"
            data-minlength="3" data-error="Wpisz co najmniej trzy znaki"
            name="ServerAlias[]" placeholder="*.example.com" required disabled>
            <span class="glyphicon form-control-feedback" aria-hidden="true"></span>
          </div>
          <div >
            <span class="glyphicon glyphicon-plus new-server-alias" aria-hidden="true"></span>
          </div>
          <div class="col-sm-4 col-sm-offset-6 server-alias-txt">
            <div class="help-block with-errors"></div>
          </div>
        </div>
      </div>
      <div class="form-group">
        <div class="row">
          <label for="vhostOptSelect" class="col-sm-2 control-label">Opcje</label>
          <div class="col-sm-8">
            <select  multiple="multiple" class="form-control" id="vhostOptSelect" name="optname"
              data-source="data/vhost_options.json"
              data-title="opcje"
              data-value='id'
              data-text='vhostopt'>
            </select>
          </div>
          <div class="col-sm-4">
            <div class="help-block with-errors"></div>
          </div>
        </div>
      </div>
      <div class="form-group">
        <div class="row">
          <label for="wwwuser" class="col-sm-2 control-label">Konto</label>
          <div class="col-sm-4">
            <select class="form-control" id="wwwuser" name="account">
              <?php  $_smarty_tpl->tpl_vars['user'] = new Smarty_Variable; $_smarty_tpl->tpl_vars['user']->_loop = false;
 $_smarty_tpl->tpl_vars['id'] = new Smarty_Variable;
 $_from = $_smarty_tpl->tpl_vars['wwwuser']->value; if (!is_array($_from) && !is_object($_from)) { settype($_from, 'array');}
foreach ($_from as $_smarty_tpl->tpl_vars['user']->key => $_smarty_tpl->tpl_vars['user']->value){
$_smarty_tpl->tpl_vars['user']->_loop = true;
 $_smarty_tpl->tpl_vars['id']->value = $_smarty_tpl->tpl_vars['user']->key;
?>
              <option value="<?php echo $_smarty_tpl->tpl_vars['id']->value;?>
"><?php echo $_smarty_tpl->tpl_vars['user']->value;?>
</option>
              <?php } ?>
            </select>
          </div>
        </div>
      </div>
      <div class="form-group">
        <div class="row">
          <label for="www_access_order" class="col-sm-2 control-label">Dostęp do strony</label>
          <div class="col-sm-8">
            <div class="panel panel-default">
              <div class="panel-heading">
                <i class="fa fa-shield fa-fw"></i> Polityka Dostępu
                <label class="radio-inline default-vhost-access" style="margin-top: -5px;">
                  <input type="radio" id="orderallow" name="access_order" value="10" checked> Allow
                </label>
                <label class="radio-inline default-vhost-access" style="margin-top: -5px;">
                  <input type="radio" id="orderdeny" name="access_order" value="01"> Deny
                </label>
                <div class="pull-right" id="accessListsExpand">
                  <button type="button" class="btn btn-default btn-xs" id="access-expandBtn">
                    <span class="fa fa-angle-up" id="expandControl"></span>
                  </button>
                </div>
              </div>
              <div class="panel-body" id="access-list-details" style="display: none;">
                <div class="form-group">
                  <div class="row col-sm-offset-2" id="accesslist">
                    <div class="col-sm-4">
                      <label class="radio-inline" id="allowfrom">
                        <input type="radio" id="allow" name="allow[0]" value="1" checked> Allow
                      </label>
                      <label class="radio-inline">
                        <input type="radio" id="deny" name="allow[0]" value="0"> Deny
                      </label>
                    </div>
                    <label for="from" class="col-sm-2 control-label access-from-label">From</label>
                    <div class="col-sm-5">
                      <input type="text" class="form-control access-from-input" id="from" name="from[0]" value="all">
                    </div>
                    <div>
                      <span class="glyphicon glyphicon-plus vhost-access vhost-access-add" aria-hidden="true"></span>
                    </div>
                  </div>
                </div>
              </div>
            </div>
          </div>
        </div>
      </div>
      <div class="form-group">
        <div class="row">
          <div class="col-sm-offset-2 enable-option">
            <label class="checkbox-inline add-vhost-checkbox">
              <input type="checkbox" id="password_enable" value="password_enable"> <strong>Zabezpieczenie hasłem</strong>
            </label>
          </div>
        </div>
      </div>
      <div class="form-group">
        <div class="col-sm-offset-2 col-sm-4">
          <select id="htusers-select" name="htusers" class="form-control select-htusers" multiple="multiple">
            <?php if ($_smarty_tpl->tpl_vars['htpasswd']->value=="NaN"){?>
             <option value="0">Brak kont</option>
             <?php }else{ ?>
             <?php  $_smarty_tpl->tpl_vars['htuser'] = new Smarty_Variable; $_smarty_tpl->tpl_vars['htuser']->_loop = false;
 $_smarty_tpl->tpl_vars['id'] = new Smarty_Variable;
 $_from = $_smarty_tpl->tpl_vars['htpasswd']->value; if (!is_array($_from) && !is_object($_from)) { settype($_from, 'array');}
foreach ($_from as $_smarty_tpl->tpl_vars['htuser']->key => $_smarty_tpl->tpl_vars['htuser']->value){
$_smarty_tpl->tpl_vars['htuser']->_loop = true;
 $_smarty_tpl->tpl_vars['id']->value = $_smarty_tpl->tpl_vars['htuser']->key;
?>
             <option value="<?php echo $_smarty_tpl->tpl_vars['id']->value;?>
"><?php echo $_smarty_tpl->tpl_vars['htuser']->value;?>
</option>
             <?php } ?>
            <?php }?>
          </select>
        </div>
      </div>
      <div class="form-group">
        <div class="row">
          <div class="col-sm-offset-2 enable-option">
            <label class="checkbox-inline add-vhost-checkbox">
              <input type="checkbox" id="enable_htaccess" value="enable_htaccess"> <strong>Konfiguracja .htaccess</strong>
            </label>
          </div>
        </div>
      </div>
      <div class="form-group">
        <div class="row">
          <label for="htaccess" class="col-sm-2 control-label">.htaccess</label>
          <div class="col-sm-4">
            <textarea class="form-control" id="htaccess" name="htaccess" rows="5" disabled></textarea>
          </div>
        </div>
      </div>
      <div class="form-group">
        <div class="row">
          <div class="col-sm-offset-2" id="new-vhost-buttons">
            <button type="button" class="btn btn-primary" id="addvhost-btn">Zapisz konfigurację</button>
            <button type="button" class="btn btn-default" id="edit-cancel">Anuluj</button>
          </div>
        </div>
      </div>
    </form>
    <!-- END: Nowy virtualhost -->
    <!-- BEGIN: Nowe konto htaccess -->
    <form role="form" class="form-horizontal" data-toggle="validator" id="new-htuser-form" style="display: none;">
      <input type="hidden" name="serverid" value="<?php echo $_GET['serverid'];?>
">
      <div class="form-group">
          <div class="row">
          <label for="login" class="col-sm-2 control-label">Login</label>
          <div class="col-sm-4">
            <input type="text" class="form-control" id="login"
            data-minlength="3" data-error="Wpisz co najmniej trzy znaki"
            name="login" required>
            <span class="glyphicon form-control-feedback" aria-hidden="true"></span>
          </div>
          <div class="col-sm-4">
            <div class="help-block with-errors"></div>
          </div>
        </div>
      </div>
      <div class="form-group">
          <div class="row">
          <label for="password" class="col-sm-2 control-label">Hasło</label>
          <div class="col-sm-4">
            <input type="password" class="form-control" id="password"
            data-minlength="6" data-error="min. sześć znaków"
            name="password" required>
            <span class="glyphicon form-control-feedback" aria-hidden="true"></span>
          </div>
          <div class="col-sm-4">
            <div class="help-block with-errors"></div>
          </div>
        </div>
      </div>
      <div class="form-group">
          <div class="row">
          <label for="confirm" class="col-sm-2 control-label">Powtórz</label>
          <div class="col-sm-4">
            <input type="password" class="form-control" id="confirm"
            data-minlength="6" data-error="min. sześć znaków"
            data-match="#password"
            data-match-error="Hasło się nie zgadza"
            name="confirm" required>
            <span class="glyphicon form-control-feedback" aria-hidden="true"></span>
          </div>
          <div class="col-sm-4">
            <div class="help-block with-errors"></div>
          </div>
        </div>
      </div>
      <div class="form-group">
        <div class="row">
          <div class="col-sm-offset-2" id="new-vhost-buttons">
            <button type="button" class="btn btn-primary" id="addhtuser-btn">Utwórz konto</button>
            <button type="button" class="btn btn-default" id="edit-cancel">Anuluj</button>
          </div>
        </div>
      </div>
    </form>
    <!-- END: Nowe konto htaccess -->
    <!-- BEGIN: Zmiana hasla htaccess -->
    <form role="form" class="form-horizontal" data-toggle="validator" id="change-htpassword-form" style="display: none;">
      <input type="hidden" name="serverid" value="<?php echo $_GET['serverid'];?>
">
      <div class="form-group">
          <div class="row">
          <label for="newpass" class="col-sm-2 control-label">Hasło</label>
          <div class="col-sm-4">
            <input type="password" class="form-control" id="newpass"
            data-minlength="6" data-error="min. sześć znaków"
            name="password" required>
            <span class="glyphicon form-control-feedback" aria-hidden="true"></span>
          </div>
          <div class="col-sm-4">
            <div class="help-block with-errors"></div>
          </div>
        </div>
      </div>
      <div class="form-group">
          <div class="row">
          <label for="confirm" class="col-sm-2 control-label">Powtórz</label>
          <div class="col-sm-4">
            <input type="password" class="form-control" id="confirm"
            data-minlength="6" data-error="min. sześć znaków"
            data-match="#newpass"
            data-match-error="Hasło się nie zgadza"
            name="confirm" required>
            <span class="glyphicon form-control-feedback" aria-hidden="true"></span>
          </div>
          <div class="col-sm-4">
            <div class="help-block with-errors"></div>
          </div>
        </div>
      </div>
      <div class="form-group">
        <div class="row">
          <div class="col-sm-offset-2" id="new-vhost-buttons">
            <button type="button" class="btn btn-primary" id="chpass-btn">Zmień hasło</button>
            <button type="button" class="btn btn-default" id="chpass-cancel">Anuluj</button>
          </div>
        </div>
      </div>
    </form>
    <!-- END: Zmiana hasla htaccess -->
    <!-- BEGIN: Sysaccount edit -->
    <form data-toggle="validator" role="form" id="edit-sysuser-form" style="display: none;">
      <input type="hidden" name="sid" value="<?php echo $_GET['serverid'];?>
">
      <input type="hidden" name="id" value="">
      <div class="panel panel-default">
        <div class="panel-heading">Basic settings</div>
        <div class="panel-body">
          <div class="row">
            <div class="col-xs-6">
              <div class="form-group">
                <label for="fullname_edit" class="control-label">Full Name</label>
                <input type="text" class="form-control" id="fullname_edit"
                data-minlength="3" data-error="At least 3 charactes required"
                name="fullname_edit" required>
                <span class="glyphicon form-control-feedback sys-account-form" aria-hidden="true"></span>
                <div class="help-block with-errors"></div>
              </div>
            </div>
            <div class="col-xs-6">
              <div class="form-group">
                <label for="email_edit" class="control-label">E-Mail</label>
                <input type="email" class="form-control" id="email_edit"
                data-error="Incorrect email address"
                name="email_edit" required>
                <span class="glyphicon form-control-feedback sys-account-form" aria-hidden="true"></span>
                <div class="help-block with-errors"></div>
              </div>
            </div>
          </div>
          <div class="row">
            <div class="col-xs-6">
              <div class="form-group">
                <label for="login_edit" class="control-label">Login</label>
                <input type="text" class="form-control" id="login_edit"
                data-minlength="3" data-error="At least 3 characters are required"
                name="login_edit" required>
                <span class="glyphicon form-control-feedback sys-account-form" aria-hidden="true"></span>
                <div class="help-block with-errors"></div>
              </div>
            </div>
          </div>
          <div class="row">
            <div class="col-xs-6">
              <div class="form-group">
                <label for="password_edit" class="control-label">Password</label>
                <input type="password" class="form-control" id="password_edit"
                data-minlength="6" data-error="at least 6 characters required"
                name="password_edit">
                <span class="glyphicon form-control-feedback sys-account-form" aria-hidden="true"></span>
                <div class="help-block with-errors"></div>
              </div>
            </div>
            <div class="col-xs-6">
              <div class="form-group">
                <label for="confirm_edit" class="control-label">Confirm</label>
                <input type="password" class="form-control" id="confirm_edit"
                data-minlength="6" data-error="at least 6 characters required"
                data-match="#password_edit"
                data-match-error="Password mismatch"
                name="confirm_edit">
                <span class="glyphicon form-control-feedback sys-account-form" aria-hidden="true"></span>
                <div class="help-block with-errors"></div>
              </div>
            </div>
          </div>
        </div>
      </div>
      <div class="panel panel-default">
        <div class="panel-heading">Advanced settings</div>
        <div class="panel-body" id="edit_advanced_settings">
          <div class="row">
            <div class="col-xs-3">
              <div class="form-group">
                <div class="checkbox">
                  <label>
                    <input type="checkbox" checked="true" name="isActive_edit"> Active
                  </label>
                </div>
              </div>
            </div>
            <div class="col-xs-3">
              <div class="form-group">
                <div class="checkbox">
                  <label>
                    <input type="checkbox" name="expEnable_edit"> Expiration
                  </label>
                </div>
              </div>
            </div>
            <div class="col-xs-3">
              <div class="form-group">
                <div class="checkbox">
                  <label>
                    <input type="checkbox" name="shell_edit"> Shell access
                  </label>
                </div>
              </div>
            </div>
            <div class="col-xs-3">
              <div class="form-group">
                <div class="checkbox">
                  <label>
                    <input type="checkbox" name="sudo_edit"> Administrator
                  </label>
                </div>
              </div>
            </div>
            <div class="col-xs-3">
              <div class="form-group">
                <div class="checkbox">
                  <label>
                    <input type="checkbox" name="sshkey_enable_edit"> SSH keys
                  </label>
                </div>
              </div>
            </div>
          </div>
          <div class="row">
            <div class="col-xs-4">
              <div class="form-group">
                <div id="expiration_edit" class="input-group date">
                  <input placeholder="dd/mm/yyyy" type="text" class="form-control"
                  data-minlength="3" data-error="Pole nie może być puste"
                  name="expdate_edit" disabled />
                  <span class="input-group-addon">
                    <span class="glyphicon glyphicon-calendar"></span>
                  </span>
                </div>
              </div>
            </div>
            <div class="col-xs-6">
              <div class="form-group" id="sshkey_edit">
                <input type="text" placeholder="ssh public key" class="form-control"
                data-minlength="3" data-error="Pole nie może być puste"
                name="sshkey_edit[0]" disabled="true" />
              </div>
            </div>
            <div class="col-xs-1">
              <span class="glyphicon glyphicon-plus"></span>
            </div>
          </div>
        </div>
      </div>
      <div class="row">
        <div class="col-xs-6">
          <div class="form-group">
              <button type="button" class="btn btn-primary" id="sysuser_edit-btn">Save changes</button>
              <button type="button" class="btn btn-default" id="edit_cancel">Cancel</button>
          </div>
        </div>
      </div>
    </form>
    <!-- END: Sysaccount edit -->
    <!-- BEGIN: Nowe konto systemowe -->
    <form data-toggle="validator" role="form" id="new-sysuser-form" style="display: none;">
      <input type="hidden" name="serverid" value="<?php echo $_GET['serverid'];?>
">
      <div class="panel panel-default">
        <div class="panel-heading">Podstawowe informacje</div>
        <div class="panel-body">
          <div class="row">
            <div class="col-xs-6">
              <div class="form-group">
                <label for="fullname" class="control-label">Imię i Nazwisko</label>
                <input type="text" class="form-control" id="fullname"
                data-minlength="3" data-error="Wpisz co najmniej trzy znaki"
                name="fullname" required>
                <span class="glyphicon form-control-feedback sys-account-form" aria-hidden="true"></span>
                <div class="help-block with-errors"></div>
              </div>
            </div>
            <div class="col-xs-6">
              <div class="form-group">
                <label for="email" class="control-label">E-Mail</label>
                <input type="email" class="form-control" id="email"
                data-error="Nieprawidłowy adres email"
                name="email" required>
                <span class="glyphicon form-control-feedback sys-account-form" aria-hidden="true"></span>
                <div class="help-block with-errors"></div>
              </div>
            </div>
          </div>
          <div class="row">
            <div class="col-xs-6">
              <div class="form-group">
                <label for="login" class="control-label">Login</label>
                <input type="text" class="form-control" id="login"
                data-minlength="3" data-error="Wpisz co najmniej trzy znaki"
                name="login" required>
                <span class="glyphicon form-control-feedback sys-account-form" aria-hidden="true"></span>
                <div class="help-block with-errors"></div>
              </div>
            </div>
          </div>
          <div class="row">
            <div class="col-xs-6">
              <div class="form-group">
                <label for="password" class="control-label">Hasło</label>
                <input type="password" class="form-control" id="userpass"
                data-minlength="6" data-error="min. sześć znaków"
                name="userpass" required>
                <span class="glyphicon form-control-feedback sys-account-form" aria-hidden="true"></span>
                <div class="help-block with-errors"></div>
              </div>
            </div>
            <div class="col-xs-6">
              <div class="form-group">
                <label for="confirmpass" class="control-label">Powtórz</label>
                <input type="password" class="form-control" id="confirmpass"
                data-minlength="6" data-error="min. sześć znaków"
                data-match="#userpass"
                data-match-error="Hasło się nie zgadza"
                name="confirm" required>
                <span class="glyphicon form-control-feedback sys-account-form" aria-hidden="true"></span>
                <div class="help-block with-errors"></div>
              </div>
            </div>
          </div>
        </div>
      </div>
      <div class="panel panel-default">
        <div class="panel-heading">Ustawienia zaawansowanane</div>
        <div class="panel-body" id="advanced_settings">
          <div class="row">
            <div class="col-xs-3">
              <div class="form-group">
                <div class="checkbox">
                  <label>
                    <input type="checkbox" checked="true" name="isActive"> Konto aktywne
                  </label>
                </div>
              </div>
            </div>
            <div class="col-xs-3">
              <div class="form-group">
                <div class="checkbox">
                  <label>
                    <input type="checkbox" name="expEnable"> Expiracja
                  </label>
                </div>
              </div>
            </div>
            <div class="col-xs-3">
              <div class="form-group">
                <div class="checkbox">
                  <label>
                    <input type="checkbox" name="shell"> Powłoka systemu
                  </label>
                </div>
              </div>
            </div>
            <div class="col-xs-3">
              <div class="form-group">
                <div class="checkbox">
                  <label>
                    <input type="checkbox" name="sudo"> Administrator
                  </label>
                </div>
              </div>
            </div>
            <div class="col-xs-3">
              <div class="form-group">
                <div class="checkbox">
                  <label>
                    <input type="checkbox" name="sshkey_enable"> Klucze ssh
                  </label>
                </div>
              </div>
            </div>
          </div>
          <div class="row">
            <div class="col-xs-4">
              <div class="form-group">
                <div id="expiration" class="input-group date">
                  <input placeholder="dd/mm/yyyy" type="text" class="form-control"
                  data-minlength="3" data-error="Pole nie może być puste"
                  name="expdate" disabled="true" />
                  <span class="input-group-addon">
                    <span class="glyphicon glyphicon-calendar"></span>
                  </span>
                </div>
              </div>
            </div>
            <div class="col-xs-6">
              <div class="form-group" id="sshkey">
                <input type="text" placeholder="ssh public key" class="form-control"
                data-minlength="3" data-error="Pole nie może być puste"
                name="sshkey[0]" disabled="true" />
              </div>
            </div>
            <div class="col-xs-1">
              <span class="glyphicon glyphicon-plus"></span>
            </div>
          </div>
        </div>
      </div>
      <div class="row">
        <div class="col-xs-6">
          <div class="form-group">
              <button type="submit" class="btn btn-primary" id="addsysuser-btn">Utwórz konto</button>
              <button type="button" class="btn btn-default" id="edit-cancel">Anuluj</button>
          </div>
        </div>
      </div>
    </form>
    <!-- END: Nowe konto systemowe -->
    <div id="wrapper">
<!-- END: Sekcja formularzy edycji danych -->

        <!-- Navigation -->
        <nav class="navbar navbar-default navbar-static-top" role="navigation" style="margin-bottom: 0">
            <div class="navbar-header">
                <button type="button" class="navbar-toggle" data-toggle="collapse" data-target=".navbar-collapse">
                    <span class="sr-only">Toggle navigation</span>
                    <span class="icon-bar"></span>
                    <span class="icon-bar"></span>
                    <span class="icon-bar"></span>
                </button>
                <a class="navbar-brand" href="?dashboard"><img src="/images/spine-logo.png"></a>
            </div>
            <!-- /.navbar-header -->

            <ul class="nav navbar-top-links navbar-right">
                <li class="dropdown">
                    <a class="dropdown-toggle" data-toggle="dropdown" href="#">
                        <i class="fa fa-envelope fa-fw"></i>  <i class="fa fa-caret-down"></i>
                    </a>
                    <ul class="dropdown-menu dropdown-messages">
                        <li>
                            <a href="#">
                                <div>
                                    <strong>John Smith</strong>
                                    <span class="pull-right text-muted">
                                        <em>Yesterday</em>
                                    </span>
                                </div>
                                <div>Lorem ipsum dolor sit amet, consectetur adipiscing elit. Pellentesque eleifend...</div>
                            </a>
                        </li>
                        <li class="divider"></li>
                        <li>
                            <a href="#">
                                <div>
                                    <strong>John Smith</strong>
                                    <span class="pull-right text-muted">
                                        <em>Yesterday</em>
                                    </span>
                                </div>
                                <div>Lorem ipsum dolor sit amet, consectetur adipiscing elit. Pellentesque eleifend...</div>
                            </a>
                        </li>
                        <li class="divider"></li>
                        <li>
                            <a href="#">
                                <div>
                                    <strong>John Smith</strong>
                                    <span class="pull-right text-muted">
                                        <em>Yesterday</em>
                                    </span>
                                </div>
                                <div>Lorem ipsum dolor sit amet, consectetur adipiscing elit. Pellentesque eleifend...</div>
                            </a>
                        </li>
                        <li class="divider"></li>
                        <li>
                            <a class="text-center" href="#">
                                <strong>Read All Messages</strong>
                                <i class="fa fa-angle-right"></i>
                            </a>
                        </li>
                    </ul>
                    <!-- /.dropdown-messages -->
                </li>
                <!-- /.dropdown -->
                <li class="dropdown">
                    <a class="dropdown-toggle" data-toggle="dropdown" href="#">
                        <i class="fa fa-tasks fa-fw"></i>  <i class="fa fa-caret-down"></i>
                    </a>
                    <ul class="dropdown-menu dropdown-tasks">
                        <li>
                            <a href="#">
                                <div>
                                    <p>
                                        <strong>Task 1</strong>
                                        <span class="pull-right text-muted">40% Complete</span>
                                    </p>
                                    <div class="progress progress-striped active">
                                        <div class="progress-bar progress-bar-success" role="progressbar" aria-valuenow="40" aria-valuemin="0" aria-valuemax="100" style="width: 40%">
                                            <span class="sr-only">40% Complete (success)</span>
                                        </div>
                                    </div>
                                </div>
                            </a>
                        </li>
                        <li class="divider"></li>
                        <li>
                            <a href="#">
                                <div>
                                    <p>
                                        <strong>Task 2</strong>
                                        <span class="pull-right text-muted">20% Complete</span>
                                    </p>
                                    <div class="progress progress-striped active">
                                        <div class="progress-bar progress-bar-info" role="progressbar" aria-valuenow="20" aria-valuemin="0" aria-valuemax="100" style="width: 20%">
                                            <span class="sr-only">20% Complete</span>
                                        </div>
                                    </div>
                                </div>
                            </a>
                        </li>
                        <li class="divider"></li>
                        <li>
                            <a href="#">
                                <div>
                                    <p>
                                        <strong>Task 3</strong>
                                        <span class="pull-right text-muted">60% Complete</span>
                                    </p>
                                    <div class="progress progress-striped active">
                                        <div class="progress-bar progress-bar-warning" role="progressbar" aria-valuenow="60" aria-valuemin="0" aria-valuemax="100" style="width: 60%">
                                            <span class="sr-only">60% Complete (warning)</span>
                                        </div>
                                    </div>
                                </div>
                            </a>
                        </li>
                        <li class="divider"></li>
                        <li>
                            <a href="#">
                                <div>
                                    <p>
                                        <strong>Task 4</strong>
                                        <span class="pull-right text-muted">80% Complete</span>
                                    </p>
                                    <div class="progress progress-striped active">
                                        <div class="progress-bar progress-bar-danger" role="progressbar" aria-valuenow="80" aria-valuemin="0" aria-valuemax="100" style="width: 80%">
                                            <span class="sr-only">80% Complete (danger)</span>
                                        </div>
                                    </div>
                                </div>
                            </a>
                        </li>
                        <li class="divider"></li>
                        <li>
                            <a class="text-center" href="#">
                                <strong>See All Tasks</strong>
                                <i class="fa fa-angle-right"></i>
                            </a>
                        </li>
                    </ul>
                    <!-- /.dropdown-tasks -->
                </li>
                <!-- /.dropdown -->
                <li class="dropdown">
                    <a class="dropdown-toggle" data-toggle="dropdown" href="#">
                        <i class="fa fa-bell fa-fw"></i>  <i class="fa fa-caret-down"></i>
                    </a>
                    <ul class="dropdown-menu dropdown-alerts">
                        <li>
                            <a href="#">
                                <div>
                                    <i class="fa fa-comment fa-fw"></i> New Comment
                                    <span class="pull-right text-muted small">4 minutes ago</span>
                                </div>
                            </a>
                        </li>
                        <li class="divider"></li>
                        <li>
                            <a href="#">
                                <div>
                                    <i class="fa fa-twitter fa-fw"></i> 3 New Followers
                                    <span class="pull-right text-muted small">12 minutes ago</span>
                                </div>
                            </a>
                        </li>
                        <li class="divider"></li>
                        <li>
                            <a href="#">
                                <div>
                                    <i class="fa fa-envelope fa-fw"></i> Message Sent
                                    <span class="pull-right text-muted small">4 minutes ago</span>
                                </div>
                            </a>
                        </li>
                        <li class="divider"></li>
                        <li>
                            <a href="#">
                                <div>
                                    <i class="fa fa-tasks fa-fw"></i> New Task
                                    <span class="pull-right text-muted small">4 minutes ago</span>
                                </div>
                            </a>
                        </li>
                        <li class="divider"></li>
                        <li>
                            <a href="#">
                                <div>
                                    <i class="fa fa-upload fa-fw"></i> Server Rebooted
                                    <span class="pull-right text-muted small">4 minutes ago</span>
                                </div>
                            </a>
                        </li>
                        <li class="divider"></li>
                        <li>
                            <a class="text-center" href="#">
                                <strong>See All Alerts</strong>
                                <i class="fa fa-angle-right"></i>
                            </a>
                        </li>
                    </ul>
                    <!-- /.dropdown-alerts -->
                </li>
                <!-- /.dropdown -->
                <li class="dropdown">
                    <a class="dropdown-toggle" data-toggle="dropdown" href="#">
                        <i class="fa fa-user fa-fw"></i>  <i class="fa fa-caret-down"></i>
                    </a>
                    <ul class="dropdown-menu dropdown-user">
                        <li><a href="#"><i class="fa fa-user fa-fw"></i> User Profile</a>
                        </li>
                        <li><a href="#"><i class="fa fa-gear fa-fw"></i> Settings</a>
                        </li>
                        <li class="divider"></li>
                        <li><a href="login.html"><i class="fa fa-sign-out fa-fw"></i> Logout</a>
                        </li>
                    </ul>
                    <!-- /.dropdown-user -->
                </li>
                <!-- /.dropdown -->
            </ul>
            <!-- /.navbar-top-links -->

            <div class="navbar-default sidebar" role="navigation">
                <div class="sidebar-nav navbar-collapse">
                    <ul class="nav" id="side-menu">
                        <li>
                            <a href="?dashboard"><i class="fa fa-dashboard fa-fw"></i> Dashboard</a>
                        </li>
                        <li>
                            <a href="#"><i class="fa fa-linux f-fw"></i> Serwery<span class="fa arrow"></span></a>
                            <ul class="nav nav-second-level">
                                <?php  $_smarty_tpl->tpl_vars['srv'] = new Smarty_Variable; $_smarty_tpl->tpl_vars['srv']->_loop = false;
 $_smarty_tpl->tpl_vars['id'] = new Smarty_Variable;
 $_from = $_smarty_tpl->tpl_vars['HostMenu']->value; if (!is_array($_from) && !is_object($_from)) { settype($_from, 'array');}
foreach ($_from as $_smarty_tpl->tpl_vars['srv']->key => $_smarty_tpl->tpl_vars['srv']->value){
$_smarty_tpl->tpl_vars['srv']->_loop = true;
 $_smarty_tpl->tpl_vars['id']->value = $_smarty_tpl->tpl_vars['srv']->key;
?>
                                   <li>
                                     <a href="#"><?php echo $_smarty_tpl->tpl_vars['srv']->value;?>
 <span class="fa arrow"></span></a>
                                     <ul class="nav nav-third-level">
                                       <li>
                                         <a href="?serverid=<?php echo $_smarty_tpl->tpl_vars['id']->value;?>
&item=info">Informacje</a>
                                       </li>
                                       <li>
                                         <a href="?serverid=<?php echo $_smarty_tpl->tpl_vars['id']->value;?>
&item=sysusers">Konta systemowe</a>
                                       </li>
                                       <li class="apache-section">
                                         <a href="?serverid=<?php echo $_smarty_tpl->tpl_vars['id']->value;?>
&item=wwwsrv">Serwer WWW</a>
                                       </li>
                                     </ul>
                                   </li>
                                <?php } ?>
                            </ul>
                            <!-- /.nav-second-level -->
                        </li>
                        <li>
                          <a href="#"><i class="fa fa-wrench f-fw"></i> Settings<span class="fa arrow"></span></a>
                          <ul class="nav nav-second-level">
                            <li>
                              <a href="?settings=smtp">SMTP</a>
                            </li>
                          </ul>
                        </li>
                        <li>
                            <a href="tables.html"><i class="fa fa-table fa-fw"></i> Tables</a>
                        </li>
                        <li>
                            <a href="forms.html"><i class="fa fa-edit fa-fw"></i> Forms</a>
                        </li>
                        <li>
                            <a href="#"><i class="fa fa-wrench fa-fw"></i> UI Elements<span class="fa arrow"></span></a>
                            <ul class="nav nav-second-level">
                                <li>
                                    <a href="panels-wells.html">Panels and Wells</a>
                                </li>
                                <li>
                                    <a href="buttons.html">Buttons</a>
                                </li>
                                <li>
                                    <a href="notifications.html">Notifications</a>
                                </li>
                                <li>
                                    <a href="typography.html">Typography</a>
                                </li>
                                <li>
                                    <a href="icons.html"> Icons</a>
                                </li>
                                <li>
                                    <a href="grid.html">Grid</a>
                                </li>
                            </ul>
                            <!-- /.nav-second-level -->
                        </li>
                        <li>
                            <a href="#"><i class="fa fa-sitemap fa-fw"></i> Multi-Level Dropdown<span class="fa arrow"></span></a>
                            <ul class="nav nav-second-level">
                                <li>
                                    <a href="#">Second Level Item</a>
                                </li>
                                <li>
                                    <a href="#">Second Level Item</a>
                                </li>
                                <li>
                                    <a href="#">Third Level <span class="fa arrow"></span></a>
                                    <ul class="nav nav-third-level">
                                        <li>
                                            <a href="#">Third Level Item</a>
                                        </li>
                                        <li>
                                            <a href="#">Third Level Item</a>
                                        </li>
                                        <li>
                                            <a href="#">Third Level Item</a>
                                        </li>
                                        <li>
                                            <a href="#">Third Level Item</a>
                                        </li>
                                    </ul>
                                    <!-- /.nav-third-level -->
                                </li>
                            </ul>
                            <!-- /.nav-second-level -->
                        </li>
                        <li>
                            <a href="#"><i class="fa fa-files-o fa-fw"></i> Sample Pages<span class="fa arrow"></span></a>
                            <ul class="nav nav-second-level">
                                <li>
                                    <a href="blank.html">Blank Page</a>
                                </li>
                                <li>
                                    <a href="login.html">Login Page</a>
                                </li>
                            </ul>
                            <!-- /.nav-second-level -->
                        </li>
                    </ul>
                </div>
                <!-- /.sidebar-collapse -->
            </div>
            <!-- /.navbar-static-side -->
        </nav>

        <div id="page-wrapper">
            <div class="row">
                <div class="col-lg-12">
                    <?php if (isset($_GET['serverid'])){?>
                    <ol class="breadcrumb">
                      <?php if ($_GET['item']=="info"){?>
                      <li class="active">Informacje ogólne</li>
                      <li><a href="?serverid=<?php echo $_GET['serverid'];?>
&item=sysusers">Konta systemowe</a></li>
                      <li class="apache-section"><a href="?serverid=<?php echo $_GET['serverid'];?>
&item=wwwsrv">Serwer WWW</a></li>
                      <?php }elseif($_GET['item']=="sysusers"){?>
                      <li><a href="?serverid=<?php echo $_GET['serverid'];?>
&item=info">Informacje ogólne</a></li>
                      <li class="active">Konta systemowe</li>
                      <li class="apache-section"><a href="?serverid=<?php echo $_GET['serverid'];?>
&item=wwwsrv">Serwer WWW</a></li>
                      <?php }elseif($_GET['item']=="wwwsrv"){?>
                      <li><a href="?serverid=<?php echo $_GET['serverid'];?>
&item=info">Informacje ogólne</a></li>
                      <li><a href="?serverid=<?php echo $_GET['serverid'];?>
&item=sysusers">Konta systemowe</a></li>
                      <li class="active">Serwer WWW</li>
                      <?php }?>
                    </ol>
                    <h3 class="page-header">
                      <?php if ($_smarty_tpl->tpl_vars['basicInfo']->value['os']=="Ubuntu"&&$_smarty_tpl->tpl_vars['basicInfo']->value['status']=="A"){?>
                      <img src="/images/server-ubuntu_ok.png" width="70">Host: <?php echo $_smarty_tpl->tpl_vars['basicInfo']->value['hostname'];?>
</h3>
                      <?php }elseif($_smarty_tpl->tpl_vars['basicInfo']->value['os']=="Ubuntu"&&$_smarty_tpl->tpl_vars['basicInfo']->value['status']=="U"){?>
                      <img src="/images/server-ubuntu_error.png" width="70">Host: <?php echo $_smarty_tpl->tpl_vars['basicInfo']->value['hostname'];?>
</h3>
                      <?php }elseif($_smarty_tpl->tpl_vars['basicInfo']->value['os']=="Ubuntu"&&$_smarty_tpl->tpl_vars['basicInfo']->value['status']=="S"){?>
                      <img src="/images/server-ubuntu_warning.png" width="70">Host: <?php echo $_smarty_tpl->tpl_vars['basicInfo']->value['hostname'];?>
</h3>
                      <?php }elseif(($_smarty_tpl->tpl_vars['basicInfo']->value['os']=="Centos6"||$_smarty_tpl->tpl_vars['basicInfo']->value['os']=="Centos7")&&$_smarty_tpl->tpl_vars['basicInfo']->value['status']=="A"){?>
                      <img src="/images/server-centos_ok.png" width="70">Host: <?php echo $_smarty_tpl->tpl_vars['basicInfo']->value['hostname'];?>
</h3>
                      <?php }elseif(($_smarty_tpl->tpl_vars['basicInfo']->value['os']=="Centos6"||$_smarty_tpl->tpl_vars['basicInfo']->value['os']=="Centos7")&&$_smarty_tpl->tpl_vars['basicInfo']->value['status']=="U"){?>
                      <img src="/images/server-centos_error.png" width="70">Host: <?php echo $_smarty_tpl->tpl_vars['basicInfo']->value['hostname'];?>
</h3>
                      <?php }elseif(($_smarty_tpl->tpl_vars['basicInfo']->value['os']=="Centos6"||$_smarty_tpl->tpl_vars['basicInfo']->value['os']=="Centos7")&&$_smarty_tpl->tpl_vars['basicInfo']->value['status']=="S"){?>
                      <img src="/images/server-centos_warning.png" width="70">Host: <?php echo $_smarty_tpl->tpl_vars['basicInfo']->value['hostname'];?>
</h3>
                      <?php }?>
                    <?php }elseif($_GET['settings']=="smtp"){?>
                      <h3 class="page-header"><img src="images/mail_settings.png" height="40px"> SMTP Settings</h3>
                        <div class="col-sm-4">
                          <form role="form" class="form-horizontal" data-toggle="validator" id="settings_smtp">
                            <div class="form-group smtp_settings_fields">
                              <div class="row">
                                <label for="smtpfrom" class="col-sm-2 control-label">From</label>
                                <div class="col-sm-6">
                                  <?php if (isset($_smarty_tpl->tpl_vars['smtp_settings']->value)){?>
                                  <input type="text" class="form-control" id="smtpfrom"
                                  data-minlength="3" data-error="Enter e-mail address"
                                  name="smtpfrom" placeholder="admin@example.com" value="<?php echo $_smarty_tpl->tpl_vars['smtp_settings']->value['spine_from'];?>
" required>
                                  <?php }else{ ?>
                                  <input type="text" class="form-control" id="smtpfrom"
                                  data-minlength="3" data-error="Enter e-mail address"
                                  name="smtpfrom" placeholder="admin@example.com" required>
                                  <?php }?>
                                  <span class="glyphicon form-control-feedback" aria-hidden="true"></span>
                                </div>
                                <div class="col-sm-4">
                                  <div class="help-block with-errors"></div>
                                </div>
                              </div>
                            </div>
                            <div class="form-group smtp_settings_fields">
                              <div class="row">
                                <label for="smtphost" class="col-sm-2 control-label">Host</label>
                                <div class="col-sm-6">
                                  <?php if (isset($_smarty_tpl->tpl_vars['smtp_settings']->value)){?>
                                  <input type="text" class="form-control" id="smtphost"
                                  data-minlength="3" data-error="Enter hostname"
                                  name="smtphost" placeholder="localhost" value="<?php echo $_smarty_tpl->tpl_vars['smtp_settings']->value['host'];?>
" required>
                                  <?php }else{ ?>
                                  <input type="text" class="form-control" id="smtphost"
                                  data-minlength="3" data-error="Enter hostname"
                                  name="smtphost" placeholder="localhost" required>
                                  <?php }?>
                                  <span class="glyphicon form-control-feedback" aria-hidden="true"></span>
                                </div>
                                <div class="col-sm-4">
                                  <div class="help-block with-errors"></div>
                                </div>
                              </div>
                            </div>
                            <div class="form-group smtp_settings_fields">
                              <div class="row">
                                <label for="smtpport" class="col-sm-2 control-label">Port</label>
                                <div class="col-sm-2">
                                  <?php if (isset($_smarty_tpl->tpl_vars['smtp_settings']->value)){?>
                                  <input type="text" class="form-control" id="smtpport"
                                  data-minlength="2" data-error="Enter port number"
                                  name="smtpport" placeholder="25" value="<?php echo $_smarty_tpl->tpl_vars['smtp_settings']->value['port'];?>
" required>
                                  <?php }else{ ?>
                                  <input type="text" class="form-control" id="smtpport"
                                  data-minlength="2" data-error="Enter port number"
                                  name="smtpport" placeholder="25"  required>
                                  <?php }?>
                                  <span class="glyphicon form-control-feedback" aria-hidden="true"></span>
                                </div>
                                <div class="col-sm-4">
                                  <div class="help-block with-errors"></div>
                                </div>
                              </div>
                            </div>
                            <div class="form-group">
                              <div class="row">
                                <div class="checkbox col-sm-offset-2">
                                  <label>
                                    <?php if (isset($_smarty_tpl->tpl_vars['smtp_settings']->value)&&$_smarty_tpl->tpl_vars['smtp_settings']->value['auth']==1){?>
                                    <input type="checkbox" id="smtp_auth_enable" name="smtp_auth" checked> Authentication options
                                    <?php }else{ ?>
                                    <input type="checkbox" id="smtp_auth_enable" name="smtp_auth"> Authentication options
                                    <?php }?>
                                  </label>
                                </div>
                              </div>
                            </div>
                            <?php if (isset($_smarty_tpl->tpl_vars['smtp_settings']->value)&&$_smarty_tpl->tpl_vars['smtp_settings']->value['auth']==1){?>
                            <div class="form-group smtp_settings_fields advanced_smtp_settings">
                              <div class="row">
                                <label for="smtplogin" class="col-sm-2 control-label">Login</label>
                                <div class="col-sm-6">
                                  <input type="text" class="form-control" id="smtplogin"
                                  data-minlength="2" data-error="Enter SMTP username"
                                  name="smtplogin" placeholder="SMTP login" value="<?php echo $_smarty_tpl->tpl_vars['smtp_settings']->value['login'];?>
" required>
                                  <span class="glyphicon form-control-feedback" aria-hidden="true"></span>
                                </div>
                                <div class="col-sm-4">
                                  <div class="help-block with-errors"></div>
                                </div>
                              </div>
                            </div>
                            <div class="form-group smtp_settings_fields advanced_smtp_settings">
                              <div class="row">
                                <label for="smtppass" class="col-sm-2 control-label">Password</label>
                                <div class="col-sm-6">
                                  <input type="password" class="form-control" id="smtppass"
                                  data-minlength="2" data-error="Enter SMTP password"
                                  name="smtppass" placeholder="SMTP password" value="<?php echo $_smarty_tpl->tpl_vars['smtp_settings']->value['password'];?>
" required>
                                  <span class="glyphicon form-control-feedback" aria-hidden="true"></span>
                                </div>
                                <div class="col-sm-4">
                                  <div class="help-block with-errors"></div>
                                </div>
                              </div>
                            </div>
                            <div class="form-group advanced_smtp_settings" style="display: none;">
                              <div class="row">
                                <div class="checkbox col-sm-offset-2">
                                  <label>
                                    <?php if ($_smarty_tpl->tpl_vars['smtp_settings']->value['ssl']==1){?>
                                    <input type="checkbox" id="smtp_ssl" name="use_ssl" checked> Use SSL
                                    <?php }else{ ?>
                                    <input type="checkbox" id="smtp_ssl" name="use_ssl"> Use SSL
                                    <?php }?>
                                  </label>
                                </div>
                              </div>
                            </div>
                            <?php }else{ ?>
                            <div class="form-group smtp_settings_fields advanced_smtp_settings" style="display: none;">
                              <div class="row">
                                <label for="smtplogin" class="col-sm-2 control-label">Login</label>
                                <div class="col-sm-6">
                                  <input type="text" class="form-control" id="smtplogin"
                                  data-minlength="2" data-error="Enter SMTP username"
                                  name="smtplogin" placeholder="SMTP login"  required>
                                  <span class="glyphicon form-control-feedback" aria-hidden="true"></span>
                                </div>
                                <div class="col-sm-4">
                                  <div class="help-block with-errors"></div>
                                </div>
                              </div>
                            </div>
                            <div class="form-group smtp_settings_fields advanced_smtp_settings" style="display: none;">
                              <div class="row">
                                <label for="smtppass" class="col-sm-2 control-label">Password</label>
                                <div class="col-sm-6">
                                  <input type="password" class="form-control" id="smtppass"
                                  data-minlength="2" data-error="Enter SMTP password"
                                  name="smtppass" placeholder="SMTP password"  required>
                                  <span class="glyphicon form-control-feedback" aria-hidden="true"></span>
                                </div>
                                <div class="col-sm-4">
                                  <div class="help-block with-errors"></div>
                                </div>
                              </div>
                            </div>
                            <div class="form-group advanced_smtp_settings" style="display: none;">
                              <div class="row">
                                <div class="checkbox col-sm-offset-2">
                                  <label>
                                    <input type="checkbox" name="use_ssl" id="smtp_ssl"> Use SSL
                                  </label>
                                </div>
                              </div>
                            </div>
                            <?php }?>
                            <div class="form-group smtp_settings_fields">
                              <button type="button" class="btn btn-primary" id="smtp_settings_save">Save changes</button>
                            </div>
                          </form>
                        </div>
                    <?php }else{ ?>
                    <h3 class="page-header">Dashboard</h3>
                    <?php }?>
                </div>
                <!-- /.col-lg-12 -->
            </div>
            <!-- /.row -->
            <?php if (isset($_GET['serverid'])&&isset($_GET['item'])){?>
              <?php if ($_GET['item']=="info"){?>
              <div class="row">
                <div class="col-sm-4">
                  <div class="panel panel-primary">
                      <div class="panel-heading">Podstawowe informacje</div>
                      <div class="panel-body">
                        <dl class="dl-horizontal">
                          <dt>Hostname</dt><dd><?php echo $_smarty_tpl->tpl_vars['sysinfo']->value['hostname'];?>
</dd>
                          <dt>IP</dt><dd><?php echo $_smarty_tpl->tpl_vars['sysinfo']->value['ip'];?>
</dd>
                          <dt>Public IP</dt><dd><?php echo $_smarty_tpl->tpl_vars['sysinfo']->value['ext_ip'];?>
</dd>
                          <dt>Uptime</dt><dd><?php echo $_smarty_tpl->tpl_vars['sysinfo']->value['uptime'];?>
</dd>
                          <dt>RAM</dt><dd><?php echo $_smarty_tpl->tpl_vars['sysinfo']->value['ram_total'];?>
 GB</dd>
                          <dt>System</dt><dd><?php echo $_smarty_tpl->tpl_vars['sysinfo']->value['system'];?>
</dd>
                          <dt>Status</dt>
                          <?php if ($_smarty_tpl->tpl_vars['sysinfo']->value['status']=="U"){?>
                          <dd><span class="label label-danger">Offline</span></dd>
                          <?php }elseif($_smarty_tpl->tpl_vars['sysinfo']->value['status']=="A"){?>
                          <dd><span class="label label-success">Online</span></dd>
                          <?php }elseif($_smarty_tpl->tpl_vars['sysinfo']->value['status']=="S"){?>
                          <dd><span class="label label-warning">Not monitored</span></dd>
                          <?php }?>
                          <?php if ($_smarty_tpl->tpl_vars['sysinfo']->value['status']=="S"){?>
                          <dt>Monitoring</dt><dd><input type="checkbox" name="monitoring" data-serverid="<?php echo $_GET['serverid'];?>
"></dd>
                          <?php }else{ ?>
                          <dt>Monitoring</dt><dd><input type="checkbox" name="monitoring" checked data-serverid="<?php echo $_GET['serverid'];?>
"></dd>
                          <?php }?>
                        </dl>
                      </div>
                  </div>
                </div>
                <div class="col-sm-5">
                  <div class="panel panel-primary">
                      <div class="panel-heading" id="gauge-heading">[HDD Free: <?php echo $_smarty_tpl->tpl_vars['sysinfo']->value['hdd_free'];?>
 GB] [RAM Free: <?php echo $_smarty_tpl->tpl_vars['sysinfo']->value['ram_free'];?>
 GB] [CPU Usage: <?php echo smarty_function_math(array('equation'=>"(".((string)$_smarty_tpl->tpl_vars['sysinfo']->value).".cpu_usage * 100)"),$_smarty_tpl);?>
 %]</div>
                      <div class="panel-body" id="gauge-panel">
                        <div class="row-fluid">
                          <div id="hddUsedGauge_<?php echo $_GET['serverid'];?>
" class="col-sm-4 epoch gauge-small hdd-gauge"><label>HDD usage</label></div>
                          <div id="ramUsedGauge_<?php echo $_GET['serverid'];?>
" class="col-sm-4 epoch gauge-small ram-gauge"><label>RAM usage</label></div>
                          <div id="CPUGauge_<?php echo $_GET['serverid'];?>
" class="col-sm-4 epoch gauge-small cpu-gauge"><label>CPU usage</label></div>
                        </div>
                      </div>
                  </div>
                </div>
              </div>
              <div class="row-fluid">
                <div class="col-sm-2">
                  <div class="panel panel-primary">
                    <div class="panel-heading">Usługi systemowe</div>
                    <div class="panel-body">
                      <?php  $_smarty_tpl->tpl_vars['state'] = new Smarty_Variable; $_smarty_tpl->tpl_vars['state']->_loop = false;
 $_smarty_tpl->tpl_vars['sname'] = new Smarty_Variable;
 $_from = $_smarty_tpl->tpl_vars['hostsrv']->value; if (!is_array($_from) && !is_object($_from)) { settype($_from, 'array');}
foreach ($_from as $_smarty_tpl->tpl_vars['state']->key => $_smarty_tpl->tpl_vars['state']->value){
$_smarty_tpl->tpl_vars['state']->_loop = true;
 $_smarty_tpl->tpl_vars['sname']->value = $_smarty_tpl->tpl_vars['state']->key;
?>
                      <div class="row">
                        <div class="col-sm-1 srv-name"><strong><?php echo $_smarty_tpl->tpl_vars['sname']->value;?>
</strong></div>
                        <div class="col-sm-2 srv-status">
                          <?php if ($_smarty_tpl->tpl_vars['state']->value=="OK"){?>
                          <img src="/images/led-on.png" class="srvstate">
                          <?php }else{ ?>
                          <img src="/images/led-off.png" class="srvstate">
                          <?php }?>
                        </div>
                      </div>
                      <?php } ?>
                    </div>
                  </div>
                </div>
                <div class="col-sm-7">
                  <div class="panel panel-primary">
                    <div class="panel-heading">Utylizacja pasma</div>
                    <div class="panel-body" data-serverid="<?php echo $_GET['serverid'];?>
">
                      <div id="traffic_<?php echo $_GET['serverid'];?>
" class="epoch category10 epoch-theme-default" style="width: 800px; height: 200px"></div>
                    </div>
                  </div>
                </div>
              </div>
              <?php }elseif($_GET['item']=="sysusers"){?>
              <div class="row">
                <div class="col-sm-4" id="user_acc_header"><h3>Konta użytkowników</h3></div>
                <div class="col-sm-4" id="user_acc_btn"><button class="btn btn-success" type="button" data-id="<?php echo $_GET['serverid'];?>
">Nowe konto</button></div>
              </div>
              <div class="row">
                <div class="col-sm-5">
                  <table class="table table-stripped" id="users_table">
                    <thead>
                      <th>Login</th><th>Imię Nazwisko</th><th>E-mail</th>
                    </thead>
                    <tbody>
                    <?php  $_smarty_tpl->tpl_vars['info'] = new Smarty_Variable; $_smarty_tpl->tpl_vars['info']->_loop = false;
 $_smarty_tpl->tpl_vars['userid'] = new Smarty_Variable;
 $_from = $_smarty_tpl->tpl_vars['sysuser']->value; if (!is_array($_from) && !is_object($_from)) { settype($_from, 'array');}
foreach ($_from as $_smarty_tpl->tpl_vars['info']->key => $_smarty_tpl->tpl_vars['info']->value){
$_smarty_tpl->tpl_vars['info']->_loop = true;
 $_smarty_tpl->tpl_vars['userid']->value = $_smarty_tpl->tpl_vars['info']->key;
?>
                    <?php if ($_smarty_tpl->tpl_vars['info']->value['isactive']==0){?>
                      <tr class="danger">
                    <?php }else{ ?>
                      <tr>
                    <?php }?>
                        <td><?php echo $_smarty_tpl->tpl_vars['info']->value['login'];?>
</td><td><?php echo $_smarty_tpl->tpl_vars['info']->value['fullname'];?>
</td><td><?php echo $_smarty_tpl->tpl_vars['info']->value['email'];?>
</td>
                        <td class="button-cell">
                          <div class="btn-group">
                            <?php if ($_smarty_tpl->tpl_vars['info']->value['login']=='root'){?>
                            <button type="button" class="btn btn-danger rm-sysuser" data-id="<?php echo $_smarty_tpl->tpl_vars['userid']->value;?>
" data-serverid="<?php echo $_GET['serverid'];?>
" disabled>Usuń</button>
                            <button type="button" class="btn btn-danger dropdown-toggle" data-toggle="dropdown" aria-haspopup="true" aria-expanded="false" disabled>
                            <?php }else{ ?>
                            <button type="button" class="btn btn-danger rm-sysuser" data-id="<?php echo $_smarty_tpl->tpl_vars['userid']->value;?>
" data-serverid="<?php echo $_GET['serverid'];?>
">Usuń</button>
                            <button type="button" class="btn btn-danger dropdown-toggle" data-toggle="dropdown" aria-haspopup="true" aria-expanded="false">
                            <?php }?>
                              <span class="caret"></span>
                              <span class="sr-only">Toggle Dropdown</span>
                            </button>
                            <ul class="dropdown-menu">
                              <li><a href="#" data-id="<?php echo $_smarty_tpl->tpl_vars['userid']->value;?>
" class="edit-user">Edytuj</a></li>
                              <?php if ($_smarty_tpl->tpl_vars['info']->value['isactive']==0){?>
                              <li><a href="#" data-id="<?php echo $_smarty_tpl->tpl_vars['userid']->value;?>
" class="block-user">Odblokuj</a></li>
                              <?php }else{ ?>
                              <li><a href="#" data-id="<?php echo $_smarty_tpl->tpl_vars['userid']->value;?>
" class="block-user">Zablokuj</a></li>
                              <?php }?>
                              <li><a href="#" data-id="<?php echo $_smarty_tpl->tpl_vars['userid']->value;?>
" class="reset-pass">Resetuj hasło</a></li>
                            </ul>
                          </div>
                        </td>
                      </tr>
                    <?php } ?>
                    </tbody>
                  </table>
                </div>
              </div>
              <?php }elseif($_GET['item']=="wwwsrv"){?>
              <div>
                  <!-- Nav tabs -->
                <ul class="nav nav-tabs" role="tablist">
                  <li role="presentation" class="active" id="apache-config"><a href="#wwwconfig" aria-controls="ogolne" role="tab" data-toggle="tab">Strony WWW</a></li>
                  <li role="presentation" id="www-users"><a href="#wwwusers" aria-controls="ogolne" role="tab" data-toggle="tab">Użytkownicy</a></li>
                </ul>

              <!-- Tab panes -->
              <div class="row div-margin-top-10">
                  <div class="col-sm-12">
                      <div class="tab-content">
                        <div role="tabpanel" class="tab-pane" id="wwwconfig">
                          <div class="row">
                            <div class="col-sm-4"><h3>Lista stron www</h3></div>
                            <div class="col-sm-4 new-item" id="new-vhost"><button class="btn btn-success" type="button" data-id="<?php echo $_GET['serverid'];?>
">Nowa strona</button></div>
                          </div>
                          <div class="col-sm-4">
                            <?php if (isset($_smarty_tpl->tpl_vars['EmptySiteList']->value)){?>
                            <h5>Brak danych</h5>
                            <?php }else{ ?>
                            <table class="table table-hover" id="vhost-table" data-id="<?php echo $_GET['serverid'];?>
">
                              <thead>
                                <th>Nazwa Strony</th><th>Status</th><th class="button-cell">Akcja</th>
                              </thead>
                              <tbody>
                                <?php  $_smarty_tpl->tpl_vars['website'] = new Smarty_Variable; $_smarty_tpl->tpl_vars['website']->_loop = false;
 $_smarty_tpl->tpl_vars['id'] = new Smarty_Variable;
 $_from = $_smarty_tpl->tpl_vars['websites']->value; if (!is_array($_from) && !is_object($_from)) { settype($_from, 'array');}
foreach ($_from as $_smarty_tpl->tpl_vars['website']->key => $_smarty_tpl->tpl_vars['website']->value){
$_smarty_tpl->tpl_vars['website']->_loop = true;
 $_smarty_tpl->tpl_vars['id']->value = $_smarty_tpl->tpl_vars['website']->key;
?>
                                <tr>
                                  <td>
                                    <a href="http://<?php echo $_smarty_tpl->tpl_vars['website']->value['ServerName'];?>
/" target="_blank"><?php echo $_smarty_tpl->tpl_vars['website']->value['ServerName'];?>
</a>
                                  </td>
                                  <td>
                                    <?php if ($_smarty_tpl->tpl_vars['website']->value['password']==1){?>
                                    <i class="fa fa-lock"></i>
                                    <?php }elseif($_smarty_tpl->tpl_vars['website']->value['access_type']==0){?>
                                    <i class="fa fa-ban"></i>
                                    <?php }elseif($_smarty_tpl->tpl_vars['website']->value['access_type']>1){?>
                                    <i class="fa fa-shield"></i>
                                    <?php }else{ ?>
                                    <i class="fa fa-globe"></i>
                                    <?php }?>
                                  </td>
                                  <td class="button-cell">
                                    <div class="btn-group">
                                      <button type="button" class="btn btn-danger rmvhost" data-id="<?php echo $_smarty_tpl->tpl_vars['id']->value;?>
" data-serverid="<?php echo $_GET['serverid'];?>
">Usuń</button>
                                      <button type="button" class="btn btn-danger dropdown-toggle" data-toggle="dropdown" aria-haspopup="true" aria-expanded="false">
                                        <span class="caret"></span>
                                        <span class="sr-only">Toggle Dropdown</span>
                                      </button>
                                      <ul class="dropdown-menu">
                                        <li><a href="#" data-id="<?php echo $_smarty_tpl->tpl_vars['id']->value;?>
" class="edit-apache-conf">Edytuj</a></li>
                                      </ul>
                                    </div>
                                  </td>
                                </tr>
                                <?php } ?>
                              </tbody>
                            </table>
                            <?php }?>
                          </div>
                        </div>
                        <div role="tabpanel" class="tab-pane" id="wwwusers">
                          <div class="row">
                            <div class="col-sm-4"><h3>Lista kont</h3></div>
                            <div class="col-sm-4 new-item" id="new-htuser"><button class="btn btn-success" type="button" data-id="<?php echo $_GET['serverid'];?>
">Nowe konto</button></div>
                          </div>
                          <div class="col-sm-4">
                            <?php if (isset($_smarty_tpl->tpl_vars['EmptyUserList']->value)){?>
                            <h5>Brak danych</h5>
                            <?php }else{ ?>
                            <table class="table table-hover" id="vhost-table">
                              <thead>
                                <th>Login</th><th class="button-cell">Akcja</th>
                              </thead>
                              <tbody>
                                <?php  $_smarty_tpl->tpl_vars['login'] = new Smarty_Variable; $_smarty_tpl->tpl_vars['login']->_loop = false;
 $_smarty_tpl->tpl_vars['id'] = new Smarty_Variable;
 $_from = $_smarty_tpl->tpl_vars['htusers']->value; if (!is_array($_from) && !is_object($_from)) { settype($_from, 'array');}
foreach ($_from as $_smarty_tpl->tpl_vars['login']->key => $_smarty_tpl->tpl_vars['login']->value){
$_smarty_tpl->tpl_vars['login']->_loop = true;
 $_smarty_tpl->tpl_vars['id']->value = $_smarty_tpl->tpl_vars['login']->key;
?>
                                <tr>
                                  <td>
                                    <?php echo $_smarty_tpl->tpl_vars['login']->value;?>

                                  </td>
                                  <td class="button-cell">
                                    <div class="btn-group">
                                      <button type="button" class="btn btn-danger rmuser" data-id="<?php echo $_smarty_tpl->tpl_vars['id']->value;?>
" data-serverid="<?php echo $_GET['serverid'];?>
">Usuń</button>
                                      <button type="button" class="btn btn-danger dropdown-toggle" data-toggle="dropdown" aria-haspopup="true" aria-expanded="false">
                                        <span class="caret"></span>
                                        <span class="sr-only">Toggle Dropdown</span>
                                      </button>
                                      <ul class="dropdown-menu">
                                        <li><a href="#" data-id="<?php echo $_smarty_tpl->tpl_vars['id']->value;?>
" class="change-htpassword">Zmień hasło</a></li>
                                      </ul>
                                    </div>
                                  </td>
                                </tr>
                                <?php } ?>
                              </tbody>
                            </table>
                            <?php }?>
                          </div>
                        </div>
                      </div>
                  </div>
              </div>
          </div>
              <?php }?>
            <?php }?>
            <?php if (isset($_GET['serverid'])||isset($_GET['settings'])){?>
            <div class="row">
            <?php }else{ ?>
            <div class="row">
                <div class="col-lg-3 col-md-6">
                    <div class="panel panel-primary">
                        <div class="panel-heading">
                            <div class="row">
                                <div class="col-xs-3">
                                    <i class="fa fa-linux fa-5x"></i>
                                </div>
                                <div class="col-xs-9 text-right">
                                    <div class="huge"><?php echo $_smarty_tpl->tpl_vars['HostTotalCount']->value;?>
</div>
                                    <div>Liczba serwerów</div>
                                </div>
                            </div>
                        </div>
                        <a href="?dashboard">
                            <div class="panel-footer">
                                <span class="pull-left">Zobacz szczegóły</span>
                                <span class="pull-right"><i class="fa fa-arrow-circle-right"></i></span>
                                <div class="clearfix"></div>
                            </div>
                        </a>
                    </div>
                </div>
                <div class="col-lg-3 col-md-6">
                    <div class="panel panel-green">
                        <div class="panel-heading">
                            <div class="row">
                                <div class="col-xs-3">
                                    <i class="fa fa-hdd-o fa-5x"></i>
                                </div>
                                <div class="col-xs-9 text-right">
                                    <div class="huge"><?php echo $_smarty_tpl->tpl_vars['FreeTotalGB']->value;?>
 GB</div>
                                    <div>Wolne miejsce na serwerach</div>
                                </div>
                            </div>
                        </div>
                        <a href="?show=hddfree">
                            <div class="panel-footer">
                                <span class="pull-left">Zobacz szczegóły</span>
                                <span class="pull-right"><i class="fa fa-arrow-circle-right"></i></span>
                                <div class="clearfix"></div>
                            </div>
                        </a>
                    </div>
                </div>
                <div class="col-lg-3 col-md-6">
                    <div class="panel panel-yellow">
                        <div class="panel-heading">
                            <div class="row">
                                <div class="col-xs-3">
                                    <i class="fa  fa-globe fa-5x"></i>
                                </div>
                                <div class="col-xs-9 text-right">
                                    <div class="huge"><?php echo $_smarty_tpl->tpl_vars['websiteCount']->value;?>
</div>
                                    <div>Strony WWW</div>
                                </div>
                            </div>
                        </div>
                        <a href="#">
                            <div class="panel-footer">
                                <span class="pull-left">View Details</span>
                                <span class="pull-right"><i class="fa fa-arrow-circle-right"></i></span>
                                <div class="clearfix"></div>
                            </div>
                        </a>
                    </div>
                </div>
                <div class="col-lg-3 col-md-6">
                    <div class="panel panel-red">
                        <div class="panel-heading">
                            <div class="row">
                                <div class="col-xs-3">
                                    <i class="fa fa-info-circle fa-5x"></i>
                                </div>
                                <div class="col-xs-9 text-right">
                                    <div class="huge">Logi</div>
                                </div>
                            </div>
                        </div>
                        <a href="?show=logs">
                            <div class="panel-footer">
                                <span class="pull-left">Pokaż logi</span>
                                <span class="pull-right"><i class="fa fa-arrow-circle-right"></i></span>
                                <div class="clearfix"></div>
                            </div>
                        </a>
                    </div>
                </div>
            </div>
            <?php }?>
            <!-- /.row -->
            <div class="row">
              <?php if ($_GET['show']=="hddfree"){?>
              <div class="col-sm-4 table-responsive" id="HDDFree">
                <ul class="media-list">
                  <?php  $_smarty_tpl->tpl_vars['info'] = new Smarty_Variable; $_smarty_tpl->tpl_vars['info']->_loop = false;
 $_smarty_tpl->tpl_vars['hostname'] = new Smarty_Variable;
 $_from = $_smarty_tpl->tpl_vars['SrvHDDFree']->value; if (!is_array($_from) && !is_object($_from)) { settype($_from, 'array');}
foreach ($_from as $_smarty_tpl->tpl_vars['info']->key => $_smarty_tpl->tpl_vars['info']->value){
$_smarty_tpl->tpl_vars['info']->_loop = true;
 $_smarty_tpl->tpl_vars['hostname']->value = $_smarty_tpl->tpl_vars['info']->key;
?>
                  <li class="media">
                    <div class="media-left">
                      <?php if ($_smarty_tpl->tpl_vars['info']->value['ostype']=="Ubuntu"){?>
                      <img class="media-object" src="/images/server-ubuntu.png" width="50">
                      <?php }else{ ?>
                      <img class="media-object" src="/images/server-centos.png" width="50">
                      <?php }?>
                    </div>
                    <div class="media-body">
                      <h4 class="media-heading"><strong><a href="?serverid=<?php echo $_smarty_tpl->tpl_vars['info']->value['serverid'];?>
&item=info"><?php echo $_smarty_tpl->tpl_vars['hostname']->value;?>
</a></strong></h4>
                        <div class="freeHDDbar"><?php echo $_smarty_tpl->tpl_vars['info']->value['pfree'];?>
</div>
                    </div>
                  </li>
                  <?php } ?>
                </ul>
              </div>
              <?php }elseif(isset($_GET['dashboard'])){?>
              <div class="col-sm-4 table-responsive">
                <ul class="media-list">
                  <?php  $_smarty_tpl->tpl_vars['info'] = new Smarty_Variable; $_smarty_tpl->tpl_vars['info']->_loop = false;
 $_smarty_tpl->tpl_vars['hostname'] = new Smarty_Variable;
 $_from = $_smarty_tpl->tpl_vars['allHostStatus']->value; if (!is_array($_from) && !is_object($_from)) { settype($_from, 'array');}
foreach ($_from as $_smarty_tpl->tpl_vars['info']->key => $_smarty_tpl->tpl_vars['info']->value){
$_smarty_tpl->tpl_vars['info']->_loop = true;
 $_smarty_tpl->tpl_vars['hostname']->value = $_smarty_tpl->tpl_vars['info']->key;
?>
                  <li class="media">
                    <div class="media-left">
                      <?php if ($_smarty_tpl->tpl_vars['info']->value['os']=="Ubuntu"&&$_smarty_tpl->tpl_vars['info']->value['status']=="A"){?>
                      <img class="media-object" src="/images/server-ubuntu_ok.png" width="50">
                      <?php }elseif($_smarty_tpl->tpl_vars['info']->value['os']=="Ubuntu"&&$_smarty_tpl->tpl_vars['info']->value['status']=="U"){?>
                      <img class="media-object" src="/images/server-ubuntu_error.png" width="50">
                      <?php }elseif($_smarty_tpl->tpl_vars['info']->value['os']=="Ubuntu"&&$_smarty_tpl->tpl_vars['info']->value['status']=="S"){?>
                      <img class="media-object" src="/images/server-ubuntu_warning.png" width="50">
                      <?php }elseif(($_smarty_tpl->tpl_vars['info']->value['os']=="Centos6"||$_smarty_tpl->tpl_vars['info']->value['os']=="Centos7")&&$_smarty_tpl->tpl_vars['info']->value['status']=="A"){?>
                      <img class="media-object" src="/images/server-centos_ok.png" width="50">
                      <?php }elseif(($_smarty_tpl->tpl_vars['info']->value['os']=="Centos6"||$_smarty_tpl->tpl_vars['info']->value['os']=="Centos7")&&$_smarty_tpl->tpl_vars['info']->value['status']=="U"){?>
                      <img class="media-object" src="/images/server-centos_error.png" width="50">
                      <?php }elseif(($_smarty_tpl->tpl_vars['info']->value['os']=="Centos6"||$_smarty_tpl->tpl_vars['info']->value['os']=="Centos7")&&$_smarty_tpl->tpl_vars['info']->value['status']=="S"){?>
                      <img class="media-object" src="/images/server-centos_warning.png" width="50">
                      <?php }?>
                    </div>
                    <div class="media-body hostList">
                      <h4 class="media-heading"><strong><a href="?serverid=<?php echo $_smarty_tpl->tpl_vars['info']->value['serverid'];?>
&item=info"><?php echo $_smarty_tpl->tpl_vars['hostname']->value;?>
</a></strong></h4>
                      <div><strong>Ostatnio aktywny:</strong> <?php echo $_smarty_tpl->tpl_vars['info']->value['lastSeen'];?>
</div>
                      <?php if ($_smarty_tpl->tpl_vars['info']->value['status']=="A"){?>
                      <div><strong>Uptime:</strong> <?php echo $_smarty_tpl->tpl_vars['info']->value['uptime'];?>
 <span class="label label-success">Online</span></div>
                      <?php }elseif($_smarty_tpl->tpl_vars['info']->value['status']=="U"){?>
                      <div><strong>Uptime:</strong> <?php echo $_smarty_tpl->tpl_vars['info']->value['uptime'];?>
 <span class="label label-danger">Offline</span></div>
                      <?php }elseif($_smarty_tpl->tpl_vars['info']->value['status']=="S"){?>
                      <div><strong>Uptime:</strong> <?php echo $_smarty_tpl->tpl_vars['info']->value['uptime'];?>
 <span class="label label-warning">Not monitored</span></div>
                      <?php }?>
                    </div>
                  </li>
                  <?php } ?>
                </ul>
              </div>
              <?php }elseif($_GET['show']=="logs"){?>
              <div class="col-sm-6 table-responsive">
                <h3>Logi systemu</h3>
                <table id="logs" class="table display">
                  <thead>
                    <th>Informacja</th><th>Kategoria</th><th>Host</th><th>Timestamp</th>
                  </thead>
                  <tbody>
                    <?php  $_smarty_tpl->tpl_vars['entry'] = new Smarty_Variable; $_smarty_tpl->tpl_vars['entry']->_loop = false;
 $_smarty_tpl->tpl_vars['id'] = new Smarty_Variable;
 $_from = $_smarty_tpl->tpl_vars['Logs']->value; if (!is_array($_from) && !is_object($_from)) { settype($_from, 'array');}
foreach ($_from as $_smarty_tpl->tpl_vars['entry']->key => $_smarty_tpl->tpl_vars['entry']->value){
$_smarty_tpl->tpl_vars['entry']->_loop = true;
 $_smarty_tpl->tpl_vars['id']->value = $_smarty_tpl->tpl_vars['entry']->key;
?>
                    <?php if ($_smarty_tpl->tpl_vars['entry']->value['state']=="A"){?>
                    <tr class="success">
                    <?php }elseif($_smarty_tpl->tpl_vars['entry']->value['state']=="U"){?>
                    <tr class="danger">
                    <?php }elseif($_smarty_tpl->tpl_vars['entry']->value['state']=="S"){?>
                    <tr class="warning">
                    <?php }elseif($_smarty_tpl->tpl_vars['entry']->value['state']=="M"){?>
                    <tr class="info">
                    <?php }?>
                    <?php if ($_smarty_tpl->tpl_vars['entry']->value['category']=="host"&&$_smarty_tpl->tpl_vars['entry']->value['state']=="U"){?>
                      <td><i class="fa fa-frown-o"></i> Host <?php echo $_smarty_tpl->tpl_vars['entry']->value['hostname'];?>
 unavailable!</td>
                    <?php }elseif($_smarty_tpl->tpl_vars['entry']->value['category']=="host"&&$_smarty_tpl->tpl_vars['entry']->value['state']=="A"){?>
                      <td><i class="fa fa-smile-o"></i> Host <?php echo $_smarty_tpl->tpl_vars['entry']->value['hostname'];?>
 UP!</td>
                    <?php }elseif($_smarty_tpl->tpl_vars['entry']->value['category']=="host"&&$_smarty_tpl->tpl_vars['entry']->value['state']=="S"){?>
                      <td><i class="fa fa-exclamation-triangle"></i> Monitoring disabled on host <?php echo $_smarty_tpl->tpl_vars['entry']->value['hostname'];?>
</td>
                    <?php }elseif($_smarty_tpl->tpl_vars['entry']->value['category']=="host"&&$_smarty_tpl->tpl_vars['entry']->value['state']=="M"){?>
                      <td><i class="fa fa-info-circle"></i> Monitoring enabled on host <?php echo $_smarty_tpl->tpl_vars['entry']->value['hostname'];?>
</td>
                    <?php }elseif(($_smarty_tpl->tpl_vars['entry']->value['category']=="httpd"||$_smarty_tpl->tpl_vars['entry']->value['category']=="sshd")&&$_smarty_tpl->tpl_vars['entry']->value['state']=="U"){?>
                      <td><i class="fafa-frown-o"></i> Service <?php echo $_smarty_tpl->tpl_vars['entry']->value['category'];?>
 on host <?php echo $_smarty_tpl->tpl_vars['entry']->value['hostname'];?>
 is down!</td>
                    <?php }elseif(($_smarty_tpl->tpl_vars['entry']->value['category']=="httpd"||$_smarty_tpl->tpl_vars['entry']->value['category']=="sshd")&&$_smarty_tpl->tpl_vars['entry']->value['state']=="A"){?>
                      <td><i class="fafa-smile-o"></i> Service <?php echo $_smarty_tpl->tpl_vars['entry']->value['category'];?>
 on host <?php echo $_smarty_tpl->tpl_vars['entry']->value['hostname'];?>
 is UP!</td>
                    <?php }?>
                      <td><?php echo $_smarty_tpl->tpl_vars['entry']->value['category'];?>
</td>
                      <td><?php echo $_smarty_tpl->tpl_vars['entry']->value['hostname'];?>
</td>
                      <td><?php echo $_smarty_tpl->tpl_vars['entry']->value['timestamp'];?>
</td>
                    </tr>
                    <?php } ?>
                  </tbody>
                </table>
              </div>
              <?php }?>
            </div>
            <!-- /.row -->
        </div>
        <!-- /#page-wrapper -->

    </div>
    <!-- /#wrapper -->

</body>

</html>
<?php }} ?>