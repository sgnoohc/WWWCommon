if [ -z $1 ]; then
  echo "Must provide the directory path to perfomr the hadding"
  exit
fi

ditto_hadd \
  -1 'www_incl' '-!' wwwincl \
  -y 'ttbar_1l,wjets_' -Y 'tt1l-W' \
  -o 'dy_,zz_2l,zz_2q,zz_4l' -O 'Z-ZZ' \
  -r 'wz_1l,wz_3l,wz_ln' -R 'WZ' \
  -b 'ww_2l2nu,ww_lnuqq,wpwp,ttbar_dilep' -B 'WW-tt2l' \
  -g 'ttg_,tth_,ttw_incl,ttz_incl' -G 'ttX' \
  -k 'tzq_,stt_,sttw_,sttwll_' -K 'tX' \
  -d WWW_v0.1.5_${1}/haddoutput -i WWW_v0.1.5_${1}/output/

#	  -2 'www_2l' '-@' wwwmia \
