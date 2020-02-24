<?php

/*==============================================================================================

	vmf.inc
	Copyright 2018-2020 Buster Schrader

	This file is part of SourceEngine Map Combiner.

    SourceEngine Map Combiner is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.

    SourceEngine Map Combiner is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with SourceEngine Map Combiner.  If not, see <https://www.gnu.org/licenses/>.
	
==============================================================================================*/

require('vmf.inc');
//echo '<!DOCTYPE html><html><body style="font-family: courier">';

header('Content-type: application/vmf');
header('Content-disposition: attachment; filename="generated.vmf"');
header('Pragma: no-cache');
header('Cache-control: no-cache, must-revalidate');
header('Expires: -1');

//$time_start = microtime(true);
$document = new vmf('test_vmf');
$document->set_skybox_tex("sky_day03_06");
vmf_read::open_vmf_file('test/base.vmf', 0,0,0, $document);
vmf_read::open_vmf_file('test/cliff.vmf', -1152,0,0, $document);
vmf_read::open_vmf_file('test/cliff.vmf', -768,0,0, $document);
vmf_read::open_vmf_file('test/cliff.vmf', -384,0,0, $document);
vmf_read::open_vmf_file('test/cliff.vmf', 0,0,0, $document);
vmf_read::open_vmf_file('test/cliff.vmf', 384,0,0, $document);
vmf_read::open_vmf_file('test/cliff.vmf', 768,0,0, $document);
vmf_read::open_vmf_file('test/cliff.vmf', 1152,0,0, $document);
/*vmf_read::open_vmf_file('test/prefab3.vmf', -384,0,0, $document);
vmf_read::open_vmf_file('test/prefab1.vmf', 0,0,0, $document);
vmf_read::open_vmf_file('test/prefab2.vmf', 384,0,0, $document);*/
//echo '<h2 style="color:red">Execution Time: ' . floor((microtime(true)-$time_start)*10000)/10 . 'ms</h2>';
$fixed = $document->export();
//$fixed = str_replace("\r\n","<br />\r\n",$fixed);
//$fixed = str_replace("\t","&nbsp;&nbsp;&nbsp;&nbsp;",$fixed);
echo $fixed;
//echo '</body></html>';

?>
