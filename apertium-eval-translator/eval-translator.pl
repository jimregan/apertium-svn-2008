
# (c) 2006 Felipe S�nchez Mart�nez
# (c) 2006 Universitat d'Alacant
#
# This software calculates the word error rate (WER) and the
# position-independent word error rate (PER) between the translation
# performed by the apertium MT system an a reference translation
# obtained by post-editing the system ouput.
# 
# The edit_distance procedure used in this script is based on 
# the Levenshtein distance implementation by Jorge Mas Trullenque 
# that can be found in http://www.merriampark.com/ldperl2.htm 
#
# This software is licensed under the GPL license version 2, or at
# your option any later version 
#

use strict; 
use warnings;

# Getting command line arguments:
use Getopt::Long;
# Documentation:
use Pod::Usage;
# I/O Handler
use IO::Handle;

my($test, $ref, $help, $beam, $version);

my($nunknown, $ntest, $nref);

my($test_corpus, $ref_corpus);

# Command line arguments
GetOptions( 'test|t=s'           => \$test,
            'ref|r=s'            => \$ref,
            'beam|b=n'           => \$beam,
            'help|h'             => \$help,
	    'version|v'          => \$version,
          ) || pod2usage(2);

if ($version) {
   print "apertium-eval-translator 1.0.0\n";
   exit 0;
}

pod2usage(2) if $help;
pod2usage(2) unless ($test);
pod2usage(2) unless ($ref);

$beam=0 unless ($beam);
$beam=0 if ($beam<0);

open(TEST, "<$test") or die "Error: Cannot open test file \'$test\': $!\n";
open(REF, "<$ref") or die "Error: Cannot open reference file \'$ref\': $!\n";

undef $/; #To read whole files at one step

$_=<TEST>;
&preprocess;
$test_corpus=$_;
$nunknown=s/[*](\w+)/$1/g;
my @words_test = split /[\s\n]+/;
$ntest=@words_test;
close(TEST);

$_=<REF>;
&preprocess;
$ref_corpus=$_;
my @words_ref = split /[\s\n]+/;
$nref=@words_ref;
close(REF);

print "Test file: '$test'\n";
print "Reference file '$ref'\n\n";

print "Statistics about input files\n";
print "-------------------------------------------------------\n";
print "Number of words in reference: $nref\n";
print "Number of words in test: $ntest\n";
print "Number of unknown words (marked with a star) in test: $nunknown\n";
print "Percentage of unknown words: ", sprintf("%.2f",($nunknown/$ntest)*100), " %\n";
print "\n";

my $distance_nounk=&edit_distance; 
print "Results when removing unknown-word marks (stars)\n";
print "-------------------------------------------------------\n";
print "Edit distance: $distance_nounk\n";
print "Word error rate (WER): ", sprintf("%.2f",($distance_nounk/$ntest)*100), " %\n";
my $per_nounk=&position_independent_errors;
print "Number of position-independent word errors: ",  $per_nounk, "\n";
print "Position-independent word error rate (PER): ", sprintf("%.2f",($per_nounk/$ntest)*100), " %\n";

print "\n";

$_=$test_corpus;
@words_test = split /[\s\n]+/;

$_=$ref_corpus;
@words_ref = split /[\s\n]+/;

my $distance=&edit_distance; 
print "Results when unknown-word marks (stars) are not removed\n";
print "-------------------------------------------------------\n";
print "Edit distance: $distance\n";
print "Word Error Rate (WER): ", sprintf("%.2f",($distance/$ntest)*100), " %\n";
my $per=&position_independent_errors;
print "Number of position-independent word errors: ",  $per, "\n";
print "Position-independent word error rate (PER): ", sprintf("%.2f",($per/$ntest)*100), " %\n";

print "\n";

print "Statistics about the translation of unknown words\n";
print "-------------------------------------------------------\n";
print "Number of unknown words which were free rides: ", $distance-$distance_nounk, "\n";
print "Percentage of unknown words that were free rides: ", 
       sprintf("%.2f",(($distance-$distance_nounk)/$nunknown)*100), " %\n";


