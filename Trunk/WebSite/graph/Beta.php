<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
<head>

  


  
  <title>Graph - Download beta</title>
  <meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">


  
  <link rel="stylesheet" href="Graph.css" type="text/css">
</head>


<body>


<div id="main">
<h1 align="center"><font color="#0000ff" size="+7"><b>Beta test</b></font></h1>


<font size="+1">
<p align="left">The link below can be used to download the newest beta
version of Graph. It is updated with irregular intervals. Notice that
the beta version is not fully tested, and it will therefore contain
bugs and not fully implemented features. You should only use it if you
are interested in participating in the test of Graph.<br>

</p>


</font>
<p align="center"> <font size="+2"> <a href="DownloadBeta.php"><?php $ini_file = parse_ini_file("GraphBeta.inf"); echo basename($ini_file["DownloadFile"]); ?>&nbsp;</a></font></p>
<p style="text-align: left;"><big>You can subscribe to a mailing list
if you want an email every time the beta version is updated.
Subscription can be done&nbsp;from the <a href="https://lists.sourceforge.net/lists/listinfo/graph-beta">info page</a>.</big></p>


</div>


<big><?php include './Menu.inc'; ?></big>
</body>
</html>
