if [ -z $1 ]; then
  echo "Must provide the directory path to perfomr the hadding"
  exit
fi

ditto_hadd \
  '-!' '2,www'        -1 'www_2l'                             \
  '-@' '4,vh'         -2 'vh_nonbb'                           \
  '-A' '7003,tt1l-W'  -a 'ttbar_1l,wjets_'                    \
  '-B' '7002,Z-ZZ'    -b 'dy_,zz_2l,zz_2q,zz_4l'              \
  '-C' '7001,WZ'      -c 'wz_1l,wz_3l,wz_ln'                  \
  '-D' '7005,WW-tt2l' -d 'ww_2l2nu,ww_lnuqq,wpwp,ttbar_dilep' \
  '-E' '7004,ttX'     -e 'ttg_,tth_,ttw_incl,ttz_incl'        \
  '-F' '7006,tX'      -f 'tzq_,stt_,sttw_'                    \
  -O ${1}/haddoutput -o ${1}/output/

#	  -2 'www_2l' '-@' wwwmia \
#	  -3 'vh_nonbb,www_2l' '-#' wwwboth \
