/************************************************************************/
/* GO TTS: Ticket tracking system                                       */
/* ============================================                         */
/*                                                                      */
/* Copyright (c) 2002 by Meir Michanie                                  */
/* http://www.riunx.com   meirm@riunx.com                               */
/*                                                                      */
/* This program is free software. You can redistribute it and/or modify */
/* it under the terms of the GNU General Public License as published by */
/* the Free Software Foundation; either version 2 of the License.       */
/************************************************************************/


This is a pre-alpha release of TTS for GROUP OFFICE.

THIS IS NOT A PRODUCTION MODULE.

I mean to redo all the code, I posted now to get feedbacks and help.
I know it is very buggy and does not match the quality of the rest of GO.
I wanted to stop talking and start doing. I strongly believe in the motto
"release early, release often".



What is it?

It is a module to administer tasks between users of GO.


How do I test it?

1: copy opentts.class.inc to ../../classes

2: you need to add the tables in the file opentts_go-alpha.sql in your
database.

Second: add your user id as administrator of the TTS ( this is not related to
GO permissions)
	put your uid in the table nuke_groups_members as gid 2,3,5 ( three
rows)

3: start playing with it.
 
