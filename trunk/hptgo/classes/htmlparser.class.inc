<?php

/*
 * Copyright (c) 2003 Jose Solorzano.  All rights reserved.
 * Redistribution of source must retain this copyright notice.
 *
 * Jose Solorzano (http://jexpert.us) is a software consultant.
 *
 * Contributions by:
 * - Leo West (performance improvements)
 */

define ("NODE_TYPE_START",0);
define ("NODE_TYPE_ELEMENT",1);
define ("NODE_TYPE_ENDELEMENT",2);
define ("NODE_TYPE_TEXT",3);
define ("NODE_TYPE_COMMENT",4);
define ("NODE_TYPE_DONE",5);

/**
 * Class HtmlParser.
 * To use, create an instance of the class passing
 * HTML text. Then invoke parse() until it's false.
 * When parse() returns true, $iNodeType, $iNodeName
 * $iNodeValue and $iNodeAttributes are updated.
 *
 * To create an HtmlParser instance you may also
 * use convenience functions HtmlParser_ForFile
 * and HtmlParser_ForURL.
 */
class HtmlParser {

    /**
     * Field iNodeType.
     * May be one of the NODE_TYPE_* constants above.
	 * @var	int
     */
    var $iNodeType;

    /**
     * Field iNodeName.
     * For elements, it's the name of the element.
	 * @var string
     */
    var $iNodeName = "";

    /**
     * Field iNodeValue.
     * For text nodes, it's the text.
	 * @var string
     */
    var $iNodeValue = "";

    /**
     * Field iNodeAttributes.
     * A string-indexed array containing attribute values
     * of the current node. Indexes are always lowercase.
	 * @var array
     */
    var $iNodeAttributes;

    // The following fields should be 
    // considered private:

	/**
	* @var		string
	* @access	private
	*/
    var $iHtmlText;
	/**
	* @var		int
	* @access	private
	*/
    var $iHtmlTextLength;
	/**
	* @var		int
	* @access	private
	*/
    var $iHtmlTextIndex = 0;
	/**
	* @var		char
	* @access	private
	*/
    var $iHtmlCurrentChar;

    /**
     * Constructor.
     * Constructs an HtmlParser instance with
     * the HTML text given.
	 * @param	string	$aHtmlText
     */
    function HtmlParser ($aHtmlText) {
        $this->iHtmlText = $aHtmlText;
        $this->iHtmlTextLength = strlen($aHtmlText);
        $this->setTextIndex (0);
    }

    /**
     * Method parse.
     * Parses the next node. Returns false only if
     * the end of the HTML text has been reached.
     * Updates values of iNode* fields.
	 * @return bool
     */
    function parse() {
        $text = $this->skipToElement();
        if ($text != "") {
            $this->iNodeType = NODE_TYPE_TEXT;
            $this->iNodeName = "Text";
            $this->iNodeValue = $text;
            return true;
        }
        return $this->readTag();
    }

	/**
	* Clear iNodeAttributes values
	*/
    function clearAttributes() {
        $this->iNodeAttributes = array();
    }

	/**
	* Read tag
	* @return bool
	*/
    function readTag() {
        if ($this->iCurrentChar != "<") {
            $this->iNodeType = NODE_TYPE_DONE;
            return false;
        }
        $this->skipInTag ("<");
        $this->clearAttributes();
        $name = $this->skipToBlanksInTag();
        $pos = strpos($name, "/");
        if ($pos === 0) {
            $this->iNodeType = NODE_TYPE_ENDELEMENT;
            $this->iNodeName = substr ($name, 1);
            $this->iNodeValue = "";
        } 
        else {
            if (!$this->isValidTagIdentifier ($name)) {
                $comment = false;
                if ($name == "!--") {
                    $rest = $this->skipToStringInTag ("-->");    
                    if ($rest != "") {
                        $this->iNodeType = NODE_TYPE_COMMENT;
                        $this->iNodeName = "Comment";
                        $this->iNodeValue = "<" . $name . $rest;
                        $comment = true;
                    }
                }
                if (!$comment) {
                    $this->iNodeType = NODE_TYPE_TEXT;
                    $this->iNodeName = "Text";
                    $this->iNodeValue = "<" . $name;
                }
                return true;
            }
            else {
                $this->iNodeType = NODE_TYPE_ELEMENT;
                $this->iNodeValue = "";
                $nameLength = strlen($name);
                if ($nameLength > 0 && substr($name, $nameLength - 1, 1) == "/") {                 $this->iNodeName = substr($name, 0, $nameLength - 1);
                }
                else {
                    $this->iNodeName = $name;
                }
            } 
        }
        while ($this->skipBlanksInTag()) {
            $attrName = $this->skipToBlanksOrEqualsInTag();
            if ($attrName != "") {
                $this->skipBlanksInTag();
                if ($this->iCurrentChar == "=") {
                    $this->skipEqualsInTag();
                    $this->skipBlanksInTag();
                    $value = $this->readValueInTag();
                    $this->iNodeAttributes[strtolower($attrName)] = $value;
                }
                else {
                    $this->iNodeAttributes[strtolower($attrName)] = "";
                }
            }
        }
        $this->skipEndOfTag();
        return true;            
    }

