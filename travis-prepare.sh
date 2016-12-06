#!/bin/bash

if [ "${TRAVIS_OS_NAME}" == "osx" ]; then
    brew install docbook docbook-xsl docbook2x
    brew uninstall libtool && brew install libtool ||
            echo "Failed fixing libtool error"
fi
