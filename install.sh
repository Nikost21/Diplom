#!/bin/sh

INSTDIR=

if [ $# -eq 0 ]; then
    INSTDIR=$PWD
else
    INSTDIR=$1
fi
cd src_libsvirtboard
make OUTDIR="$INSTDIR/libvb"
cd ..
cd src_qtvirt_board 
echo $INSTDIR
qmake "LIBSDIR=$INSTDIR/libvb"
make
cd ..
cp src_qtvirt_board/qtvirt_board "$INSTDIR"
cp src_qtvirt_board/chip.png src_qtvirt_board/vbledoff.png src_qtvirt_board/vbledon.png src_qtvirt_board/vbbtnon.png src_qtvirt_board/vbbtnoff.png src_qtvirt_board/backgrnd.png "$INSTDIR"
echo "SystemCPath:" > "$INSTDIR/.config"
echo "VirtBoardPath:$INSTDIR/libvb/lib" >> "$INSTDIR/.config"
echo "inclVirtBoard:$INSTDIR/libvb/include" >> "$INSTDIR/.config"
echo "inclSystemC:" >> "$INSTDIR/.config"
echo "LexPath:$INSTDIR/libvb/" >> "$INSTDIR/.config"
echo "GccPath:" >> "$INSTDIR/.config"
