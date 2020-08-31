TARGET=DeformableBullet
CONFIG-=app_bundle
CONFIG+=c++14
QMAKE_CXXFLAGS += -std=c++1z
QMAKE_CXXFLAGS += -Wall -Wextra
QMAKE_CXXFLAGS += -g
OBJECTS_DIR=obj
SOURCES += src/*.cpp tests/*.cpp

HEADERS +=  include/*.h
INCLUDEPATH += include
LIBS += -lOpenImageIO
LIBS+= -L/usr/local/lib -lgtest -lgtest_main -pthread
QMAKE_CXXFLAGS+=$$system(pkg-config --cflags bullet)
LIBS+= $$system(pkg-config --libs bullet)
win32 :{
    message(Make sure that Bullet is installed using vcpkg install SDL2 )
  #  message("package dir is" $$VCPK)
  INCLUDEPATH += $$(HOMEDRIVE)\\$$(HOMEPATH)\vcpkg\installed\x86-windows\include\
  INCLUDEPATH += $$(HOMEDRIVE)\\$$(HOMEPATH)\vcpkg\installed\x86-windows\include\bullet

  PRE_TARGETDEPS+=$$(HOMEDRIVE)\\$$(HOMEPATH)\vcpkg\installed\x86-windows\lib\Bullet3Common.lib
  LIBS+=-L$$(HOMEDRIVE)\\$$(HOMEPATH)\vcpkg\installed\x86-windows\lib\ -lBullet3Common

  PRE_TARGETDEPS+=$$(HOMEDRIVE)\\$$(HOMEPATH)\vcpkg\installed\x86-windows\lib\BulletCollision.lib
  LIBS+=-L$$(HOMEDRIVE)\\$$(HOMEPATH)\vcpkg\installed\x86-windows\lib\ -lBulletCollision

  PRE_TARGETDEPS+=$$(HOMEDRIVE)\\$$(HOMEPATH)\vcpkg\installed\x86-windows\lib\BulletDynamics.lib
  LIBS+=-L$$(HOMEDRIVE)\\$$(HOMEPATH)\vcpkg\installed\x86-windows\lib\ -lBulletDynamics

  PRE_TARGETDEPS+=$$(HOMEDRIVE)\\$$(HOMEPATH)\vcpkg\installed\x86-windows\lib\LinearMath.lib
  LIBS+=-L$$(HOMEDRIVE)\\$$(HOMEPATH)\vcpkg\installed\x86-windows\lib\ -lLinearMath

}

include($(HOME)/NGL/UseNGL.pri)
