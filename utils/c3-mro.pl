#!/usr/bin/env perl

# Abstract: Check if c3 mro is well understood.
# License: AGL, see LICENSE file for copyright and license details.
#
# General structure of a small DSL for defining classes with their
# methods and inheritance relationships. This program parses the
# DSL following __DATA__ to perform c3 linearizations and method
# resolutions with results tested against that of Perl's c3 module.
#
# CLASS_NAME: METHOD_NAME
# SUBCLASS_NAME -> ( SUPERCLASS_NAME ... ) : METHOD_NAME ...
#
# Example:
# O: meth1 meth2 meth3
# A -> (O) : meth2
# B -> (O) : meth1
# C -> (A B) : meth2
#
# The above example defines a section, sections are separated by
# the token '###'. Sections are tested individually.

use strict;
use warnings;
use feature qw(say current_sub postderef_qq);

use Data::Dumper;

use Test::More;

# Parse a section and return the result
sub parse_section {
   my $def_str = shift;
   my $section;

   while ($def_str =~
          /\G ([A-Z]) (?: \h* -> \h* \( (?<s>[A-Z](?:\h++[A-Z])*) \) )? (?: \h* : \h* (?<m>\w+[@]?(?:\h++\w++)*) )? \s* /gx) {

      $section->{$1} = undef;
      if (exists $+{s}) {
         my @supers = split " ", $+{s};
         my @undef  = grep { $_ ne $1 && !exists $section->{$_} } @supers;

         die "undefined class(es): [@undef]\n" if @undef;
         $section->{$1}{s} = [@supers];
      }

      $section->{$1}{m} = [split " ", $+{m}] if defined($+{m});
   }

   return $section;
}

