#!/bin/sh

curl localhost:3882/rest/block/$1.json > resources/test-resources/blocks/$2.json
curl localhost:3882/rest/block/$1.bin > resources/test-resources/blocks/$2.bin

