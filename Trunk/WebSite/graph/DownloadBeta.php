<?php
  $ini_file = parse_ini_file("GraphBeta.inf");
  $download_file = $ini_file["DownloadFile"];
  header("Location: " . $download_file);
?>
