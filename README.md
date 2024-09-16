### Program option C-library
    My library for handling program option.

### How to use
When you compile the file example.c and type the following script:
```
$ ./example --help
```
then you get
```
Secure things
    rsa        Encode data
    genprime   Generate a prime

Bigint operator
    add        Add two bigint
    mul        Multiply two bigint

Startup:
-V,  --version                   display the version of Wget and exit
-h,  --help                      print this help
-b,  --background                go to background after startup
-e,  --execute=COMMAND           execute a `.wgetrc'-style command

Logging and input file:
-o,  --output-file=FILE          log messages to FILE
-a,  --append-output=FILE        append messages to FILE
-d,  --debug                     print lots of debug information
-q,  --quiet                     quiet (no output)
-v,  --verbose                   be verbose (default)
-nv,  --no-verbose                turn off verbose, without being quiet
    --report-speed=TYPE         output bandwidth as TYPE, TYPE can be bits

Download:
-t,  --tries=NUMBER              number of trials before failed
    --speed-limit=NUMBER        limit download spped
```
