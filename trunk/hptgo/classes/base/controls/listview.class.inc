<?php
/*
	 Copyright HPTVietnam 2004
	 Author: Nguyễn Thái Ngọc Duy <duyntn@hptvietnam.com.vn>

	 This program is free software; you can redistribute it and/or modify it
	 under the terms of the GNU General Public License as published by the
	 Free Software Foundation; either version 2 of the License, or (at your
	 option) any later version.
 */

/**
 * Listview class.
 *
 * This class is intended to simplify listview rendering.
 * Using this class gives you a consistent look with pagination.
 * 
 * Usage:
 * $listview = new listview('my_id');
 * $my_columns = array();
 * $listview->colums = $my_columns;
 * $listview->count = total_lines;
 * $listview->print_head();
 * while (<cond>)
 * {
 *   $myline = array(...);
 *   $listview->print_line($myline);
 * }
 * $listview->print_foot();
 *
 * The structure of $my_columns and $myline is similar.
 * They are an array of items.
 * If an item is a string, then it's will be rendered as text.
 * If an item is an array, then index 'html' or 'text' should be set.
 * 'html' would overwrite 'text'. 'html' is a html code. 'text' is a text content.
 * 'text' will be htmlspecialchars()ed while 'html' is not
 * If you want to set other properties, set 'attributes' to an array of attributes.
 * If there is only one tag inside, set 'tag' to tag name and 'tag_attributes' to an array of the tag's attributes.
 * If you want to set an <a> tag, just simply set 'href' and 'text' (or 'html')
 */
class listview
{
	/**
	 * Description of column headers.
	 * @access public
	 * @var array
	 */
	var $columns = array();
	var $max_rows;
	var $first;
	var $sort_order;
	var $sort_column;
	/**
	 * Total number of rows. It's not the number of display rows
	 * @access public
	 * @var int
	 */
	var $count;
	/**
	 * Used internally to count print_line()
	 * @access private
	 * @var int
	 */
	var $lines = 0;

	var $id;
	var $form_name;


	/**
	 * constructor sets basic properties
	 *
	 * @param string $id
	 * @param string $form_name default is 'forms[0]'
	 */
	function listview($id, $form_name='forms[0]')
	{
		$this->id = $id;
		$this->form_name=$form_name;

		$this->max_rows = isset($_REQUEST[$this->id.'_max_rows']) ? $_REQUEST[$this->id.'_max_rows'] : $_SESSION['GO_SESSION']['max_rows_list'];
		$this->first = isset($_REQUEST[$this->id.'_first']) ? $_REQUEST[$this->id.'_first'] : 0;
		$this->sort_order = isset($_REQUEST[$this->id.'_sort_order']) ? $_REQUEST[$this->id.'_sort_order'] : 'ASC';
		$this->sort_column = isset($_REQUEST[$this->id.'_sort_field']) ? $_REQUEST[$this->id.'_sort_field'] : '';
	}

	/**
	 * Print tabtable's head.
	 * last three functions should be called in this order
	 * $tabtable->print_head();
	 * require($tabtable->get_active_tab_id());
	 * $tabtable->print_foot();
	 *
	 * @return none
	 */
	function print_head()
	{
		global $GO_THEME;
?>
<script language="javascript">
function <?=$this->id?>_sort(field, direction)
{
	document.<?=$this->form_name?>.<?=$this->id?>_sort_field.value=field;
	document.<?=$this->form_name?>.<?=$this->id?>_sort_order.value=direction;
	document.<?=$this->form_name?>.submit();
}

function <?=$this->id?>_change_list(first, max_rows)
{
	document.<?=$this->form_name?>.<?=$this->id?>_first.value=first;
	document.<?=$this->form_name?>.<?=$this->id?>_max_rows.value=max_rows;
	document.<?=$this->form_name?>.submit();
}

</script>
<input type="hidden" name="<?=$this->id?>_first" value="<?=$this->first?>" />
<input type="hidden" name="<?=$this->id?>_max_rows" value="<?=$this->max_rows?>" />
<input type="hidden" name="<?=$this->id?>_sort_field" value="<?=$this->sort_column?>" />
<input type="hidden" name="<?=$this->id?>_sort_order" value="<?=$this->sort_order?>" />
<?php

		echo '<table border="0" cellspacing="0" cellpadding="0" width="100%">';
		echo '<tbody>';

		echo '<tr'.(isset($this->head_attr) ? attributes_implode($this->head_attr) : '').'>';
		$this->column_count = 0;
		foreach ($this->columns as $i)
		{
			if (is_array($i))
			{
				$tdattr = '';
				if (!isset($i['attributes']) || !is_array($i['attributes']))
					$i['attributes'] = array();
				$i['attributes']['class'] = 'TableHead2';
				if (isset($i['sort']))
				{
					$i['href'] = 'javascript:'.$this->id.'_sort(\''.$i['sort'].'\',\''.($this->sort_order == 'ASC' ? 'DESC' : 'ASC').'\');';
					if (!isset($i['tag_attributes']) || !is_array($i['tag_attributes']))
						$i['tag_attributes'] = array();
					$i['tag_attributes']['class'] = 'TableHead2';
					if (!isset($i['html']))
					{
						if (isset($i['text']))
							$i['html'] = htmlspecialchars($i['text']);
						else
							$i['html'] = '';
					}
					if ($this->sort_column == $i['sort'])
						$i['html'] .= '&nbsp;<img src="'.($this->sort_order == 'ASC' ? $GO_THEME->images['arrow_down'] : $GO_THEME->images['arrow_up']).'" border="0" />';
				}


				echo '<th nowrap'.(isset($i['attributes']) ? attributes_implode($i['attributes']) : '').'>';
				if (isset($i['href']))
				{
					$i['tag'] = 'a';
					if (!isset($i['tag_attributes']))
						$i['tag_attributes'] = array();
					$i['tag_attributes']['href'] = $i['href'];
				}

				echo htmltag(isset($i['tag']) ? $i['tag'] : '',$i,'tag_attributes');

				echo '</th>';
			}
			else
			{
				echo '<th nowrap>'.htmlspecialchars($i).'</th>';
			}
			$this->column_count ++;
		}
		echo '</tr>';
	}

