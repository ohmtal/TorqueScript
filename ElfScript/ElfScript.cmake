# ElfScript Source Files definition

# version flags when something is changed to keep compat.
# Example raylib-elfScript resource manager .

add_compile_definitions(ELFSCRIPT_VERSION_0_4)
add_compile_definitions(ELFSCRIPT_VERSION_0_5)
add_compile_definitions(ELFSCRIPT_VERSION_0_6)
add_compile_definitions(ELFSCRIPT_VERSION_0_7)

option(ELF_ENABLE_ZIP "ElfScript: Enable the ZIP Support" OFF)
option(ELF_ENABLE_CONSOLE_VECTOR "ElfScript: Enable variable vector support" ON)

option(ELF_ENABLE_SLIMOBJECT "ElfScript: Disable Torque3D Editor methods." ON)

# note after testing i can set it default ON again:
option(ELF_ENABLE_FIELDCACHE "ElfScript:  Enable FieldCache - - this is faster but you need to initialize the fields to get a good result. Disable this is you use old lazy written scripts or need to save memory" OFF)
option(ELF_ENABLE_CV_CALLBACK "ElfScript: ConsoleValue callback - used by ValueVector class" OFF)

option(ELF_ENABLE_EXEC_OVERWRITE "ElfScript: Disable the default exec to overwrite it with custom function" OFF)
option(ELF_ENABLE_DSO_GENERATION "ElfScript: Enable generation of byte code dso files" OFF)

set(ELF_FILE_EXTENSION "elf" CACHE STRING "ElfScript: File extension")
set(ELF_APP_NAME "ElfApp" CACHE STRING "ElfScript: App-Name used in scripts")
set(ELF_APP_VERSION "700" CACHE STRING "ElfScript: App-Version used in scripts")
set(ELF_APP_VERSION_STRING "Version 0.7" CACHE STRING "ElfScript: App-Version String used in scripts")


# overwrite example:  set(ELF_ENABLE_DOC_EXPORTER ON CACHE BOOL "ElfScript: Enable the Doc Exporter addon" FORCE)

option(ELF_ENABLE_DOC_EXPORTER "ElfScript: Enable the Doc Exporter addon" OFF)
option(ELF_ENABLE_NATIV_CONSOLE "ElfScript: Enable nativ console addon (unix only)" OFF)


if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(TORQUE_DEBUG)
endif()