	/**
	* Check valid tag
	* @param	string	$name	Tag name
	* @return	bool	
	*/
    function isValidTagIdentifier ($name) {
        return ereg ("[A-Za-z0-9]+", $name);
    }
    
	/**
	* Skip blanks in tag
	* @return string
	*/
    function skipBlanksInTag() {
        return "" != ($this->skipInTag (array (" ", "\t", "\r", "\n" )));
    }

	/**
	* Skip to blanks or equals in tag
	* @return string
	*/
    function skipToBlanksOrEqualsInTag() {
        return $this->skipToInTag (array (" ", "\t", "\r", "\n", "=" ));
    }

	/**
	* Skip to blanks in tag
	* @return string
	*/
    function skipToBlanksInTag() {
        return $this->skipToInTag (array (" ", "\t", "\r", "\n" ));
    }

	/**
	* Skip equals in tag
	* @return string
	*/
    function skipEqualsInTag() {
        return $this->skipInTag (array ( "=" ));
    }

	/**
	* Read value in tag
	* @return string
	*/
    function readValueInTag() {
        $ch = $this->iCurrentChar;
        $value = "";
        if ($ch == "\"") {
            $this->skipInTag (array ( "\"" ));
            $value = $this->skipToInTag (array ( "\"" ));
            $this->skipInTag (array ( "\"" ));
        }
        else if ($ch == "\'") {
            $this->skipInTag (array ( "'" ));
            $value = $this->skipToInTag (array ( "'" ));
            $this->skipInTag (array ( "'" ));
        }                
        else {
            $value = $this->skipToBlanksInTag();
        }
        return $value;
    }

	/**
	* Set text index
	* @param	int	$index
	*/
    function setTextIndex ($index) {
        $this->iHtmlTextIndex = $index;
        if ($index >= $this->iHtmlTextLength) {
            $this->iCurrentChar = -1;
        }
        else {
            $this->iCurrentChar = $this->iHtmlText{$index};
        }
    }

	/**
	* Move next Htmltext
	* @return bool
	*/
    function moveNext() {
        if ($this->iHtmlTextIndex < $this->iHtmlTextLength) {
            $this->setTextIndex ($this->iHtmlTextIndex + 1);
            return true;
        }
        else {
            return false;
        }
    }

	/**
	* Skip end of tag
	* @return string
	*/
    function skipEndOfTag() {
        $sb = "";
        if (($ch = $this->iCurrentChar) !== -1) {
            $match = ($ch == ">");
            if (!$match) {
                return $sb;
            }
            $sb .= $ch;
            $this->moveNext();
        }
        return $sb;
    }

	/**
	* Skip in tag
	* @param	string	$chars
	* @return	string
	*/
    function skipInTag ($chars) {
        $sb = "";
        while (($ch = $this->iCurrentChar) !== -1) {
            if ($ch == ">") {
                return $sb;
            } else {
                $match = false;
                for ($idx = 0; $idx < count($chars); $idx++) {
                    if ($ch == $chars[$idx]) {
                        $match = true;
                        break;
                    }
                }
                if (!$match) {
                    return $sb;
                }
                $sb .= $ch;
                $this->moveNext();
            }
        }
        return $sb;
    }

	/**
	* Skip to in tag
	* @param	string	$chars
	* @return	string
	*/
    function skipToInTag ($chars) {
        $sb = "";
        while (($ch = $this->iCurrentChar) !== -1) {
            $match = $ch == ">";
            if (!$match) {
                for ($idx = 0; $idx < count($chars); $idx++) {
                    if ($ch == $chars[$idx]) {
                        $match = true;
                        break;
                    }
                }
            }
            if ($match) {
                return $sb;
            }
            $sb .= $ch;
            $this->moveNext();
        }
        return $sb;
    }

	/**
	* Skip to element
	* @return	string
	*/
    function skipToElement() {
        $sb = "";
        while (($ch = $this->iCurrentChar) !== -1) {
            if ($ch == "<") {
                return $sb;
            }
            $sb .= $ch;
            $this->moveNext();
        }
        return $sb;             
    }

    /**
	 * Skip to string in tag
     * @param	int	$needle
	 * @returns string	Text between current position and $needle,
     * inclusive, or "" if not found. The current index is moved to a point
     * after the location of $needle, or not moved at all
     * if nothing is found.
     */
    function skipToStringInTag ($needle) {
        $pos = strpos ($this->iHtmlText, $needle, $this->iHtmlTextIndex);
        if ($pos === false) {
            return "";
        }
        $top = $pos + strlen($needle);
        $retvalue = substr ($this->iHtmlText, $this->iHtmlTextIndex, $top - $this->iHtmlTextIndex);
        $this->setTextIndex ($top);
        return $retvalue;
    }
}

/**
* Parser a file to Html
* @param	string	$fileName	File Name
* @return	HtmlParser
*/
function HtmlParser_ForFile ($fileName) { 
    return new HtmlParser (file_get_contents($filename)); 
}

/**
* Parser a URL to Html
* @param	string	$url
* @return	HtmlParser
*/
function HtmlParser_ForURL ($url) {
    $fp = fopen ($url, "r");
    $content = "";
    while (true) {
        $data = fread ($fp, 8192);
        if (strlen($data) == 0) {
            break;
        }
        $content .= $data;
    }
    fclose ($fp);
    return new HtmlParser ($content);
}

?>
