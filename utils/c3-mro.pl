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
use feature qw(say);

use Data::Dumper;

sub parse_class_defs_section {
   my $def_str = shift;
   my $defs;

   while ($def_str =~ /\G ([A-Z]) (?: \h+ -> \h+ \( (?<s>[A-Z](?:\h++[A-Z])*) \) )? (?: \h+ : \h+ (?<m>\w+(?:\h++\w++)*) )? \s* /gx) {
      $defs->{$1} = undef;
      if (defined $+{s}) {
         my @supers = split " ", $+{s};
         my @undefs = grep { !exists $defs->{$_} } @supers;
         die "undefined class(es): [@undefs]\n" if @undefs;
         $defs->{$1}{s} = [@supers];
      }
      $defs->{$1}{m} = [ split " ", $+{m} ] if defined($+{m});
   }

   return $defs;
}

sub parse_class_defs_sections {
   my $data = shift;
   my $sections;

   push @$sections, parse_class_defs_section($1) while $data =~ /\G\s*(.+?)(?:###|\z)/gs;
   return $sections;
}

sub gen_classdefs_perl_code {
   my $sections = shift;
   my ($section_codes, $perl_code);

   foreach my $section (@$sections) {
      $perl_code .= "\n\nuse feature qw(say);\n\n";
      foreach my $class (sort keys %$section) {
         my $class_info = $section->{$class};

         $perl_code .= "package $class {\n\n";
         $perl_code .= "\tuse parent -norequire qw(@{$class_info->{s}});\n\n" if exists $class_info->{s};
         if (exists $class_info->{m}) {
            foreach my $method (@{$class_info->{m}}) {
               my $name = $method =~ s/@$//r;

               $perl_code .= "\tsub $name { say 'A::$name'; " . ( $method =~ /@$/ ? "'$class'->next::method(); }" : "; }" );
               $perl_code .= "\n\n";
               $perl_code .= "}\n\n";
            }
         }
      }
      push @$section_codes, [ $section, $perl_code ];
   }

   return $section_codes;
}

sub c3_linearize {
   my $section = shift;
   my %linearized;

   foreach my $class (sort keys %$section) {

   }
}

{
   local $/;
   my $sections = parse_class_defs_sections(<DATA>);
   say Dumper gen_perl_code($sections);
}

__DATA__
O : meth1 meth2 meth3
A -> (O) : meth2
B -> (O) : meth1
C -> (A B) : meth2
###
O
