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
# ;
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
# Sections are separated by '-'s. We test each section independently of the other
# ones. The optional '@' following a method name implies that in its code the
# method unconditionally calls its super method. A perl code is generated for each
# section and our own implementation which is proven to be totally valid is used
# to the validity of that of perl.

use strict;
use warnings;
use v5.36;

use builtin qw( indexed true false );

no warnings 'experimental';

use Getopt::Std;
use List::Util qw( uniq );
use Test::More;

use Data::Dumper;

my $data_re = qr/
   \G
   (?(DEFINE) (?<c> \s* (?m: ; (?>.*) $ (?>\s*) )* ) )  # Comment RE
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
   (?(DEFINE) (?<i> ))                      # Identifier
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

my $meth_re = qr/^ ( [^@] ) ( \@ )? $/x;

my $config = {lang => 'perl', secs => undef, action => 'test', nsecs => false};

sub main () {
  parse_args();

  # Output the number of sections.
  if ($config->{nsecs}) {
     get_n_sections();
     exit 0;
  }

  for ($config->{actions}) {
     done_testing(test_c3_mro()), last if /test/;
     display_lang_scode(),        last if /gen/;
     display_sections(),          last if /secs/
     display_class_diag(),        last if /diag/;
  }
}

sub parse_args () {
  my $help = <<"EOH";
A c3 method resolution order (MRO) test Perl script. When this script
is simply invoked with no arguments, it performs a series c3-mro tests
on all sections foind in the script.

Usage: $0 [-hn] [ -l LANG ] [ -a ACTION ] [ -s ID, ... ]

 -h           Display this help message and exit.
 -n           Display the number of sections in the script.
 -s ID, ...   A Comma separated list of IDs of sections to operate
              on, it defaults to all the sections in the script.
 -l LANG      The name of the programming language to test c3-mro.
              LANG can either be "perl", "raku", and "python".
 -a ACTION    Specify what the script does, *ACTION* can either be:
               - "diag", display for each section its class
                 relationship diagram.
               - "test", this is the default action, it does c3
                 mro tests on the specified section(s).
               - "lang", to output in the specified programming
                 language the code of the specified sections.
               - "sec", to output the sections themselves.
EOH

  my %args;
  getopts('hnl:a:s:', \%args);

  # Process arguments.
  die "$help\n"           if defined $args{h};
  $config->{nsecs} = true if defined $args{n};

  if (defined $args{l}) {
    die "supported arguments to `-l' are perl/raku/python.\n" unless $args{s} =~ /^(?:perl|raku|python)$/i;
    $config->{lang} = lc $args{l};
  }

  if (defined $args{s}) {
    die "invalid argument to `-s'.\n" unless $arg{s} =~ /\d+(?:(?>,+)(?>\d+))*/a;
    @{$config->{secs}} = map { $_ => 1 } split /,+/, $args{s};
  }

  if (defined $args{a}) {
    die "use `-h' for more info.\n" unless $args{a} =~ /^(?:diag|test|gen|lang|sec)$/i;
    $config->{action} = $args{a};
  }
}

# Get the number of sections under __DATA__.
sub get_n_sections () {
  return scalar parse_sections()->@*;
}

