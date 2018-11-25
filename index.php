<?php

require("vmf.inc");
echo '<!DOCTYPE html><html><body style="font-family: courier">';
$document = new vmf('test_vmf');
$fixed = str_replace("\r\n","<br />\r\n",$document->export());
$fixed = str_replace("\t","&nbsp;&nbsp;&nbsp;&nbsp;",$fixed);
echo $fixed;
echo '</body></html>';

?>