#!/bin/sh

templ generate
go run .

rm -rf ../docs
mkdir -p ../docs
cp -r static/* ../docs/
rm -rf static
