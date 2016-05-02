#!/bin/bash

cd gh-pages/n2/doxygen/
git rm -rf *
cp -r ../../code/n2/doc/html ./
git add *
git commit * -m "replacing old docs with new"

git push origin gh-pages


