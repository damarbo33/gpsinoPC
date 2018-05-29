#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=MinGW2_64-Windows
CND_DLIB_EXT=dll
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/5f719cc8/Ioutil.o \
	${OBJECTDIR}/_ext/785a7f66/image565.o \
	${OBJECTDIR}/_ext/511e4115/Icogestor.o \
	${OBJECTDIR}/_ext/511e4115/Iofrontend.o \
	${OBJECTDIR}/_ext/27549cb5/geodrawer.o \
	${OBJECTDIR}/_ext/27549cb5/gpxloader.o \
	${OBJECTDIR}/_ext/27549cb5/mercatorprojection.o \
	${OBJECTDIR}/_ext/f5645dfe/KalmanFilter.o \
	${OBJECTDIR}/_ext/511e4115/main.o \
	${OBJECTDIR}/_ext/e0359366/scrapper.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-L../../../ExternalLibs/crosslib/crosslib/dist/Release/MinGW32-Windows -L../../../ExternalLibs/curl-7.40.0-devel-mingw32/lib -L../../../ExternalLibs/SDL/libs/lib -L../../../ExternalLibs/SDL/libs/lib/x86 -lcrosslib -lsdlgfx -lmingw32 -lSDLmain -lSDL -lSDL_mixer -lSDL_ttf -lSDL_image -lcrypto -lcurl -lcurldll -lidn -lrtmp -lssh2 -lssh2dll -lssl -lz -lzdll -lws2_32

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/gpsinopc.exe

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/gpsinopc.exe: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/gpsinopc ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/_ext/5f719cc8/Ioutil.o: ../../../ExternalLibs/crosslib/src/uiobjects/common/Ioutil.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/5f719cc8
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DWIN -DWIN32 -I../../../ExternalLibs/SDL/libs/include -I../../../ExternalLibs/SDL/libs/include/SDL -I../../../ExternalLibs/crosslib/src/ziputils/zlib -I../../../ExternalLibs/crosslib/src/uiobjects -I../../../ExternalLibs/crosslib/src/sqllite -I../../../ExternalLibs/crosslib/src/libjpeg -I../../../ExternalLibs/crosslib/src/ziputils/unzip -I../../../ExternalLibs/curl-7.40.0-devel-mingw32/include -I../../../ExternalLibs/crosslib/src/tidy/include -I../../../ExternalLibs/crosslib/src/tidy/src -I../../../ExternalLibs/crosslib/src/gumbo-parser-master/src -I../../../ExternalLibs/crosslib/src/httpcurl -I../../../ExternalLibs/crosslib/src/httpcurl/jsoncpp-0.10.5/include -I../../../ExternalLibs/crosslib/src/rijndael -I../../BmpRLE -I../src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/5f719cc8/Ioutil.o ../../../ExternalLibs/crosslib/src/uiobjects/common/Ioutil.cpp

${OBJECTDIR}/_ext/785a7f66/image565.o: ../../BmpRLE/image565.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/785a7f66
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DWIN -DWIN32 -I../../../ExternalLibs/SDL/libs/include -I../../../ExternalLibs/SDL/libs/include/SDL -I../../../ExternalLibs/crosslib/src/ziputils/zlib -I../../../ExternalLibs/crosslib/src/uiobjects -I../../../ExternalLibs/crosslib/src/sqllite -I../../../ExternalLibs/crosslib/src/libjpeg -I../../../ExternalLibs/crosslib/src/ziputils/unzip -I../../../ExternalLibs/curl-7.40.0-devel-mingw32/include -I../../../ExternalLibs/crosslib/src/tidy/include -I../../../ExternalLibs/crosslib/src/tidy/src -I../../../ExternalLibs/crosslib/src/gumbo-parser-master/src -I../../../ExternalLibs/crosslib/src/httpcurl -I../../../ExternalLibs/crosslib/src/httpcurl/jsoncpp-0.10.5/include -I../../../ExternalLibs/crosslib/src/rijndael -I../../BmpRLE -I../src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/785a7f66/image565.o ../../BmpRLE/image565.cpp

