#!/bin/bash

cd ../gh-pages/n2
git rm -rf *
cp -r ../../tmp_doc/* .
git add *
git commit -m "replacing old docs with new"

git push origin gh-pages


