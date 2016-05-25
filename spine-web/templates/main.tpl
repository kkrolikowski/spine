<!DOCTYPE html>
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

    <!-- Notification plugin -->
    <script src="https://cdnjs.cloudflare.com/ajax/libs/bootstrap-growl/1.0.0/jquery.bootstrap-growl.min.js"></script>

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

      <!-- Customowy skrypt strony i css -->
      <script src="js/spine.js"></script>
      <link href="/css/custom.css" rel="stylesheet">
</head>

<body>
  <!-- BEGIN: Sekcja formularzy edycji danych -->
    <!-- BEGIN: Edycja konfiguracji virtualhostow -->
    <form id="vhostEditForm" method="post" class="form-horizontal" role="form" style="display: none;">
      <input type="hidden" name="id" value="">
      <input type="hidden" name="serverid" value="{$smarty.get.serverid}">
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
              {foreach from=$wwwuser key=id item=user}
              <option value="{$id}">{$user}</option>
              {/foreach}
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
      <input type="hidden" name="serverid" value="{$smarty.get.serverid}">
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
              {foreach from=$wwwuser key=id item=user}
              <option value="{$id}">{$user}</option>
              {/foreach}
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
                <a class="navbar-brand" href="index.php"><img src="/images/spine-logo.png"></a>
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
                            <a href="index.php"><i class="fa fa-dashboard fa-fw"></i> Dashboard</a>
                        </li>
                        <li>
                            <a href="#"><i class="fa fa-linux f-fw"></i> Serwery<span class="fa arrow"></span></a>
                            <ul class="nav nav-second-level">
                                {foreach from=$HostMenu item=srv key=id}
                                   <li>
                                     <a href="#">{$srv} <span class="fa arrow"></span></a>
                                     <ul class="nav nav-third-level">
                                       <li>
                                         <a href="?serverid={$id}&item=info">Informacje</a>
                                       </li>
                                       <li>
                                         <a href="?serverid={$id}&item=sysusers">Konta systemowe</a>
                                       </li>
                                       <li class="apache-section">
                                         <a href="?serverid={$id}&item=wwwsrv">Serwer WWW</a>
                                       </li>
                                     </ul>
                                   </li>
                                {/foreach}
                            </ul>
                            <!-- /.nav-second-level -->
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
                    {if isset($smarty.get.serverid)}
                    <ol class="breadcrumb">
                      {if $smarty.get.item == "info"}
                      <li class="active">Informacje ogólne</li>
                      <li><a href="?serverid={$smarty.get.serverid}&item=sysusers">Konta systemowe</a></li>
                      <li class="apache-section"><a href="?serverid={$smarty.get.serverid}&item=wwwsrv">Serwer WWW</a></li>
                      {elseif $smarty.get.item == "sysusers"}
                      <li><a href="?serverid={$smarty.get.serverid}&item=info">Informacje ogólne</a></li>
                      <li class="active">Konta systemowe</li>
                      <li class="apache-section"><a href="?serverid={$smarty.get.serverid}&item=wwwsrv">Serwer WWW</a></li>
                      {elseif $smarty.get.item == "wwwsrv"}
                      <li><a href="?serverid={$smarty.get.serverid}&item=info">Informacje ogólne</a></li>
                      <li><a href="?serverid={$smarty.get.serverid}&item=sysusers">Konta systemowe</a></li>
                      <li class="active">Serwer WWW</li>
                      {/if}
                    </ol>
                    <h3 class="page-header">
                      <img src="/images/server.png" width="70">Host: {$HostName}</h3>
                    {else}
                    <h3 class="page-header">Dashboard</h3>
                    {/if}
                </div>
                <!-- /.col-lg-12 -->
            </div>
            <!-- /.row -->
            {if isset($smarty.get.serverid) && isset($smarty.get.item)}
              {if $smarty.get.item == "info"}
              <div class="row">
                <div class="col-sm-4">
                  <div class="panel panel-primary">
                      <div class="panel-heading">Podstawowe informacje</div>
                      <div class="panel-body">
                        <dl class="dl-horizontal">
                          <dt>Hostname</dt><dd>{$sysinfo.hostname}</dd>
                          <dt>IP</dt><dd>{$sysinfo.ip}</dd>
                          <dt>Public IP</dt><dd>{$sysinfo.ext_ip}</dd>
                          <dt>Uptime</dt><dd>{$sysinfo.uptime}</dd>
                          <dt>RAM</dt><dd>{$sysinfo.ram_total} GB</dd>
                          <dt>System</dt><dd>{$sysinfo.system}</dd>
                        </dl>
                      </div>
                  </div>
                </div>
                <div class="col-sm-5">
                  <div class="panel panel-primary">
                      <div class="panel-heading">Dysk / RAM</div>
                      <div class="panel-body">
                        <div class="row">
                          <div class="col-sm-8">
                            <center><strong>Filesystem: /</strong></center>
                          </div>
                        </div>
                        <div class="row">
                          <div class="col-sm-4">
                            <div id="diskusage_free" data="{$sysinfo.hdd_percentage_free}"></div>
                          </div>
                          <div class="col-sm-4">
                            <div id="diskusage_used" data="{$sysinfo.hdd_percentage_used}"></div>
                          </div>
                          <div class="col-sm-4">
                            <div id="ram_free" data="{$sysinfo.ram_percentage_free}"></div>
                          </div>
                        </div>
                        <div class="row">
                          <div class="col-sm-4">
                            <center><strong>{$sysinfo.hdd_free} GB</strong></center>
                          </div>
                          <div class="col-sm-4">
                            <center><strong>{$sysinfo.hdd_used} GB</strong></center>
                          </div>
                          <div class="col-sm-4">
                            <center><strong>{$sysinfo.ram_free} GB</strong></center>
                          </div>
                        </div>
                      </div>
                  </div>
                </div>
              </div>
              {elseif $smarty.get.item == "sysusers"}
              <div class="row">
                <h3>Lista kont użytkowników w systemie</h3>
                <div class="col-sm-6">
                  <table class="table table-stripped">
                    <thead>
                      <th>Login</th><th>Imię Nazwisko</th><th>E-mail</th>
                    </thead>
                    <tbody>
                    {foreach from=$sysuser key=userid item=info}
                      <tr>
                        <td>{$info.login}</td><td>{$info.fullname}</td><td>{$info.email}</td>
                      </tr>
                    {/foreach}
                    </tbody>
                  </table>
                </div>
              </div>
              {elseif $smarty.get.item == "wwwsrv"}
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
                            <div class="col-sm-4" id="new-vhost"><button class="btn btn-success" type="button" data-id="{$smarty.get.serverid}">Nowa strona</button></div>
                          </div>
                          <div class="col-sm-4">
                            {if isset($EmptySiteList)}
                            <h5>Brak danych</h5>
                            {else}
                            <table class="table table-hover" id="vhost-table">
                              <thead>
                                <th>Nazwa Strony</th><th class="button-cell">Akcja</th>
                              </thead>
                              <tbody>
                                {foreach from=$websites key=id item=website}
                                <tr>
                                  <td>
                                    <a href="http://{$website.ServerName}/" target="_blank">{$website.ServerName}</a>
                                  </td>
                                  <td class="button-cell">
                                    <div class="btn-group">
                                      <button type="button" class="btn btn-danger" data-id="{$id}">Usuń</button>
                                      <button type="button" class="btn btn-danger dropdown-toggle" data-toggle="dropdown" aria-haspopup="true" aria-expanded="false">
                                        <span class="caret"></span>
                                        <span class="sr-only">Toggle Dropdown</span>
                                      </button>
                                      <ul class="dropdown-menu">
                                        <li><a href="#" data-id="{$id}" class="edit-apache-conf">Edytuj</a></li>
                                      </ul>
                                    </div>
                                  </td>
                                </tr>
                                {/foreach}
                              </tbody>
                            </table>
                            {/if}
                          </div>
                        </div>
                        <div role="tabpanel" class="tab-pane" id="wwwusers">
                          <div class="row">
                            <div class="col-sm-4"><h3>Lista kont</h3></div>
                            <div class="col-sm-4" id="new-htuser"><button class="btn btn-success" type="button" data-id="{$smarty.get.serverid}">Nowe konto</button></div>
                          </div>
                          <div class="col-sm-4">
                            {if isset($EmptyUserList)}
                            <h5>Brak danych</h5>
                            {else}
                            <table class="table table-hover" id="vhost-table">
                              <thead>
                                <th>Nazwa Strony</th><th class="button-cell">Akcja</th>
                              </thead>
                              <tbody>
                                {foreach from=$htusers key=id item=login}
                                <tr>
                                  <td>
                                    {$login}
                                  </td>
                                  <td class="button-cell">
                                    <div class="btn-group">
                                      <button type="button" class="btn btn-danger" data-id="{$id}">Usuń</button>
                                      <button type="button" class="btn btn-danger dropdown-toggle" data-toggle="dropdown" aria-haspopup="true" aria-expanded="false">
                                        <span class="caret"></span>
                                        <span class="sr-only">Toggle Dropdown</span>
                                      </button>
                                      <ul class="dropdown-menu">
                                        <li><a href="#" data-id="{$id}" class="edit-apache-conf">Edytuj</a></li>
                                      </ul>
                                    </div>
                                  </td>
                                </tr>
                                {/foreach}
                              </tbody>
                            </table>
                            {/if}
                          </div>
                        </div>
                      </div>
                  </div>
              </div>
          </div>
              {/if}
            {/if}
            {if isset($smarty.get.serverid)}
            <div class="row">
            {else}
            <div class="row">
                <div class="col-lg-3 col-md-6">
                    <div class="panel panel-primary">
                        <div class="panel-heading">
                            <div class="row">
                                <div class="col-xs-3">
                                    <i class="fa fa-linux fa-5x"></i>
                                </div>
                                <div class="col-xs-9 text-right">
                                    <div class="huge">{$HostTotalCount}</div>
                                    <div>Liczba serwerów</div>
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
                    <div class="panel panel-green">
                        <div class="panel-heading">
                            <div class="row">
                                <div class="col-xs-3">
                                    <i class="fa fa-hdd-o fa-5x"></i>
                                </div>
                                <div class="col-xs-9 text-right">
                                    <div class="huge">{$FreeTotalGB} GB</div>
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
                                    <div class="huge">{$websiteCount}</div>
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
                                    <i class="fa fa-support fa-5x"></i>
                                </div>
                                <div class="col-xs-9 text-right">
                                    <div class="huge">13</div>
                                    <div>Support Tickets!</div>
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
            </div>
            {/if}
            <!-- /.row -->
            <div class="row">
              {if $smarty.get.show == "hddfree"}
              <div class="col-sm-4 table-responsive" id="HDDFree">
                <ul class="media-list">
                  {foreach from=$SrvHDDFree key=hostname item=info}
                  <li class="media">
                    <div class="media-left">
                      {if $info.ostype == "Ubuntu"}
                      <img class="media-object" src="/images/server-ubuntu.png" width="50">
                      {else}
                      <img class="media-object" src="/images/server-centos.png" width="50">
                      {/if}
                    </div>
                    <div class="media-body">
                      <h4 class="media-heading"><strong>{$hostname}</strong></h4>
                        <div class="freeHDDbar">{$info.pfree}</div>
                    </div>
                  </li>
                  {/foreach}
                </ul>
              </div>
              {/if}
            </div>
            <!-- /.row -->
        </div>
        <!-- /#page-wrapper -->

    </div>
    <!-- /#wrapper -->

</body>

</html>