${OBJECTDIR}/_ext/511e4115/Icogestor.o: ../src/Icogestor.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/511e4115
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DWIN -DWIN32 -I../../../ExternalLibs/SDL/libs/include -I../../../ExternalLibs/SDL/libs/include/SDL -I../../../ExternalLibs/crosslib/src/ziputils/zlib -I../../../ExternalLibs/crosslib/src/uiobjects -I../../../ExternalLibs/crosslib/src/sqllite -I../../../ExternalLibs/crosslib/src/libjpeg -I../../../ExternalLibs/crosslib/src/ziputils/unzip -I../../../ExternalLibs/curl-7.40.0-devel-mingw32/include -I../../../ExternalLibs/crosslib/src/tidy/include -I../../../ExternalLibs/crosslib/src/tidy/src -I../../../ExternalLibs/crosslib/src/gumbo-parser-master/src -I../../../ExternalLibs/crosslib/src/httpcurl -I../../../ExternalLibs/crosslib/src/httpcurl/jsoncpp-0.10.5/include -I../../../ExternalLibs/crosslib/src/rijndael -I../../BmpRLE -I../src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/511e4115/Icogestor.o ../src/Icogestor.cpp

${OBJECTDIR}/_ext/511e4115/Iofrontend.o: ../src/Iofrontend.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/511e4115
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DWIN -DWIN32 -I../../../ExternalLibs/SDL/libs/include -I../../../ExternalLibs/SDL/libs/include/SDL -I../../../ExternalLibs/crosslib/src/ziputils/zlib -I../../../ExternalLibs/crosslib/src/uiobjects -I../../../ExternalLibs/crosslib/src/sqllite -I../../../ExternalLibs/crosslib/src/libjpeg -I../../../ExternalLibs/crosslib/src/ziputils/unzip -I../../../ExternalLibs/curl-7.40.0-devel-mingw32/include -I../../../ExternalLibs/crosslib/src/tidy/include -I../../../ExternalLibs/crosslib/src/tidy/src -I../../../ExternalLibs/crosslib/src/gumbo-parser-master/src -I../../../ExternalLibs/crosslib/src/httpcurl -I../../../ExternalLibs/crosslib/src/httpcurl/jsoncpp-0.10.5/include -I../../../ExternalLibs/crosslib/src/rijndael -I../../BmpRLE -I../src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/511e4115/Iofrontend.o ../src/Iofrontend.cpp

${OBJECTDIR}/_ext/27549cb5/geodrawer.o: ../src/gpx/geodrawer.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/27549cb5
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DWIN -DWIN32 -I../../../ExternalLibs/SDL/libs/include -I../../../ExternalLibs/SDL/libs/include/SDL -I../../../ExternalLibs/crosslib/src/ziputils/zlib -I../../../ExternalLibs/crosslib/src/uiobjects -I../../../ExternalLibs/crosslib/src/sqllite -I../../../ExternalLibs/crosslib/src/libjpeg -I../../../ExternalLibs/crosslib/src/ziputils/unzip -I../../../ExternalLibs/curl-7.40.0-devel-mingw32/include -I../../../ExternalLibs/crosslib/src/tidy/include -I../../../ExternalLibs/crosslib/src/tidy/src -I../../../ExternalLibs/crosslib/src/gumbo-parser-master/src -I../../../ExternalLibs/crosslib/src/httpcurl -I../../../ExternalLibs/crosslib/src/httpcurl/jsoncpp-0.10.5/include -I../../../ExternalLibs/crosslib/src/rijndael -I../../BmpRLE -I../src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/27549cb5/geodrawer.o ../src/gpx/geodrawer.cpp

${OBJECTDIR}/_ext/27549cb5/gpxloader.o: ../src/gpx/gpxloader.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/27549cb5
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DWIN -DWIN32 -I../../../ExternalLibs/SDL/libs/include -I../../../ExternalLibs/SDL/libs/include/SDL -I../../../ExternalLibs/crosslib/src/ziputils/zlib -I../../../ExternalLibs/crosslib/src/uiobjects -I../../../ExternalLibs/crosslib/src/sqllite -I../../../ExternalLibs/crosslib/src/libjpeg -I../../../ExternalLibs/crosslib/src/ziputils/unzip -I../../../ExternalLibs/curl-7.40.0-devel-mingw32/include -I../../../ExternalLibs/crosslib/src/tidy/include -I../../../ExternalLibs/crosslib/src/tidy/src -I../../../ExternalLibs/crosslib/src/gumbo-parser-master/src -I../../../ExternalLibs/crosslib/src/httpcurl -I../../../ExternalLibs/crosslib/src/httpcurl/jsoncpp-0.10.5/include -I../../../ExternalLibs/crosslib/src/rijndael -I../../BmpRLE -I../src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/27549cb5/gpxloader.o ../src/gpx/gpxloader.cpp

