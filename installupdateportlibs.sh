#! /bin/bash
# 
7z x -y ./loader/libs/portlibs.zip -o${DEVKITPRO}  
mkdir portlib_repos
cd portlib_repos
((git clone https://github.com/Maschell/dynamic_libs.git -b lib && (7z x -y ./dynamic_libs/libs/portlibs.zip -o${DEVKITPRO})) || (cd dynamic_libs && git pull))
(git clone https://github.com/dimok789/libiosuhax.git || (cd libiosuhax && git pull))
(git clone https://github.com/Maschell/libfat.git || (cd libfat && git pull))   
(git clone https://github.com/Maschell/libntfs-wiiu.git || (cd libntfs-wiiu && git pull))   
(git clone https://github.com/Maschell/libutils.git || (cd libutils && git pull))   
((git clone https://github.com/Maschell/libgui.git && (7z x -y ./libgui/libs/portlibs.zip -o${DEVKITPRO})) || (cd libgui && git pull))   
(git clone https://github.com/Maschell/fs_wrapper.git || (cd fs_wrapper && git pull))  
(git clone https://github.com/Maschell/controller_patcher.git || (cd controller_patcher && git pull))
(cd dynamic_libs && ((make -j8 | grep -c "built ... ") && make install && echo "installed" ) || (echo "no need for make install" && make))
(cd libiosuhax && make -j8 && make install) 
(cd libfat && make wiiu-release && make wiiu-install)
(cd libntfs-wiiu && make wiiu-install)
(cd libutils && ((make -j8 | grep -c "built ... ") && make install && echo "installed" ) || (echo "no need for make install" && make))
(cd libgui && ((make -j8 | grep -c "built ... ") && make install) || echo "no need for make install")
(cd fs_wrapper && ((make -j8 | grep -c "built ... ") && make install && echo "installed" ) || (echo "no need for make install" && make))
(cd controller_patcher && ((make -j8 | grep -c "built ... ") && make install && echo "installed" ) || (echo "no need for make install" && make))
