#!/bin/bash

# import REPOSITORY_ROOT_DIR/CURRENT_FILE_NAME/PrintLog
source common.sh

PrintLog ${CURRENT_FILE_NAME} "CURRENT_FILE_NAME=${CURRENT_FILE_NAME}"
PrintLog ${CURRENT_FILE_NAME} "REPOSITORY_ROOT_DIR=${REPOSITORY_ROOT_DIR}"
PrintLog ${CURRENT_FILE_NAME} "BUILD_NUM=${BUILD_NUM}"
PrintLog ${CURRENT_FILE_NAME} "BUILD_VERSION=${BUILD_VERSION}"
PrintLog ${CURRENT_FILE_NAME} "BUILD_TARGET=${BUILD_TARGET}"



# must in source root dir
cd ${REPOSITORY_ROOT_DIR}


# init
apt-get --quiet update --yes
apt-get --quiet install --yes wget tar unzip lib32stdc++6 lib32z1
# ANDROID_COMPILE_SDK is the version of Android you're compiling with.
# It should match compileSdkVersion.
# sdkmanager --sdk_root=${ANDROID_HOME} "platforms;android-${ANDROID_COMPILE_SDK}"
# sdkmanager --list --sdk_root="$ANDROID_HOME" | grep platforms
export ANDROID_COMPILE_SDK="34"

# ANDROID_BUILD_TOOLS is the version of the Android build tools you are using.
# It should match buildToolsVersion.
# https://developer.android.google.cn/studio/releases/build-tools
# sdkmanager --sdk_root=${ANDROID_HOME} "build-tools;${ANDROID_BUILD_TOOLS}"
# sdkmanager --list --sdk_root="$ANDROID_HOME" | grep build-tools
export ANDROID_BUILD_TOOLS="35.0.1"

# It's what version of the command line tools we're going to download from the official site.
# Official Site-> https://developer.android.com/studio/index.html
# There, look down below at the cli tools only, sdk tools package is of format:
#        commandlinetools-os_type-ANDROID_SDK_TOOLS_latest.zip
# when the script was last modified for latest compileSdkVersion, it was which is written down below
export ANDROID_SDK_TOOLS="11076708"

# sdkmanager --list --sdk_root="$ANDROID_HOME" | grep ndk
# sdkmanager --sdk_root="$ANDROID_HOME" "ndk;26.3.11579264"
export ANDROID_NDK="26.3.11579264"

# jdk11
# mkdir /JDK
# wget --output-document=/JDK/jdk11.tgz https://download.java.net/java/GA/jdk11/9/GPL/openjdk-11.0.2_linux-x64_bin.tar.gz
# pushd /JDK
# tar -xzf jdk11.tgz 
# popd

#apt install openjdk-17-jdk
export JAVA_HOME="/JDK/jdk-11.0.2/"
export PATH="${PATH}:${JAVA_HOME}/bin/"
java -version


# android sdk
cd java/android
# Setup path as android_home for moving/exporting the downloaded sdk into it
export ANDROID_HOME="/ANDROID_HOME"
# Create a new directory at specified location
# install -d $ANDROID_HOME
# Here we are installing androidSDK tools from official source,
# (the key thing here is the url from where you are downloading these sdk tool for command line, so please do note this url pattern there and here as well)
# after that unzipping those tools and
# then running a series of SDK manager commands to install necessary android SDK packages that'll allow the app to build
# wget --output-document=$ANDROID_HOME/cmdline-tools.zip https://dl.google.com/android/repository/commandlinetools-linux-${ANDROID_SDK_TOOLS}_latest.zip

# move to the archive at ANDROID_HOME
# pushd $ANDROID_HOME
# unzip -d cmdline-tools.zip
# chmod +x cmdline-tools/bin/sdkmanager
# popd
export PATH="${PATH}:${ANDROID_HOME}/cmdline-tools/bin/"

# Nothing fancy here, just checking sdkManager version
sdkmanager --version

# use yes to accept all licenses
# yes | sdkmanager --sdk_root=${ANDROID_HOME} --licenses || true
# sdkmanager --sdk_root=${ANDROID_HOME} "platforms;android-${ANDROID_COMPILE_SDK}"
# sdkmanager --sdk_root=${ANDROID_HOME} "platform-tools"
# sdkmanager --sdk_root=${ANDROID_HOME} "build-tools;${ANDROID_BUILD_TOOLS}"
# sdkmanager --list --sdk_root="$ANDROID_HOME" | grep ndk
# Not necessary, but just for surity
chmod +x ./gradlew

# must in source root dir
# build aar
#./gradlew -Dhttps.proxyHost=192.168.17.1 -Dhttps.proxyPort=10088
./gradlew xxxxxxx:assemble


echo 'compile done!'
