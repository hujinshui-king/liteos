echo "Must enter Tools directory to invoke upgrading script"
echo "Configuring uisp..."

cd ./uisp
./configure
./make
./make install

echo "Configuring build script for applications..."

cd ..
cd ..
cd ./Apps
echo "Current directory is: "
pwd
cp ./build /usr/local/bin


echo "Compilation the latest kernel..."

cd ..
cd ./sourceCode
cd ./LiteOS_Kernel

./build micaz

echo "Copying the Kernel images..."

cd ..
cd ..

cd Tools
cd JavaTools
cd classes


cp ../../../SourceCode/LiteOS_Kernel/bin/LiteOS.hex .

