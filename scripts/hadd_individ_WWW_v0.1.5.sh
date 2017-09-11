if [ -z $1 ]; then
  echo "Must provide the directory path to perfomr the hadding"
  exit
fi

ditto_hadd \
  '-!' '2,www'          -1 'www_2l'                             \
  '-@' '4,vh'           -2 'vh_nonbb'                           \
  '-A' '2005,tt1l'      -a 'ttbar_1l'                           \
  '-B' '2008,tt2l'      -b 'ttbar_dilep'                        \
  '-C' '2009,tX'        -c 'tzq_,stt_,sttw_,sttwll_,sts_'       \
  '-D' '2006,W'         -d 'wjets_'                             \
  '-E' '2010,Z'         -e 'dy_'                                \
  '-F' '2007,WW'        -f 'ww_2l2nu,ww_lnuqq,wpwpjj'           \
  '-H' '2003,WZ'        -h 'wz_1l,wz_3l,wz_ln'                  \
  '-I' '2011,ZZ'        -i 'zz_2l,zz_2q,zz_4l'                  \
  '-J' '2004,ttZ-all'   -j 'ttg_,tth_,ttz_incl'                 \
  '-K' '2002,ttW'       -k 'ttw_incl'                           \
  '-L' '616,VVV'        -k 'zzz_incl,wwz_incl,wzz_incl'         \
  -O ${1}/haddoutput -o ${1}/output/

#  '-G' '2001,ssWW'      -g 'ww_2l2nu_dbl,wpwp'                  \

#	  -2 'www_2l' '-@' wwwmia \
#	  -3 'vh_nonbb,www_2l' '-#' wwwboth \
