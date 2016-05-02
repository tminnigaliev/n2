# n2

How to work with this:
1) git clone https://github.com/tminnigaliev/n2.git
2) ./co.sh  - restores source tree with the following paths:
  ./gh-pages/n2/doxygen/html/...    - online documentation generated by Doxygen
  ./code/n2/doc/Doxyfile            - Doxygen config file
  ./code/n2/doc/images/...          - images for online documentation
  ./code/n2/doc/src/...             - sources for online documentation
  ./code/n2/src/...                 - all files related to source code
  ./code/n2/ut/...                  - unit tests

  ./doc_gen.sh                      - script for generating online documentation to folder ./code/n2/doc/html/...
  ./doc_ci.sh                       - script for committing documentation to gh-pages branch of github  (documentation will be visible from here: http://tminnigaliev.github.io/n2/doxygen/html/index.html )


