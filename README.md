mod_sid
============

Basic apache module to extract the sid (Session ID) field from the cookie field of an HTTP request and stores it to a file.
The module can be compiled with [APXS](https://httpd.apache.org/docs/2.4/programs/apxs.html) :

```
apxs -c mod_sid.c```
