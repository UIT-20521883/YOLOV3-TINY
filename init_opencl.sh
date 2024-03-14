#!/bin/bash
# This script updates PATH and LD_LIBRARY_PATH to add SDK's and board's libraries

# Make sure the script is being source'd, not executed.
# Otherwise, environment variables set here will not stick.
if [ "${BASH_SOURCE[0]}" == "$0" ]; then
  echo "Proper usage: source init_opencl.sh"
  exit 1
fi

if [ "$INTELFPGAOCLSDKROOT" == "" ]; then
  echo "INTELFPGAOCLSDKROOT is not set"
  export INTELFPGAOCLSDKROOT=$(cd "$(dirname "${BASH_SOURCE[0]}")"; pwd)
  echo "Using script's current directory ($INTELFPGAOCLSDKROOT)";
else
  echo "INTELFPGAOCLSDKROOT is set to $INTELFPGAOCLSDKROOT. Using that."
fi
echo

if [ ! -d "$INTELFPGAOCLSDKROOT" ]; then
  echo "Error: INTELFPGAOCLSDKROOT is set but is not a directory!"
  return 1
fi

# Detect if Full SDK is installed or the RTE
if [ ! -f "$INTELFPGAOCLSDKROOT/bin/aoc" ] && [ -f "$INTELFPGAOCLSDKROOT/bin/aocl" ]; then
  echo "aoc was not found, but aocl was found. Assuming only RTE is installed."
  RTE_ONLY=1
else
  RTE_ONLY=0
fi

# Detect the operating environment
# The same code as in aocl
ARCH=$AOCL_ARCH_OVERRIDE
if [ -z "$ARCH" ]; then
   ARCH=`uname -m`
fi
arch_type=
case $ARCH in
   x86_64) 
      arch_type=linux64
      arch_type_name="Linux x86-64"
      ;;
   armv7l) 
      arch_type=arm32
      arch_type_name="Linux SoC"
      ;;
   ppc64le)
      arch_type=ppc64le
      arch_type_name="Linux Power Little Endian"
      ;;
   *)
      echo "The '$ARCH' environment is not supported by the Intel(R) FPGA SDK for OpenCL(TM)"
      exit 1;
      ;;
esac

# Add to path if not already there
pathadd() {
  echo "Adding $1 to PATH"
  if [ -d "$1" ] && [[ ":$PATH:" != *":$1:"* ]]; then
    export PATH="$1:$PATH"
  fi
}
# Add to ld_library _path if not already there
ldpathadd() {
  echo "Adding $1 to LD_LIBRARY_PATH"
  if [ -d "$1" ]; then
    if [[ ":$LD_LIBRARY_PATH:" != *":$1:"* ]]; then
      # For non-empty, only add if not already there
      export LD_LIBRARY_PATH="$1:$LD_LIBRARY_PATH"
    fi
  fi
}

# Check for quartus the same way our entry wrappers do
if [ $RTE_ONLY == 0 ]; then
  if [ -d "$QUARTUS_ROOTDIR_OVERRIDE" ]; then
    echo "Will use \$QUARTUS_ROOTDIR_OVERRIDE=" $QUARTUS_ROOTDIR_OVERRIDE " to find Quartus"
  else # Check the peer directories for quartus
    if [ -d "$INTELFPGAOCLSDKROOT/../quartus" ] || [ -d "$INTELFPGAOCLSDKROOT/../../quartus" ]; then
      if [ -d "$INTELFPGAOCLSDKROOT/../quartus" ]; then
        export QUARTUS_ROOTDIR_OVERRIDE=$(cd "$INTELFPGAOCLSDKROOT/../quartus"; pwd)
      else
        export QUARTUS_ROOTDIR_OVERRIDE=$(cd "$INTELFPGAOCLSDKROOT/../../quartus"; pwd)
      fi
        echo "Found a Quartus directory at $QUARTUS_ROOTDIR_OVERRIDE. Using that."
    else # Check for the next environment variable
      if [ -d "$QUARTUS_ROOTDIR" ]; then
        echo "Will use \$QUARTUS_ROOTDIR=" $QUARTUS_ROOTDIR " to find Quartus"
      else
        #Is Quartus on the path?
        q_absolute=`which quartus_sh 2>/dev/null`;

        if [ "$q_absolute" == "" ]; then
            echo "Error: Quartus (quartus_sh) is not on the path!";
            return;
        else
            echo "Found Quartus at" `dirname $q_absolute` " will be using that";
        fi
      fi
    fi
  fi
fi

echo
AOCL_BOARD_PACKAGE_ROOT="/home/martin/intelFPGA_pro/18.1/hld/board/a10soc"

if [ "$AOCL_BOARD_PACKAGE_ROOT" != "" ]; then
  echo "AOCL_BOARD_PACKAGE_ROOT is set to $AOCL_BOARD_PACKAGE_ROOT. Using that."
else
  echo "AOCL_BOARD_PACKAGE_ROOT path is not set in environment."
  echo "Setting to default a10_ref board."
  echo "If you want to target another board, do "
  echo "   export AOCL_BOARD_PACKAGE_ROOT=<board_pkg_dir>"
  echo "and re-run this script"
  export AOCL_BOARD_PACKAGE_ROOT="/home/martin/intelFPGA_pro/18.1/hld/board/a10soc"
fi

pathadd "$INTELFPGAOCLSDKROOT/bin"
ldpathadd "$INTELFPGAOCLSDKROOT/host/$arch_type/lib"
ldpathadd "$AOCL_BOARD_PACKAGE_ROOT/$arch_type/lib"
export AOCL_BOARD_PACKAGE_ROOT="/home/martin/intelFPGA_pro/18.1/hld/board/a10soc"
#Is aoc (or aocl) now on the path?
if [ $RTE_ONLY == 0 ]; then
  a_absolute=`which aoc 2>/dev/null`;
  if [ "$a_absolute" == "" ]; then
      echo "Error: Cannot not find aoc. Please make sure this script is located in the SDK's root directory and run the script";
      return;
  fi
else
  a_absolute=`which aocl 2>/dev/null`;
  if [ "$a_absolute" == "" ]; then
      echo "Error: Cannot not find aocl. Please make sure this script is located in the RTE's root directory and run the script";
      return;
  fi
fi

unset RTE_ONLY
