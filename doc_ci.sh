#!/bin/bash

cd gh-pages/n2/doxygen/
git rm -rf html
cp -r ../../../code/n2/doc/html ./
git add html
git commit html -m "replacing old docs with new"

git push origin gh-pages


