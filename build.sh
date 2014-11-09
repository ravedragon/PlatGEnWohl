#/bin/bash

QMake="qmake"; #
#QMake="qmake-qt5"; # for CentOS

LRelease="lrelease"; #
#LRelease="lrelease-qt5"; # for CentOS


#=======================================================================
errorofbuid()
{
	printf "\n\n=========ERROR!!===========\n\n"
	exit 1
}
#=======================================================================
# build translations of the editor
cd Editor
$LRelease *.pro

if [ $? -eq 0 ]
then
  echo "[good]"
else
  errorofbuid
fi

cd ..

#=======================================================================
# build all components
$QMake CONFIG+=release CONFIG-=debug

if [ $? -eq 0 ]
then
  echo "\ngood\n"
else
  errorofbuid
fi

#=======================================================================
make

if [ $? -eq 0 ]
then
  echo "[good]"
else
  errorofbuid
fi

#=======================================================================
# copy data and configs into the build directory
make install
if [ $? -eq 0 ]
then
  echo "[good]"
else
  errorofbuid
fi

#=======================================================================
printf "\n\n=========BUILT!!===========\n\n"
read -n 1
exit 0
