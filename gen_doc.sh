#!/bin/bash

mkdir tmp_doc
cd tmp_doc
cp ../gh-pages/n2/Doxyfile .
cp -r ../gh-pages/n2/src .
doxygen Doxyfile

cd ../gh-pages/n2
git rm -rf *
cp -r ../../tmp_doc/* .
git add *
git commit -m "replacing old docs with new"

git push origin gh-pages


