#-------------------------------------------------------------------
# FLASH makefile definitions for ALCF (argonne) production BG/P (intrepid)
#  started from seaborg (NERSC) makefile
#
# Changes made by Lynn 3/4/2009 to reflect Chris's use of O4 
#  Use -test with setup line to get highly optimized version
#
#-------------------------------------------------------------------

#----------------------------------------------------------------------------
# Set the HDF/HDF5 library paths -- these need to be updated for your system
#----------------------------------------------------------------------------

#HDF5_PATH = /soft/apps/hdf5-1.6.7
HDF5_PATH = /homes/tpeterka/hdf5-install
ZLIB_PATH =

HPM_PATH =
PNG_PATH = 

NCMPI_PATH = 
MPI_PATH = /usr

#----------------------------------------------------------------------------
# Compiler and linker commands
#
#  We use the f90 compiler as the linker, so some C libraries may explicitly
#  need to be added into the link line.
#----------------------------------------------------------------------------

ifdef PDTDIR
OPT=-optPDBFile=merged.pdb -optTauSelectFile="select.tau" -optReset="" -optVerbose
else
#LIB_MPI = -L/$(MPI_PATH)/lib -lmpich.rts -lmsglayer.rts -ldevices.rts -lrts.rts -ldevices.rts
LIB_MPI = 
endif

FCOMP   = mpif90
CCOMP   = mpicc -I${MPI_PATH}/include
CPPCOMP = mpif90 -I${MPI_PATH}/include
LINK    = mpif90

#CONFIG_LIB = -L/bgl/BlueLight/ppcfloor/bglsys/lib -lmpich.rts -lmsglayer.rts -ldevices.rts -lrts.rts -ldevices.rts
#-----------------------------------------------------------------------------
# Compilation flags
#
#  Three sets of compilation/linking flags are defined: one for optimized code
#  code ("-opt"), one for debugging ("-debug"), and one for testing ("-test").
#  Passing these flags to the setup script will cause the value associated with
#  the corresponding keys (i.e. those ending in "_OPT", "_DEBUG", or "_TEST") to
#  be incorporated into the final Makefile. For example, passing "-opt" to the
#  setup script will cause the flags following "FFLAGS_OPT" to be assigned to
#  "FFLAGS" in the final Makefile. If none of these flags are passed, the default
#  behavior will match that of the "-opt" flag.
#  In general, "-opt" is meant to optimize compilation and linking. "-debug"
#  should enable runtime bounds checking, debugger symbols, and other compiler-
#  specific debugging options. "-test" is useful for testing different
#  combinations of compiler flags particular to your individual system.
#----------------------------------------------------------------------------
               
FFLAGS_OPT   = -L${HDF5_PATH}/lib -lhdf5 -g -c -O3 -fdefault-real-8 -DH5_USE_16_API

               
               
FFLAGS_TEST  = -g -O4 -qintsize=4 -qrealsize=8 -qfixed -qnosave -c \
               -qinline -qmaxmem=16384 \
               -qsuffix=cpp=F -qarch=450 -qtune=auto -qcache=auto 
              
FFLAGS_DEBUG = -O -g -qintsize=4 -qrealsize=8 -qfixed -qnosave -c \
               -qarch=450 -qmaxmem=16384

F90FLAGS     =
f90FLAGS     =

# if we are using HDF5, we need to specify the path to the include files

CFLAGS_OPT   = -L${HDF5_PATH}/lib -lhdf5 -O3 -D_LARGEFILE64_SOURCE -c -DH5_USE_16_API
CFLAGS_TEST  = -g -O4 -DNOUNDERSCORE -c \
               -qarch=450 -qtune=auto -qcache=auto -qmaxmem=16384 -D_FILE_OFFSET_BITS=64
CFLAGS_DEBUG = -g  -DNOUNDERSCORE -c \
               -qarch=450 -qmaxmem=16384 -D_FILE_OFFSET_BITS=64

CFLAGS_HDF5  = -I${HDF5_PATH}/include
CFLAGS_NCMPI = -I$(NCMPI_PATH)/include

MDEFS =

.SUFFIXES: .o .c .f .F .h .fh .F90 .f90

#----------------------------------------------------------------------------
# Linker flags
#
#  There is a seperate version of the linker flags for each of the _OPT, 
#  _DEBUG, and _TEST cases.
#----------------------------------------------------------------------------

LFLAGS_OPT   = -O3 -o
LFLAGS_TEST  = -O4 -o
LFLAGS_DEBUG = -g -o

#----------------------------------------------------------------------------
# Library specific linking
#
#  If a FLASH module has a 'LIBRARY xxx' line in its Config file, we need to
#  create a macro in this Makefile.h for LIB_xxx, which will be added to the
#  link line when FLASH is built.  This allows us to switch between different
#  (incompatible) libraries.  We also create a _OPT, _DEBUG, and _TEST
#  library macro to add any performance-minded libraries (like fast math),
#  depending on how FLASH was setup.
#----------------------------------------------------------------------------


LIB_MPI   = 
LIB_HDF5  = -L${HDF5_PATH}/lib -lhdf5 -lz
LIB_NCMPI = -L$(NCMPI_PATH)/lib -lpnetcdf

LIB_MATH  = 

# TEST-MOD-FROM: LIB_OPT   = -L/homes/tpeterka/itl/lib -litl -L/homes/tpeterka/diy/lib -ldiy -lstdc++
# TO:
LIB_OPT   = -L/homes/leeten/workspace/itl/lib -lITLib -L/homes/leeten/workspace/diy/trunk/lib -ldiy -lstdc++ -lnetcdf
# TEST-MOD-END

LIB_DEBUG =
LIB_TEST  =

#----------------------------------------------------------------------------
# Additional machine-dependent object files
#
#  Add any machine specific files here -- they will be compiled and linked
#  when FLASH is built.
#----------------------------------------------------------------------------

MACHOBJ = 

#----------------------------------------------------------------------------
# Additional commands
#---------------------------------------------------------------------------- 

MV = mv -f
AR = ar -r
RM = rm -f
CD = cd
RL = ranlib
ECHO = echo

# This next section only applies to compiling FLASH, not some library.
# Anything that mentions a specific file should be within this ifeq block
ifeq ($(FLASHBINARY),true)

# The problem with local_tree.F90 is that (according to Chris) there are two variables within a 
# subroutine which essentiall alias each other (maybe a pointer?).  One is changed within the code but the other isn't.
#  Hence the compiler gets confused.  The -qalias=nostd is the trick that tells -O4 that this is a nonstandard implementation
local_tree_module.mod local_tree.mod local_tree.o : local_tree.F90
	${FCOMP} ${FFLAGS} -qalias=nostd ${F90FLAGS} ${FDEFINES} $<

# Previous things that didn't work with full optimization but have since been fixed
#runtime_parameters.o : runtime_parameters.F90
#	${FCOMP} ${FFLAGS_TEST}  $(F90FLAGS) $<

#amr_%.o : amr_%.F90
#	${FCOMP} ${FFLAGS_OPT_NOIPA} ${F90FLAGS} ${FDEFINES} $<


Simulation += io_meminfoMallinfo.o io_memory_usage_mallinfo.o
CDEFINES += -DHAVE_MALLINFO
$(Simulation) : Makefile.h
endif