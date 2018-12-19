<?php

/*==============================================================================================

	vmf.inc
	Copyright 2018 Buster Schrader

	This file is part of SourceMap Combiner.

    SourceMap Combiner is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.

    SourceMap Combiner is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with SourceMap Combiner.  If not, see <https://www.gnu.org/licenses/>.
	
==============================================================================================*/

require('vmf.inc');
echo '<!DOCTYPE html><html><body style="font-family: courier">';

/*header('Content-type: application/vmf');
header('Content-disposition: attachment; filename="generated.vmf"');
header('Pragma: no-cache');
header('Cache-control: no-cache, must-revalidate');
header('Expires: -1');*/

$time_start = microtime(true);
$document = new vmf('test_vmf');
vmf_read::open_vmf_file('partial.vmf', $document);
echo '<h2 style="color:red">Execution Time: ' . floor((microtime(true)-$time_start)*10000)/10 . 'ms</h2>';
$fixed = $document->export();
$fixed = str_replace("\r\n","<br />\r\n",$fixed);
$fixed = str_replace("\t","&nbsp;&nbsp;&nbsp;&nbsp;",$fixed);
echo $fixed;
echo '</body></html>';

?>