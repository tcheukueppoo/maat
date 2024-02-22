#!/usr/bin/env perl

# License: MAATL, see LICENSE file for copyright and license details.
#
# Test if c3 algorithm works. To do that we define the general structure of a small
# DSL for defining classes with their methods and inheritance relationships. This
# program parses the DSL following __DATA__ to perform c3 linearization and method
# call tests.
#
# Syntax of the DSL:
#
# ;
# ; (Above comment)
#
# CLASS_NAME [ '->' '(' SUPERCLASS_NAME ... ')' ] [ ':' METHOD_NAME[@] ... ]
# ...
#
# ;
# ; (Below comment)
# ;
#
# -------------------------------------------
#   (Line above is a section separator)
#   (Define another section here below)
#
#
# Example:
#
# O: meth1 meth2 meth3
# A -> (O) : meth2
# B -> (O) : meth1
# C -> (A B) : meth2@
# ------------------------------------------------
#
# The above example defines a section, sections are separated by '-'s. We test each
# section independently of the other ones. The optional '@' following a method name
# implies that in its code the method unconditionally calls its super method. A perl
# code is generated for each section and our own implementation which is proven to
# be totally valid is used to the validity of that of perl.

use strict;
use warnings;
use feature qw(say state current_sub postderef_qq signatures);

use Data::Dumper;
use List::Util qw(uniq);
use Test::More;

my $data_re = qr/
   \G
   (?(DEFINE) (?<c> \s* (?m: ; (?>.*) $ (?>\s*) )* ) )
   (?:
     (?:
       (?&c)                                            # Above comment
       ( (?: -(?!-) | [^-;] )+ )                        # Section, not mixed with comments
       (?&c)                                            # Below comment
     )
    | (?: --+ )                                         # Section separator
    | \z
   )                    
/x;

my $section_re = qr/
   \G
   (\w+)                                    # Class name
   (?:
     \h* -> \h*
     \( ( \w+ (?: (?>\h+) (?>\w+) )* )? \)  # Based classes
   )?
   (?:
     \h* : \h*
     (
      \w+ \@?                               # Methods
      (?: (?>\h+) (?>\w+) (?>\@?) )*
     )
   )?
   \s*? \n \s*                              # At least a newline!
/x;

# Report an error if there's a syntax error at a point in the dsl.
sub syntax_error ($data, $pos //= pos $$data, $offset //= 0) {
  my $start      = rindex($$data, "\n", $offset + $pos);
  my $error_line = substr($$data, $start, $offset + $pos - $start);

  die "Syntax error near --> '$error_line'.\n";
}

# Parse a section and return the result.
sub parse_section ($data, $section_str, $offset, $n) {
  my $section;

  while ($section_str =~ m/$section_re/gc) {

    die "[section $n]: already defined class '$1'\n" if exists $section->{$1};

    # Could just auto-vivify but we can have empty classes.
    $section->{$1} = {};
    $section->{$1}{m} = [split ' ', $3] if defined $3;

    next unless defined $2;

    my %defined;
    foreach my $sup (split ' ', $2) {
      die "[section $n]: Class '$1' can't inherit himself.\n" if $1 eq $sup;
      die "[section $n]: Class '$sup' is undefined.\n" unless exists $section->{$sup};
      die "[section $n]: Duplicate class '$sup'.\n" if exists $defined{$sup};

      $defined{$sup} = 1;
      push $section->{$1}{s}->@*, $sup;
    }
  }

  syntax_error($data, pos $section_str, $offset) if pos $section_str != length $section_str;

  return $section;
}

# Parse each section and return the result.
sub parse_sections ($data) {
  my $sections;
  my ($n, $offset) = (0, 0);

  while ($data =~ m/$data_re/gc) {
    defined $2 and push @$sections, parse_section(\$data, $2, $offset, $n++);
    $offset = pos $data;
  }

  syntax_error(\$data) if pos $data != length $data;

  return $sections;
}

