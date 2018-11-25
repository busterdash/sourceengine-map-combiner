<?php

function repeat_char($char,$times)
{
	$out = '';
	
	for ($i = 0; $i < $times; $i++)
	{
		$out .= $char;
	}
	
	return $out;
}

class kv
{
	protected $pair_key;
	protected $pair_value;
	
	public function __construct($key, $value)
	{
		$this->pair_key = $key;
		$this->pair_value = $value;
	}
	
	public function export()
	{
		$out = '';
		$out .= repeat_char("\t",section::$indentation);
		$out .= "\"$this->pair_key\" \"$this->pair_value\"\r\n";
		return $out;
	}
}

class section
{
	public static $indentation = 0;
	protected $items = array();
	protected $title = "entity";
	
	public function __construct($name)
	{
		$this->title = strtolower($name);
	}
	
	public function add_section($name)
	{
		$sec = new section($name);
		$this->items[] = $sec;
		return $sec;
	}
	
	public function add_pair($k, $v)
	{
		$pair = new kv($k,$v);
		$this->items[] = $pair;
		return $pair;
	}
	
	public function export()
	{
		$out = '';
		$ind = repeat_char("\t",self::$indentation);
		$out .= $ind . $this->title . "\r\n";
		$out .= $ind . "{\r\n";
		self::$indentation++;
		
		foreach ($this->items as $individual)
		{
			$out .= $individual->export();
		}
		
		self::$indentation--;
		$out .= $ind . "}\r\n";
		return $out;
	}
}

class vmf
{
	protected $entity_id = 1;
	protected $solid_id = 1;
	protected $side_id = 1;
	protected $proj_name = 'untitled';
	
	public function __construct($name)
	{
		$this->proj_name = $name;
	}
	
	public function export()
	{
		$doc = '';

		$sec = new section('versioninfo');
		$sec->add_pair('editorversion','400');
		$sec->add_pair('editorbuild','6412');
		$sec->add_pair('mapversion','1');
		$sec->add_pair('formatversion','100');
		$sec->add_pair('prefab','0');
		$doc .= $sec->export();
		unset($sec);

		$sec = new section('visgroups');
		$doc .= $sec->export();
		unset($sec);

		$sec = new section('viewsettings');
		$sec->add_pair('bSnapToGrid','1');
		$sec->add_pair('bShowGrid','1');
		$sec->add_pair('bShowLogicalGrid','0');
		$sec->add_pair('nGridSpacing','64');
		$sec->add_pair('bShow3DGrid','0');
		$doc .= $sec->export();
		unset($sec);

		$world = new section('world');
		$world->add_pair('id','1');
		$world->add_pair('mapversion','1');
		$world->add_pair('classname','worldspawn');
		$world->add_pair('skyname','sky_day01_01');
		$world->add_pair('maxpropscreenwidth','-1');
		$world->add_pair('detailvbsp','detail.vbsp');
		$world->add_pair('detailmaterial','detail/detailsprites');
		$doc .= $world->export();

		$sec = new section('cameras');
		$sec->add_pair('activecamera','-1');
		$doc .= $sec->export();
		unset($sec);

		$sec = new section('cordon');
		$sec->add_pair('mins','(-1024 -1024 -1024)');
		$sec->add_pair('maxs','(1024 1024 1024)');
		$sec->add_pair('active','0');
		$doc .= $sec->export();
		unset($sec);
		
		return $doc;
	}
}

?>