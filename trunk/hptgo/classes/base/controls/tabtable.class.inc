<?php
/*
   Copyright Intermesh 2003
   Author: Merijn Schering <mschering@intermesh.nl>
   Version: 1.0 Release date: 08 July 2003

   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2 of the License, or (at your
   option) any later version.
 */

class tabtable
{
  var $tabs = array();
  var $docs = array();
  var $id;
  var $width;
  var $height;
  var $tabwidth;
  //arguments are extra vars that are passed in the links from the tabs
  var $arguments;
  var $submit_form;
  var $active_tab;
  var $form_name;

  //css classes

  var $css_tab = 'Tab';
  var $css_active_tab = 'Activetab';
  var $css_tab_vert = 'TabVert';
  var $css_active_tab_vert = 'ActivetabVert';
  var $css_table = 'tabtable';
  var $css_tab_background = 'TabBackground';
  var $css_tab_background_vert = 'TabBackgroundVert';

  /**
   * constructor sets basic properties
   *
   * @param $id
   * @param $title default is empty
   * @param $width default is empty
   * @param $height default is empty
   * @param $tabwidth default is 120
   * @param $arguments default is empty
   * @param $submit_form default is false
   * @param $align is left
   * @param $valign is top
   * @param $form_name default is 'forms[0]'
   * @param $tab_direction default is horizontal
   * @param $right_title default is empty
   * @return none
   */
  function tabtable($id, $title='', $width='', $height='', $tabwidth='120', $arguments='', $submit_form=false, $align='left', $valign='top', $form_name='forms[0]', $tab_direction='horizontal', $right_title = '')
  {
    $this->id = $id;
    $this->title = $title;
    $this->right_title = $right_title;
    $this->width = $width;
    $this->height = $height;
    $this->tabwidth = $tabwidth;
    $this->submit_form = $submit_form;
    $this->arguments = $arguments;
    $this->align=$align;
    $this->valign=$valign;
    $this->form_name=$form_name;
    $this->tab_direction=$tab_direction;

    $active_tab_ref = $this->id;
    $this->active_tab = isset($_REQUEST[$active_tab_ref]) ? $_REQUEST[$active_tab_ref] : 0;
  }

  /**
   * Set css info. FIXME
   */
  function set_classnames($table, $tab, $active_tab, $tab_background)
  {
    $this->css_table= $table;
    $this->css_tab= $tab;
    $this->css_active_tab = $active_tab;
    $this->css_tab_background = $tab_background;
  }

  /**
   * Add a tab with $id and name $name.
   *
   * @param $id Hint: Id could be a document to include
   * @param $name
   * @return none
   */
  function add_tab($id, $name)
  {
    $this->tabs[] = $name;
    $this->docs[] = $id;
  }

  /**
   * set the active tab manually call this before the print_head() function
   *
   * @param $index
   * @return true/false
   * @see print_head
   */
  function set_active_tab($index)
  {
    if (isset($this->tabs[$index]))
    {
      $this->active_tab = $index;
      return true;
    }else
    {
      return false;
    }
  }

  /**
   * set active tab by name
   *
   * @param $name
   * @return true/false
   */
  function set_active_tab_by_name($name)
  {
	if ($needle=array_search ($name, $this->docs))
    {
      $this->active_tab = $needle;
      return true;
    }else
    {
      return false;
    }
  }

  /**
   * get active tab by name
   *
   * @param $name
   * @return index or 0
   */
  function get_active_tab_by_name($name)
  {
        if ($needle=array_search ($name, $this->docs))
    {
     return $needle;
    }else
    {
      return 0;
    }
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
    $active_tab_ref = $this->id;

    if ($this->tab_direction == 'vertical')
    {
      $this->css_active_tab = $this->css_active_tab_vert;
      $this->css_tab = $this->css_tab_vert;
      $this->css_tab_background = $this->css_tab_background_vert;
    }

    $tab_count = count($this->tabs);
    if ($tab_count > 0 && $this->submit_form)
    {
      echo '
	<script type="text/javascript">
	function change_tab(activetab)
	{
	  document.'.$this->form_name.'.action.value = document.forms[0].action.value+"'.$this->arguments.'";
	  document.'.$this->form_name.'.'.$active_tab_ref.'.value = activetab;
	  document.'.$this->form_name.'.submit();
	}
      </script>
	';
    }
    echo '<input type="hidden" name="'.$active_tab_ref.'" value="'.$this->active_tab.'" />';
    echo '<table border="0" cellpadding="0" cellspacing="0" id="'.$this->css_table.'" width="'.$this->width.'">';
    if ($this->title != '')
    {
      echo '<tr>'.
				'<td colspan="100">'.
					'<table width="100%" border="0" cellspacing="0" cellpadding="0">'.
						'<tr>'.
    						'<th width="99%" nowrap>'.$this->title.'</th>'.
    						'<th width="1%" nowrap>'.$this->right_title.'&nbsp;</th>'.
						'</tr>'.
					'</table>'.
				'</td>'.
	  		'</tr>'.
//	  <th colspan="99">'.$this->title.'</th>'.($this->right_title != '' ? '<th nowrap>'.$this->right_title.'</th>' : '').
	  '</tr>';
    }

    $remaining_width= $this->width - ($tab_count * $this->tabwidth);
    if ($tab_count > 0)
    {
      echo '<tr><td class="'.$this->css_tab_background.'" valign="top">';
      echo '<table border="0" cellpadding="0" cellspacing="0">';
      if ($this->tab_direction == 'horizontal')
      {
	echo '<tr>';
      }
      //draw tabs and select the active doc
      for ($i=0;$i<$tab_count;$i++)
      {
	if ($this->tab_direction == 'vertical')
	{
	  echo '<tr>';
	}
	if ($i == $this->active_tab)
	{
	  echo '<td class="'.$this->css_active_tab.'" width="'.$this->tabwidth.'" nowrap>'.$this->tabs[$i].'</td>';
	}else
	{
	  if ($this->submit_form)
	  {
	    echo '<td class="'.$this->css_tab.'" width="'.$this->tabwidth.'" nowrap><a class="Tab" href="javascript:change_tab('.$i.')">'.$this->tabs[$i].'</a></td>';
	  }else
	  {
	    echo '<td class="'.$this->css_tab.'" width="'.$this->tabwidth.'" nowrap><a class="Tab" href="'.$_SERVER['PHP_SELF'].'?'.$active_tab_ref.'='.$i.$this->arguments.'">'.$this->tabs[$i].'</a></td>';
	  }
	}
	if ($this->tab_direction == 'vertical')
	{
	  echo '</tr>';
	}
      }
      if ($this->tab_direction == 'horizontal')
      {
	echo '</tr>';
      }
      echo '</table>';
      echo '</td>';
    }

    if ($this->tab_direction == 'horizontal')
    {
      echo '</tr><tr>';
    }
    echo '<td height="'.$this->height.'" valign="'.$this->valign.'" width="100%" align="'.$this->align.'" style="padding: 10px;">';

  }

  /**
   * get active tab id
   *
   * @return id or false
   */
  function get_active_tab_id()
  {
    if (isset($this->docs[$this->active_tab]))
    {
      return $this->docs[$this->active_tab];
    }else
    {
      return false;
    }
  }

  /**
   * Print tabtable's foot.
   */
  function print_foot()
  {
    echo '</td></tr></table>';
  }
}
?>