sub display_lang_scode () {
  my $sections = parse_sections();

  foreach my $i (@{$config->{secs} // [0..$#$sections]}) {
     warn "no section with id $i\n" unless 0 <= $i < @$sections;

     print <<"MARK" . gen_lang_scode($sections->[$i]);
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> section $i
MARK
  }
}

sub display_sections () {
   my $sections = parse_sections();

   foreach my $i (@{$config->{secs} // [0..$#$sections]}) {
     warn "no section with id $i\n" unless 0 <= $i < @$sections;

     my $section = $sections->[$i];
     print <<"MARK"
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> section $i
MARK
     foreach my $class (@{$section->[0]}) {
       print "$class";
       print " -> ($section->[1]{s}->@*)" if exists $section->[1]{s};
       print " : $section->[1]{m}->@*" if exists $section->[1]{m};
       print "\n";
     }
   }
}

sub display_class_diag () {}

# With certainty that our own c3 implementation works, let's test Perl's
# implementation and perform series of method calls to see if chained
# super calls resolve correctly.
sub test_c3_mro () {
  my $t        = 0;
  my $sections = parse_sections();
  my @codes    = map gen_lang_scode($_), @$sections;

  local $" = ', ';
section: foreach my $i (@{$config->{secs} // [0..$#$sections]}) {
    warn "no section with id $i\n" unless 0 <= $i < @$sections;

    my $c3s = {};
    my ($classes, $defs) = @{$sections->[$i]};

    foreach my $class (@$classes) {
      $t++;

      # Test C3 linearization.
      my $langc3 = lang_run(lang_c3_linearize_class($class, $codes[$i]));
      c3_linearize_class($section, $class, $c3s);

      if (ref $c3s->{$class}) {
        my $expected = join ' ', @{$c3s->{$class}};

        is($langc3, $expected, "[section $i]: is c3 of '$class' [@{$c3s->{$class}}]?");
      }
      else {
        ($c3s->{$class}, $langc3) = (cmp_val($c3s->{$class}), cmp_val($langc3));
        ok($langc3 =~ /\Q$c3s->{$class}\E/, "[section $i]: failed to find c3 of '$class'?");
        next section;
      }
    }

    # Test method calls.
    my $rmeths = resolve_methods($section, $c3s);
    foreach my $class (@$classes) {
      next unless my $meths = $rmeths->{$class};
      test_call($rmeths, $class, $_, $codes[$i], $i), $t++ foreach keys %$meths, '__should_fail__';
    }
  }

  return $t;
}

sub cmp_val ($val) {
  return lc $val =~ s/["',]//gr =~ s/\s+//gr;
}

# Parse each section and return the result.
sub parse_sections () {
  my $sections;

  local $/;
  my $data = <DATA>;

  my ($n, $offset) = (0, 0);
  while ($data =~ /$data_re/gc) {
    defined $2 and push @$sections, parse_section(\$data, $2, $offset, $n++);
    $offset = pos $data;
  }

  syntax_error(\$data) if pos $data != length $data;

  return $sections;
}

# Parse a section and return the result.
sub parse_section ($data, $section_str, $offset, $n) {

  # 2 dim array containing the list of parsed classes names and the parsed result.
  my $section;

  while ($section_str =~ /$section_re/gc) {
    die "[section $n]: already defined class '$1'\n" if exists $section->[1]{$1};

    push @{$section->[0]}, $1;

    # Could use auto-vivification but we've empty classes.
    $section->[1]{$1} = {};

    push $section->[1]{$1}{m}->@*, split ' ', $3 if defined $3;

    next unless defined $2;

    my %defined;
    foreach my $super (split ' ', $2) {
      die "[section $n]: Class '$1' can't inherit himself.\n" if $1 eq $super;
      die "[section $n]: Class '$super' is undefined.\n" unless exists $section->[1]{$super};
      die "[section $n]: '$super' is a duplicated class.\n" if exists $defined{$super};

      $defined{$super} = 1;
      push $section->[1]{$1}{s}->@*, $super;
    }
  }

  syntax_error($data, pos $section_str, $offset) if pos $section_str != length $section_str;
  return $section;
}

# Report a syntax error at a point in the dsl.
sub syntax_error ($data, $pos //= pos $$data, $offset //= 0) {
  my $start      = rindex($$data, "\n", $offset + $pos);
  my $error_line = (split(/\R/, substr($$data, $start, $offset + $pos - $start)))[0];

  die "Syntax error near --> '$error_line'.\n";
}

# Generate the Perl/Raku/python code for the given section.
sub gen_lang_scode ($section) {
  local $_ = $config->{lang};
  return /perl/ ? gen_perl_scode($section) : /raku/ ? gen_raku_scode($section) : gen_python_scode($section);
}

# Generate the Perl code for the given section.
sub gen_perl_scode ($section) {
  state $uses = <<~'EOF';
     use strict;
     use warnings;
     use mro "c3";
     EOF

  my $code = $uses;

  foreach my $class (@{$section->[0]}) {
    my $classdef = $section->[1]{$class};

    # Class defintion
    $code = "package $class {";

    # Superclasses.
    if (exists $classdef->{s}) {
      $code .= exists $classdef->{m} ? "\n " : '';
      $code .= "use parent -norequire, qw(@{$classdef->{s}});";
    }

    # Move on to the next class if this one has no methods.
    $code .= " }\n" and next unless exists $classdef->{m};

    # Methods.
    my $delim  = @{$classdef->{m}} > 1 || exists $classdef->{s} ? "\n" : ' ';
    my $indent = $delim eq ' '                                  ? ''   : $delim;
    foreach my $meth (@{$classdef->{m}}) {
      my ($name, $next) = $meth =~ $meth_re;

      $code .= qq/${delim}${indent}sub $name { say "${class}::$name"/;
      $code .= '; $_[0]->next::method()' if defined $next;
      $code .= ' }';
    }

    $code .= "$delim}\n";
  }

  return $code;
}

# Generate the Raku code for the given section.
sub gen_raku_scode ($section) {
  my $code;

  foreach my $class (@{$section->[0]}) {
    my $classdef = $section->[1]{$class};

    # Class definition.
    $code .= "class $class";

    # Superclasses.
    $code .= ' is' . join('is ', @{$classdef->{s}}) if exists $classdef->{s};
    $code .= ' {';

    # Move on to the next class if this one has no methods.
    $code .= " }\n" and next unless exists $classdef->{m};

    # Methods.
    my $delim  = @{$classdef->{m}} > 1 || exists $classdef->{s} ? "\n" : ' ';
    my $indent = $delim eq ' '                                  ? ''   : $delim;
    foreach my $meth (@{$classdef->{m}}) {
      my ($name, $next) = $meth =~ $meth_re;

      $code .= qq/${delim}${indent}method $name { say "${class}::$name"/;
      $code .= '; nextsame' if defined $next;
      $code .= ' }';
    }

    $code .= "$delim}\n";
  }

  return $code;
}

# Generate the Python code for the given section.
sub gen_python_scode ($section) {
  my $code;

  foreach my $class (@{$section->[0]}) {
    my $classdef = $section->[1]{$class};

    # Class definition.
    $code .= "class $class(";

    # Superclasses.
    $code .= join(', ', @{$classdef->{s} // []}) . "):\n\t";

    # Move on to the next class if this one has no methods.
    $code .= "pass\n" and next unless exists $classdef->{m};

    # Methods.
    foreach my $meth (@{$classdef->{m}}) {
      my ($name, $next) = $meth =~ $meth_re;

      $code .= qq/def $name(self):\n\t\tprint("${class}::$name" end = "")\n/;
      $code .= "\t\tsuper().$name()\n" if defined $next;
    }
  }

  return $code;
}

# Compute the C3 linearization of a section's class. $c3s keeps track
# of all c3 results of classes defined in the section of concern.
sub c3_linearize_class ($section, $class, $c3s) {
  $c3s->{$class} = [$class];

  # Has no superclass? probably the first class.
  return unless my $supers = $section->[1]{$class}{s};

  # Single inheritance? simply push.
  if (@$supers == 1) {
    push @{$c3s->{$class}}, $c3s->{$supers->[0]}->@*;
    return;
  }

  state $error = {
    python => "TypeError: Cannot create a consistent method resolution order (MRO) for bases %s",
    raku   => "Could not build C3 linearization: ambiguous hierarchy at",
    perl   => <<~'EOP',
               Inconsistent hierarchy during C3 merge of class %s: current merge results
               [ %s, %s ] merging failed on %s at,
              EOP
  };

  my $prev_sol;
  my $sub_sol = $c3s->{$supers->[0]};
  my %merged  = ($supers->[0] => 0);

MERGE: foreach my $s (1 .. $#$supers) {
    my $start     = 0;
    my $super     = $supers->[$s];
    my $to_insert = $c3s->{$super};

    $prev_sol = $sub_sol;
    $sub_sol  = [];

  INSERT: foreach my ($i, $prev) (indexed @$prev_sol) {
      foreach my ($j, $ins) (indexed @$to_insert) {

        if (exists $merged{$ins}) {

          # Detected an inconsistent hierarchy.
          $c3s->{$class} = do {
            local $_ = $config->{lang};

                /perl/ ? sprintf($error->{perl}, $class, $class, join(' ', @$supers[0 .. $merged{$ins} - 1]), $ins)
              : /raku/ ? $error->{raku}
              :          sprintf($error->{python}, join ', ', @$supers);
          };
          last MERGE;
        }

        # Insert '$ins' with safe-to-insert classes encountered since the '$start'
        if ($prev eq $ins) {
          push @$sub_sol, $to_insert->@[$start .. $j];
          $start = $j + 1;
          next INSERT;
        }
      }

      # Safe to insert '$prev' as heirachy is consistent.
      push @$sub_sol, $prev;
    }

    # To detect inconsistency.
    $merged{$super} = 1;
  }

  # Failed to linearize.
  return unless ref $c3s->{$class};

  # At the end '$sub_sol' gets the merged result.
  push @{$c3s->{$class}}, @$sub_sol;
}

sub lang_c3_linearize_class ($class, $code) {
  my $getc3l;

  for ($config->{lang}) {
    $getc3l = qq/print qq[\@{mro::get_linear_isa("$class", "c3")}];/,                         last if /perl/;
    $getc3l = qq/print $class.^mro».HOW».name.join(" ");/,                                    last if /raku/;
    $getc3l = qq/print(" ".join(map(lambda m: m.__name__, $class.__mro__[0:-1])), end = "")/, last if /python/;
  }

  return $code . "\n$getc3l\n";
}

# For each class in a section, deduce all the possible method calls and
# directly bind these methods to their respective classes.
sub resolve_methods ($section, $c3s) {
  my $rmeths;

  foreach my $class (@{$section->[0]}) {
    foreach my $elem (@{$c3s->{$class}}) {
      foreach my $meth ($section->[1]{$elem}{m}->@*) {
        push $rmeths->{$class}{$elem =~ s/\@$//r}->@*, "${elem}::$meth";
      }
    }
  }

  return $rmeths;
}

sub test_call ($rmeths, $class, $meth, $code, $i) {
  my $result   = join '->', split "\n", lang_run(lang_call_method($class, $meth, $code));
  my $expected = join '->', call_method($rmeths, $class, $meth);

  ok($result =~ /\Q$expected\E/, "[section $i] '$class'->$meth(): '$result' matches '$expected'?");
}

sub lang_call_method ($class, $meth, $code) {
  local $_ = $config->{lang};
  my $meth_call = /perl/ ? "$class->$meth();" : /raku/ ? "$class.new.$meth();" : "$class().$meth()";

  return $code . "\n$meth_call\n";
}

# Run the given `$code' and return its stdout + stderr
sub lang_run ($code) {
  local $_ = $config->{lang};

  my $exec_opt = /perl|raku/ ? 'e' : 'c';
  open(my $fh, '-|', "$config->{lang} -$exec_opt '$code' 2>&1") or die "$!\n";

  local $/;
  my $output = <$fh>;

  $output =~ s/^Trackback.*?(?=^(?:[A-Z](?>[a-z]+))+:)//ma if /python/;
  return $output;
}

# Fake the '$method' method call on object '$object', mimicking language
# error messages to help you grasp the abstraction easily.
sub call_method ($rmeths, $object, $meth) {
  local $_ = $config->{lang};

  return
      /perl/ ? "Can't locate object method $meth via package $object at"
    : /raku/ ? "No such method '$meth' for invocant of type '$object'."
    : "AttributeError: '$object' object has no attribute '$meth'"
    unless my $meth_list = $rmeths->{$object}{$meth};

  my @output;
  foreach my ($i, $meth) (indexed @$meth_list) {
    push @output, $meth =~ s/(\@)$//r;

    last unless defined $1;
    push @output,
        /perl/ ? "No next::method $meth found for $object at"
      : /raku/ ? "No next method"
      : "AttributeError: 'super' object has no attribute '$meth'"
      if $i == $#$meth_list;
  }

  return @output;
}

main();

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

; my $tab = !exists $classdef->{s} && ($i - 1) < 0 && ($i + 1 == @{$classdef->{m}}) ? '' : "\t";
