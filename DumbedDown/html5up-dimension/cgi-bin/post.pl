#!/usr/bin/perl

print "<html>";
print "<head>";
print "<title>PERL POST</title>";
print "</head>";
print "<body>";
print "<h2>SERVER_SOFTWARE = $ENV{'SERVER_SOFTWARE'}</h2>";
print "<h2>SERVER_NAME = $ENV{'SERVER_NAME'}</h2>";
print "<h2>GATEWAY_INTERFACE = $ENV{'GATEWAY_INTERFACE'}</h2>";
print "<h2>SERVER_PROTOCOL = $ENV{'SERVER_PROTOCOL'}</h2>";
print "<h2>SERVER_PORT = $ENV{'SERVER_PORT'}</h2>";
print "<h2>REQUEST_METHOD = $ENV{'REQUEST_METHOD'}</h2>";
print "<h2>PATH_INFO = $ENV{'PATH_INFO'}</h2>";
print "<h2>PATH_TRANSLATED = $ENV{'PATH_TRANSLATED'}</h2>";
print "<h2>SCRIPT_NAME = $ENV{'SCRIPT_NAME'}</h2>";
print "<h2>QUERY_STRING = $ENV{'QUERY_STRING'}</h2>";
print "<h2>REMOTE_ADDR = $ENV{'REMOTE_ADDR'}</h2>";
print "<h2>CONTENT_TYPE = $ENV{'CONTENT_TYPE'}</h2>";
print "<h2>CONTENT_LENGTH = $ENV{'CONTENT_LENGTH'}</h2>";
print "</body>";
print "</html>";