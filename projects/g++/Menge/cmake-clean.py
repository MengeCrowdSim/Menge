#!/usr/bin/env python
#
# Copyright (c) 2012-2012 Tom Schutter
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
#    - Redistributions of source code must retain the above copyright
#      notice, this list of conditions and the following disclaimer.
#    - Redistributions in binary form must reproduce the above
#      copyright notice, this list of conditions and the following
#      disclaimer in the documentation and/or other materials provided
#      with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
# FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
# COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
# ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#

"""
Clean cmake generated files.
"""

import optparse
import os
import shutil
import subprocess
import sys

# Do not cleanup anything in these subdirectories.
PRUNE_DIRS = [".svn", ".git", "CVS"]


def make_clean(directory):
    """Run 'make clean' in directory.

    Arguments:
    - `directory`: target directory
    """
    args = [
        "make",
        "--directory=%s" % directory,
        "--quiet",
        "clean"
    ]
    process = subprocess.Popen(args)
    return process.wait()


def clean(
    directory
    ):
    """Clean cmake files.

    Arguments:
    - `directory`: target directory
    """

    # Toplevel files.
    for filename in [
        "CMakeCache.txt",
        "CPackConfig.cmake",
        "CPackSourceConfig.cmake",
        "install_manifest.txt"
    ]:
        pathname = os.path.join(directory, filename)
        if os.path.exists(pathname):
            os.remove(pathname)

    # Toplevel directories.
    for dirname in ["_CPack_Packages"]:
        pathname = os.path.join(directory, dirname)
        if os.path.exists(pathname):
            shutil.rmtree(pathname)

    # CMakeFiles, Makefile, cmake_install.cmake.
    for dirpath, dirnames, filenames in os.walk(directory):
        # Prune subdirs.
        for dirname in dirnames:
            if dirname in PRUNE_DIRS:
                dirnames.remove(dirname)

        if "CMakeFiles" in dirnames:
            for filename in ["Makefile", "cmake_install.cmake"]:
                if filename in filenames:
                    pathname = os.path.join(dirpath, filename)
                    if os.path.exists(pathname):
                        os.remove(pathname)
            shutil.rmtree(os.path.join(dirpath, "CMakeFiles"))
            dirnames.remove("CMakeFiles")

    # Remove empty directories.  The "repeat" construct is needed
    # because the dirnames list for the parent is generated before the
    # parent is processed.  When a directory is removed, there is no
    # way to remove it from the parent's dirnames list.  Note that
    # setting topdown=False will not help here, and it complicates the
    # pruning logic.
    repeat = True
    while repeat:
        repeat = False
        for dirpath, dirnames, filenames in os.walk(directory):
            # We must check for emptiness before pruning.  Otherwise
            # we could try to remove a directory that contains only
            # prunable subdirs.
            if len(dirnames) == 0 and len(filenames) == 0:
                os.rmdir(dirpath)
                repeat = True

            # Prune subdirs.
            for dirname in dirnames:
                if dirname in PRUNE_DIRS:
                    dirnames.remove(dirname)


def main():
    """main"""
    option_parser = optparse.OptionParser(
        usage="usage: %prog [DIR...]\n" +
            "  Clean cmake generated files."
    )

    (_, args) = option_parser.parse_args()
    if len(args) == 0:
        args.append(".")

    for arg in args:
        make_clean(arg)
        clean(arg)

    return 0

if __name__ == "__main__":
    sys.exit(main())
