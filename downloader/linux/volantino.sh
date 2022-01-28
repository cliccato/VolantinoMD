#!/bin/bash

currentdate=`date +"%Y-%m-%d"`
url="https://mdspa2.cdn.hugecloud.eu/upload/presente/volantini/nord-p-g/volantino.pdf"
name="volantino".$currentdate".pdf"

curl $url -o $name -s

echo "Download eseguito"
