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

////////////////////////////////////////////////
// Please change any access modifiers for     //
// class methods to be tested from private or //
// protected to public access before using.   //
////////////////////////////////////////////////

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
		
		$out .= '</span><br />';
		return $out;
	}

	public static function compare_result($status, $func, $str)
	{
		if ($status)
			test::$cause = "$func!=$str";
		
		return (strcmp($func,$str) === 0) and $status;
	}
	
	public static function induced_cause($msg)
	{
		test::$cause = 'INDUCED: ' . $msg;
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
						vmf_read::translate('-1.1 2.3 -3.4',$x,$y,$z),
						($x-1.1).' '.($y+2.3).' '.($z-3.4)
					);
					
					$pass = test::compare_result($pass,
						vmf_read::translate('(-1.1 2.3 -3.4)',$x,$y,$z),
						'('.($x-1.1).' '.($y+2.3).' '.($z-3.4).')'
					);
					
					$pass = test::compare_result($pass,
						vmf_read::translate('[-1.1 2.3 -3.4]',$x,$y,$z),
						'['.($x-1.1).' '.($y+2.3).' '.($z-3.4).']'
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
		$pass = test::compare_result($pass,
			vmf_read::translate('-3.2 4.5 82.3',-0.1,-5.3,3.4),
			'-3.3 -0.8 85.7'
		);
		
		$pass = test::compare_result($pass,
			vmf_read::translate('(-405 -2.5 1005.35)',-300,-20.41,640),
			'(-705 -22.91 1645.35)'
		);
		
		$pass = test::compare_result($pass,
			vmf_read::translate('[-45 -3.5 1005.35]',-30,-20.41,40),
			'[-75 -23.91 1045.35]'
		);
		
		$pass = test::compare_result($pass,
			vmf_read::translate('(-3.2 4.5 82.3) (-405 -2.5 1005.35) (-300 -20.41 640)',-0.1,-5.3,3.4),
			'(-3.3 -0.8 85.7) (-405.1 -7.8 1008.75) (-300.1 -25.71 643.4)'
		);
	}
	
	return $pass;
}

function test_pull_key($rigorous)
{
	$pass = true;
	
	if ($rigorous)
	{
		test::induced_cause('No rigorous test available.');
		$pass = false;
	}
	else
	{
		$pass = test::compare_result($pass,vmf_read::pull_key('"" ""'),'');
		$pass = test::compare_result($pass,vmf_read::pull_key('"a" ""'),'a');
		$pass = test::compare_result($pass,vmf_read::pull_key('"a" "b"'),'a');
		$pass = test::compare_result($pass,vmf_read::pull_key('"aa" "b"'),'aa');
		$pass = test::compare_result($pass,vmf_read::pull_key('"aa" "bb"'),'aa');
		$pass = test::compare_result($pass,vmf_read::pull_key('"aaa" "bb"'),'aaa');
	}
	
	return $pass;
}

function test_pull_value($rigorous)
{
	$pass = true;
	
	if ($rigorous)
	{
		test::induced_cause('No rigorous test available.');
		$pass = false;
	}
	else
	{
		$pass = test::compare_result($pass,vmf_read::pull_value('"" ""'),'');
		$pass = test::compare_result($pass,vmf_read::pull_value('"a" ""'),'');
		$pass = test::compare_result($pass,vmf_read::pull_value('"a" "b"'),'b');
		$pass = test::compare_result($pass,vmf_read::pull_value('"aa" "b"'),'b');
		$pass = test::compare_result($pass,vmf_read::pull_value('"aa" "bb"'),'bb');
		$pass = test::compare_result($pass,vmf_read::pull_value('"aaa" "bb"'),'bb');
	}
	
	return $pass;
}

require('vmf.inc');
echo '<!DOCTYPE html><html><body style="font-family: courier">';
echo '<h1>Unit Tests for VMF.inc Class Methods</h1>';
echo '<h3>VMF_READ STATIC CLASS</h3>';
echo test::display_result('translate',test_translate(false));
echo test::display_result('pull_key',test_pull_key(false));
echo test::display_result('pull_value',test_pull_value(false));
echo '</body></html>';

?>