sub position_independent_errors {

  #Words that have no matching counterparts are counted as
  #substitution errors. Depending on which corpus, test or reference,
  #is longer the remaining words result in either insertion or
  #deletion errors.  The number of position-independent errors are
  #always less or equal to the edit distance.

  my (%hash_test, %hash_ref);

  foreach (sort @words_test) {
    if (defined($hash_test{$_})) {
      $hash_test{$_}++;
    } else {
      $hash_test{$_}=1;
    }
  }

  foreach (sort @words_ref) {
    if (defined($hash_ref{$_})) {
      $hash_ref{$_}++;
    } else {
      $hash_ref{$_}=1;
    }
  }

  my $ntest = @words_test;
  my $nref = @words_ref;

  my $sum=0;

  foreach (keys %hash_test) {
    if(defined($hash_ref{$_})) {
      $sum+=abs($hash_test{$_}-$hash_ref{$_});
    } else {
      $sum+=$hash_test{$_};
    }
    $hash_test{$_}=0;
    $hash_ref{$_}=0;
  }

  foreach (keys %hash_ref) {
    if(defined($hash_test{$_})) {   
      $sum+=abs($hash_ref{$_}-$hash_test{$_});
    } else {
      $sum+=$hash_ref{$_};
    }
    $hash_ref{$_}=0;
    $hash_test{$_}=0;
  }

  return (abs($ntest-$nref)+$sum)/2;
}

sub edit_distance {
  my @W=(0..@words_ref);
  my ($i, $j, $cur, $next);

  my ($lim_inf, $lim_sup, $best_j);
  $best_j=0;
  for $i (0..$#words_test) {
    $cur=$i+1;

    if($beam>0) {
      $lim_inf=$best_j-$beam; 
      $lim_inf=0 if ($lim_inf<0);

      $lim_sup=$best_j+$beam;
      $lim_sup=$#words_ref if ($lim_sup>$#words_ref);
    } else {
      $lim_inf=0;
      $lim_sup=$#words_ref;
    }

    for $j ($lim_inf..$lim_sup){
      my $cost=($words_test[$i] ne $words_ref[$j]);
      $next=min([$W[$j+1]+1, $cur+1, $cost+$W[$j]]);
      $W[$j]=$cur;

      $best_j=$j+1 if ($cur > $next);

      $cur=$next;
    }
    $W[@words_ref]=$next;
  }
  return $next;
}

sub min {
  my @list = @{$_[0]};
  my $min = $list[0];

  foreach my $i (@list) {
    $min = $i if ($i < $min);
  }
   return $min;
}

sub preprocess {
  #Insert spaces before and after  punctuation marks 
  #s/([.,;:%�?�!()\[\]{}<>])/ $1 /g;
}


__END__


=head1 NAME

=head1 SYNOPSIS

apertium-eval-translator -test testfile -ref reffile [-beam <n>]

Options:

  -test|-t     Specify the file with the translation to evaluate 
  -ref|-r      Specify the file with the reference translation 
  -beam|-b     Perform a beam search by looking only to the <n> previous 
               and <n> posterior neigboring words (optional parameter 
               to make the evaluation much faster)
  -help|-h     Show this help message
  -version|-v  Show version information and exit
  
Note: The <n> value provided with -beam is language-pair dependent.
      The closer the languages involved are, the lesser <n> can be
      without affecting the evaluation results.  This parameter only
      affects the WER evaluation.

Note: Reference translation MUST have no unknown-word marks, even if
      they are free rides.

This software calculates (at document level) the word error rate (WER)
and the postion-independent word error rate (PER) between a
translation performed by the Apertium MT system and a reference
translation obtained by post-editing the system ouput. 

It is assumed that unknow words are marked with a start (*), as
Apertium does; nevertheless, it can be easily adapted to evaluate
other MT systems that do not mark unknown words with a star.

(c) 2006 Felipe S�nchez Mart�nez
(c) 2006 Universitat d'Alacant

This software is licensed under the GNU GENERAL PUBLIC LICENSE version
2, or at your option any latter version. See
http://www.gnu.org/copyleft/gpl.html for a complete version of the
license.
