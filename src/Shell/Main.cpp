/*
 * Main.cpp
 * 
 * This file is part of the XShaderCompiler project (Copyright (c) 2014-2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include <Xsc/Xsc.h>
#include "Shell.h"
#include <iostream>

using namespace Xsc;
using namespace Xsc::Util;


int main(int argc, char** argv)
{
    Shell shell(std::cout);
    CommandLine cmdLine(argc - 1, argv + 1);
    shell.ExecuteCommandLine(cmdLine);
    return 0;
}
