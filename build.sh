#!/bin/sh

if [ "$1" = "rpm" ]; then
    # A very simplistic RPM build scenario
    if [ -e SourceVITA49.spec ]; then
        mydir=`dirname $0`
        tmpdir=`mktemp -d`
        cp -r ${mydir} ${tmpdir}/SourceVITA49-2.0.1
        tar czf ${tmpdir}/SourceVITA49-2.0.1.tar.gz --exclude=".svn" -C ${tmpdir} SourceVITA49-2.0.1
        rpmbuild -ta ${tmpdir}/SourceVITA49-2.0.1.tar.gz
        rm -rf $tmpdir
    else
        echo "Missing RPM spec file in" `pwd`
        exit 1
    fi
else
    for impl in cpp ; do
        cd $impl
        if [ -e build.sh ]; then
            ./build.sh $*
        elif [ -e reconf ]; then
            ./reconf && ./configure && make
        else
            echo "No build.sh found for $impl"
        fi
        cd -
    done
fi