set(CONSOLE_SRC
    # ${CMAKE_CURRENT_LIST_DIR}/console/arrayObject.cpp
    # ${CMAKE_CURRENT_LIST_DIR}/console/arrayObject.h
    ${CMAKE_CURRENT_LIST_DIR}/console/console.cpp
    ${CMAKE_CURRENT_LIST_DIR}/console/consoleDoc.cpp
    ${CMAKE_CURRENT_LIST_DIR}/console/consoleDoc.h
    ${CMAKE_CURRENT_LIST_DIR}/console/consoleFunctions.cpp
    ${CMAKE_CURRENT_LIST_DIR}/console/consoleFunctions.h
    ${CMAKE_CURRENT_LIST_DIR}/console/console.h
    ${CMAKE_CURRENT_LIST_DIR}/console/consoleInternal.cpp
    ${CMAKE_CURRENT_LIST_DIR}/console/consoleInternal.h
    ${CMAKE_CURRENT_LIST_DIR}/console/consoleObject.cpp
    ${CMAKE_CURRENT_LIST_DIR}/console/consoleObject.h
    ${CMAKE_CURRENT_LIST_DIR}/console/consoleParser.cpp
    ${CMAKE_CURRENT_LIST_DIR}/console/consoleParser.h
    ${CMAKE_CURRENT_LIST_DIR}/console/consoleTypes.cpp
    ${CMAKE_CURRENT_LIST_DIR}/console/consoleTypes.h
    ${CMAKE_CURRENT_LIST_DIR}/console/consoleValueStack.h
    ${CMAKE_CURRENT_LIST_DIR}/console/dynamicTypes.cpp
    ${CMAKE_CURRENT_LIST_DIR}/console/dynamicTypes.h
    ${CMAKE_CURRENT_LIST_DIR}/console/engineAPI.h
    ${CMAKE_CURRENT_LIST_DIR}/console/engineDoc.cpp
    ${CMAKE_CURRENT_LIST_DIR}/console/engineExports.cpp
    ${CMAKE_CURRENT_LIST_DIR}/console/engineExports.h
    ${CMAKE_CURRENT_LIST_DIR}/console/engineFunctions.cpp
    ${CMAKE_CURRENT_LIST_DIR}/console/engineFunctions.h
    ${CMAKE_CURRENT_LIST_DIR}/console/engineObject.cpp
    ${CMAKE_CURRENT_LIST_DIR}/console/engineObject.h
    ${CMAKE_CURRENT_LIST_DIR}/console/enginePrimitives.cpp
    ${CMAKE_CURRENT_LIST_DIR}/console/enginePrimitives.h
    ${CMAKE_CURRENT_LIST_DIR}/console/engineStructs.cpp
    ${CMAKE_CURRENT_LIST_DIR}/console/engineStructs.h
    ${CMAKE_CURRENT_LIST_DIR}/console/engineTypeInfo.cpp
    ${CMAKE_CURRENT_LIST_DIR}/console/engineTypeInfo.h
    ${CMAKE_CURRENT_LIST_DIR}/console/engineTypes.cpp
    ${CMAKE_CURRENT_LIST_DIR}/console/engineTypes.h
    # ${CMAKE_CURRENT_LIST_DIR}/console/fileSystemFunctions.cpp
    ${CMAKE_CURRENT_LIST_DIR}/console/fixedTuple.h
    ${CMAKE_CURRENT_LIST_DIR}/console/ICallMethod.h
    ${CMAKE_CURRENT_LIST_DIR}/console/module.h
    ${CMAKE_CURRENT_LIST_DIR}/console/propertyParsing.cpp
    ${CMAKE_CURRENT_LIST_DIR}/console/propertyParsing.h
    ${CMAKE_CURRENT_LIST_DIR}/console/returnBuffer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/console/returnBuffer.h
    ${CMAKE_CURRENT_LIST_DIR}/console/runtimeClassRep.cpp
    ${CMAKE_CURRENT_LIST_DIR}/console/runtimeClassRep.h
    ${CMAKE_CURRENT_LIST_DIR}/console/runtime.h
    ${CMAKE_CURRENT_LIST_DIR}/console/scriptFilename.cpp
    ${CMAKE_CURRENT_LIST_DIR}/console/scriptFilename.h
    ${CMAKE_CURRENT_LIST_DIR}/console/script.h
    ${CMAKE_CURRENT_LIST_DIR}/console/scriptObjects.cpp
    ${CMAKE_CURRENT_LIST_DIR}/console/scriptObjects.h
    ${CMAKE_CURRENT_LIST_DIR}/console/simBase.h
    ${CMAKE_CURRENT_LIST_DIR}/console/sim.cpp
    # ${CMAKE_CURRENT_LIST_DIR}/console/simDatablock.cpp
    # ${CMAKE_CURRENT_LIST_DIR}/console/simDatablock.h
    ${CMAKE_CURRENT_LIST_DIR}/console/simDictionary.cpp
    ${CMAKE_CURRENT_LIST_DIR}/console/simDictionary.h
    ${CMAKE_CURRENT_LIST_DIR}/console/simEvents.cpp
    ${CMAKE_CURRENT_LIST_DIR}/console/simEvents.h
    ${CMAKE_CURRENT_LIST_DIR}/console/simFieldDictionary.cpp
    ${CMAKE_CURRENT_LIST_DIR}/console/simFieldDictionary.h
    ${CMAKE_CURRENT_LIST_DIR}/console/sim.h
    ${CMAKE_CURRENT_LIST_DIR}/console/simManager.cpp
    ${CMAKE_CURRENT_LIST_DIR}/console/simObject.cpp
    ${CMAKE_CURRENT_LIST_DIR}/console/simObject.h
    ${CMAKE_CURRENT_LIST_DIR}/console/simObjectList.cpp
    ${CMAKE_CURRENT_LIST_DIR}/console/simObjectList.h
    ${CMAKE_CURRENT_LIST_DIR}/console/simObjectRef.h
    ${CMAKE_CURRENT_LIST_DIR}/console/simPersistID.cpp
    ${CMAKE_CURRENT_LIST_DIR}/console/simPersistID.h
    ${CMAKE_CURRENT_LIST_DIR}/console/simPersistSet.cpp
    ${CMAKE_CURRENT_LIST_DIR}/console/simPersistSet.h
    ${CMAKE_CURRENT_LIST_DIR}/console/simSerialize.cpp
    ${CMAKE_CURRENT_LIST_DIR}/console/simSet.cpp
    ${CMAKE_CURRENT_LIST_DIR}/console/simSet.h
    ${CMAKE_CURRENT_LIST_DIR}/console/stringStack.cpp
    ${CMAKE_CURRENT_LIST_DIR}/console/stringStack.h
    ${CMAKE_CURRENT_LIST_DIR}/console/typeValidators.cpp
    ${CMAKE_CURRENT_LIST_DIR}/console/typeValidators.h
    ${CMAKE_CURRENT_LIST_DIR}/console/torquescript/astAlloc.cpp
    ${CMAKE_CURRENT_LIST_DIR}/console/torquescript/ast.h
    ${CMAKE_CURRENT_LIST_DIR}/console/torquescript/astNodes.cpp
    ${CMAKE_CURRENT_LIST_DIR}/console/torquescript/CMDgram.cpp
    ${CMAKE_CURRENT_LIST_DIR}/console/torquescript/CMDgram.h
    ${CMAKE_CURRENT_LIST_DIR}/console/torquescript/CMDscan.cpp
    ${CMAKE_CURRENT_LIST_DIR}/console/torquescript/codeBlock.cpp
    ${CMAKE_CURRENT_LIST_DIR}/console/torquescript/codeBlock.h
    # ${CMAKE_CURRENT_LIST_DIR}/console/torquescript/compiledEval.cpp
    ${CMAKE_CURRENT_LIST_DIR}/console/torquescript/compiler.cpp
    ${CMAKE_CURRENT_LIST_DIR}/console/torquescript/compiler.h
    ${CMAKE_CURRENT_LIST_DIR}/console/torquescript/evalState.cpp
    ${CMAKE_CURRENT_LIST_DIR}/console/torquescript/evalState.h
    ${CMAKE_CURRENT_LIST_DIR}/console/torquescript/optimizer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/console/torquescript/parser.cpp
    ${CMAKE_CURRENT_LIST_DIR}/console/torquescript/parser.h
    ${CMAKE_CURRENT_LIST_DIR}/console/torquescript/runtime.cpp
    ${CMAKE_CURRENT_LIST_DIR}/console/torquescript/runtime.h

    # XXTH my extras ...
    ${CMAKE_CURRENT_LIST_DIR}/console/consoleExtras.h
    ${CMAKE_CURRENT_LIST_DIR}/console/consoleExtras.cpp

    # this is rock and roll!!
    ${CMAKE_CURRENT_LIST_DIR}/console/scriptPreprocessor.h
    ${CMAKE_CURRENT_LIST_DIR}/console/scriptPreprocessor.cpp

    # local var tools
    ${CMAKE_CURRENT_LIST_DIR}/console/localVar.cpp


    # new monster with direct threading:
    ${CMAKE_CURRENT_LIST_DIR}/console/torquescript/compiledEval_optimize.cpp

)

