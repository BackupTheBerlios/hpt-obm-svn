<?php
require_once("../../Group-Office.php");

$smileys = array(
	'0:-)' => 'yahoo_angel.gif',
	'O:-)' => 'yahoo_angel.gif',
	'o:-)' => 'yahoo_angel.gif',
	'x(' => 'yahoo_angry.gif',
	'X(' => 'yahoo_angry.gif',
	'x-(' => 'yahoo_angry.gif',
	'X-(' => 'yahoo_angry.gif',
	';;)' => 'yahoo_batting.gif',
	':-d' => 'yahoo_bigsmile.gif',
	':d' => 'yahoo_bigsmile.gif',
	':-D' => 'yahoo_bigsmile.gif',
	':D' => 'yahoo_bigsmile.gif',
	':&quot;&gt;' => 'yahoo_blush.gif',
	'=;' => 'yahoo_bye.gif',
	'=d&gt;' => 'yahoo_clap.gif',
	'=D&gt;' => 'yahoo_clap.gif',
	':0)' => 'yahoo_clown.gif',
	':O)' => 'yahoo_clown.gif',
	':o)' => 'yahoo_clown.gif',
	':-((' => 'yahoo_cry.gif',
	':((' => 'yahoo_cry.gif',
	'&gt;:)' => 'yahoo_devil.gif',
	'#-O' => 'yahoo_doh.gif',
	'#-o' => 'yahoo_doh.gif',
	'=p~' => 'yahoo_drool.gif',
	'=P~' => 'yahoo_drool.gif',
	'/:-)' => 'yahoo_eyebrow.gif',
	'/:)' => 'yahoo_eyebrow.gif',
	'8-|' => 'yahoo_eyeroll.gif',
	':-b' => 'yahoo_glasses.gif',
	':-B' => 'yahoo_glasses.gif',
	':-*' => 'yahoo_kiss.gif',
	':*' => 'yahoo_kiss.gif',
	':-))' => 'yahoo_laughloud.gif',
	':))' => 'yahoo_laughloud.gif',
	':-X' => 'yahoo_love.gif',
	':X' => 'yahoo_love.gif',
	':-x' => 'yahoo_love.gif',
	':x' => 'yahoo_love.gif',
	':-&gt;' => 'yahoo_mean.gif',
	':&gt;' => 'yahoo_mean.gif',
	':-|' => 'yahoo_neutral.gif',
	':|' => 'yahoo_neutral.gif',
	':-o' => 'yahoo_ooooh.gif',
	':o' => 'yahoo_ooooh.gif',
	':-O' => 'yahoo_ooooh.gif',
	':O' => 'yahoo_ooooh.gif',
	':-/' => 'yahoo_question.gif',
	':-\\' => 'yahoo_question.gif',
	':-(' => 'yahoo_sad.gif',
	':(' => 'yahoo_sad.gif',
	':-$' => 'yahoo_shhhh.gif',
	':-&amp;' => 'yahoo_sick.gif',
	'[-(' => 'yahoo_silent.gif',
	'8-}' => 'yahoo_silly.gif',
	'|-)' => 'yahoo_sleep.gif',
	'I-)' => 'yahoo_sleep.gif',
	'(-:' => 'yahoo_smiley.gif',
	'(:' => 'yahoo_smiley.gif',
	':)' => 'yahoo_smiley.gif',
	':-)' => 'yahoo_smiley.gif',
	'b-)' => 'yahoo_sunglas.gif',
	'B-)' => 'yahoo_sunglas.gif',
	'(:|' => 'yahoo_tired.gif',
	':-?' => 'yahoo_think.gif',
	':P' => 'yahoo_tongue.gif',
	':p' => 'yahoo_tongue.gif',
	':-P' => 'yahoo_tongue.gif',
	':-p' => 'yahoo_tongue.gif',
	';)' => 'yahoo_wink.gif',
	';-)' => 'yahoo_wink.gif',
	':-S' => 'yahoo_worried.gif',
	':-s' => 'yahoo_worried.gif',
	'=:-)' => 'yahoo_alien.gif',
	'=:)' => 'yahoo_alien.gif',
	'&gt;-)' => 'yahoo_alien2.gif',
	'B-(' => 'yahoo_beatup.gif',
	'b-(' => 'yahoo_beatup.gif',
	'~:&gt;' => 'yahoo_chicken.gif',
	'~O)' => 'yahoo_coffee.gif',
	'~o)' => 'yahoo_coffee.gif',
	'3:-o' => 'yahoo_cow.gif',
	'3:-O' => 'yahoo_cow.gif',
	'3:-0' => 'yahoo_cow.gif',
	'&lt;):)' => 'yahoo_cowboy.gif',
	'\:d/' => 'yahoo_dance.gif',
	'\:D/' => 'yahoo_dance.gif',
	'@};-' => 'yahoo_flower.gif',
	':-l' => 'yahoo_frustrated.gif',
	':-L' => 'yahoo_frustrated.gif',
	'8-x' => 'yahoo_ghost.gif',
	'8-X' => 'yahoo_ghost.gif',
	'&gt;:d&lt;' => 'yahoo_huggs.gif',
	'&gt;:D&lt;' => 'yahoo_huggs.gif',
	'@-)' => 'yahoo_hypnotized.gif',
	'*-:)' => 'yahoo_idea.gif',
	':^O' => 'yahoo_liar.gif',
	':^o' => 'yahoo_liar.gif',
	':(|)' => 'yahoo_monkey.gif',
	'$-)' => 'yahoo_moneyeyes.gif',
	':)&gt;-' => 'yahoo_peace.gif',
	':@)' => 'yahoo_pig.gif',
	'[-O&lt;' => 'yahoo_pray.gif',
	'[-o&lt;' => 'yahoo_pray.gif',
	'(~~)' => 'yahoo_pumpkin.gif',
	'[-x' => 'yahoo_shame.gif',
	'[-X' => 'yahoo_shame.gif',
	'**==' => 'yahoo_flag.gif',
	'%%-' => 'yahoo_shamrock.gif',
	':-&quot;' => 'yahoo_whistling.gif',
	'O-&gt;' => 'yahoo_malefighter1.gif',
	'o-&gt;' => 'yahoo_malefighter1.gif',
	'O=&gt;' => 'yahoo_malefighter2.gif',
	'o=&gt;' => 'yahoo_malefighter2.gif',
	'O-+' => 'yahoo_femalefighter.gif',
	'o-+' => 'yahoo_femalefighter.gif',
	'(%)' => 'yahoo_yingyang.gif',
	'=((' => 'yahoo_brokenheart.gif',
	'#:-s' => 'yahoo_sweating.gif',
	'#:-S' => 'yahoo_sweating.gif',
	'=))' => 'yahoo_rotfl.gif',
	'l-)' => 'yahoo_loser.gif',
	'L-)' => 'yahoo_loser.gif',
	'&lt;:-p' => 'yahoo_party.gif',
	'&lt;:-P' => 'yahoo_party.gif',
	':-ss' => 'yahoo_nailbiting.gif',
	':-sS' => 'yahoo_nailbiting.gif',
	':-Ss' => 'yahoo_nailbiting.gif',
	':-SS' => 'yahoo_nailbiting.gif',
	':-W' => 'yahoo_waiting.gif',
	':-w' => 'yahoo_waiting.gif',
	':-&lt;' => 'yahoo_sighing.gif',
	'&gt;:p' => 'yahoo_madtongue.gif',
	'&gt;:P' => 'yahoo_madtongue.gif',
	'&gt;:/' => 'yahoo_waving.gif',
	';))' => 'yahoo_giggle.gif',
	':-@' => 'yahoo_talktohand.gif',
	'^:)^' => 'yahoo_worship.gif',
	':-J' => 'yahoo_youkiddingme.gif',
	':-j' => 'yahoo_youkiddingme.gif',
	'(*)' => 'yahoo_star.gif'
);

