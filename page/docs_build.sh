#!/bin/sh

templ generate
go run .

mv static ../docs