set(CORE_SRC
    ${CMAKE_CURRENT_LIST_DIR}/core/crc.cpp
    ${CMAKE_CURRENT_LIST_DIR}/core/bitVector.cpp
    ${CMAKE_CURRENT_LIST_DIR}/core/iTickable.cpp
    ${CMAKE_CURRENT_LIST_DIR}/core/virtualMountSystem.cpp
    ${CMAKE_CURRENT_LIST_DIR}/core/util/str.h
    ${CMAKE_CURRENT_LIST_DIR}/core/util/str.cpp
    ${CMAKE_CURRENT_LIST_DIR}/core/util/byteBuffer.cpp


    ${CMAKE_CURRENT_LIST_DIR}/core/util/hashFunction.cpp
    ${CMAKE_CURRENT_LIST_DIR}/core/util/tVector.cpp
    ${CMAKE_CURRENT_LIST_DIR}/core/util/tSignal.cpp
    ${CMAKE_CURRENT_LIST_DIR}/core/util/tDictionary.cpp
    ${CMAKE_CURRENT_LIST_DIR}/core/util/md5.cpp

    ${CMAKE_CURRENT_LIST_DIR}/core/util/refBase.cpp

    ${CMAKE_CURRENT_LIST_DIR}/core/frameAllocator.cpp
    ${CMAKE_CURRENT_LIST_DIR}/core/util/timeClass.cpp

    ${CMAKE_CURRENT_LIST_DIR}/core/util/journal/journal.cpp
    ${CMAKE_CURRENT_LIST_DIR}/core/util/journal/process.cpp

    ${CMAKE_CURRENT_LIST_DIR}/core/util/path.cpp
    ${CMAKE_CURRENT_LIST_DIR}/core/util/uuid.cpp

    ${CMAKE_CURRENT_LIST_DIR}/core/idGenerator.cpp
    ${CMAKE_CURRENT_LIST_DIR}/core/volume.cpp
    ${CMAKE_CURRENT_LIST_DIR}/core/fileObject.cpp

    ${CMAKE_CURRENT_LIST_DIR}/core/stringTable.cpp
    ${CMAKE_CURRENT_LIST_DIR}/core/stringTable.h

    ${CMAKE_CURRENT_LIST_DIR}/core/stream/stream.cpp
    ${CMAKE_CURRENT_LIST_DIR}/core/stream/fileStream.cpp
    ${CMAKE_CURRENT_LIST_DIR}/core/stream/memStream.cpp
    # ${CMAKE_CURRENT_LIST_DIR}/core/stream/bitStream.cpp
    ${CMAKE_CURRENT_LIST_DIR}/core/stream/streamObject.cpp
    ${CMAKE_CURRENT_LIST_DIR}/core/stream/fileStreamObject.cpp

    ${CMAKE_CURRENT_LIST_DIR}/core/strings/findMatch.cpp
    ${CMAKE_CURRENT_LIST_DIR}/core/strings/findMatch.h
    ${CMAKE_CURRENT_LIST_DIR}/core/strings/stringFunctions.cpp
    ${CMAKE_CURRENT_LIST_DIR}/core/strings/stringFunctions.h
    ${CMAKE_CURRENT_LIST_DIR}/core/strings/stringUnit.cpp
    ${CMAKE_CURRENT_LIST_DIR}/core/strings/stringUnit.h
    ${CMAKE_CURRENT_LIST_DIR}/core/strings/unicode.cpp
    ${CMAKE_CURRENT_LIST_DIR}/core/strings/unicode.h

    ${CMAKE_CURRENT_LIST_DIR}/core/util/messaging/message.cpp
)


