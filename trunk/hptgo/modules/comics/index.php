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
$GO_MODULES->authenticate( 'comics' );

// Load language data.
require( $GO_LANGUAGE->get_language_file( 'comics' ) );

// This is the title of this page. Needed in header.inc for displaying the
// correct title in the titlebar of the browser.
$page_title = "comics - garfield";

// Require theme-header, most times this will be the navigation with some
// design issues.
require( $GO_THEME->theme_path."header.inc" );

// Find out if we got a date (a unix timestamp to be exact) as parameter, and
// if not find out the current time.
$date = isset( $_REQUEST['date'] ) ? $_REQUEST['date'] : time();

// Try to open the garfield from this date. The URL will be something like
// this: http://images.ucomics.com/comics/ga/2003/ga031203.gif
$file = @fopen(
              "http://images.ucomics.com/comics/ga/".date( 'Y', $date ).
	      "/ga".date( 'ymd', $date ).".gif",
	      "r"
	      );

// Test if we were able to open this file. If not decrement date until we can
// fetch a file or tested 30 files.
if ( !$file ) {
  // We were not able to fetch the file. So we initialies our test-counter.
  $tries = 0;
  // Try to read some url's.
  do {
    // Decrement date, so that we fetch the comic of $date-one day.
    $date -= 60*60*24;
    // Increment tries, so that we can give an error if we weren't able to
    // fetch 30 days in the past, this prevents an infinite loop if e.g. the
    // ucomics server is down.
    $tries++;
    // Fetch the comic that was released at $date.
    $file = @fopen(
                  "http://images.ucomics.com/comics/ga/".date( 'Y', $date ).
                  "/ga".date( 'ymd', $date ).".gif",
                  "r"
                  );
    // Do this till you find a file or tried 30 times.
  } while ( !$file && $tries < 30 );
}

// If we found a file (either in the beginning or in the loop) then we should
// close this again, but set a variable to mark that we found the file.
if ( $file ) {
  // Close the file.
  fclose( $file );
  // We found a file.
  $file = 1;
} else {
  // We found no file.
  $file = 0;
}

// All output should be aligned in a table to have correct distances
// between the window-borders and our output.
echo "<table border='0' cellpadding='0' cellspacing='0'>";

// If we found a file, then we display it.
if ( $file ) {
  echo "<tr><td align='center' colspan=3>";
  echo "<img src='http://images.ucomics.com/comics/ga/".date('Y',$date).
       "/ga".date('ymd',$date).".gif'>";
  echo "</td></tr>";
  // Display links for last, next and todays strip.
  echo "<tr><td align='left' width='150'>";
  echo "<a href='".$_SERVER['PHP_SELF']."?date=".($date-60*60*24)."'>".
	$cmdPrevious."</a></td><td align='center'>";
  echo "<a href='".$_SERVER['PHP_SELF']."?date=".time()."'>".
	$comics_today."</a></td><td align='right' width='150'>";
  echo "<a href='".$_SERVER['PHP_SELF']."?date=".($date+60*60*24)."'>".
	$cmdNext."</a></td></tr>";
}

// Since all our output goes into a table we have to close the following tags
echo "</table>";

// Load theme-footer, this is probably some kind of "Group-Office Version..."
require( $GO_THEME->theme_path."footer.inc" );

// That's it, we've printed what the user wanted to do and can now exit.
// Maybe that would be the correct place to close database connections...
?>
