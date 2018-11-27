<?php

function repeat_char($char,$times)
{
	$out = '';
	
	for ($i = 0; $i < $times; $i++)
		$out .= $char;
	
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
	
	public function get_key() {return $this->pair_key;}
	public function get_value() {return $this->pair_value;}
	public function set_value($value) {$this->pair_value = $value;}
	
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
	public $title = "entity";
	public static $indentation = 0;
	protected $items = array();
	
	public function __construct($name) {$this->title = strtolower($name);}
	public function append_section($obj) {$this->items[] = $obj;}
	
	public function create_section($name)
	{
		$sec = new section($name);
		$this->items[] = $sec;
		return $sec;
	}
	
	public function create_pair($k, $v)
	{
		$pair = new kv($k,$v);
		$this->items[] = $pair;
		return $pair;
	}
	
	public function get_field_value($key)
	{
		$out = false;
		
		foreach ($this->items as $item)
		{
			if ($item instanceof kv)
			{
				if ($item->get_key() == $key)
				{
					$out = $item->get_value();
					break;
				}
			}
		}
		
		return $out;
	}
	
	public function set_field_value($key, $new_val)
	{
		$done = false;
		
		foreach ($this->items as $item)
		{
			if ($item instanceof kv)
			{
				if ($item->get_key() == $key)
				{
					$item->set_value($new_val);
					$done = true;
					break;
				}
			}
		}
		
		if (!$done)
		{
			$this->create_pair($key, $new_val);
		}
	}
	
	public function get_sections($name)
	{
		$out = array();
		
		foreach ($this->items as $item)
		{
			if ($item instanceof section)
			{
				if ($item->title == $name)
				{
					$out[] = $item;
				}
			}
		}
		
		return $out;
	}
	
	public function export()
	{
		$out = '';
		$ind = repeat_char("\t",self::$indentation);
		$out .= $ind . $this->title . "\r\n";
		$out .= $ind . "{\r\n";
		self::$indentation++;
		
		foreach ($this->items as $individual)
			$out .= $individual->export();
		
		self::$indentation--;
		$out .= $ind . "}\r\n";
		return $out;
	}
}

class vmf
{
	public static $entity_id = 1;
	public static $solid_id = 0;
	public static $side_id = 1;
	protected $proj_name = 'untitled';
	protected $sky_name = 'sky_day01_01';
	protected $solids = array();
	protected $ents = array();
	
	public function __construct($name)
	{
		$this->proj_name = $name;
	}
	
	public function add_item($obj)
	{
		if ($obj instanceof section)
		{
			if ($obj->title == 'solid')
				$this->solids[] = $obj;
			elseif ($obj->title == 'entity')
				$this->ents[] = $obj;
		}
	}
	
	public function export()
	{
		$doc = '';

		$sec = new section('versioninfo');
		$sec->create_pair('editorversion','400');
		$sec->create_pair('editorbuild','6412');
		$sec->create_pair('mapversion','1');
		$sec->create_pair('formatversion','100');
		$sec->create_pair('prefab','0');
		$doc .= $sec->export();
		unset($sec);

		$sec = new section('visgroups');
		$doc .= $sec->export();
		unset($sec);

		$sec = new section('viewsettings');
		$sec->create_pair('bSnapToGrid','1');
		$sec->create_pair('bShowGrid','1');
		$sec->create_pair('bShowLogicalGrid','0');
		$sec->create_pair('nGridSpacing','64');
		$sec->create_pair('bShow3DGrid','0');
		$doc .= $sec->export();
		unset($sec);

		$world = new section('world');
		$world->create_pair('id',self::$entity_id);
		$world->create_pair('mapversion','1');
		$world->create_pair('classname','worldspawn');
		$world->create_pair('skyname',$this->sky_name);
		$world->create_pair('maxpropscreenwidth','-1');
		$world->create_pair('detailvbsp','detail.vbsp');
		$world->create_pair('detailmaterial','detail/detailsprites');
		
		foreach($this->solids as $solid)
		{
			
			$world->append_section($solid);
			self::$solid_id++;
			$solid->set_field_value('id',self::$solid_id);
			
			//Go through solid's sides and reset ids.
			$items = $solid->get_sections('side');
			foreach ($items as $side)
			{
				$side->set_field_value('id',self::$side_id);
				self::$side_id++;
			}
		}
		
		$doc .= $world->export();

		foreach($this->ents as $entity)
		{
			self::$entity_id++; //This is placed first because we use it on world id.
			
			$entity->set_field_value('id',self::$entity_id);
			
			//If the entity contains solids, go through and reset ids for sides.
			$items = $entity->get_sections('solid');
			foreach($items as $solid)
			{
				self::$solid_id++;
				$solid->set_field_value('id',self::$solid_id);
				
				//Go through solid's sides and reset ids.
				$items = $solid->get_sections('side');
				foreach ($items as $side)
				{
					$side->set_field_value('id',self::$side_id);
					self::$side_id++;
				}
			}
			
			$doc .= $entity->export();
		}

		$sec = new section('cameras');
		$sec->create_pair('activecamera','-1');
		$doc .= $sec->export();
		unset($sec);

		$sec = new section('cordon');
		$sec->create_pair('mins','(-1024 -1024 -1024)');
		$sec->create_pair('maxs','(1024 1024 1024)');
		$sec->create_pair('active','0');
		$doc .= $sec->export();
		unset($sec);
		
		return $doc;
	}
}

?>