#!/bin/sh
LIBRARY=shapelib
INSTALL_DIR=${HOME}/local/shapelib

Compile()
{
    ./configure --prefix=${INSTALL_DIR}
    make
}

Clean()
{
    make distclean
}

Install()
{
    make install
}

Uninstall()
{
    rm -rf ${INSTALL_DIR}
}

Usage ()
{
    echo "$0 [clean|install|uninstall|rebuild]"
}

cd ${LIBRARY}
if [ $# -eq 0 ]; then
    Compile
elif [ $# -eq 1 ]; then
    if [ "$1" = "clean" ]; then
	Clean
    elif [ "$1" = "install" ]; then
	Install
    elif [ "$1" = "uninstall" ]; then
	Uninstall
    elif [ "$1" = "rebuild" ]; then
	Clean
	Compile
    else
	Usage
    fi
else
    Usage
fi
cd -