set(PLATFORM_SRC
    ${CMAKE_CURRENT_LIST_DIR}/platform/platformFileIO.cpp
    ${CMAKE_CURRENT_LIST_DIR}/platform/platformAssert.cpp
    ${CMAKE_CURRENT_LIST_DIR}/platform/platformMemory.cpp
    ${CMAKE_CURRENT_LIST_DIR}/platform/platformVolume.cpp

)

set ( OBJECTS_SRC
    # some neat objects to work with
    ${CMAKE_CURRENT_LIST_DIR}/objects/pointStorageObject.cpp
    ${CMAKE_CURRENT_LIST_DIR}/objects/objectAliases.cpp
    ${CMAKE_CURRENT_LIST_DIR}/objects/NeuralNetObject.cpp
    ${CMAKE_CURRENT_LIST_DIR}/objects/ConsoleLogObject.cpp

    # ElfScript Array using ConsoleValue
    ${CMAKE_CURRENT_LIST_DIR}/objects/Array.h
    ${CMAKE_CURRENT_LIST_DIR}/objects/Array.cpp

    # previous called ArrayObject:
    ${CMAKE_CURRENT_LIST_DIR}/objects/KeyValueStringTable.h
    ${CMAKE_CURRENT_LIST_DIR}/objects/KeyValueStringTable.cpp


)