# Generate and return Perl code for a section.
sub gen_perl_code ($section) {

  state $uses = <<~'EOF';
   use strict;
   use warnings;
   use mro "c3";
   EOF

  my $code = $uses;
  foreach my $class (sort keys %$section) {
    my $class_info = $section->{$class};

    $code .= "package $class {";
    if (exists $class_info->{s}) {
      $code .= exists $class_info->{m} ? "\n\t" : ' ';
      $code .= "use parent -norequire, qw(@{$class_info->{s}});";
    }

    $code .= " }\n" and next unless exists $class_info->{m};

    my $len = $class_info->{m}->@*;
    my $sep = $len > 1 || exists $class_info->{s} ? "\n" : ' ';

    for (my $i = 0; $i < $len; $i++) {
      my $method = $class_info->{m}[$i];
      my $name   = $method =~ s/[@]$//r;
      my $tab    = !exists $class_info->{s} && ($i - 1) < 0 && ($i + 1 == $len) ? '' : "\t";

      $code .= qq[${sep}${tab}sub $name { print "${class}::$name\\n"];
      $code .= '; $_[0]->next::method()' if $method =~ m/\@$/;
      $code .= ' }';
    }

    $code .= "$sep}\n";
  }

  return $code;
}

# Compute the C3 linearization of a section's class. The dynamic hash table $c3s keeps
# track of all c3 results of classes defined in the section of concern, set it to
# `undef' when entering a new section.
sub c3_linearize_class ($section, $class, $c3s) {
  $c3s->{$class} = [$class];

  # Has no superclass? probably the first class.
  return unless exists $section->{$class}{s};

  my ($supers, $i) = ($section->{$class}{s}, -1);

  # Just in case subroutine `test_c3' does not linearize classes in their definition order.
  while (++$i < @$supers) {
    __SUB__->($section, $supers->[$i], $c3s) unless exists $c3s->{$supers->[$i]};
    return                                   unless ref $c3s;
  }

  # Single inheritance? simply push.
  if (@$supers == 1) {
    push @{$c3s->{$class}}, $c3s->{$supers->[0]}->@*;
    return;
  }

  my ($prev_sol, $error);
  my $sub_sol = $c3s->{$supers->[0]};
  my %merged  = ($supers->[0] => 0);

  foreach my $c (1 .. $#$supers) {
    my $start     = 0;
    my $to_insert = $c3s->{$supers->[$c]};

    $prev_sol = $sub_sol;
    $sub_sol  = [];

  insertion: for (my $i = 0; $i < @$prev_sol; $i++) {
      for (my $j = $start; $j < @$to_insert; $j++) {

        # Mimic that particular perl error message
        if (exists $merged{$to_insert->[$j]}) {
          $c3s->{$class} = <<~"EOE";
               Inconsistent hierarchy during C3 merge of class $class:
                  current merge results
                  [ $class, @$supers[0 .. $merged{$to_insert->[$j]} - 1] ]
                  merging failed on $to_insert->[$j] at
               EOE
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
  push $c3s->{$class}->@*, @$sub_sol;
}

# For each class in a section, deduce all the possible method calls and directly bind
# these methods to that class.
sub resolve_methods ($section, $c3s) {
  my $rmeths;

  foreach my $class (sort keys %$section) {
    foreach my $sup ($c3s->{$class}->@*) {
      push $rmeths->{$class}{$_ =~ s/\@$//r}->@*, "${sup}::$_" foreach $section->{$sup}{m}->@*;
    }
  }

  return $rmeths;
}

# Fake the '$method' method call on object '$object'. Just mimicking perl error messages
# to help you grasp the abstraction easily.
sub call_method ($rmeths, $object, $meth) {
  return "Can't locate object method $meth via package $object at" unless exists $rmeths->{$object}{$meth};

  my @output;
  my $meth_list = $rmeths->{$object}{$meth};

  for (my $i = 0; $i < @$meth_list; $i++) {
    push @output, $meth_list->[$i] =~ s/(\@)$//r;

    last unless defined $1;
    push @output, "No next::method $meth found for $object at" if $i == $#$meth_list;
  }

  return @output;
}

sub perl_c3_linearize_class ($class, $code) {
  return $code . <<~"EOP";
   print qq[\@{mro::get_linear_isa("$class", "c3")}];
   EOP
}

sub perl_call_method ($class, $meth, $code) {
  return $code . "\n$class->$meth();";
}

# Run generated perl code and return its stdout + stderr
sub perl_run ($code) {
  local $/;

  open(my $fh, '-|', "perl -E '$code' 2>&1") or die "$!\n";
  return <$fh>;
}

sub chain (@call) {
  return join '->', map { m/^(?:No|Can)/ ? '_fail_' : $_ } @call;
}

sub test_call ($rmeths, $class, $meth, $code, $i) {
  my $result   = chain(split "\n", perl_run(perl_call_method($class, $meth, $code)));
  my $expected = chain(call_method($rmeths, $class, $meth));

  is($result, $expected, "[section $i] '$class'->$meth(): '$result' eq '$expected'?");
}

sub cmp_val ($val) {
  return lc $val =~ s/["',]//gr =~ s/\s+/ /gr;
}

# With certainty that our own c3 implementation works, let's test Perl's implementation
# and perform series of method calls to see if chained super calls resolve correctly.
sub test_test_test () {
  local $/;

  my $t        = 0;
  my $sections = parse_sections(<DATA>);
  my @codes    = map gen_perl_code($_), @$sections;

  local $" = ', ';
section: for (my $i = 0; $i < @$sections; $i++) {
    my $c3s     = {};
    my $section = $sections->[$i];

    # It's preferable you define classes in unicode order but it isn't mandatory.
    foreach my $class (sort keys %$section) {
      my $perlc3 = perl_run(perl_c3_linearize_class($class, $codes[$i]));

      $t++;
      c3_linearize_class($section, $class, $c3s);
      if (ref $c3s->{$class}) {
        my $expected = join ' ', $c3s->{$class}->@*;

        is($perlc3, $expected, "[section $i]: is c3 of '$class' [$c3s->{$class}->@*]?");
      }
      else {
        ($c3s->{$class}, $perlc3) = (cmp_val($c3s->{$class}), cmp_val($perlc3));
        ok($perlc3 =~ m/^\Q$c3s->{$class}\E/, "[section $i]: failed to find c3 of '$class'?");
        next section;
      }
    }

    # Test method calls
    my $rmeths = resolve_methods($section, $c3s);
    foreach my $class (sort keys %$section) {
      test_call($rmeths, $class, $_, $codes[$i], $i), $t++
        foreach keys $rmeths->{$class}->%*,
        grep { !exists $rmeths->{$class}{$_} } uniq map { map s/\@$//r, $section->{$_}{m}->@* } keys %$section;
    }
  }

  return $t;
}

done_testing(test_test_test());

__DATA__

;
; Class diagram for visual testing
;
 ;
 ;
 ;
 ;
 ;
 ;
 ;
 ;
 ;

A
B -> (A)
C -> (A)
D -> (C A B)

-------------------------------------------------------------------------------------------------

;
; Just an empty class.
;

A -> ()

 ----------------------------------------------------------------------------------------------

A

B -> (A)
C -> (A)

D -> (A B C)

------------------------------------------------------------------------------------------------

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

-----------------------------------------------------------------------------------------------------

A
B -> (A)
C -> (A)
D -> (A)
E -> (B)
F -> (E C D)
