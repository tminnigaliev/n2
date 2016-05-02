#!/bin/bash

mkdir code
mkdir gh-pages

cd code
git clone https://github.com/tminnigaliev/n2.git
cd n2
git checkout code

cd ../..
cd gh-pages
git clone https://github.com/tminnigaliev/n2.git
cd n2
git checkout gh-pages

cd ../..

