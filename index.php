<?php
require("vmf.inc");
echo '<!DOCTYPE html><html><body style="font-family: courier">';
$document = new vmf('test_vmf');

$s1 = new section('solid');
$s1->create_pair('id','0');

$e1 = new section('entity');
$e1->create_pair('id','0');
$e1->create_pair('test','asdf');
$s2 = $e1->create_section('solid');
$s2->create_pair('id','0');
$s2->create_section('side')->create_pair('id','0');
$s2->create_section('side')->create_pair('id','0');
$s2->create_section('side')->create_pair('id','0');

$document->add_item($s1);
$document->add_item($e1);

$fixed = str_replace("\r\n","<br />\r\n",$document->export());
$fixed = str_replace("\t","&nbsp;&nbsp;&nbsp;&nbsp;",$fixed);
echo $fixed;
echo '</body></html>';

?>