<?php
require("vmf.inc");
echo '<!DOCTYPE html><html><body style="font-family: courier">';

$time_start = microtime(true);
$document = new vmf('test_vmf');
vmf_read::open_vmf_file('partial1.vmf', $document);
echo '<h2 style="color:red">Execution Time: ' . floor((microtime(true)-$time_start)*10000)/10 . 'ms</h2>';
$fixed = $document->export();
$fixed = str_replace("\r\n","<br />\r\n",$fixed);
$fixed = str_replace("\t","&nbsp;&nbsp;&nbsp;&nbsp;",$fixed);
echo $fixed;
echo '</body></html>';

?>