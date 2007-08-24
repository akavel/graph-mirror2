<?php
  $counter_file = '../stat/GraphVersionCount.txt';
  $inf_file = 'Graph.inf';
  clearstatcache();
  ignore_user_abort(true);     ## prevent refresh from aborting file operations and hosing file
  $fh = fopen($counter_file, 'r+b');     ## use 'r+b' so file can be read and written
  if ($fh)
  {
    if (flock($fh, LOCK_EX))     ## don't do anything unless lock is successful
    {
      $count = fread($fh, filesize($counter_file));
      rewind($fh);
      $count++;
      fwrite($fh, $count);
      fflush($fh);
      ftruncate($fh, ftell($fh));     ## better than truncating to 0 before writing
      flock($fh, LOCK_UN);
    } 
    else 
      echo "Could not lock counter file '$counter_file'";
    fclose($fh);
  } 
  else  
    echo "Could not open counter file '$counter_file'";
  ignore_user_abort(false);     ## put things back to normal

$host  = $_SERVER['HTTP_HOST'];
$uri   = rtrim(dirname($_SERVER['PHP_SELF']), '/\\');
##  if($_GET["Version"] == "4.0" || $_GET["Version"] == "4.0.1" || $_GET["Version"] == "4.1")
    header("Location: http://$host$uri/$inf_file");
##  else
##    header("Location: http://www.padowan.dk/graph/GraphOld.inf");
?>