set(ZIP_SRC
    ${CMAKE_CURRENT_LIST_DIR}/core/filterStream.cpp
    # ----
    ${CMAKE_CURRENT_LIST_DIR}/ext/zlib/adler32.c
    ${CMAKE_CURRENT_LIST_DIR}/ext/zlib/compress.c
    ${CMAKE_CURRENT_LIST_DIR}/ext/zlib/crc32.c
    ${CMAKE_CURRENT_LIST_DIR}/ext/zlib/deflate.c
    ${CMAKE_CURRENT_LIST_DIR}/ext/zlib/gzclose.c
    ${CMAKE_CURRENT_LIST_DIR}/ext/zlib/gzlib.c
    ${CMAKE_CURRENT_LIST_DIR}/ext/zlib/gzread.c
    ${CMAKE_CURRENT_LIST_DIR}/ext/zlib/gzwrite.c
    ${CMAKE_CURRENT_LIST_DIR}/ext/zlib/infback.c
    ${CMAKE_CURRENT_LIST_DIR}/ext/zlib/inffast.c
    ${CMAKE_CURRENT_LIST_DIR}/ext/zlib/inflate.c
    ${CMAKE_CURRENT_LIST_DIR}/ext/zlib/inftrees.c
    ${CMAKE_CURRENT_LIST_DIR}/ext/zlib/trees.c
    ${CMAKE_CURRENT_LIST_DIR}/ext/zlib/uncompr.c
    ${CMAKE_CURRENT_LIST_DIR}/ext/zlib/zutil.c
    # adler32.c  compress.c  crc32.c  deflate.c  gzclose.c  gzlib.c  gzread.c  gzwrite.c
    # infback.c  inffast.c  inflate.c  inftrees.c  trees.c  uncompr.c  zutil.c
    # ----
    ${CMAKE_CURRENT_LIST_DIR}/core/util/zip/centralDir.cpp
    ${CMAKE_CURRENT_LIST_DIR}/core/util/zip/compressor.cpp
    ${CMAKE_CURRENT_LIST_DIR}/core/util/zip/extraField.cpp
    ${CMAKE_CURRENT_LIST_DIR}/core/util/zip/fileHeader.cpp
    ${CMAKE_CURRENT_LIST_DIR}/core/util/zip/zipArchive.cpp
    ${CMAKE_CURRENT_LIST_DIR}/core/util/zip/zipCryptStream.cpp
    ${CMAKE_CURRENT_LIST_DIR}/core/util/zip/zipObject.cpp
    ${CMAKE_CURRENT_LIST_DIR}/core/util/zip/zipSubStream.cpp
    ${CMAKE_CURRENT_LIST_DIR}/core/util/zip/zipTempStream.cpp
    ${CMAKE_CURRENT_LIST_DIR}/core/util/zip/zipVolume.cpp

)

