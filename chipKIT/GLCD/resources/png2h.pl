#!/usr/bin/perl

use strict;
use GD;
my $file = shift;

my $tval = 0;
if($file eq "-i")
{
	$tval = 1;
	$file = shift;
}

if(!-f $file)
{
	die("Unable to open $file");
}

my $out = $file . ".h";
my $name = $file;
if($file =~ /^(.*)\.png/i)
{
	$out = $1 . ".h";
	$name = $1;
}

my $img = new GD::Image($file);
open(OUT,">".$out);

print OUT "const __attribute__((space(prog))) unsigned char " . $name . "[8][128] = {\n";

my $y = 0;
while($y < 8)
{
	my $x = 0;
	while($x < 128)
	{
		my $cval = 0;
		my $z = 0;
		while($z < 8)
		{
			my $c = $img->getPixel($x,($y*8)+$z);
			$cval = $cval >> 1;
			if($c == $tval)
			{
				$cval |= 0x80;
			}
			$z++;
		}
		my $o = sprintf("0x%02x,",$cval);
		print OUT $o;
		$x++;
	}
	print OUT "\n";
	$y++;
}
print OUT "};\n";
close(OUT);
