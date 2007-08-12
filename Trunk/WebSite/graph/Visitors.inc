<?php
  $counter_file = '/customers/padowan.dk/padowan.dk/httpd.www/stat/Visitors.txt';
  $fh = fopen($counter_file, 'r+b');     ## use 'r+b' so file can be read and written
  if ($fh)
  {
    if (flock($fh, LOCK_EX))     ## don't do anything unless lock is successful
    {
      $count = fread($fh, filesize($counter_file));
      rewind($fh);
      $count++;
			echo $count;
      fwrite($fh, $count);
      fflush($fh);
      ftruncate($fh, ftell($fh));     ## better than truncating to 0 before writing
      flock($fh, LOCK_UN);
    } 
    else 
      echo "Could not lock counter file '$counter_file'";
    fclose($fh);
  } 
?>