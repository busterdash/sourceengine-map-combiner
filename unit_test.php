<?php

/*==============================================================================================

	unit_test.php
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

class test
{
	private static $cause = '';
	
	public static function display_result($method, $test)
	{
		$out = $method . '(...)';
		$out .= '&nbsp;<span style="color:';
		
		if ($test)
		{
			$out .= 'green' . '">' . 'PASS';
		}
		else
		{
			$out .= 'red' . '">' . 'FAIL [' . test::$cause . ']';
		}
		
		$out .= '</span>';
		return $out;
	}

	public static function compare_result($status, $func, $str)
	{
		if ($status)
			test::$cause = "$func!=$str";
		
		return (strcmp($func,$str) === 0) and $status;
	}
}

function test_translate($rigorous)
{
	$pass = true;
	
	if ($rigorous)
	{
		for ($z = -2; $z < 2; $z+=0.5)
		{
			for ($y = -2; $y < 2; $y+=0.5)
			{
				for ($x = -2; $x < 2; $x+=0.5)
				{
					$pass = test::compare_result($pass,
						vmf_read::translate('1 2 3',$x,$y,$z),
						($x+1).' '.($y+2).' '.($z+3)
					);
					
					$pass = test::compare_result($pass,
						vmf_read::translate('(1 2 3)',$x,$y,$z),
						'('.($x+1).' '.($y+2).' '.($z+3).')'
					);
					
					$pass = test::compare_result($pass,
						vmf_read::translate('(1 2 3) (4 5 6) (7 8 9)',$x,$y,$z),
						'('.($x+1).' '.($y+2).' '.($z+3).') ' .
						'('.($x+4).' '.($y+5).' '.($z+6).') ' .
						'('.($x+7).' '.($y+8).' '.($z+9).')'
					);
				}
			}
		}
	}
	else
	{
		//Less rigorous test here later.
	}
	
	return $pass;
}

require('vmf.inc');
echo '<!DOCTYPE html><html><body style="font-family: courier">';
echo '<h1>Unit Tests for VMF.inc Class Methods</h1>';
echo '<h3>VMF_READ STATIC CLASS</h3>';
echo test::display_result('translate',test_translate(true));
echo '</body></html>';

?>