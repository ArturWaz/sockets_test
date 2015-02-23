CMAKE_MINIMUM_REQUIRED( VERSION 2.8 )

SET( PROJ_PATH ${CMAKE_SOURCE_DIR} )
SET( PROJ_OUT_PATH ${CMAKE_BINARY_DIR} )



# Section for project specific settings
SET( SERVER_PROJ_NAME "server" )
SET( CLIENT_PROJ_NAME "client" )

#SET( OPENSSL_INCLUDES "C:/Program Files (x86)/GnuWin32/include" )
#SET( OPENSSL_LIBRARIES "C:/Program Files (x86)/GnuWin32/lib/ssleay32.lib" "C:/Program Files (x86)/GnuWin32/lib/libeay32.lib" )
SET( BOOST_INCLUDES ${OPENSSL_INCLUDES} "C:/boost_1_57_0/include/boost-1_57" )
SET( BOOST_LIBRARIES ${OPENSSL_LIBRARIES} "C:/boost_1_57_0/lib/libboost_system-mgw48-mt-1_57.a" "C:/boost_1_57_0/lib/libboost_thread-mgw48-mt-1_57.a" "C:/boost_1_57_0/lib/libboost_date_time-mgw48-mt-1_57.a" "C:/boost_1_57_0/lib/libboost_regex-mgw48-mt-1_57.a" "C:/boost_1_57_0/lib/libboost_serialization-mgw48-mt-1_57.a" )

SET( PROJ_LIBRARIES ${BOOST_LIBRARIES} "ws2_32" "wsock32" )
SET( PROJ_INCLUDES ${BOOST_INCLUDES} "src" "lib" )
FILE( GLOB_RECURSE PROJ_HEADERS )
FILE( GLOB_RECURSE SERVER_PROJ_SOURCES server.cpp )
FILE( GLOB_RECURSE CLIENT_PROJ_SOURCES client.cpp )

# End of special section


SET( CMAKE_CXX_FLAGS "-std=c++11" )
IF ( UNIX )
    SET( CMAKE_CXX_FLAGS "-pthread ${CMAKE_CXX_FLAGS}" )
ENDIF ( UNIX )




PROJECT( ${SERVER_PROJ_NAME} )
INCLUDE_DIRECTORIES( ${PROJ_INCLUDES} )
ADD_EXECUTABLE( ${SERVER_PROJ_NAME} ${SERVER_PROJ_SOURCES} )
TARGET_LINK_LIBRARIES( ${SERVER_PROJ_NAME} ${PROJ_LIBRARIES} )

PROJECT( ${CLIENT_PROJ_NAME} )
INCLUDE_DIRECTORIES( ${PROJ_INCLUDES} )
ADD_EXECUTABLE( ${CLIENT_PROJ_NAME} ${CLIENT_PROJ_SOURCES} )
TARGET_LINK_LIBRARIES( ${CLIENT_PROJ_NAME} ${PROJ_LIBRARIES} )