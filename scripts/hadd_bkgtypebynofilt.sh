if [ -z $1 ]; then
  echo "Must provide the directory path to perfomr the hadding"
  exit
fi

ditto_hadd \
  '-!' '2,www'           -1 'www_2l'                                        \
  '-@' '4,vh'            -2 'vh_nonbb'                                      \
  '-A' '7003,tt1l-W-tX'  -a 'ttbar_1l,wjets_,tzq_,stt_,sttw_'               \
  '-B' '2011,ZZ'         -b 'zz_2l,zz_2q,zz_4l'                             \
  '-C' '2003,WZ'         -c 'wz_1l,wz_3l,wz_ln'                             \
  '-D' '2007,WW-tt2l-Z'  -d 'dy_,ww_2l2nu_powheg,ww_lnuqq,ttbar_dilep'      \
  '-E' '2001,ttX-ssWW'   -e 'ttg_,tth_,ttw_incl,ttz_incl,wpwp,ww_2l2nu_dbl' \
  -O ${1}/haddoutput -o ${1}/output/

#	  -2 'www_2l' '-@' wwwmia \
#	  -3 'vh_nonbb,www_2l' '-#' wwwboth \