	function print_line($values,&$attr)
	{
		if (!isset($attr) || !is_array($attr))
			$attr = array();
		$attr['class'] = ($this->lines % 2 ? 'TableInside1' : 'TableInside2');

		echo '<tr'.(isset($attr) ? attributes_implode($attr) : '').'>';
		foreach ($values as $i)
		{
			if (is_array($i))
			{
				echo '<td nowrap'.(isset($i['attributes']) ? attributes_implode($i['attributes']) : '').'>';
				if (isset($i['href']))
				{
					$i['tag'] = 'a';
					if (!isset($i['tag_attributes']))
						$i['tag_attributes'] = array();
					$i['tag_attributes']['href'] = $i['href'];
				}

				echo htmltag(isset($i['tag']) ? $i['tag'] : '',$i,'tag_attributes');

				echo '</td>';
			}
			else
			{
				echo '<td nowrap>'.htmlspecialchars($i).'</td>';
			}
		}
		echo '</tr>';
		$this->lines ++;
	}

	function print_foot()
	{
		global $cmdNext,$cmdPrevious,$cmdShowAll;
		global $GO_THEME;

		$links = '';
		$max_links=10;
		if ($this->max_rows != 0)
		{
			if ($this->count > $this->max_rows)
			{
				$links = '<table border="0" cellpadding="0" cellspacing="0" width="100%"><tr><td>';
				$next_start = $this->first+$this->max_rows;
				$previous_start = $this->first-$this->max_rows;
				if ($this->first != 0)
				{
					$links .= '<a href="javascript:'.$this->id.'_change_list(0, '.$this->max_rows.');">&lt;&lt;</a>&nbsp;';
					$links .= '<a href="javascript:'.$this->id.'_change_list('.$previous_start.', '.$this->max_rows.');">'.$cmdPrevious.'</a>&nbsp;';
				}else
				{
					$links .= '<font color="#cccccc">&lt;&lt; '.$cmdPrevious.'</font>&nbsp;';
				}

				$start = ($this->first-(($max_links/2)*$this->max_rows));

				$end = ($this->first+(($max_links/2)*$this->max_rows));

				if ($start < 0)
				{
					$end = $end - $start;
					$start=0;
				}
				if ($end > $this->count)
				{
					$end = $this->count;
				}
				if ($start > 0)
				{
					$links .= '...&nbsp;';
				}

				for ($i=$start;$i<$end;$i+=$this->max_rows)
				{
					$page = ($i/$this->max_rows)+1;
					if ($i==$this->first)
					{
						$links .= '<b><i>'.$page.'</i></b>&nbsp;';
					}else
					{

						$links .= '<a href="javascript:'.$this->id.'_change_list('.$i.', '.$this->max_rows.');">'.$page.'</a>&nbsp;';
					}
				}

				if ($end < $this->count)
				{
					$links .= '...&nbsp;';
				}

				$last_page = floor($this->count/$this->max_rows)*$this->max_rows;

				if ($this->count > $next_start)
				{
					$links .= '<a href="javascript:'.$this->id.'_change_list('.$next_start.', '.$this->max_rows.');">'.$cmdNext.'</a>&nbsp;';
					$links .= '<a href="javascript:'.$this->id.'_change_list('.$last_page.', '.$this->max_rows.');">&gt;&gt;</a>';
				}else
				{
					$links .= '<font color="#cccccc">'.$cmdNext.' &gt;&gt;</font>';
				}
				$links .= '</td><td align="right"><a class="normal" href="javascript:'.$this->id.'_change_list(0, 0);">'.$cmdShowAll.'</a></td></tr></table>';

				echo '<tr height="20"><td colspan="99">'.$links.'</td></tr>';
				echo '<tr><td colspan="99" height="1"><img src="'.$GO_THEME->images['cccccc'].'" border="0" height="1" width="100%" /></td></tr>';
			}
		}
		echo '</tbody></table>';
	}
}

function attributes_implode(&$attrs)
{
	$tdattr = '';
	if (isset($attrs) && is_array($attrs))
	{
		foreach ($attrs as $id => $val)
			$tdattr .= ' '.$id.'="'.htmlspecialchars($val).'"';
	}
	return $tdattr;
}

function htmltag($tag,&$values,$attr_index = 'attributes')
{
	if ($tag != '')
		$html = '<';
	else
		$html = '';
	$content = '';
	if (isset($values) && is_array($values))
	{
		if (isset($values[$attr_index]))
			$html .= $tag.attributes_implode($values[$attr_index]);
		if (isset($values['html']))
			$content =  $values['html'];
		else
		{
			if (isset($values['text']))
				$content =  htmlspecialchars($values['text']);
		}
	}
	if ($tag != '')
	{
		if ($content != '')
			$html .= '>'.$content.'</'.$tag.'>';
		else
			$html .= '/>';
	}
	else
		$html .= $content;
	return $html;
}

?>
