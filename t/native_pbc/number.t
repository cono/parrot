#! perl -w

use Parrot::Test tests => 2;
use Test::More;

output_is(<<CODE, <<OUTPUT, "i386 double float 32 bit int");
 # number_1.pbc
CODE
1.000000
4.000000
16.000000
64.000000
256.000000
1024.000000
4096.000000
16384.000000
65536.000000
262144.000000
1048576.000000
4194304.000000
16777216.000000
67108864.000000
268435456.000000
1073741824.000000
4294967296.000000
17179869184.000000
68719476736.000000
274877906944.000000
1099511627776.000000
4398046511104.000000
17592186044416.000000
70368744177664.000000
281474976710656.000000
1125899906842620.000000
OUTPUT

output_is(<<CODE, <<OUTPUT, "i386 long double float 32 bit int");
 # number_2.pbc
CODE
1.000000
4.000000
16.000000
64.000000
256.000000
1024.000000
4096.000000
16384.000000
65536.000000
262144.000000
1048576.000000
4194304.000000
16777216.000000
67108864.000000
268435456.000000
1073741824.000000
4294967296.000000
17179869184.000000
68719476736.000000
274877906944.000000
1099511627776.000000
4398046511104.000000
17592186044416.000000
70368744177664.000000
281474976710656.000000
1125899906842620.000000
OUTPUT

