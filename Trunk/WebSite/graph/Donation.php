<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
<head>
  <title>Graph - Donate</title>
  <meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
  <link rel="stylesheet" href="Graph.css" type="text/css">
</head>
<body>

<div id="main">
<h1 align="center"><font color="#0000ff" size="+7"><b> Donate</b></font></h1>

<p><font size="+1">Graph is
completely free of charge, but there are some expenses involved in the
development of the program, like web hosting and development utilities.
If you want to express your appreciation of the time spent on the
development, I accept and appreciate donations to help cover the
expenses.<br>
The button below may be used to make donations through PayPal.<br>
Thank you in advance!
</font>
</p>

<form action="https://www.paypal.com/cgi-bin/webscr" method="post" target="_top">
  <div align="center">
  Currency: <select name="currency_code" size="1">
	  <option value="AUD">AUD&nbsp;&ndash;&nbsp;$&nbsp;</option>
	  <option value="CAD">CAD&nbsp;&ndash;&nbsp;$</option>
	  <option value="CHF">CHF&nbsp;&ndash;&nbsp;</option>
	  <option value="CZK">CZK&nbsp;&ndash;&nbsp;K&#269;</option>
	  <option value="DKK">DKK&nbsp;&ndash;&nbsp;kr</option>
	  <option value="EUR">EUR&nbsp;&ndash;&nbsp;&euro;</option>
	  <option value="HKD">HKD&nbsp;&ndash;&nbsp;HK$</option>
	  <option value="HUF">HUF&nbsp;&ndash;&nbsp;Ft</option>
	  <option value="GBP">GBP&nbsp;&ndash;&nbsp;&pound;</option>
	  <option value="JPY">JPY&nbsp;&ndash;&nbsp;&yen;</option>
	  <option value="NZD">NZD&nbsp;&ndash;&nbsp;NZ$</option>
	  <option value="NOK">NOK&nbsp;&ndash;&nbsp;kr</option>
	  <option value="PLN">PLN&nbsp;&ndash;&nbsp;z&#322;</option>
	  <option value="SGD">SGD&nbsp;&ndash;&nbsp;S$</option>
	  <option value="SEK">SEK&nbsp;&ndash;&nbsp;kr</option>
	  <option value="USD" selected="selected">USD&nbsp;&ndash;&nbsp;$</option>
  </select>
	<br>
	<input name="cmd" value="_xclick" type="hidden">
	<input name="business" value="graph@padowan.dk" type="hidden">
	<input name="item_name" value="Graph donation" type="hidden">
  <input name="image_url" value="http://padowan.dk/graph/images/Icon96.png" type="hidden">
  <input name="no_shipping" value="1" type="hidden"><input name="return" value="http://www.padowan.dk" type="hidden">
  <input name="cancel_return" value="http://www.padowan.dk" type="hidden">
  <input name="tax" value="0" type="hidden"><input name="cn" value="Comments" type="hidden">
  <input name="submit" value="Make donation" type="submit">
	</div>
</form>
</div>

<?php include './Menu.inc'; ?>
</body>
</html>
