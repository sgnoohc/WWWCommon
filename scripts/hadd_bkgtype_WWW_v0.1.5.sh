if [ -z $1 ]; then
  echo "Must provide the directory path to perfomr the hadding"
  exit
fi

ditto_hadd \
  -1 'www_2l' '-!' www \
  -2 'vh_nonbb' '-@' vh \
  -y 'ttbar_1l,ttbar_dilep' -Y 'tt' \
  -r 'wz_1l,wz_3l,wz_ln,zz_2l,zz_2q,zz_4l' -R 'WZ-ZZ' \
  -b 'ww_2l2nu_dbl,wpwp' -B 'ssWW' \
  -g 'ttg_,tth_,ttw_incl,ttz_incl' -G 'ttX' \
  -k 'tzq_,stt_,sttw_,dy_,wjets_,ww_2l2nu_powheg,ww_lnuqq' -K 'tX-Z-W-osWW' \
  '-!' '2,www'        -1 'www_2l'                              \
  '-@' '4,vh'         -2 'vh_nonbb'                            \
  '-A' '7003,tt'      -a 'ttbar_1l,ttbar_dilep'                \
  '-B' '7002,WZ-ZZ'   -b 'wz_1l,wz_3l,wz_ln,zz_2l,zz_2q,zz_4l' \
  '-C' '7001,WZ'      -c 'ww_2l2nu_dbl,wpwp'                   \
  '-D' '7005,WW-tt2l' -d 'ww_2l2nu,ww_lnuqq,wpwp,ttbar_dilep'  \
  '-E' '7004,ttX'     -e 'ttg_,tth_,ttw_incl,ttz_incl'         \
  '-F' '7006,tX'      -f 'tzq_,stt_,sttw_'                     \
  -d ${1}/haddoutput -i ${1}/output/

#	  -2 'www_2l' '-@' wwwmia \
#	  -3 'vh_nonbb,www_2l' '-#' wwwboth \
