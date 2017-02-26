Hashr
=====

Single-thread hashtable daemon implementation for Linux/MacOS
- written in pure C, OOP architecture, only stdlib is used
- comes with tests, manual and some developer documentation
- client is available: CLI or single-shot command mode


Quick Tour
----------
Launch ./demo script (requires tmux):
- make will build the server, then
- server and client will start split-screen
- after exiting the client, tests will be built and run


Contents
--------

harvest/
  Useful material downloaded from the internet

doc/
  Developer documentation, requirements, planned command
  list, protocol design etc.

man/
  Manuals (plain text format):
  - protocol.txt: explanation of the client-sever 
    communication protocol, including example with hexdump
  - commands.txt: list of available commands, parameters,
    reply status codes
  - hash_methods.txt: list of available hash methods
  - error_codes: complete list of errors, with unique IDs

client/
  The CLI client written in Python3
  Usage:
    client.py [host [port]] [-c <command>]
    If no command is specified, CLI client will be started

server/
  Server main directory. Contains Makefile and some quick
  launch scripts

server/src/
  Source code of the server

server/test
  Source code of the tests. Tests are using catch.hpp 
  framework, included

server/bin/<platform>/
  Executable files. Excluded from version control