${OBJECTDIR}/_ext/27549cb5/mercatorprojection.o: ../src/gpx/mercatorprojection.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/27549cb5
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DWIN -DWIN32 -I../../../ExternalLibs/SDL/libs/include -I../../../ExternalLibs/SDL/libs/include/SDL -I../../../ExternalLibs/crosslib/src/ziputils/zlib -I../../../ExternalLibs/crosslib/src/uiobjects -I../../../ExternalLibs/crosslib/src/sqllite -I../../../ExternalLibs/crosslib/src/libjpeg -I../../../ExternalLibs/crosslib/src/ziputils/unzip -I../../../ExternalLibs/curl-7.40.0-devel-mingw32/include -I../../../ExternalLibs/crosslib/src/tidy/include -I../../../ExternalLibs/crosslib/src/tidy/src -I../../../ExternalLibs/crosslib/src/gumbo-parser-master/src -I../../../ExternalLibs/crosslib/src/httpcurl -I../../../ExternalLibs/crosslib/src/httpcurl/jsoncpp-0.10.5/include -I../../../ExternalLibs/crosslib/src/rijndael -I../../BmpRLE -I../src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/27549cb5/mercatorprojection.o ../src/gpx/mercatorprojection.cpp

${OBJECTDIR}/_ext/f5645dfe/KalmanFilter.o: ../src/kalman/KalmanFilter.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/f5645dfe
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DWIN -DWIN32 -I../../../ExternalLibs/SDL/libs/include -I../../../ExternalLibs/SDL/libs/include/SDL -I../../../ExternalLibs/crosslib/src/ziputils/zlib -I../../../ExternalLibs/crosslib/src/uiobjects -I../../../ExternalLibs/crosslib/src/sqllite -I../../../ExternalLibs/crosslib/src/libjpeg -I../../../ExternalLibs/crosslib/src/ziputils/unzip -I../../../ExternalLibs/curl-7.40.0-devel-mingw32/include -I../../../ExternalLibs/crosslib/src/tidy/include -I../../../ExternalLibs/crosslib/src/tidy/src -I../../../ExternalLibs/crosslib/src/gumbo-parser-master/src -I../../../ExternalLibs/crosslib/src/httpcurl -I../../../ExternalLibs/crosslib/src/httpcurl/jsoncpp-0.10.5/include -I../../../ExternalLibs/crosslib/src/rijndael -I../../BmpRLE -I../src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f5645dfe/KalmanFilter.o ../src/kalman/KalmanFilter.cpp

${OBJECTDIR}/_ext/511e4115/main.o: ../src/main.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/511e4115
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DWIN -DWIN32 -I../../../ExternalLibs/SDL/libs/include -I../../../ExternalLibs/SDL/libs/include/SDL -I../../../ExternalLibs/crosslib/src/ziputils/zlib -I../../../ExternalLibs/crosslib/src/uiobjects -I../../../ExternalLibs/crosslib/src/sqllite -I../../../ExternalLibs/crosslib/src/libjpeg -I../../../ExternalLibs/crosslib/src/ziputils/unzip -I../../../ExternalLibs/curl-7.40.0-devel-mingw32/include -I../../../ExternalLibs/crosslib/src/tidy/include -I../../../ExternalLibs/crosslib/src/tidy/src -I../../../ExternalLibs/crosslib/src/gumbo-parser-master/src -I../../../ExternalLibs/crosslib/src/httpcurl -I../../../ExternalLibs/crosslib/src/httpcurl/jsoncpp-0.10.5/include -I../../../ExternalLibs/crosslib/src/rijndael -I../../BmpRLE -I../src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/511e4115/main.o ../src/main.cpp

${OBJECTDIR}/_ext/e0359366/scrapper.o: ../src/scrapper/scrapper.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/e0359366
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -DWIN -DWIN32 -I../../../ExternalLibs/SDL/libs/include -I../../../ExternalLibs/SDL/libs/include/SDL -I../../../ExternalLibs/crosslib/src/ziputils/zlib -I../../../ExternalLibs/crosslib/src/uiobjects -I../../../ExternalLibs/crosslib/src/sqllite -I../../../ExternalLibs/crosslib/src/libjpeg -I../../../ExternalLibs/crosslib/src/ziputils/unzip -I../../../ExternalLibs/curl-7.40.0-devel-mingw32/include -I../../../ExternalLibs/crosslib/src/tidy/include -I../../../ExternalLibs/crosslib/src/tidy/src -I../../../ExternalLibs/crosslib/src/gumbo-parser-master/src -I../../../ExternalLibs/crosslib/src/httpcurl -I../../../ExternalLibs/crosslib/src/httpcurl/jsoncpp-0.10.5/include -I../../../ExternalLibs/crosslib/src/rijndael -I../../BmpRLE -I../src -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/e0359366/scrapper.o ../src/scrapper/scrapper.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
