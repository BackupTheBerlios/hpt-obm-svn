%typemap = (
	"char" => "C",
	"varchar" => "C",
	"text" => "X",
	"tinyint" => "L",
	"bigint" => "I",
	"int" => "I",
	"smallint" => "I",
	"clob" => "X",
	"blob" => "B",
	"longblob" => "B",
	"mediumblob" => "B",
	"date" => "D",
	"timestamp" => "T",
	"datetime" => "T",
	"enum" => "C",
	"set" => "C",
	"double" => "F",
	"float" => "F"
);
print '<?xml version="1.0"?>'."\n";
#print '<!DOCTYPE schema SYSTEM "adodb.dtd">'."\n";
print '<schema version="0.2">'."\n";
while (<>) {
	next if m/^\s*#/;
	if (m/CREATE TABLE `?(\w*)`? \(/) {
		$start = 1;
		$tablename = $1;
		%f = ();
		next;
	}
	if (m/\) TYPE=/)
	{
		$comment = $1 if m/COMMENT='([^']*)'/;
		print "\t<table name=\"$tablename\">\n";
		foreach $i (values(%f))
		{
			print "\t\t<field name=\"".$i->{"name"}."\" type=\"".$i->{"type"}."\"";
			print " size=\"".$i->{"size"}."\"" if defined($i->{"size"});
			print ">\n";
			if (defined($i->{"notnull"})) {
				print "\t\t\t<NOTNULL/>\n";
			}elsif (defined($i->{"primary"})) {
				print "\t\t\t<PRIMARY/>\n";
			} elsif (defined($i->{"key"})) {
				print "\t\t\t<KEY/>\n";
			}
			print "\t\t\t<DEFAULT value=\"".$i->{"default"}."\"/>\n" if defined($i->{"default"});
			print "\t\t</field>\n";
		}
		print "\t\t<descr>$comment</descr>\n";
		print "\t</table>\n\n";
		$start = 0;
		next;
	}
	if ($start)
	{
		s/^\s*(.*)\s*$/\1/;
		$_ =~ s/,\s*$//g;
		while ($_ =~ m/\([^\)]*\s+[^\)]*\)/g)
		{
			$_ =~ s/\(([^\)]*)\s+([^\)]*)\)/\(\1\2\)/;
		}
		@s = split(/\s+/,$_);
		$fieldname = $s[0];
		if ($fieldname =~ m/^PRIMARY$/i)
		{
			$fields = $s[2];
			$fields =~ s/^\(//g;
			$fields =~ s/\)$//g;
			$fields =~ s/`//g;
			@fields = split(/,/,$fields);
			foreach $i (@fields)
			{
				$f{$i}->{"primary"} = 1;
				$f{$i}->{"name"} = $i;
			}
		}
		elsif ($fieldname =~ m/^UNIQUE$/i)
		{
			$fields = $s[3];
			$fields =~ s/^\(//g;
			$fields =~ s/\)$//g;
			$fields =~ s/`//g;
			@fields = split(/,/,$fields);
			foreach $i (@fields)
			{
				$f{$i}->{"unique"} = 1;
				$f{$i}->{"name"} = $i;
			}
		}
		elsif ($fieldname =~ m/^KEY$/i)
		{
			$fields = $s[2];
			$fields =~ s/^\(//g;
			$fields =~ s/\)$//g;
			$fields =~ s/`//g;
			@fields = split(/,/,$fields);
			foreach $i (@fields)
			{
				$f{$i}->{"key"} = 1;
				$f{$i}->{"name"} = $i;
			}
		}
		else
		{
			$fieldname =~ s/`//g;
			next if $fieldname eq "";
			$f{$fieldname}->{"name"} = $fieldname;
			if ($s[1] =~ m/(.*)\((.*)\)/)
			{
				$f{$fieldname}->{"type"} = $typemap{lc($1)};
				$f{$fieldname}->{"size"} = $2 if ($1 =~ m/^enum$/i) || ($1 =~ m/^set$/i);
			}
			else
			{
				$f{$fieldname}->{"type"} = $typemap{lc($s[1])};
			}

			if ($s[2] =~ m/NULL/i)
			{
				$f{$fieldname}->{"null"} = 1;
				$id = 3;
			}
			if ($s[2] =~ m/NOT/i && $s[3] =~ m/NULL/i)
			{
				$f{$fieldname}->{"notnull"} = 1;
				$id = 4;
			}
			if ($s[$id] =~ m/default/i)
			{
				$value = $s[$id+1];
				$value =~ s/^'(.*)'$/\1/;
				$f{$fieldname}->{"default"} = $value;
				$id += 2;
			}
		}
		
	}
}
print '</schema>'."\n";
