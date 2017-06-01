if [ -z $1 ]; then
  echo "Must provide the directory path to perfomr the hadding"
  exit
fi

ditto_hadd \
  -1 'www_incl' '-!' wwwincl \
  -5 'www_2l' '-%' www2l \
  -y 'ttbar_dilep' -Y 'tt2l' \
  -2 'ttbar_1l' '-@' 'tt1l' \
  -3 'wjets_' '-#' 'W' \
  -o 'dy_' -O 'Z' \
  -r 'wz_1l,wz_3l,wz_ln' -R 'WZ' \
  -b 'ww_2l2nu,ww_lnuqq,wpwp' -B 'WW' \
  -4 'zz_2l,zz_2q,zz_4l' '-$' 'ZZ' \
  -g 'ttg_,tth_,ttw_incl,ttz_incl' -G 'ttX' \
  -k 'stt_,sttw_,sttwll_' -K 'tX' \
  -d ${1}/sync_haddoutput -i ${1}/output/
#	  -k 'tzq_,stt_,sttw_,sttwll_' -K 'tX' \
