<?php
/*
 * Displays the daily garfield strip from ucomics.com
 *
 * Author: Markus Schabel <markus.schabel@tgm.ac.at>
 *
 * TODO add support for multiple comics
 */

// Require main configuration file
require( "../../Group-Office.php" );

// Check if a user is logged in. If not try to login via cookies. If that
// also fails then show the login-screen.
$GO_SECURITY->authenticate();

// Check if the user is allowed to access this module.
$GO_MODULES->authenticate( 'test' );

// Load language data.
require( $GO_LANGUAGE->get_language_file( 'test' ) );

// This is the title of this page. Needed in header.inc for displaying the
// correct title in the titlebar of the browser.
$page_title = "test - pclouds";

// Require theme-header, most times this will be the navigation with some
// design issues.
require( $GO_THEME->theme_path."header.inc" );

// Find out if we got a date (a unix timestamp to be exact) as parameter, and
// if not find out the current time.
$date = isset( $_REQUEST['date'] ) ? $_REQUEST['date'] : time();

echo "Hello world!";

// Load theme-footer, this is probably some kind of "Group-Office Version..."
require( $GO_THEME->theme_path."footer.inc" );

// That's it, we've printed what the user wanted to do and can now exit.
// Maybe that would be the correct place to close database connections...
?>
