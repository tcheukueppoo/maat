#!/usr/bin/env perl

# Abstract: Check if c3 mro is well understood.
# License: AGL, see LICENSE file for copyright and license details.
#
# General structure of a small DSL for defining classes with their
# methods and inheritance relationships. This program parses the
# DSL following __DATA__ and perform series of tests of c3 method
# resolution with validation done with the help of Perl's c3 mro.
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
use feature qw(say current_sub);

use Data::Dumper;

# Parse a section
sub parse_section {
   my $def_str = shift;
   my $section;

   while ($def_str =~
          /\G ([A-Z]) (?: \h+ -> \h+ \( (?<s>[A-Z](?:\h++[A-Z])*) \) )? (?: \h+ : \h+ (?<m>\w+(?:\h++\w++)*) )? \s* /gx) {
      $section->{$1} = undef;
      if (defined $+{s}) {
         my @supers = split " ", $+{s};
         my @undef  = grep { $_ ne $1 && !exists $section->{$_} } @supers;
         die "undefined class(es): [@undef]\n" if @undef;
         $section->{$1}{s} = [@supers];
      }
      $section->{$1}{m} = [split " ", $+{m}] if defined($+{m});
   }

   return $section;
}

# Simple parse sections, don't you see?
sub parse_sections {
   my $data = shift;
   my $sections;

   push @$sections, parse_section($1) while $data =~ /\G\s*(.+?)(?:###|\z)/gs;
   return $sections;
}

# Generate Perl code that correspond to each section and return
# them in an anonymous array.
sub gen_perl_code {
   my $sections = shift;
   my $codes;

   foreach my $section (@$sections) {
      my $perl_code = "\n\nuse feature qw(say);\n\n";

      foreach my $class (sort keys %$section) {
         my $class_info = $section->{$class};

         $perl_code .= "package $class {";
         $perl_code .= "\n\n\tuse parent -norequire qw(@{$class_info->{s}});\n\n"
           if exists $class_info->{s};
         if (exists $class_info->{m}) {
            foreach my $method (@{$class_info->{m}}) {
               my $name = $method =~ s/@$//r;

               $perl_code .= "\tsub $name { say 'A::$name'; "
                 . (
                    $method =~ /@$/
                    ? "'$class'->next::method(); }"
                    : "; }"
                   );
               $perl_code .= "\n\n";
               $perl_code .= "}\n\n";
            }
         }
         else {
            $perl_code .= '}' if $perl_code =~ /{$/;
         }
      }
      push @$codes, $perl_code;
   }

   return $codes;
}

# Compute the C3 linearization of a class from a section. The dynamic
# hash table $c3 keeps track of c3 results of superclasses, it contains
# all linearizations of the classes defined in a section, set it to
# undef when entering for a new section.
sub c3_linearize_class {
   my ($class, $undef) = @_;

   state $c3;
   $c3 = undef, return if $undef;
   $c3->{$class} = [$class];

   # Has no superclass? probably 'A'
   return $c3 unless exists $class->{s};

   foreach my $sup (grep { !exists $c3->{$_} } $class->{s}->@*) {
      my $result = __SUB__->($sup);
      return $result unless ref $result;
   }

   # Single inheritance? simply push
   if ($class->{s}->@* == 1) {
      push @{$c3->{$class}}, $c3->{$class->{s}[0]};
      return $c3;
   }

   my ($prev_sol, $error);
   my $sub_sol = $class->{s}[0];

 class: foreach my $c (1 .. $#{$class->{s}}) {
      my $start = 0;

      $prev_sol  = $sub_sol;
      $sub_sol   = [];
      $to_insert = $c3->{$class->{s}[$c]};

    insertion: for (my $i = 0 ; $i < @$prev_sol ; $i++) {
         for (my $j = $start ; $j < @$to_insert ; $j++) {
            if ($prev_sol->[$i] eq $to_insert->[$j]) {
               if ($s == $j) {

                  # Mimic Perl's error message
                  my $error = <<~"EOE";
                  Inconsistent hierarchy during C3 merge of class '$class':
                          current merge results \[
                  EOE
                  $error .= "\n" . ' ' x 16 . "$_,\n" foreach @$sub_sol;
                  $error .= ' ' x 8 . "\]\n" . ' ' x 8;
                  $error .= "merging failed on '$to_insert->[$j]' at .+\$\n";
                  last class;
               }
               push @$sub_sol, @$prev_sol{$s .. $j};
               $start = $j + 1;
               next insertion;
            }
         }
         push @$sub_sol, $prev_sol->[$i];
      }
   }

   return $error if $error;
   push @{$c3->{$class}}, @$sub_sol;
   return $c3;
}

# Just like we'll do when compiling Maat code, get all the possible
# methods a class might inherit and for each of them, build an array
# of its supermethod.
sub resolve_methods {
   my ($section, $c3) = @_;

   my $meths_resolved;
   foreach my $class (sort keys @$section) {
      my $resolved_sups = $c3->{$class};
      foreach my $sup (@$resolved_sups) {
         push $meths_resolved->{$class}{$section->{$sup}{m}[$_ =~ s/@$//r]}->@*, "${e}::$_" foreach $section->{$sup}{m}->@*;
      }
   }

   return $meths_resolved;
}

sub call_method {
   my ($meths_resolved, $class, $method) = @_;

   my $output = qq{Can't locate object method "$method" via package "$class" at .+};
   if (defined $meths_resolved{$class}) {
      my $meth_list = $meths_resolved{$class};

      $output = '';
      for (my $i = 0; $i < @$meth_list; $i++) {
         $output .= $meth_list[$i] =~ s/@$//r;
         if ($meth_list[$i] =~ /@$/) {
            $output .= "No next::method '$method' found for $class at .+" if $i == $#$meth_list[$i];
            next;
         }
         last;
      }
   }

   return $output;
}


# The whole story is about testing linearization and method calls, if
# it all tests pass then this means well understood the C3 algorithm.
sub test_all {
   local $/;
   my $sections = parse_sections(<DATA>);
   say Dumper gen_perl_code($sections);
}

__DATA__
O : meth1 meth2 meth3
A -> (O) : meth2
B -> (O) : meth1
C -> (A B) : meth2
###
O