set(POSIX_SRC
   ${CMAKE_CURRENT_LIST_DIR}/platform/posix/posixVolume.cpp

)
set(WIN_SRC
   ${CMAKE_CURRENT_LIST_DIR}/platform/win/winVolume.cpp
)


set(MATH_SRC
   ${CMAKE_CURRENT_LIST_DIR}/math/mConsoleFunctions.cpp
)

set(EXT_SRC
   ${CMAKE_CURRENT_LIST_DIR}/ext/tinyexpr.c
   ${CMAKE_CURRENT_LIST_DIR}/ext/genann.c
)




if(WIN32)
    list(APPEND PLATFORM_SRC ${WIN_SRC})
elseif(UNIX)
    list(APPEND PLATFORM_SRC ${POSIX_SRC})
endif()



set(ELFSCRIPT_SRC
    ${CMAKE_CURRENT_LIST_DIR}/main/engineGlue.cpp
    ${CONSOLE_SRC}
    ${CORE_SRC}
    ${PLATFORM_SRC}
    ${MATH_SRC}
    ${OBJECTS_SRC}
    ${EXT_SRC}
    ${ANN_ELFSCRIPT_SRC}
)

if (ELF_ENABLE_CONSOLE_VECTOR)
     add_compile_definitions(ENABLE_CONSOLE_VECTOR)
     list(APPEND ELFSCRIPT_SRC ${CMAKE_CURRENT_LIST_DIR}/objects/ConsoleVectorScript.cpp)
endif()

if (ELF_ENABLE_ZIP)
     add_compile_definitions(ENABLE_ZIP_SUPPORT)
     list(APPEND ELFSCRIPT_SRC ${ZIP_SRC})
     add_compile_definitions(TORQUE_LOWER_ZIPCASE)
else()
    add_compile_definitions(TORQUE_DISABLE_VIRTUAL_MOUNT_SYSTEM)
endif()

if (ELF_ENABLE_DOC_EXPORTER)
    list(APPEND ELFSCRIPT_SRC ${ELF_MODULE}/addons/console/consoleDocExporter.cpp)
endif()

if (ELF_ENABLE_NATIV_CONSOLE)
    if(UNIX)
      list(APPEND ELFSCRIPT_SRC ${ELF_MODULE}/addons/shellConsole/POSIXConsole.cpp)
    endif()
endif()

if (NOT ELF_ENABLE_DSO_GENERATION)
     add_compile_definitions(TORQUE_NO_DSO_GENERATION)
endif()

if (ELF_ENABLE_EXEC_OVERWRITE)
     add_compile_definitions(ELFSCRIPT_EXEC_OVERWRITE)
endif()

add_compile_definitions(TORQUE_SCRIPT_EXTENSION="${ELF_FILE_EXTENSION}")
add_compile_definitions(TORQUE_APP_NAME="${ELF_APP_NAME}")
add_compile_definitions(TORQUE_APP_VERSION=${ELF_APP_VERSION})
add_compile_definitions(TORQUE_APP_VERSION_STRING="${ELF_APP_VERSION_STRING}")

# # if (ELF_ENABLE_GARBAGECOLLECTION)
# #      add_compile_definitions(ELFSCRIPT_GARBAGECOLLECTION)
# # endif()

if ( ELF_ENABLE_FIELDCACHE )
     add_compile_definitions(ELFSCRIPT_ENABLE_FIELDCACHE)
endif()

if (ELF_ENABLE_SLIMOBJECT)
     add_compile_definitions(ELFSCRIPT_SLIM_OBJECT)
endif()

if (ELF_ENABLE_CV_CALLBACK)
     add_compile_definitions(ENABLE_CONSOLE_VALUE_CALLBACK)
endif()

add_compile_definitions(TORQUE_DISABLE_MEMORY_MANAGER)