$smiley_patterns = array(
	'/(\:&quot;&gt;)([\s+|<])/',
	'/(&gt;\:D&lt;)([\s+|<])/',
	'/(&gt;\:d&lt;)([\s+|<])/',
	'/(\:\-&quot;)([\s+|<])/',
	'/(&lt;\:\-P)([\s+|<])/',
	'/(&lt;\:\-p)([\s+|<])/',
	'/(\[\-O&lt;)([\s+|<])/',
	'/(\[\-o&lt;)([\s+|<])/',
	'/(&lt;\)\:\))([\s+|<])/',
	'/(\:\)&gt;\-)([\s+|<])/',
	'/(\:\-&amp;)([\s+|<])/',
	'/(o\=&gt;)([\s+|<])/',
	'/(&gt;\-\))([\s+|<])/',
	'/(\~\:&gt;)([\s+|<])/',
	'/(\:\-&gt;)([\s+|<])/',
	'/(o\-&gt;)([\s+|<])/',
	'/(&gt;\:\))([\s+|<])/',
	'/(O\-&gt;)([\s+|<])/',
	'/(O\=&gt;)([\s+|<])/',
	'/(&gt;\:\/)([\s+|<])/',
	'/(&gt;\:p)([\s+|<])/',
	'/(\=D&gt;)([\s+|<])/',
	'/(&gt;\:P)([\s+|<])/',
	'/(\=d&gt;)([\s+|<])/',
	'/(\:\-&lt;)([\s+|<])/',
	'/(\:&gt;)([\s+|<])/',
	'/(\^\:\)\^)([\s+|<])/',
	'/(\*\*\=\=)([\s+|<])/',
	'/(\:\(\|\))([\s+|<])/',
	'/(\:\-\(\()([\s+|<])/',
	'/(\@\}\;\-)([\s+|<])/',
	'/(3\:\-O)([\s+|<])/',
	'/(\=\:\-\))([\s+|<])/',
	'/(3\:\-o)([\s+|<])/',
	'/(O\:\-\))([\s+|<])/',
	'/(3\:\-0)([\s+|<])/',
	'/(0\:\-\))([\s+|<])/',
	'/(\\\:D\/)([\s+|<])/',
	'/(\\\:d\/)([\s+|<])/',
	'/(o\:\-\))([\s+|<])/',
	'/(\*\-\:\))([\s+|<])/',
	'/(\(\~\~\))([\s+|<])/',
	'/(\:\-\)\))([\s+|<])/',
	'/(\:\-ss)([\s+|<])/',
	'/(\#\:\-S)([\s+|<])/',
	'/(\/\:\-\))([\s+|<])/',
	'/(\#\:\-s)([\s+|<])/',
	'/(\:\-sS)([\s+|<])/',
	'/(\:\-SS)([\s+|<])/',
	'/(\:\-Ss)([\s+|<])/',
	'/(\=\)\))([\s+|<])/',
	'/(8\-x)([\s+|<])/',
	'/(\:\-L)([\s+|<])/',
	'/(\:\-l)([\s+|<])/',
	'/(\:\-w)([\s+|<])/',
	'/(\~o\))([\s+|<])/',
	'/(\~O\))([\s+|<])/',
	'/(\:\-W)([\s+|<])/',
	'/(8\-X)([\s+|<])/',
	'/(L\-\))([\s+|<])/',
	'/(l\-\))([\s+|<])/',
	'/(\@\-\))([\s+|<])/',
	'/(\[\-x)([\s+|<])/',
	'/(\;\)\))([\s+|<])/',
	'/(\:\-\@)([\s+|<])/',
	'/(\[\-X)([\s+|<])/',
	'/(\(\%\))([\s+|<])/',
	'/(o\-\+)([\s+|<])/',
	'/(\%\%\-)([\s+|<])/',
	'/(\:\@\))([\s+|<])/',
	'/(\=\(\()([\s+|<])/',
	'/(O\-\+)([\s+|<])/',
	'/(\:\-j)([\s+|<])/',
	'/(\:\^O)([\s+|<])/',
	'/(\:\^o)([\s+|<])/',
	'/(\:\-J)([\s+|<])/',
	'/(\$\-\))([\s+|<])/',
	'/(\(\*\))([\s+|<])/',
	'/(\;\-\))([\s+|<])/',
	'/(\:\-B)([\s+|<])/',
	'/(\:\-\*)([\s+|<])/',
	'/(\:\-b)([\s+|<])/',
	'/(8\-\|)([\s+|<])/',
	'/(\/\:\))([\s+|<])/',
	'/(\:\)\))([\s+|<])/',
	'/(\:\-X)([\s+|<])/',
	'/(\:\-O)([\s+|<])/',
	'/(b\-\()([\s+|<])/',
	'/(\:\-\|)([\s+|<])/',
	'/(\:\-x)([\s+|<])/',
	'/(\=P\~)([\s+|<])/',
	'/(\=p\~)([\s+|<])/',
	'/(\:\-d)([\s+|<])/',
	'/(\:\-D)([\s+|<])/',
	'/(\;\;\))([\s+|<])/',
	'/(X\-\()([\s+|<])/',
	'/(x\-\()([\s+|<])/',
	'/(\:0\))([\s+|<])/',
	'/(\:O\))([\s+|<])/',
	'/(\#\-o)([\s+|<])/',
	'/(\#\-O)([\s+|<])/',
	'/(\:\(\()([\s+|<])/',
	'/(\:o\))([\s+|<])/',
	'/(\:\-\/)([\s+|<])/',
	'/(\:\-o)([\s+|<])/',
	'/(B\-\))([\s+|<])/',
	'/(\(\:\|)([\s+|<])/',
	'/(b\-\))([\s+|<])/',
	'/(\:\-\\\\)([\s+|<])/',
	'/(\(\-\:)([\s+|<])/',
	'/(\:\-\?)([\s+|<])/',
	'/(\:\-P)([\s+|<])/',
	'/(\=\:\))([\s+|<])/',
	'/(B\-\()([\s+|<])/',
	'/(\:\-s)([\s+|<])/',
	'/(\:\-S)([\s+|<])/',
	'/(\:\-p)([\s+|<])/',
	'/(I\-\))([\s+|<])/',
	'/(\:\-\))([\s+|<])/',
	'/(\[\-\()([\s+|<])/',
	'/(\:\-\()([\s+|<])/',
	'/(8\-\})([\s+|<])/',
	'/(\:\-\$)([\s+|<])/',
	'/(\|\-\))([\s+|<])/',
	'/(\:\()([\s+|<])/',
	'/(\:d)([\s+|<])/',
	'/(\:o)([\s+|<])/',
	'/(\(\:)([\s+|<])/',
	'/(x\()([\s+|<])/',
	'/(X\()([\s+|<])/',
	'/(\:O)([\s+|<])/',
	'/(\;\))([\s+|<])/',
	'/(\=\;)([\s+|<])/',
	'/(\:x)([\s+|<])/',
	'/(\:\))([\s+|<])/',
	'/(\:X)([\s+|<])/',
	'/(\:P)([\s+|<])/',
	'/(\:p)([\s+|<])/',
	'/(\:\|)([\s+|<])/',
	'/(\:\*)([\s+|<])/',
	'/(\:D)([\s+|<])/',
);

$smiley_path = $GO_CONFIG->full_url.'smileys/';

function smiley_symbols_to_images($matches)
{
	global $smileys, $smiley_path;
	return '<img src="'.$smiley_path.$smileys[trim($matches[1])].'">'.$matches[2];
}
?>