# Parse each section and return the result
sub parse_sections {
   my $data = shift;
   my $sections;

   push @$sections, parse_section($1) while $data =~ /\G\s*(.+?)(?:###|\z)/gs;
   return $sections;
}

# Generate and return Perl code of a section
sub gen_perl_code {
   my $section   = shift;
   my $perl_code = <<~'EOF';
   use strict;
   use warnings;
   use mro "c3";
   EOF

   foreach my $class (sort keys %$section) {
      my $class_info = $section->{$class};

      $perl_code .= "package $class {";
      if (exists $class_info->{s}) {
         $perl_code .= exists $class_info->{m} ? "\n\t" : ' ';
         $perl_code .= "use parent -norequire, qw(@{$class_info->{s}});";
      }

      if (exists $class_info->{m}) {
         my $len = $class_info->{m}->@*;
         my $sep = $len > 1 || exists $class_info->{s} ? "\n" : ' ';

         for (my $i = 0 ; $i < $len ; $i++) {
            my $method = $class_info->{m}[$i];
            my $name   = $method =~ s/[@]$//r;
            my $tab    = !exists $class_info->{s} && ($i - 1) < 0 && ($i + 1 == $len) ? '' : "\t";

            $perl_code .= qq[${sep}${tab}sub $name { print "${class}::$name"];
            $perl_code .= '; $_[0]->next::method()' if $method =~ /[@]$/;
            $perl_code .= ' }';
         }

         $perl_code .= "$sep}\n";
      }
      else {
         $perl_code .= " }\n";
      }
   }

   return $perl_code;
}

# Compute the C3 linearization of a class from a section. The dynamic
# hash table $c3 keeps track of all c3 results of classes defined in
# the section of concern, set it to `undef' when entering a new section.
# In Maat, linearization is done at compilation
sub c3_linearize_class {
   my ($section, $class) = (shift, shift);

   $_[0]->{$class} = [$class];

   # Has no superclass? probably 'A'
   return unless exists $section->{$class}{s};

   my ($supers, $i) = ($section->{$class}{s}, -1);

   # Just in case subroutine `test' does not linearize in order of class creation
   while (++$i < @$supers) {
      __SUB__->($section, $supers->[$i], $_[0]) unless exists $_[0]->{$supers->[$i]};
      return                                    unless ref $_[0];
   }

   # Single inheritance? simply push
   if (@$supers == 1) {
      push @{$_[0]->{$class}}, $_[0]->{$supers->[0]}->@*;
      return;
   }

   my ($prev_sol, $error);
   my $sub_sol    = $_[0]->{$supers->[0]};
   my %merged = ($supers->[0] => 0);

   foreach my $c (1 .. $#$supers) {
      my $start     = 0;
      my $to_insert = $_[0]->{$supers->[$c]};

      $prev_sol = $sub_sol;
      $sub_sol  = [];

    insertion: for (my $i = 0 ; $i < @$prev_sol ; $i++) {
         for (my $j = $start ; $j < @$to_insert ; $j++) {
            if (exists $merged{$to_insert->[$j]}) {
               $error = <<~"EOE";
               Inconsistent hierarchy during C3 merge of class '$class':
                       current merge results \[
                               $class,
               EOE
               $error .= "\n" . ' ' x 16 . "$_,\n" foreach @$supers[0 .. $merged{$to_insert->[$j]}];
               $error .= ' ' x 8 . "\]\n" . ' ' x 8;
               $error .= "merging failed on '$to_insert->[$j]' at .+";
               $_[0] = $error;
               return;
            }
            if ($prev_sol->[$i] eq $to_insert->[$j]) {
               push @$sub_sol, $to_insert->@[$start .. $j];
               $start = $j + 1;
               next insertion;
            }
         }
         push @$sub_sol, $prev_sol->[$i];
      }

      $merged{$supers->[$c]} = $c;
   }
   push @{$_[0]->{$class}}, @$sub_sol;
}

# For each class in a section, resolve all its methods, including
# the inherited ones is done at class creation, this makes vm
# execution fast. This is done by walking down the C3 list containing
# the directly or indirectly inherited classes to bind their methods
# to the base class so that the base class has direct access to
# them during method calls.
sub resolve_methods {
   my ($section, $c3s) = @_;
   my $resolved_meths;

   foreach my $class (sort keys %$section) {
      my $class_c3 = $c3s->{$class};

      foreach my $sup (@$class_c3) {
         push $resolved_meths->{$class}{$_ =~ s/[@]$//r}->@*, "${sup}::$_" foreach $section->{$sup}{m}->@*;
      }
   }

   return $resolved_meths;
}

# Simulate a method call $method on object $object
sub call_method {
   my ($resolved_meths, $object, $method) = @_;

   if (exists $resolved_meths->{$object}{$method}) {
      my $output;
      my $meth_list = $resolved_meths->{$object}{$method};

      for (my $i = 0 ; $i < @$meth_list ; $i++) {
         $output .= $meth_list->[$i] =~ s/[@]$//r;
         if ($meth_list->[$i] =~ /[@]$/) {
            $output .= "No next::method '$method' found for $object at .+" if $i == $#$meth_list;
            next;
         }
         return $output;
      }
   }

   return qq{Can't locate object method "$method" via package "$object" at .+};
}

sub perl_c3_linearize_class {
   my $class = shift;
   return $_[0] . <<~"EOP";
   print qq[\@{mro::get_linear_isa("$class", "c3")}];
   EOP
}

sub perl_method_call {
   my ($class, $method) = (shift, shift);
   return $_[0] . "\n$class->$method();";
}

# Run generated perl code and return its stdout+stderr
sub perl_run {
   my $code = shift;

   local $/;
   open(my $fh, "-|", "perl -E '$code'") or die "$!\n";

   return <$fh>;
}

# This is the entry point our program. It tests with the help of Perl's C3
# our own c3 implementation, It compares Perl's linearization of a class
# with our own c3 list of the class, it performs series of method calls
# to check.
sub test_c3_maat_way {
   local $/;
   my $ntests   = 0;
   my $sections = parse_sections(<DATA>);
   my @codes    = map gen_perl_code($_), @$sections;

 section: for (my $i = 0 ; $i < @$sections ; $i++) {
      my $c3s;
      my $section = $sections->[$i];

      # Just like in real compilers, linearize classes in order of their definitions
      foreach my $class (sort keys %$section) {
         my $expected = perl_run(perl_c3_linearize_class($class, $codes[$i]));

         $ntests++;
         c3_linearize_class($section, $class, $c3s);
         if (ref $c3s) {
            my $c3_result = join ' ', $c3s->{$class}->@*;
            is($c3_result, $expected, "[section $i]: is c3 of '$class' [$c3s->{$class}->@*]?");
         }
         else {
            ok($expected =~ qr/$c3s/s, "[section $i]: failed to c3 resolve '$class'?");
            next section;
         }
      }

      say Dumper $c3s;

      # Test if method resolution works
      my $resolved_meths = resolve_methods($section, $c3s);
      foreach my $class (sort keys %$section) {
         foreach my $meth (keys $resolved_meths->{$class}->%*) {
            my $expected = perl_run(perl_method_call($class, $meth, $codes[$i]));
            my $result   = call_method($resolved_meths, $class, $meth);

            is($result, $expected, "[section $i] call $meth on $class, r: [$resolved_meths->{$class}{$meth}->@*]");
            $ntests++;
         }
      }
   }

   return $ntests;
}

done_testing(test_c3_maat_way());

__DATA__
A : meth1 meth2 meth3

B -> (A) : meth2
C -> (A) : meth1@
D -> (A) : meth2

E -> (B)
F -> (C)
G -> (C)
H -> (C)
I -> (C)
J -> (D)

K -> (E)
L -> (F I)
M -> (I J)

N -> (K E L M)
