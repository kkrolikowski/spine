<?php /* Smarty version Smarty-3.1-DEV, created on 2016-04-27 23:31:14
         compiled from "./templates/main.tpl" */ ?>
<?php /*%%SmartyHeaderCode:2040616705571fd4f2a2e5b8-91589385%%*/if(!defined('SMARTY_DIR')) exit('no direct access allowed');
$_valid = $_smarty_tpl->decodeProperties(array (
  'file_dependency' => 
  array (
    '48ad11f68be179e1830edf3ce56b82b0712baeff' => 
    array (
      0 => './templates/main.tpl',
      1 => 1461792662,
      2 => 'file',
    ),
  ),
  'nocache_hash' => '2040616705571fd4f2a2e5b8-91589385',
  'function' => 
  array (
  ),
  'version' => 'Smarty-3.1-DEV',
  'unifunc' => 'content_571fd4f2aa02a7_43965124',
  'variables' => 
  array (
    'HostMenu' => 0,
    'id' => 0,
    'srv' => 0,
    'HostName' => 0,
    'sysinfo' => 0,
    'sysuser' => 0,
    'info' => 0,
    'EmptySiteList' => 0,
    'websites' => 0,
    'website' => 0,
    'wwwuser' => 0,
    'user' => 0,
    'HostTotalCount' => 0,
    'FreeTotalGB' => 0,
    'websiteCount' => 0,
  ),
  'has_nocache_code' => false,
),false); /*/%%SmartyHeaderCode%%*/?>
<?php if ($_valid && !is_callable('content_571fd4f2aa02a7_43965124')) {function content_571fd4f2aa02a7_43965124($_smarty_tpl) {?><!DOCTYPE html>
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

    <!-- Form validator plugin -->
    <script type="text/javascript" src="https://cdnjs.cloudflare.com/ajax/libs/1000hz-bootstrap-validator/0.9.0/validator.min.js"></script>

    <!-- Bootbox - wyskakujace okienka -->
    <script type="text/javascript" src="https://cdnjs.cloudflare.com/ajax/libs/bootbox.js/4.4.0/bootbox.min.js"></script>

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

      <!-- Customowy skrypt strony i css -->
      <script src="js/spine.js"></script>
      <link href="/css/custom.css" rel="stylesheet">
</head>

<body>
  <!-- BEGIN: Sekcja formularzy edycji danych -->
    <form id="vhostEditForm" method="post" class="form-horizontal" role="form" style="display: none;">
      <input type="hidden" name="id" value="">
      <div class="form-group">
        <div class="row">
          <div class="col-sm-offset-2 enable-option">
            <label class="checkbox-inline">
              <input type="checkbox" id="enable_sa" value="enable_sa"> <strong>Konfiguracja ServerAlias</strong>
            </label>
          </div>
        </div>
      </div>
      <div class="form-group" id="sa-group">
        <div class="row">
          <label for="sa" class="col-sm-2 control-label">ServerAlias</label>
          <div class="col-sm-4">
            <input type="text" class="form-control" id="sa"
            data-minlength="3" data-error="Wpisz co najmniej trzy znaki"
            name="sa[0]" placeholder="*.example.com" required disabled>
            <span class="glyphicon form-control-feedback" aria-hidden="true"></span>
          </div>
          <div >
            <span class="glyphicon glyphicon-plus" aria-hidden="true"></span>
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
    </form>
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
                                <?php  $_smarty_tpl->tpl_vars['srv'] = new Smarty_Variable; $_smarty_tpl->tpl_vars['srv']->_loop = false;
 $_smarty_tpl->tpl_vars['id'] = new Smarty_Variable;
 $_from = $_smarty_tpl->tpl_vars['HostMenu']->value; if (!is_array($_from) && !is_object($_from)) { settype($_from, 'array');}
foreach ($_from as $_smarty_tpl->tpl_vars['srv']->key => $_smarty_tpl->tpl_vars['srv']->value){
$_smarty_tpl->tpl_vars['srv']->_loop = true;
 $_smarty_tpl->tpl_vars['id']->value = $_smarty_tpl->tpl_vars['srv']->key;
?>
                                   <li><a href="?serverid=<?php echo $_smarty_tpl->tpl_vars['id']->value;?>
"><?php echo $_smarty_tpl->tpl_vars['srv']->value;?>
</a></li>
                                <?php } ?>
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
                    <?php if (isset($_GET['serverid'])){?>
                    <h3 class="page-header">
                      <img src="/images/server.png" width="70">Host: <?php echo $_smarty_tpl->tpl_vars['HostName']->value;?>
</h3>
                    <?php }else{ ?>
                    <h3 class="page-header">Dashboard</h3>
                    <?php }?>
                </div>
                <!-- /.col-lg-12 -->
            </div>
            <!-- /.row -->
            <?php if (isset($_GET['serverid'])){?>
            <div class="row">
                <div>
                    <!-- Nav tabs -->
                  <ul class="nav nav-tabs" role="tablist">
                    <li role="presentation" class="active"><a href="#ogolne" aria-controls="ogolne" role="tab" data-toggle="tab">Ogólne</a></li>
                    <li class="dropdown">
                      <a class="dropdown-toggle" data-toggle="dropdown" href="#">Użytkownicy <span class="caret"></span></a>
                      <ul class="dropdown-menu">
                        <li role="presentation"><a href="#sysusers" aria-controls="sysusers" role="tab" data-toggle="tab">Lista</a></li>
                        <li role="presentation"><a href="#" aria-controls="sysusers" role="tab" data-toggle="tab">Nowy użytkownik</a></li>
                      </ul>
                    </li>
                    <li class="dropdown">
                      <a class="dropdown-toggle" data-toggle="dropdown" href="#">Serwer WWW <span class="caret"></span></a>
                      <ul class="dropdown-menu">
                        <li role="presentation"><a href="#wwwconfig" aria-controls="wwwconfig" role="tab" data-toggle="tab">Strony WWW</a></li>
                        <li role="presentation"><a href="#wwwconfignew" aria-controls="sysusers" role="tab" data-toggle="tab">Nowa strona</a></li>
                      </ul>
                    </li>
                  </ul>

                <!-- Tab panes -->
                <div class="row div-margin-top-10">
                    <div class="col-sm-12">
                        <div class="tab-content">
                            <div role="tabpanel" class="tab-pane active" id="ogolne">
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
                                            <dt>Uptime</dt><dd><?php echo $_smarty_tpl->tpl_vars['sysinfo']->value['uptime'];?>
</dd>
                                            <dt>RAM</dt><dd><?php echo $_smarty_tpl->tpl_vars['sysinfo']->value['ram_total'];?>
 GB</dd>
                                            <dt>System</dt><dd><?php echo $_smarty_tpl->tpl_vars['sysinfo']->value['system'];?>
</dd>
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
                                              <div id="diskusage_free" data="<?php echo $_smarty_tpl->tpl_vars['sysinfo']->value['hdd_percentage_free'];?>
"></div>
                                            </div>
                                            <div class="col-sm-4">
                                              <div id="diskusage_used" data="<?php echo $_smarty_tpl->tpl_vars['sysinfo']->value['hdd_percentage_used'];?>
"></div>
                                            </div>
                                            <div class="col-sm-4">
                                              <div id="ram_free" data="<?php echo $_smarty_tpl->tpl_vars['sysinfo']->value['ram_percentage_free'];?>
"></div>
                                            </div>
                                          </div>
                                          <div class="row">
                                            <div class="col-sm-4">
                                              <center><strong><?php echo $_smarty_tpl->tpl_vars['sysinfo']->value['hdd_free'];?>
 GB</strong></center>
                                            </div>
                                            <div class="col-sm-4">
                                              <center><strong><?php echo $_smarty_tpl->tpl_vars['sysinfo']->value['hdd_used'];?>
 GB</strong></center>
                                            </div>
                                            <div class="col-sm-4">
                                              <center><strong><?php echo $_smarty_tpl->tpl_vars['sysinfo']->value['ram_free'];?>
 GB</strong></center>
                                            </div>
                                          </div>
                                        </div>
                                    </div>
                                  </div>
                                </div>
                            </div>
                            <div role="tabpanel" class="tab-pane" id="sysusers">
                              <h3>Lista kont użytkowników w systemie</h3>
                              <div class="col-sm-6">
                                <table class="table table-stripped">
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
                                    <tr>
                                      <td><?php echo $_smarty_tpl->tpl_vars['info']->value['login'];?>
</td><td><?php echo $_smarty_tpl->tpl_vars['info']->value['fullname'];?>
</td><td><?php echo $_smarty_tpl->tpl_vars['info']->value['email'];?>
</td>
                                    </tr>
                                  <?php } ?>
                                  </tbody>
                                </table>
                              </div>
                            </div>

                            <div role="tabpanel" class="tab-pane" id="wwwconfig">
                              <h3>Lista stron www</h3>
                              <div class="col-sm-4">
                                <?php if (isset($_smarty_tpl->tpl_vars['EmptySiteList']->value)){?>
                                <h5>Brak danych</h5>
                                <?php }else{ ?>
                                <table class="table table-hover">
                                  <thead>
                                    <th>Nazwa Strony</th><th class="button-cell">Akcja</th>
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
                                      <td class="button-cell">
                                        <div class="btn-group">
                                          <button type="button" class="btn btn-danger" data-id="<?php echo $_smarty_tpl->tpl_vars['id']->value;?>
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
                            <div role="tabpanel" class="tab-pane" id="wwwconfignew">
                              <blockquote>
                                <p class="lead"><em>Konfiguracja virtualhosta</em></p>
                              </blockquote>
                              <div class="col-sm-6" id="vhost-config">
                                <form role="form" class="form-horizontal" data-toggle="validator" id="addvhost">
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
                                        <label class="checkbox-inline">
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
                                        <span class="glyphicon glyphicon-plus" aria-hidden="true"></span>
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
                                      <div class="col-sm-offset-2 enable-option">
                                        <label class="checkbox-inline">
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
                                      <div class="col-sm-offset-4">
                                        <button type="submit" class="btn btn-primary" id="addvhost-btn">Zapisz konfigurację</button>
                                      </div>
                                    </div>
                                  </div>
                                </form>
                              </div>
                            </div>
                        </div>
                    </div>
                </div>
            </div>
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
                                    <div class="huge"><?php echo $_smarty_tpl->tpl_vars['FreeTotalGB']->value;?>
 GB</div>
                                    <div>Wolne miejsce na serwerach</div>
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
            <?php }?>
            <!-- /.row -->
            <div class="row">
            </div>
            <!-- /.row -->
        </div>
        <!-- /#page-wrapper -->

    </div>
    <!-- /#wrapper -->

</body>

</html>
<?php }} ?>