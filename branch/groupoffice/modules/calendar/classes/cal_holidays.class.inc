<?php
/*
Copyright Intermesh 2003
Author: Georg Lorenz <georg@lonux.de>
Version: 1.0 Release date: 08 July 2003

This program is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 2 of the License, or (at your
option) any later version.
*/

class holidays extends db
{
	var $locale;
	var $in_holidays;
	var $holidays = array();

	function holidays()
	{
		global $GO_LANGUAGE;
		
		$this->db();		
		require_once($GO_LANGUAGE->language_path.'holidays.inc');
		$this->in_holidays = $input_holidays;
	}

	function get_regions($language)
	{
		global $GO_LANGUAGE;
		
		require($GO_LANGUAGE->language_path.'locale.inc');
		$regions = array();
		foreach($locale as $reg => $lang)
		{
			if(array_key_exists($reg, $this->in_holidays["fix"]) || array_key_exists($reg, $this->in_holidays["var"]) ||array_key_exists($reg, $this->in_holidays["spc"]))
			{
				$regions[] = $reg;
			}
		}

		return $regions;
	}

	function add_holidays($uid, $calendar_id, $year, $region)
	{
		if($this->generate_holidays($region, $year))
		{
			$this->delete_holidays($uid, $calendar_id, $year);
			foreach($this->holidays as $date => $name)
			{
				$this->add_holiday($uid, $calendar_id, $region, $date, $name);
			}
		}else
		{
			return false;
		}
		return true;
	}

	function add_holiday($uid, $calendar_id, $region, $date, $name)
	{
		$next_id = $this->nextid("cal_holidays");
		if ($next_id > 0)
		{
			$name = addslashes($name);
			$sql = "INSERT INTO cal_holidays (id, user_id, calendar_id, region, date, name) VALUES ('$next_id', '$uid', '$calendar_id', '$region', '$date', '$name')";
   			$this->query($sql);
			if ($this->affected_rows() > 0)
			{
				return true;
			}
		}
		return false;
	}

	function update_holiday($id, $date, $name)
	{
		$sql = "UPDATE cal_holidays SET date='$date', name='$name'";
		$sql .= " WHERE id='$id'";
		return ($this->query($sql));
	}

	function delete_holiday($id)
	{
		$sql = "DELETE FROM cal_holidays WHERE id='$id'";
		$this->query($sql);
		if ($this->affected_rows() > 0)
			return true;
		else
			return false;
	}

	function delete_holidays($uid, $calendar_id, $year="")
	{
		$sql = "DELETE FROM cal_holidays WHERE user_id='$uid' AND calendar_id='$calendar_id'";

		if(!empty($year))
		{
		    $date_from = mktime(0,0,0,12,31,$year-1);
		    $date_to = mktime(0,0,0,1,1,$year+1);
		    $sql .= " AND date>'$date_from' AND date<'$date_to'";
		}

		$this->query($sql);
		if ($this->affected_rows() > 0)
			return true;
		else
			return false;
	}

	function get_region($uid, $calendar_id)
	{
		$sql = "SELECT region FROM cal_holidays WHERE user_id='$uid' AND calendar_id='$calendar_id'";

		$this->query($sql);
		if ($this->next_record())
			return $this->f('region');
	}

	function get_holiday($uid, $calendar_id, $date)
	{
		$sql = "SELECT * FROM cal_holidays WHERE user_id='$uid' AND calendar_id='$calendar_id' AND date='$date'";

		$this->query($sql);
		if ($this->next_record())
			return true;
		else
			return false;
	}

	function get_holiday_by_id($id)
	{
		$sql = "SELECT * FROM cal_holidays WHERE id='$id'";
		
		$this->query($sql);
		if ($this->next_record())
			return true;
		else
			return false;
	}

	function get_holidays($uid, $calendar_id, $year="" )
	{
		$sql = "SELECT * FROM cal_holidays WHERE user_id='$uid' AND calendar_id='$calendar_id'";

		if(!empty($year))
		{
		    $date_from = mktime(0,0,0,12,31,$year-1);
		    $date_to = mktime(0,0,0,1,1,$year+1);
		    $sql .= " AND date>'$date_from' AND date<'$date_to'";
		}
		$sql .= " ORDER BY date ASC";

		$this->query($sql);
		return $this->num_rows();
	}

	function generate_holidays($region, $year="")
	{
		$holidays = array();

		if(is_array($this->in_holidays))
		{
			foreach($this->in_holidays as $key => $sub_array)
			{
				if(array_key_exists($region, $sub_array))
				{
					if($sub_array[$region])
					{
						$holidays[$key] = $sub_array[$region];
					}
				}
			}
		}

		if(empty($year))
		{
			$current_date = getdate();
			$year = $current_date["year"];
		}

		if(isset($holidays['fix']))
		{
			foreach($holidays['fix'] as $key => $value)
			{
				$month_day = explode("-", $key);
				$date = mktime(0,0,0,$month_day[0],$month_day[1],$year);
				$this->holidays[$date] = $value;
			}
		}

		if(isset($holidays['var']))
		{
			$easter_day = $this->get_easter_day($year);
			foreach($holidays['var'] as $key => $value)
			{
				$date = strtotime($key." days", $easter_day);
				$this->holidays[$date] = $value;
			}
		}

		if(isset($holidays['spc']))
		{
			$weekday = $this->get_weekday("24","12",$year);
			foreach($holidays['spc'] as $key => $value)
			{
				$count = $key - $weekday;
				$date = strtotime($count." days", mktime(0,0,0,"12","24",$year));
				$this->holidays[$date] = $value;
			}
		}

		if(empty($this->holidays))
		{
			return false;
		}else
		{
			ksort($this->holidays, SORT_NUMERIC);
			return true;
		}
	}

	function get_easter_day($year)
	{
	        if($year < 2100)
			$n = 5;
	        else
			$n = 6;

	        $a = bcmod($year, 19);
	        $b = bcmod($year, 4);
	        $c = bcmod($year, 7);
	        $d = bcmod((19 * $a + 24), 30);
	        $e = bcmod((2 * $b + 4 * $c + 6 * $d + $n), 7);

	        if(($d + $e + 22) <= 31)
	        {
			$day = $d + $e + 22;
			$month = 3;
	        }else
	        {
			$day = $d + $e - 9;
			$month = 4;
	        }

	        if($day == 26 && $month == 4)
			$day = 19;

	        if($day == 25 && $month == 4 && $a > 10 && $d == 28)
			$day = 18;

	        return mktime(0,0,0,$month,$day,$year);
	}

	function get_weekday($day, $month, $year)
	{
	        $date = getdate(mktime(0, 0, 0, $month, $day, $year));
	        return $date["wday"];
	}

}
