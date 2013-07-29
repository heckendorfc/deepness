#!/usr/local/bin/perl

my $lineno=0;
my $line;
my $lastgroup="";
my $damtype="";
my $counters="";
my $mp=0;
my $ctr=0;
my $jp=0;
my $mod=0;
my $range=0;
my $range_v=0;
my $aoe=0;
my $aoe_v=0;
my $dir=0;

my @flist;

sub get_elem{
	my $ch = shift;

	if($ch eq "F"){
		return "ELEM_FIRE";
	}
	if($ch eq "I"){
		return "ELEM_ICE";
	}
	if($ch eq "L"){
		return "ELEM_LIGHTNING";
	}
	if($ch eq "A"){
		return "ELEM_WIND";
	}
	if($ch eq "W"){
		return "ELEM_WATER";
	}
	if($ch eq "E"){
		return "ELEM_EARTH";
	}
	if($ch eq "H"){
		return "ELEM_HOLY";
	}
	if($ch eq "D"){
		return "ELEM_DARK";
	}

	return 0;
}

sub get_function{
	my $orig = shift;

	my $name=lc($orig);

	$name=~s/ /_/g;
	$name=~s/_([0-9]+)$/\1/;

	#$flist[$#flist-1]=$name;
	push(@flist,$name);

	return $name;
}

open(FILE,"<FFT_PS_Mechanics.txt");

while($line=<FILE>){
	$lineno=$lineno+1;
	#next if $lineno<2250;
	#exit if $lineno>5830;
	next if $lineno<2250;
	if($lineno>5830){
		print " // $lastgroup\n";

		print "\n";
		for $x (@flist){
			print "static void $x(struct battle_char *origin, struct battle_char **targets, int num){\n}\n\n";
		}
		exit;
	}

	chomp($line);

	if($line=~/^\| \[([A-Za-z0-9 ]+)\][ ]+\[ ([A-F0-9]+) \][ ]+([()A-Z\- ]+)/){
		unless($3 eq $lastgroup){
			print "}, //$lastgroup\n{";
			$lastgroup=$3;
		}
		my $funcname=get_function($1);
		print "{.f.af=$funcname,";
	}
	elsif($line=~/^\| ([a-z]+)[ ]+\| CBG: (.) \|  MP:[ ]+([0-9]+)/){
		if($1 eq "physical"){
			$damtype="AFLAG_PHYSICAL|";
		}
		elsif($1 eq "magical"){
			$damtype="AFLAG_MAGIC|";
		}
		else{
			$damtype="";
		}

		if($2 eq "+"){
			$counters="AFLAG_COUNTER|";
		}
		else{
			$counters="";
		}

		$mp=$3;
	}
	elsif($line=~/^\| REFL: (.)[ ]+\|  CM: (.) \| CTR:[ ]+([0-9]+)/){
		if($1 eq "+"){
			$counters.="AFLAG_REFLECT|";
		}

		if($2 eq "+"){
			$counters.="AFLAG_COUNTER_MAGIC|";
		}

		$ctr=$3;
	}
	elsif($line=~/^\| CALC: (.)[ ]+\|  CF: (.) \|  JP:[ ]*([0-9]+)/){
		if($1 eq "+"){
			$counters.="AFLAG_MATH|";
		}

		if($2 eq "+"){
			$counters.="AFLAG_COUNTER_FLOOD|";
		}

		$jp=$3;
	}
	elsif($line=~/^\| ELEM: (.)[ ]+\| EVD: (.) \| MOD:[ ]*([0-9]+)/){
		if($1 eq "-"){
			$elem=0;
		}
		else{
			$elem=get_elem($1);
		}

		if($2 eq "+"){
			$counters.="AFLAG_EVADE|";
		}

		$mod=$3;
	}
	elsif($line=~/^\| Range: ([0-9a-zA-Z]+)/){
		if($1 eq "Auto"){
			$range=0;
			$range_v=0;
		}
		elsif($1 eq "weapon"){
			$range="RANGE_WEAPON";
			$range_v=0;
		}
		else{
			my @sp=split(/v/,$1);
			$range=$sp[0];
			if(defined $sp[1]){
				$range_v=$sp[1];
			}
			else{
				$range_v=0;
			}
		}

		if($line=~/\/ Effect: ([0-9a-zA-Z]+)/){
			if($1 eq "All"){
				$aoe=0;
				$aoe_v=0;
			}
			else{
				my @sp=split(/v/,$1);
				$aoe=$sp[0];
				if(defined $sp[1]){
					$aoe_v=$sp[1];
				}
				else{
					$aoe_v=0;
				}
			}
		}
	}
	elsif($line=~/^\| Effect: ([0-9a-zA-Z]+)/){
		if($1 eq "All"){
			$aoe=0;
			$aoe_v=0;
		}
		else{
			my @sp=split(/v/,$1);
			$aoe=$sp[0];
			if(defined $sp[1]){
				$aoe_v=$sp[1];
			}
			else{
				$aoe_v=0;
			}
		}
	}
	elsif($line=~/^ -/){
		print "$jp,${damtype}${counters}0,$elem,$ctr,$mod,$mp,{$range,$range_v,$aoe,$aoe_v,0}},";
	}
}


# 2250
# 5830
