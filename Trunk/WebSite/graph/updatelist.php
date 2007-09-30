<?php
    $ToAddr = "graphupdate-subscribe@yahoogroups.com";
    $Body = "";
    $FromAddr = $HTTP_POST_VARS["email"];
    $Subject = "";
    $Redirect = "Subscribed.php";

    if(mail($ToAddr, $Subject, $Body, "From: ".$FromAddr."\n"))
    {
	header("Location: ".$Redirect);
	exit;
    }
    else
    {
	echo "Error sending mail";
	exit;
    }
?>