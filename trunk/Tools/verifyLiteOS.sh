#!/bin/bash

echo "This script will verify LiteOS has been successfully installed"
echo "This script must be invoked under the Tools directory"

#cd ./uisp
#./configure
#./make
#./make install
#cd ..

echo "Configuring build script for applications..."

cd ..
cd ./Apps
echo "Current directory is: "
pwd
cp ./build /usr/local/bin


echo "Compilation the latest kernel for micaz..."

cd ..
cd ./sourceCode
cd ./LiteOS_Kernel

./build micaz

echo "Compilation the latest kernel for iris..."

./build iris 

echo "Copying the Kernel images..."

cd ..
cd ..

cd Tools
cd JavaTools
cd classes


cp ../../../SourceCode/LiteOS_Kernel/bin/micaz/LiteOS.hex ./LiteOS_micaz.hex 
cp ../../../SourceCode/LiteOS_Kernel/bin/iris/LiteOS.hex ./LiteOS_iris.hex

cd ..
cd ..
cd ..
cd Binary

cp ../SourceCode/LiteOS_Kernel/bin/micaz/LiteOS.hex ./LiteOS_micaz.hex 
cp ../SourceCode/LiteOS_Kernel/bin/iris/LiteOS.hex ./LiteOS_iris.hex

echo "If you don't see any other messages other than 'Copying the kernel images', your verification should be successful. Otherwise, please consult the manual for possible errors. "