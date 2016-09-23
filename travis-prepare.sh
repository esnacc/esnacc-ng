#!/bin/bash

if [ "${TRAVIS_OS_NAME}" == "osx" ]; then
    brew uninstall libtool && brew install libtool ||
            echo "Failed fixing libtool error"
fi
