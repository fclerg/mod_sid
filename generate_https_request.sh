#!/bin/bash

# This script allows to test the apache module.
# It sends an HTTPS GET request with the SID given as argument.

DEST_IP=localhost
DEST_PORT=443

# PATH of the URL
URL_PATH="/sid_counter.ck"


if [[ $# -eq 0 ]] ; then
    echo 'specify a sid. eg. : "./generate_request 3JOlJhyecqtG3qUpTm0uhPi1IX_WXV66CUq_e1F3sN8uT9NzvFsiBjOuk5DKyBA3ZtlL-4G2gl00UIuu"'
    exit 1
fi

SID="$1"


head='GET '"${URL_PATH}"' HTTP/1.1\r\nHost: localhost\nConnection: keep-alive\nAccept: application/json, text/javascript, */*; q=0.01\nX-Requested-With: XMLHttpRequest\nUser-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/50.0.2661.75 Safari/537.36\nAccept-Encoding: gzip, deflate, sdch\nAccept-Language: en-GB,en-US;q=0.8,en;q=0.6\nCookie: SID='"${SID}"';\r\n\r\n'; echo -ne $head | openssl s_client -quiet -connect $DEST_IP:$DEST_PORT
