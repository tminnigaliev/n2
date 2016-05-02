#!/bin/bash

mkdir tmp_doc
cd tmp_doc
cp ../gh-pages/n2/Doxyfile .
cp -r ../gh-pages/n2/src .
doxygen Doxyfile


