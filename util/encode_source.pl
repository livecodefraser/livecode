#!/usr/bin/env perl

use warnings;
use File::Basename;

# Incoming arguments. The basename function is applied to the
# varName parameter to work around a Gyp bug on Windows where
# it incorrectly interprets the parameter as a path and adds
# a relative path in front of the name.
my $sourceFile	= $ARGV[0];
my $destFile	= $ARGV[1];
my $varName	= basename($ARGV[2]);

#! /usr/bin/revolution -ui

# $1 = source file
# $2 = dst file
# $3 = var name


# Read in the source file
open INPUT, "<$sourceFile"
	or die "Could not open source file \"$sourceFile\": $!";
my @lines = <INPUT>;
close INPUT;

# Write to the output file
open OUTPUT, ">$destFile"
	or die "Could not open output file \"$destFile\": $!";
	
print OUTPUT "const char *${varName} [] =\n{\n";
foreach my $line (@lines)
{
	if ((length $line) == 0)
	{
		next;
	}
	$line =~ s/\\/\\\\/g;
	$line =~ s/\"/\\"/g;
	$line =~ s/\n/\\n/g;
	print OUTPUT "\t\"$line\",\n";
}

print OUTPUT "\t0\n";
print OUTPUT "};\n";
close OUTPUT;
