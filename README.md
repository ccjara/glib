# glib

A set of tools to work with glib file archives which were used to store assets
of the game GALACTIX (MS-DOS) by Cygnus Software Inc.

## Specification
Files inside a glib archive are not encrypted or compressed. The archive layout
is a continuous sequence of file headers followed by the data of the
respective files, in the same order as the headers.

The name of a file stored inside a glib file header is fixed to 22 bytes and
zero-padded until its limit. A file cannot be larger than 2^16-1 bytes as the 
file size is stored in 2 bytes. No file inside any official archive was bigger
than 64000 bytes.

See [glib_defs](./lib/glib_defs.hxx) for more information.

## Building

This project is split into [backend](./lib) (glib implementation), 
[frontend](./src) (cli) and [tests](./test) (using Catch2).

You can use `cmake` to build the project. 

C++17 is required to build this project as it makes use of the 
`std::filesystem`.

