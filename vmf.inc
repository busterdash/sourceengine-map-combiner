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

//Often used to produce indentation in the output file.
function repeat_char($char,$times)
{
	$out = '';
	
	for ($i = 0; $i < $times; $i++)
		$out .= $char;
	
	return $out;
}

/*==============================================================================================
	KV INSTANTIABLE STRUCT
	Purpose: Encapsulates and formats data used as a key and value pair.
	
	Methods:
	- publ get_key()		Return the key name.
	- publ get_value()		Return the value.
	- publ set_value(str)	Set the value, notice there is no set key method.
	- publ export()			Output vmf-formatted key-value pair as a string.
==============================================================================================*/
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

/*==============================================================================================
	SECTION INSTANTIABLE STRUCT
	Purpose: Vehicle for data referring to an object, may also contain other objects within.
	
	Methods:
	- publ append_section(obj(section))		Add to me an existing section.
	- publ create_section(str)				Create and add to me a new section. Returns handle.
	- publ create_pair(str,str)				Create and add to me a key-val pair. Returns handle.
	- publ get_field_value(str)				Return value of key contained in me by key name.
	- publ set_field_value(str,str)			Change or create key-value pair contained by me.
	- publ get_sections(str)				Return array of contained sections by input name.
	- publ export()							Output section and contents as a string.
==============================================================================================*/
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
			if ($item instanceof kv) //Because we can't call get_key() on section instances.
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
			if ($item instanceof kv) //Because we can't call get_key() on section instances.
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
			$this->create_pair($key, $new_val); //If field doesn't exist, make it.
		}
	}
	
	public function get_sections($name)
	{
		$out = array();
		
		foreach ($this->items as $item)
		{
			if ($item instanceof section) //Only return sections.
			{
				if ($item->title == $name) //Only return sections with name.
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

/*==============================================================================================
	VMF INSTANTIABLE CLASS
	Purpose: Contain all exportable document objects and format them for output as VMF file.
	
	Methods:
	- publ add_item(obj(section)) 	Add entity or solid to vmf document via section instance.
	- publ set_skybox_tex(str)		Set texture name of skybox.
	- publ export()					Output entire vmf file as a string.
==============================================================================================*/
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
	
	public function set_skybox_tex($tex_name)
	{
		$this->sky_name = strtolower($tex_name);
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
		
		//Allocate new id values to world solids and then export them.
		foreach($this->solids as $solid)
		{
			$world->append_section($solid);
			self::$solid_id++;
			$solid->set_field_value('id',self::$solid_id);
			
			//Go through solid's sides and allocate new ids.
			$items = $solid->get_sections('side');
			foreach ($items as $side)
			{
				$side->set_field_value('id',self::$side_id);
				self::$side_id++;
			}
		}
		
		$doc .= $world->export();

		//Allocate new id values to entities and then export them.
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

/*==============================================================================================
	VMF_READ STATIC CLASS
	Purpose: Encapsulate functions for importing entities and solids from source files.
	
	Methods:
	- prot pull_key(str) 				Pulls a key out of a key-value line in a VMF.
	- prot pull_value(str)				Pulls a value out of a key-value line in a VMF.
	- prot translate(str,num,num,num)	Moves an entity or plane in space to a new location.
	- publ open_vmf_file(str,num,num,num,obj(vmf))	Import VMF contents into our doc. structure.
==============================================================================================*/
class vmf_read
{
	protected static function pull_key($line)
	{
		$out = false;
		$line = ltrim($line);
		$i = 0;
		$capture = '';
		
		if (!empty($line) and $line[$i] == '"') //Line must be a vmf key-value set.
		{
			$i++;
			
			while ($i < strlen($line)) //Stream through characters of line.
			{
				if ($line[$i] != '"') //Collect chars...
				{
					$capture .= $line[$i];
				}
				else //...until we find a double quote character.
				{
					$out = $capture;
					break;
				}
				
				$i++;
			}
		}
		
		return $out;
	}
	
	protected static function pull_value($line)
	{
		$out = false;
		$line = ltrim($line);
		$qcount = 1;
		$i = 0;
		$capture = '';
		
		if (!empty($line) and $line[$i] == '"') //Line must be a vmf key-value set.
		{
			$i++;
			
			while ($i < strlen($line)) //Stream through characters of line.
			{
				//We need to see three double quote chars before we capture chars
				//so that we know we have skipped the key name information.
				if ($qcount > 2) 
				{
					if ($line[$i] != '"') //Collect chars...
					{
						$capture .= $line[$i];
					}
					else //...until we find a double quote character.
					{
						$out = $capture;
						break;
					}
				}
				else //Count quotes until we are ready to capture data.
				{
					if ($line[$i] == '"')
					{
						$qcount++;
					}
				}
				
				$i++;
			}
		}
		
		return $out;
	}
	
	protected static function translate($origin, $x, $y, $z)
	{
		$len = strlen($origin);
		$parenthesis = 0; //0 for none, 1 for '()', 2 for '[]'
		$in_vals = array();
		$capture = '';
		$stage = 0;
		
		for ($i = 0; $i < $len; $i++)
		{
			if ($origin[$i] != '(' and $origin[$i] != ')'
			and $origin[$i] != '[' and $origin[$i] != ']')
			{
				if ($origin[$i] != ' ')
					$capture .= $origin[$i];
				else
				{
					$in_vals[] = floatval($capture);
					$capture = '';
				}
			}
			else
			{
				if ($origin[$i] == '(' or $origin[$i] == ')')
					$parenthesis = 1;
				else
					$parenthesis = 2;
			}
		}
		
		$in_vals[] = floatval($capture);
		$len = count($in_vals);
		$capture = '';
		
		for ($i = 0; $i < $len; $i++)
		{
			$stage = $i % 3;
			
			switch($stage)
			{
				case 0:
					$in_vals[$i] += $x; break;
				case 1:
					$in_vals[$i] += $y; break;
				case 2:
					$in_vals[$i] += $z; break;
			}
		}
		
		if ($parenthesis == 1 and $len > 3)
		{
			if ($len % 3 == 0)
			{
				for ($i = 0; $i < $len/3; $i++)
				{
					$capture .= '(';
					$capture .= strval($in_vals[$i*3]) . ' ';
					$capture .= strval($in_vals[$i*3+1]) . ' ';
					$capture .= strval($in_vals[$i*3+2]) . ') ';
				}
			}
			//Error handling here later.
			
			$capture = substr($capture, 0, -1);
		}
		else
		{
			for ($i = 0; $i < $len-1; $i++)
				$capture .= strval($in_vals[$i]) . ' ';
			
			$capture .= strval($in_vals[$len-1]);
			
			if ($parenthesis == 1)
				$capture = '(' . $capture . ')';
			elseif ($parenthesis == 2)
				$capture = '[' . $capture . ']';
		}
		
		return $capture;
	}
	
	public static function open_vmf_file($filename, $offset_x, $offset_y, $offset_z, $to_document)
	{
		$wait_for_world = true;
		$wait_for_world_solids = true;
		
		$fi = fopen($filename,'r');
		$last_line = ''; //Prev line buffer, used for section headers retrieval.
		$nest_stack = array(); //Parent objects are stored on stack until they can be closed.
		$nest_top = -1; //Hairpin{} depth
		
		if ($fi)
		{
			while (($line = fgets($fi)) !== false)
			{
				$line = trim($line);
				
				if ($wait_for_world) //Skip reading until we find world section.
				{
					if ($line == 'world')
						$wait_for_world = false;
				}
				else
				{
					if ($wait_for_world_solids) //Skip reading until we get to world solids.
					{
						if ($line == 'solid')
							$wait_for_world_solids = false;
					}
					else
					{
						if ($line == '}') //We are ending a section, finalize an object.
						{
							$p = array_pop($nest_stack);
							$nest_top--;
							
							if ($nest_top == -1)
								$to_document->add_item($p);
							elseif ($nest_top > -1)
								$nest_stack[$nest_top]->append_section($p);
							else
							{
								//We didn't collect the world opening hairpin.
								//This is a workaround to ignore the world closing hairpin.
								$nest_top = -1;
							}
						}
						elseif ($line == '{') //We are beginning a new section. We go deeper.
						{
							$nest_stack[] = new section($last_line);
							$nest_top++;
						}
						else //Not starting or ending section. Probably key-value pairs, if not then ignore.
						{
							if (($key = vmf_read::pull_key($line)) !== false)
							{
								if (($val = vmf_read::pull_value($line)) !== false)
								{
									if ($key == "plane" or $key == "origin" or $key == "startposition")
										$val = vmf_read::translate($val,$offset_x,$offset_y,$offset_z);
									
									$nest_stack[$nest_top]->create_pair($key,$val);
								}
							}
						}						
					}
				}
				
				$last_line = $line;
			}
			
			fclose($fi);
		}
		else
		{
			die('Couldn\'t open file for reading!');
		}
	}
}

?>