# Copyright: 2005 The Perl Foundation.  All Rights Reserved.
# $Id: cc_shared.pl $

=head1 NAME

config/inter/shlibs.pm - Flags for shared libraries.

=head1 DESCRIPTION

Asks the user which flags are needed for compiling position-independent
code for use in shared libraries.  Eventually, other
shared-library-related prompts may end up here.

This is a separate unit from config/inter/progs.pm because the answers
depend on which compiler is in use.  Thus it should come after the
gccversion test.

=cut

package inter::shlibs;

use strict;
use vars qw($description $result @args);

use base qw(Parrot::Configure::Step::Base);

use Parrot::Configure::Step ':inter';

$description = 'Determining flags for building shared libraries...';

@args = qw(ask verbose cc_shared);

sub runstep {
    my ($self, $conf) = (shift, shift);
  my ($ask, $verbose, $cc_shared) = @_;
    $cc_shared = integrate($conf->data->get('cc_shared'), $cc_shared);
    $cc_shared=prompt(
	"\nWhat flags instruct your compiler to compile code suitable for use in a shared library?",
	$cc_shared) if $ask;
    $conf->data->set(cc_shared =>  $cc_shared);
    $result =
        ( $cc_shared =~ m/^ ?$/ ) ? 'done' : $cc_shared;
}
1;
